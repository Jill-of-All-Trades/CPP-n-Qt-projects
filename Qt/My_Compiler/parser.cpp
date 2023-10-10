#include "parser.h"
#include <iomanip>

//#define DEBUG
#ifdef DEBUG
    #define debug_mess(txt) std::cout << txt << std::endl
#else
#define debug_mess(txt) {}
#endif

// AST NODE
ASTNode::ASTNode()
{

}

ASTNode::~ASTNode()
{
    // recursive delete
    if(hasChilds()) {
        QListIterator<ASTNode*> node(childs);
        while(node.hasNext()) delete node.next();
    }
    // Lexer will destroy all tokens in destructor
}

Token::Type ASTNode::getType() const
{
    if(!token) return Token::Type::Unexpected;
    return token->type();
}

const Token* ASTNode::getToken() const
{
    return token;
}

void ASTNode::setToken(const Token *t)
{
    token = t;
}

bool ASTNode::is_operator()
{
//    return getToken()->is_one_of(Token::Type::LessThan, Token::Type::LessEqualThan,
//                                 Token::Type::GreaterThan, Token::Type::GreaterEqualThan,
//                                 Token::Type::Plus, Token::Type::Minus,
//                                 Token::Type::Equal, Token::Type::Assigment);
    return is_logic_operator() || is_arith_operator() || getType() == Token::Type::Assigment;
}

bool ASTNode::is_logic_operator()
{
    return getToken()->is_one_of(Token::Type::LessThan, Token::Type::LessEqualThan,
                                 Token::Type::GreaterThan, Token::Type::GreaterEqualThan,
                                 Token::Type::Equal);
}

bool ASTNode::is_arith_operator()
{
    return getToken()->is_one_of(Token::Type::Plus, Token::Type::Minus);
}

void ASTNode::setParent(ASTNode *newpar)
{
    parent = newpar;
}

ASTNode *ASTNode::addChild(ASTNode *child)
{
    childs.append(child); return child;
}

void ASTNode::setChild(ASTNode *child, int num)
{
    childs[num] = child;
}

ASTNode *ASTNode::getParent() const
{
    return parent;
}

int ASTNode::getChildCount() const
{
    return childs.size();
}

ASTNode* ASTNode::getChild(int num) const
{
    return childs.at(num);
}

QList<ASTNode *> &ASTNode::getChilds()
{
    return childs;
}

bool ASTNode::hasChilds() const
{
    return !childs.isEmpty();
}

// AST (Abstract syntax tree)
AST::AST()
{
    Token* root_token = new Token(Token::Type::Unexpected);
    root_token->setLexeme("ROOT");
    root = new ASTNode(root_token); //new root
    current = root;
}

AST::~AST()
{
    //Delete root token
    delete root->getToken();
    //delete nodes
    delete root;
}

ASTNode *AST::getRoot() const
{
    return root;
}

ASTNode *AST::getCurrent() const
{
    return current;
}

void AST::toRoot()
{
    setCurrent(getRoot());
}

void AST::toParent()
{
    if(!current) return;
    current = current->getParent();
}

void AST::toChild(int num)
{
    if(!current) return;
    current = current->getChild(num);
}

void AST::toBlockBegin()
{
    const Token* tmp;
    do
    {
        toParent();
        tmp = current->getToken();
    }while( !(
               tmp->is(Token::Type::Unexpected) || // root
               (tmp->is(Token::Type::Keyword) && tmp->lexeme() == "if") ||
               (tmp->is(Token::Type::Keyword) && tmp->lexeme() == "then") ||
               (tmp->is(Token::Type::Keyword) && tmp->lexeme() == "else")
             )
            );
}

ASTNode *AST::getBlockBegin() const
{
    const Token* tmp;
    ASTNode* tmp_cur = current;
    do
    {
        tmp_cur = tmp_cur->getParent();
        tmp = tmp_cur->getToken();
    }
    while( !(isBlockBegin(tmp)));

    return tmp_cur;
}

bool AST::isBlockBegin(const Token* t) const
{
    return (
                t->is(Token::Type::Unexpected) || // root
                (t->is(Token::Type::Keyword) && t->lexeme() == "if") ||
                (t->is(Token::Type::Keyword) && t->lexeme() == "then") ||
                (t->is(Token::Type::Keyword) && t->lexeme() == "else")
            );
}

void AST::addBranch(ASTNode *branch)
{
    branch->setParent(current);
    current->addChild(branch);
    setCurrent(branch); // jump to branch
}

void AST::insertBranch(ASTNode *new_node, int num)
{
    ASTNode* child = current->getChild(num); //prev child
    current->setChild(new_node, num);
    new_node->addChild(child);
    new_node->setParent(current);
    child->setParent(new_node);

    current = new_node;
}

void AST::show(ASTNode* start_node, int offset) const
{
    ASTNode* node_pos = start_node; // use getRoot() for all tree
    std::cout << std::setw(offset) << node_pos->getToken()->lexeme() << "\n";
    if(node_pos->hasChilds())
    {
       // foreach(ASTNode* child, node_pos->getChilds()) show(child);
        QListIterator<ASTNode*> child(node_pos->getChilds());
        while(child.hasNext()) show(child.next(), offset + 4);
    }

}

void AST::setCurrent(ASTNode *n)
{
    current = n;
}

// PARSER
Parser::Parser(QList<Token> *t)
{
    // Convert from tokens to ASTNodes
    QListIterator<Token> tkn(*t);
    while(tkn.hasNext()) nodes.append(new ASTNode(&tkn.next()));

    // Init
    setState(State::root_body);
    tree = new AST();
    currentTokenInx = 0;
}

Parser::~Parser()
{
    delete tree;
}

AST* Parser::analys()
{
    int node_count = nodes.count();
    currentTokenInx = 0;
    try {
        while(currentTokenInx < node_count)
        {
            currentToken = nodes[currentTokenInx];    // get new token

            Token::Type tkn_type = currentToken->getType();

            // parsing
            switch (tkn_type)
            {
            case Token::Type::Identifier:       // var
                parseIdentifier();
                break;

            case Token::Type::Number:           // 0-9...
                parseNumber();
                break;

            case Token::Type::Assigment:        // =
                parseAssigment();
                break;

            case Token::Type::Semicolon:        // ;
                parseSemicolon();
                break;

            case Token::Type::Plus:             // +, -
            case Token::Type::Minus:
                parseArithOperator();
                break;

            case Token::Type::LessThan:
            case Token::Type::LessEqualThan:
            case Token::Type::GreaterThan:
            case Token::Type::GreaterEqualThan:
            case Token::Type::Equal:
                parseLogicOperator();
                break;

            case Token::Type::LeftParen:
                parseParenLeft();
                break;

            case Token::Type::RightParen:
                parseParenRight();
                break;

            case Token::Type::LeftCurly:
                parseCurlyLeft();
                break;

            case Token::Type::RightCurly:
                parseCurlyRight();
                break;


            case Token::Type::Keyword:
                {
                    std::string keyw_type = currentToken->getToken()->lexeme();
                    if(keyw_type == "if")           parseIf();
                    else if (keyw_type == "then")   parseThen();
                    else if (keyw_type == "else")   parseElse();
                    else if (keyw_type == "and")    parseAnd();
                    else if (keyw_type == "not")    parseNot();

                }
                break;

            default:
                //shift(currentToken);
                break;
            }


            //std::cout << "[" << currentTokenInx << "]" << std::endl;

            ++currentTokenInx;
        }
    }
    catch(const char* e) {
        std::cerr << "Parse Error: " << e << std::endl;
    }

    return tree;
}

void Parser::shift(ASTNode* tkn)
{
#ifdef DEBUG
    std::cout << "Shift: " << tkn->getToken()->lexeme() << std::endl;
#endif
    storage.push(tkn);
}

ASTNode *Parser::peek_next(int n) const
{
    int peek_inx = currentTokenInx + n;
    if(peek_inx < 0 || peek_inx > nodes.count() - 1) return nullptr;
    return nodes[peek_inx];
}

ASTNode* Parser::peek_storage() const
{
    if(storage.isEmpty()) return nullptr;
    return storage.top();
}

ASTNode *Parser::pop_storage()
{
#ifdef DEBUG
    std::cout << "Pop" << std::endl;
#endif
    return storage.pop();
}

void Parser::clear_storage()
{
    // Clear storage
    while(!storage.isEmpty()) pop_storage();
}

// Parser - parse methods
void Parser::parseIdentifier()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(!last_node) {shift(currentToken); return;}

    if(last_node->getToken()->is_one_of(Token::Type::Number, Token::Type::Identifier)) {
        throw "Indetifier - two ident in row | left number!";
    }
    shift(currentToken);
}

void Parser::parseNumber()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(!last_node) throw "Number - its a rvalue!";

    if(last_node->is_operator())
    {
        shift(currentToken);
    }
    else throw "Number - left operand invalid!";
}

void Parser::parseAssigment()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(!last_node) throw "Assigment - left operand is undeclared!";

    if(last_node->getType() == Token::Type::Identifier) // Its var
    {
        last_node = pop_storage();          // get out last node from storage
        currentToken->addChild(last_node);  // left oper
        last_node->setParent(currentToken);
        tree->addBranch(currentToken);

        shift(currentToken);
    }
    else {
        throw "Assigment - left operand isn't identifier!";
    }
}

void Parser::parseArithOperator()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(!last_node) throw "ArithOp - there is no left operand!";

    if(last_node->getToken()->is_one_of(Token::Type::Identifier, Token::Type::Number)) {
        last_node = pop_storage();          // get out last node from storage
        currentToken->addChild(last_node);  // left oper
        last_node->setParent(currentToken);
        tree->addBranch(currentToken);

        shift(currentToken);
    }
    else throw "ArithOp - invalid left operand!";
}

void Parser::parseLogicOperator()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(!last_node) throw "LogOp - there is no left operand!";

    if(last_node->getToken()->is_one_of(Token::Type::Number, Token::Type::Identifier) // x OR 0-9 OR +,-
            || last_node->is_arith_operator())
    {
        last_node = pop_storage();
        currentToken->addChild(last_node);
        last_node->setParent(currentToken);
        tree->addBranch(currentToken);

        shift(currentToken);
    }

}

void Parser::parseSemicolon()
{
    if(getState() == Parser::State::if_cond) throw "SemiColon - ; cannot be in IF condition";

    ASTNode* last_node = peek_storage();    // check storage
    if(last_node) { // There is node in storage
        if (last_node->getToken()->is_one_of(Token::Type::Identifier, Token::Type::Number)) {
            last_node = pop_storage();                  // get out last node from storage
            tree->addBranch(last_node);
        }
        else throw "SemiColon - invalid statement";
    }
    tree->toBlockBegin(); //

    // Clear storage
    clear_storage();

#ifdef DEBUG
    std::cout << "### LINE BREAK ###" << std::endl;
#endif
}

void Parser::parseIf()
{
    ASTNode* last_node = peek_storage();    // check storage
    bool success = false;
    if(!last_node) success = true;  // if from new string
    else if (last_node->getType() == Token::Type::Keyword){ // if inside other if-stat
        std::string keyw_type = last_node->getToken()->lexeme();
        if(keyw_type == "then" || keyw_type == "else") success = true;
    }

    if(success)
    {
        tree->addBranch(currentToken); // just add if branch
        shift(currentToken);

        //setState(State::if_cond);
    }
    else {
        throw "If - left node isn't THEN or ELSE!";
    }
}

void Parser::parseThen()
{
    ASTNode* last_node = tree->getCurrent();    // check storage
    if(!last_node) throw "Then/Else - cannot found If statement!";
    else if (last_node->getType() == Token::Type::Keyword) {
        std::string keyw_type = last_node->getToken()->lexeme();
        if(keyw_type == "if") {
            tree->addBranch(currentToken); // just add if branch
            shift(currentToken);
            return;
        }
    }
    throw "Then/Else - If required!";
}

void Parser::parseElse()
{
    parseThen();
}

void Parser::parseAnd()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(!last_node) throw "And - required If-statement!";

    if(last_node->getToken()->is_one_of(Token::Type::Identifier, Token::Type::Number)) {
        // Complete prev expression
        last_node = pop_storage();
        tree->addBranch(last_node);
        tree->toParent();

        // Check parent; IF/AND/NOT - insert between
        tree->toParent();
        ASTNode* block_beg = tree->getCurrent();
        std::string keyw_type = block_beg->getToken()->lexeme();
        if( keyw_type == "if" || keyw_type == "and" || keyw_type == "not") { //its if statement
            if(keyw_type == "not") tree->toParent();    // for right leaf
            tree->insertBranch(currentToken, static_cast<int>(AST::Root::Condition));
            shift(currentToken);
        }
        else throw "And - invalid par!";
    }
    else {
        throw "And - previous express uncompleted!";
    }

//    std::string keyw_type = last_node->getToken()->lexeme();
//    if(keyw_type == "if" || keyw_type == "and" || keyw_type == "not") {

//    }
//    else {
//        throw "And - cannot found If/And/Not keywords!";
//    }

}

void Parser::parseNot()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(!last_node) throw "Not - required If/And-statement!";

    // Check parent; IF/AND - insert between
    //tree->toParent();
    ASTNode* block_beg = tree->getCurrent();
    std::string keyw_type = block_beg->getToken()->lexeme();
    if( keyw_type == "if" || keyw_type == "and") { //its if statement
        //tree->insertBranch(currentToken, static_cast<int>(AST::Root::Condition));
        tree->addBranch(currentToken);
        shift(currentToken);
    }
    else throw "Not - invalid parent!";


}

void Parser::parseParenLeft()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(!last_node) throw "ParenLeft - there is no if-statement!";
    if(last_node->getToken()->lexeme() == "if") {

        if(getState() == State::if_cond) throw "ParenLeft - already if condition!";
        setState(State::if_cond);

//        if(getState() == State::root_body)
//            setState(State::if_cond);
//        else throw "ParenLeft - already if condition!";
    }
    else throw "ParenLeft - invalid if-statement!";
}

void Parser::parseParenRight()
{
    if(getState() == State::if_cond) {
        //setState(State::root_body);
        popState(); // to previous state;

        ASTNode* last_node = peek_storage();    // check storage
        if(!last_node) throw "ParenRight - expression uncompleted!";
        if(last_node->getToken()->is_one_of(Token::Type::Identifier, Token::Type::Number)) {
            // Complete prev expression
            last_node = pop_storage();
            tree->addBranch(last_node);
            tree->toParent();

            // to If statement
            tree->toBlockBegin();

            // Clear storage
            clear_storage();
            //while(peek_storage()->getToken()->lexeme() != "if") pop_storage();

            // check THEN
            ASTNode* next_node = peek_next(1); //nodes[currentTokenInx + 1]
            if(! next_node) throw "ParenRight - uncompleted if-statement!";
            if(next_node->getToken()->lexeme() != "then") throw "ParenRight - there is no THEN after if-cond!";
        }
        else throw "ParenRight - expression uncompleted!!";

    } else throw "ParenRight - there is no if cond!";
}

void Parser::parseCurlyLeft()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(!last_node) throw "CurlyLeft - there is no then/else-statement!";
    if(getState() == State::if_cond) throw "CurlyLeft - Cannot use { } into if-statement!";

    std::string keyw_type = last_node->getToken()->lexeme();
    if(keyw_type == "then") {
        // Remember state
        setState(State::if_then_body);
        pop_storage();
    }
    else if (keyw_type == "else") {
        setState(State::if_else_body);
        pop_storage();
    }
    else throw "CurlyLeft - invalid then/else-statement!";
}

void Parser::parseCurlyRight()
{
    ASTNode* last_node = peek_storage();    // check storage
    if(last_node) throw "CurlyRight - expression uncompleted!";

    if(getState() == State::root_body || getState() == State::if_cond)
        throw "CurlyRight - there is no if cond!";

    if(getState() == State::if_then_body) {
        ASTNode* next_node = peek_next(1);
        if(next_node)
        {
            if(next_node->getToken()->lexeme() != "else")
            {
                tree->toBlockBegin();
                debug_mess("{ -> not else after then - jump under of if!");
            }
        }
        else debug_mess("{ -> next node is END!");
    }
    else if (getState() == State::if_else_body) tree->toBlockBegin();
    popState(); // to prev state


    // to If statement
    tree->toBlockBegin(); // to if, cuz ; -> to then

    // Clear storage
    clear_storage();
}

//
void Parser::setState(Parser::State s)
{
    state.push(s);
}

Parser::State Parser::getState() const
{
    return state.top();
}

Parser::State Parser::popState()
{
    if (state.count() <= 1) return getState();
    return  state.pop();
}

//
AST *Parser::getTree() const
{
    return tree;
}


