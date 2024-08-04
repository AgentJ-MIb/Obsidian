#include "../helper/errors.hpp"
#include "lexer.hpp"
#include <unordered_map>
#include <bits/stdc++.h>

Lexer::Lexer(const std::string& source, const std::string& filename) {
    scanner.current = source;
    scanner.source = source;
    scanner.start = 0;
    scanner.column = 1;
    scanner.line = 1;
}

void Lexer::reset() {
    scanner.current = scanner.start;
    scanner.column = 1;
    scanner.line = 1;
}

char Lexer::peekNext() {
    return scanner.current[scanner.start + 1];
}

char Lexer::advance() {
  char currentChar = scanner.current[scanner.start];
  scanner.start++;
  scanner.column++;
  return currentChar;
}

char Lexer::peek() {
  return scanner.current[scanner.start];
}

const char* Lexer::lineStart(int line) {
    if (line <= 0) {
        return "";
    }

    const char* start = scanner.source.c_str();
    int currentLine = 1;

    while (currentLine < line && *start) {
        if (*start == '\n') {
            currentLine++;
        }
        start++;
    }

    return start;
}

bool Lexer::match(char expected) {
    if (isAtEnd())
        return false;
    if (scanner.current[scanner.start] != expected)
        return false;

    scanner.start++;
    scanner.column++;
    return true;
}

bool Lexer::isAtEnd() {
    return scanner.start >= scanner.current.size() || scanner.current[scanner.start] == '\0';
}

std::unique_ptr<Lexer::Token> Lexer::errorToken(const std::string& message) {
    Error::error(&token, message, *this);
    return makeToken(TokenKind::ERROR_TOKEN);
}

std::unique_ptr<Lexer::Token> Lexer::makeToken(TokenKind kind) {
  auto tokenPtr = std::make_unique<Token>();
  tokenPtr->length = scanner.start;
  tokenPtr->column = scanner.column;
  tokenPtr->line = scanner.line;
  tokenPtr->kind = kind;
  return tokenPtr;
}

std::unique_ptr<Lexer::Token> Lexer::identifier() {
    while (isalnum(peek()))
        advance();
    return makeToken(identifierType());
}

std::unique_ptr<Lexer::Token> Lexer::number() {
    while (isdigit(peek()))
        advance();

    if (peek() == '.' && isdigit(peekNext())) {
        advance();

        while (isdigit(peek()))
            advance();
    }

    return makeToken(TokenKind::NUMBER);
}

std::unique_ptr<Lexer::Token> Lexer::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n')
            token.line++;
        advance();
    }

    if (isAtEnd())
        return errorToken("Unterminated string.");
    advance();

    return makeToken(TokenKind::STRING);
}

TokenKind Lexer::checkKeyword(const std::string& identifier) {
    const std::unordered_map<std::string, TokenKind> keyword = {
        {"fn", TokenKind::FN},
        {"let", TokenKind::VAR},
        {"loop", TokenKind::LOOP},
        {"local", TokenKind::LOCAL},
        {"global", TokenKind::GLOBAL},
        {"print", TokenKind::PRINT},
        {"if", TokenKind::IF},
        {"else", TokenKind::ELSE},
        {"exit", TokenKind::EXIT},
        {"return", TokenKind::RETURN},
        {"const", TokenKind::CONST},
        {"class", TokenKind::CLASS},
        {"switch", TokenKind::SWITCH},
        {"case", TokenKind::CASE},
        {"break", TokenKind::BREAK},
        {"this", TokenKind::THIS},
    };

    auto it = keyword.find(identifier);
    return (it != keyword.end()) ? it->second : TokenKind::IDNET;
}

TokenKind Lexer::identifierType() {
  size_t tokenLength = token.length;

  if (scanner.start >= tokenLength) {
    std::string keyword(scanner.source.substr(scanner.start - tokenLength, tokenLength));
    return checkKeyword(keyword);
  } else {
    return TokenKind::IDNET;
  }
}

void Lexer::skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                scanner.column = 0;
                advance();
                break;
            case '#':
                while (peek() != '\n' && !isAtEnd()) {
                    advance();
                }
                break;
            default:
                return;
        }
    }
}

std::unique_ptr<Lexer::Token> Lexer::scanToken() {
  skipWhitespace();

  scanner.start = scanner.current.find_first_not_of(" \t\r", scanner.start);

  if (isAtEnd())
    return makeToken(TokenKind::END_OF_FILE);

  token = *makeToken(TokenKind::ERROR_TOKEN);
  char c = advance();

  if (isalpha(c))
    return identifier();
  if (isdigit(c))
    return number();

  switch (c) {
  case '(':
    return makeToken(TokenKind::LPAREN);
  case ')':
    return makeToken(TokenKind::RPAREN);
  case '{':
    return makeToken(TokenKind::LBRACE);
  case '}':
    return makeToken(TokenKind::RBRACE);
  case ';':
    return makeToken(TokenKind::SEMICOLON);
  case ',':
    return makeToken(TokenKind::COMMA);
  case '.':
    return makeToken(TokenKind::DOT);
  case '-':
    return makeToken(TokenKind::MINUS);
  case '+':
    return makeToken(TokenKind::PLUS);
  case '/':
    return makeToken(TokenKind::SLASH);
  case '*':
    return makeToken(TokenKind::STAR);
  case '[':
    return makeToken(TokenKind::LBRACKET);
  case ']':
    return makeToken(TokenKind::RBRACKET);
  case '=':
    return makeToken(match('=') ? TokenKind::EQUAL_EQUAL : TokenKind::EQUAL);
  case '!':
    return makeToken(match('=') ? TokenKind::BANG_EQUAL : TokenKind::BANG);
  case '<':
    return makeToken(match('=') ? TokenKind::LESS_EQUAL : TokenKind::LESS);
  case '>':
    return makeToken(match('=') ? TokenKind::GREATER_EQUAL : TokenKind::GREATER);
  case '"':
    return string();
  default:
    Error::error(&token, "Unexpected character: " + std::string(1, c), *this);
    return makeToken(TokenKind::ERROR_TOKEN);
  }
}
