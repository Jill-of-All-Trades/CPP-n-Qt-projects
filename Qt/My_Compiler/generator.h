#ifndef GENERATOR_H
#define GENERATOR_H

#include "parser.h"
#include <QMap>

class Generator
{
public:
    Generator(AST* tr);
    ~Generator();

    std::string generate();

private:
    // methods
    void child_remember_current();
    int  child_get_last() const;
    int  child_pop_last();

    void move_down();
    void move_up();

    void add_code_from_node(ASTNode* nd);
    void add_endl();

    // data
    AST* tree = nullptr;        // ref to AST
    ASTNode* current = nullptr;

    QStack<int> child_data; // last current child & block

    int bench_child_count = 0;
    int bench_child_current = 0;// current child

    bool not_flag = false;  // for logical not

    // Variables table
    QMap<int, QVector<std::string>*> variables;
    QStack<int> block_mem;
    int block_current = 0;      // current block of {} (0 ~ root)
    int block_max = 0;

    void add_variable(int block, std::string name);
    bool check_variable(std::string name) const;
    bool check_variable(int block, std::string name) const;

    void into_new_block();
    int pop_block();

    std::string output_program = "";
};

#endif // GENERATOR_H
