#ifndef LEXER_H
#define LEXER_H

#include <QList>
#include <iostream>
#include <string>

using LEXEME = std::string;

// Лексема (Токен)
class Token {
public:
    enum class Type {
        Number,
        Identifier,
        Keyword,
        LeftParen,          // ()
        RightParen,
        LeftCurly,          // {}
        RightCurly,
        LessThan,           // <>
        GreaterThan,
        LessEqualThan,      // <=
        GreaterEqualThan,   // >=
        Equal,              // ==
        Plus,
        Minus,
        Assigment,          // =
        Comment,            //
        Semicolon,          // ;
        End,
        Unexpected
    };

    // Конструктор (тип)
    explicit Token(Type type) noexcept : m_type(type) {}

    // Тип, нач. символ, длина строки
    Token(Type type, const char* begin, std::size_t len) noexcept
        : m_type(type), m_lexeme(begin, len) {}

    // Тип, нач. символ, кон. символ
    Token(Type type, const char* begin, const char* end) noexcept
        : m_type(type), m_lexeme(begin, std::distance(begin, end)) {}

    // Тип
    Type type() const noexcept {return m_type;}

    void setType(Type type) noexcept {m_type = type;}

    // Соответсвия типу (-ам)
    bool is(Type type) const noexcept {return m_type == type;}

    bool is_not(Type type) const noexcept {return m_type != type;}

    bool is_one_of(Type t1, Type t2) const noexcept {return is(t1) || is(t2);}

    template<typename... Ts>
    bool is_one_of(Type t1, Type t2, Ts... ts) const noexcept {
        return is(t1) || is_one_of(t2, ts...);
    }

    // Лексема (значение)
    LEXEME lexeme() const noexcept {return m_lexeme;}

    void setLexeme(LEXEME lex) noexcept {m_lexeme = std::move(lex);}

    // Получить название токена с помощью <<
    friend std::ostream& operator<<(std::ostream& os, const Token::Type& type) {
        return os << names[static_cast<int>(type)];
    }

private:
    Type m_type;
    LEXEME m_lexeme;

    static std::string const names[];
};

// Лексический анализатор
class Lexer
{
public:
    explicit Lexer(const char* begin) noexcept : m_cur_char(begin) {tokenList = new QList<Token>;}
    ~Lexer() {delete tokenList;}

    QList<Token>* analys() noexcept;
    QList<Token>* tokens() const noexcept;



private:
    const char* m_cur_char = nullptr;
    //const char* program = nullptr;
    QList<Token>* tokenList = nullptr;

    Token identifier() noexcept;
    Token number() noexcept;
    Token comment() noexcept;
    Token less_or_lesseq() noexcept;
    Token greater_or_greatereq() noexcept;
    Token assign_or_equal() noexcept;
    Token atom(Token::Type) noexcept;

    char peek() const noexcept {return *m_cur_char;}
    char get() noexcept {return *m_cur_char++;}

    // Следующий токен
    Token next() noexcept;

    static QList<std::string> const keywords;
};

// Функции
bool is_space(char c) noexcept;
bool is_identifier_char(char c) noexcept;
bool is_digit(char c) noexcept;
bool is_letter(char c) noexcept;

#endif // LEXER_H
