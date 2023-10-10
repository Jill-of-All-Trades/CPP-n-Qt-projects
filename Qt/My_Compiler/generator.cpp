#include "generator.h"


Generator::Generator(AST *tr)
{
    tree = tr;
}

Generator::~Generator()
{
    auto vecs = variables.values();
    QVector<std::string>* tmp = nullptr;
    foreach(tmp, vecs) {
        delete tmp;
    }
}

std::string Generator::generate()
{
    // 1) start pos to root
    Q_ASSERT(tree);
    tree->toRoot();

    // 2) setup output
    output_program = "#include <stdlib.h>\nint main() {\n";

    // 3) Generate
    current = tree->getCurrent();
    bench_child_count = current->getChildCount();
    bench_child_current = 0;

    // while there is at least 1 another child and we are at the root
    while ((bench_child_current < bench_child_count || child_get_last() != -1)) {

        bool is_block_beg = tree->isBlockBegin(current->getToken());

        if( is_block_beg) // add endl
        {
            add_endl();
        }

        bool all_child_parsed = (bench_child_current >= bench_child_count);

        if (!all_child_parsed) // Not all parsed - to parse
        {
            if(current->hasChilds()) { // has childs
                bool is_op = (current->is_operator() && bench_child_current == 1);

                auto lex_check = current->getToken()->lexeme();
                bool is_keyw_log_and = ((lex_check == "and") && bench_child_current == 1);
                bool is_keyw_log_not = ((lex_check == "not") && bench_child_current == 0);
                bool is_keyw_block = ((lex_check == "if" || lex_check == "then" || lex_check == "else") && bench_child_current == 0);

                if(is_op || is_keyw_log_and || is_keyw_log_not || is_keyw_block) // left part already parsed - add self
                {
                    add_code_from_node(current);
                }
                move_down();
            }
            else {std::cout << "Generate err: curr node hasn't any child, but bench_child_count is more than 0!" << std::endl;}
        }
        else { // all parsed - move up
            if(bench_child_count == 0) add_code_from_node(current);
            move_up();
        }

    }

    //
    //output_program += "\0";
    output_program += "\nreturn 0;\n}";
    return output_program;
}

void Generator::add_code_from_node(ASTNode *nd)
{
    const Token* tkn = nd->getToken(); // get token
    std::string part = "";

    switch (tkn->type()) {
        case Token::Type::Identifier:
        {
            std::string lex = tkn->lexeme();
            bool new_var = false;

            if(child_get_last() == 0 || tree->isBlockBegin(nd->getParent()->getToken()))    // left side check
            {
               bool var_exists = check_variable(lex);
               if(!var_exists) {add_variable(block_current, lex); new_var = true;}
            }
            else {  // right side check
                bool var_exists = check_variable(lex);
                if(!var_exists) {
                    std::cout << "[Generation warning]: variable " << lex << " isn't declared at all!" << std::endl;
                    add_variable(block_current, lex);
                }
            }

            std::string add_part = (new_var) ? "unsigned int " : "";
            part = add_part + lex;

        }
        break;

        case Token::Type::Keyword:
        {
            if(tree->isBlockBegin(tkn)) // if then else
            {
                std::string lex = tkn->lexeme();
                std::string add_part = (lex == "if") ? (lex + "(") : ((lex == "then") ? "{\n" : ((lex == "else") ? lex + "{\n" : ""));
                part = add_part;

                // into new block
                into_new_block();
            }
            else {
                std::string lex = tkn->lexeme();
                bool is_and = (lex == "and");
                bool is_not = (lex == "not");

                if(not_flag && is_and) {
                    part = ")"; not_flag = false;
                }
                else if (is_not) {not_flag = true;}
                part += (is_and) ? " && " : ((is_not ? "!(" : "")); // and
            }

        }
        break;

        default:
            part = tkn->lexeme();
        break;
    }

    // add txt
    output_program += part;
}

void Generator::add_endl() // only at block begin (root, if/then/else)
{
    std::string part = "";
    const Token* tkn = current->getToken(); // get token

    switch (tkn->type()) {
    case Token::Type::Keyword:
    {
        std::string lexeme = tkn->lexeme();
        if(lexeme == "if") {
            if(bench_child_current == 1) { // if condition parsed
                if(not_flag) {part = ")"; not_flag = false;}
                part += ")\n";
                pop_block();
            }
        }
        else if( (lexeme == "then" || lexeme == "else")) { // then else parsed
            if(bench_child_current != 0) part = ";\n";
            if(bench_child_current >= bench_child_count) { part += "}\n"; pop_block();}
        }
    }
    break;

    case Token::Type::Unexpected: // root
        if(bench_child_current != 0) part = ";\n";
    break;

    default:
        //part = ";\n";
    break;
    }

    if(part != "")
        output_program += part;
}

void Generator::add_variable(int block, std::string name)
{
    bool block_exists = variables.contains(block);
    if(!block_exists) {
        QVector<std::string>* vec = new QVector<std::string>;
        variables.insert(block, vec);
    }

    auto vec = variables.value(block); // get vec
    vec->append(name);
}

bool Generator::check_variable(std::string name) const
{
   bool res = check_variable(block_current, name);
   if(!res && block_current != 0) {
       res = check_variable(0, name); // global scope
   }
   return res;
}

bool Generator::check_variable(int block, std::string name) const
{
    auto vec = variables.value(block, nullptr); // get vec
    if(vec) return vec->contains(name);
    else return false;
}

void Generator::into_new_block()
{
    block_mem.push(block_current);
    block_current = ++block_max;
}

int Generator::pop_block()
{
    if(block_mem.isEmpty()) block_current = 0;
    else block_current = block_mem.pop();
    return block_current;
}

void Generator::child_remember_current()
{
    child_data.push(bench_child_current);
}

int Generator::child_get_last() const
{
    if (child_data.isEmpty()) return -1;
    return child_data.top();
}

int Generator::child_pop_last()
{
    if (child_data.isEmpty()) return -1;
    return child_data.pop();
}

void Generator::move_down() // e.g. go to child
{
    child_remember_current();
    tree->toChild(bench_child_current);
    current = tree->getCurrent();

    bench_child_count = current->getChildCount();
    bench_child_current = 0;
}

void Generator::move_up()
{
    tree->toParent();
    current = tree->getCurrent();
    bench_child_count = current->getChildCount();
    bench_child_current = child_pop_last() + 1;
}
