#include "lexer.h"

#include <iomanip>
#include <QChar>

// Названия токенов
std::string const Token::names[] = {
    "Number",       "Identifier",   "Keyword",      "LeftParen",    "RightParen",   "LeftCurly",
    "RightCurly",   "LessThan",     "GreaterThan",  "LessEqualThan","GreaterEqualThan",
    "Equal",        "Plus",         "Minus",        "Assigment",    "Comment",
    "Semicolon",    "End",          "Unexpected"
};

// Ключевые слова
QList<std::string> const Lexer::keywords = {"if", "then", "else", "and", "not"};

bool is_space(char c) noexcept
{
    return QChar(c).isSpace();
}

bool is_digit(char c) noexcept
{
    return  QChar(c).isDigit();
}

bool is_letter(char c) noexcept
{
    return QChar(c).isLetter() || c == '_';
}

bool is_identifier_char(char c) noexcept
{
    return QChar(c).isLetterOrNumber() || c == '_';
}


// Lexer
QList<Token> *Lexer::analys() noexcept
{
    // Очистить перед заполнением
    if(!tokenList->isEmpty()) tokenList->clear();

    for(auto token = next();
        !token.is(Token::Type::End); // !token.is_one_of(Token::Type::End, Token::Type::Unexpected)
        token = next())
    {
        tokenList->append(token);
        std::cout << std::setw(16) << token.type() << " |" << token.lexeme() << "|\n";
    }
    std::cout << std::endl; // для виз. разделения

    return tokens();
}

QList<Token> *Lexer::tokens() const noexcept
{
    return tokenList;
}

Token Lexer::next() noexcept
{
    while(is_space(peek())) get();

    auto ch = peek();

    //
    if(is_letter(ch)) return identifier();
    else if(is_digit(ch)) return number();
    //
    else
    switch (ch) {
    case '\0':
        return Token(Token::Type::End, m_cur_char, 1);
    case '(':
        return atom(Token::Type::LeftParen);
    case ')':
        return atom(Token::Type::RightParen);
    case '{':
        return atom(Token::Type::LeftCurly);
    case '}':
        return atom(Token::Type::RightCurly);
    case '<':
        return less_or_lesseq();
    case '>':
        return greater_or_greatereq();
    case '=':
        return assign_or_equal();
    case '+':
        return atom(Token::Type::Plus);
    case '-':
        return atom(Token::Type::Minus);
    case '/':
        return comment();
    case ';':
        return atom(Token::Type::Semicolon);
    default:
        return atom(Token::Type::Unexpected);
    }
}

Token Lexer::identifier() noexcept
{
    const char* start = m_cur_char;
    get();
    while(is_identifier_char(peek())) get();
    Token _token = Token(Token::Type::Identifier, start, m_cur_char);
    if(Lexer::keywords.contains(_token.lexeme())) _token.setType(Token::Type::Keyword);
    return _token;
}

Token Lexer::number() noexcept
{
    bool unexpect = false;
    bool loop = true;
    Token::Type ttype = Token::Type::Number;

    const char* start = m_cur_char;
    /*get();
    while(is_digit(peek())) get(); */
    get();
    do {
        loop = is_identifier_char(peek());
        unexpect = is_letter(peek());
        if(loop) get();
        if(unexpect) ttype = Token::Type::Unexpected;
    } while(loop);

    return Token(ttype, start, m_cur_char);
}

Token Lexer::comment() noexcept
{
    const char* start = m_cur_char;
     get();
     if (peek() == '/') {
       get();
       start = m_cur_char;
       while (peek() != '\0') {
         if (get() == '\n') {
           return Token(Token::Type::Comment, start,
                        std::distance(start, m_cur_char) - 1);
         }
       }
       return Token(Token::Type::Unexpected, m_cur_char, 1);
     } else {
       return Token(Token::Type::Unexpected, start, 1);
     }
}

Token Lexer::less_or_lesseq() noexcept
{
    const char* start = m_cur_char;
    get();
    if(peek() == '=') return Token(Token::Type::LessEqualThan, start, ++m_cur_char);
    else return Token(Token::Type::LessThan,start,1);
}

Token Lexer::greater_or_greatereq() noexcept
{
    const char* start = m_cur_char;
    get();
    if(peek() == '=') return Token(Token::Type::GreaterEqualThan, start, ++m_cur_char);
    else return Token(Token::Type::GreaterThan,start,1);
}

Token Lexer::assign_or_equal() noexcept
{
    const char* start = m_cur_char;
    get();
    if(peek() == '=') return Token(Token::Type::Equal, start, ++m_cur_char);
    else return Token(Token::Type::Assigment,start,1);
}

Token Lexer::atom(Token::Type type) noexcept
{
    return Token(type, m_cur_char++, 1);
}



