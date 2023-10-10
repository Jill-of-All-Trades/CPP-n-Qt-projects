#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <QStack>

class AST;
class ASTNode;

using TokenStorage = QStack<ASTNode*>;

class Parser
{
public:
    enum class State{
      root_body, if_cond, if_then_body, if_else_body
    };

    explicit Parser(QList<Token>* t);    // token init
    ~Parser();

    AST* analys();
    AST* getTree() const;
private:
    // Methods
    void parse();
    void shift(ASTNode*);
    ASTNode* peek_next(int n) const;
    ASTNode* peek_storage() const;
    ASTNode* pop_storage();
    void clear_storage();

    void parseIdentifier();
    void parseNumber();
    void parseAssigment();
    void parseArithOperator();
    void parseLogicOperator();
    void parseSemicolon();
    void parseIf();
    void parseThen();
    void parseElse();
    void parseAnd();
    void parseNot();

    void parseParenLeft(); // ( )
    void parseParenRight();
    void parseCurlyLeft();
    void parseCurlyRight();

    void  setState(State s);
    State getState() const;
    State popState();
    //
    QStack<State> state;

    QList<ASTNode*> nodes;          // all tokens in nodes
    ASTNode* currentToken = nullptr;
    int currentTokenInx = 0;        //

    AST* tree = nullptr;            // Our tree

    TokenStorage storage;
};


class AST
{
public:
    enum class Root {
        Left = 0,       // for log/math operators (=, <, > ...)
        Right = 1,
        Condition = 0   // for if-statement
    };

    AST();  // create root node
    ~AST();

    ASTNode* getRoot() const;
    ASTNode* getCurrent() const;
    void toRoot();
    void toParent();
    void toChild(int num);

    void toBlockBegin();
    ASTNode* getBlockBegin() const;
    bool isBlockBegin(const Token* t) const;

    void addBranch(ASTNode*);
    void insertBranch(ASTNode*, int num);

    void show(ASTNode*, int offset) const;

private:
    void setCurrent(ASTNode*);

    ASTNode* root;
    ASTNode* current;
};

class ASTNode {
public:
    ASTNode(); // for root
    ASTNode(const Token* t) : token(t) {}
    ASTNode(const Token* t, ASTNode* par) : token(t), parent(par) {}
    ~ASTNode();

    Token::Type  getType() const;
    const Token* getToken() const;
    void setToken(const Token* t);

    bool is_operator();
    bool is_logic_operator();
    bool is_arith_operator();

    void     setParent(ASTNode* newpar);
    ASTNode* addChild(ASTNode* child);
    void     setChild(ASTNode* child, int num);
    ASTNode* getParent() const;
    int      getChildCount() const;
    ASTNode* getChild(int num) const;
    QList<ASTNode*>& getChilds();
    bool     hasChilds() const;

private:
    const Token*  token = nullptr;
    ASTNode* parent = nullptr;
    QList<ASTNode*> childs;
};

#endif // PARSER_H
