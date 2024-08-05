#include "lexer.hpp"
#include "../helper/errors.hpp"
#include <cinttypes>
#include <string>
#include <unordered_map>
// #include <bits/stdc++.h>

Lexer::Lexer(const std::string &source, const std::string &filename) {
  scanner.current = source.c_str();
  scanner.source = source.c_str();
  scanner.start = source.c_str();
  scanner.column = 1;
  scanner.line = 1;

  initMaps(); // initialize the maps when the lexer is created
}

void Lexer::reset() {
  scanner.current = scanner.start;
  scanner.column = 1;
  scanner.line = 1;
}

char Lexer::advance() {
  scanner.current++;
  scanner.column++;
  return scanner.current[-1];
}

char Lexer::peek() { return *scanner.current; }

const char *Lexer::lineStart(int line) {
  const char *start = scanner.source;
  int cLine = 1;

  while (cLine != line) {
    if (*start == '\n')
      cLine++;
    start++;
  }

  return start;
}

bool Lexer::match(char expected) {
  if (isAtEnd())
    return false;
  if (*scanner.current != expected)
    return false;

  scanner.current++;
  scanner.column++;
  return true;
}

bool Lexer::isAtEnd() { return *scanner.current == '\0'; }

std::unique_ptr<Lexer::Token> Lexer::errorToken(const std::string &message) {
  Error::error(&token, message, *this);
  return makeToken(TokenKind::ERROR_TOKEN);
}

std::unique_ptr<Lexer::Token> Lexer::makeToken(TokenKind kind) {
  auto tokenPtr = std::make_unique<Token>();
  tokenPtr->lexem = std::string(scanner.start, scanner.current);
  tokenPtr->length = scanner.current - scanner.start;
  tokenPtr->column = scanner.column;
  tokenPtr->line = scanner.line;
  tokenPtr->kind = kind;
  return tokenPtr;
}

std::unique_ptr<Lexer::Token> Lexer::identifier() {
  while (peek() == '_' || isalnum(peek())) advance();
  std::string keyword(scanner.start, scanner.current);
  return makeToken(checkKeyword(keyword));
}

std::unique_ptr<Lexer::Token> Lexer::number() {
  while (isdigit(peek()))
    advance();
  if (peek() == '.' && isdigit(peek() + 1)) {
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

void Lexer::skipWhitespace() {
  for (;;) {
    char c = peek();
    auto it = skipWhiteSpaceFunctions.find(c);
    if (it == skipWhiteSpaceFunctions.end())
      return;
    it->second(*this);
  }
}

std::unique_ptr<Lexer::Token> Lexer::scanToken() {
  skipWhitespace();

  scanner.start = scanner.current;

  if (isAtEnd())
    return makeToken(TokenKind::END_OF_FILE);

  char c = advance();

  if (isalpha(c))
    return identifier();
  if (isdigit(c))
    return number();
  if (c == '"')
    return string();

  auto res = sc_dcLookup(std::string(1, c), c);
  if (res != TokenKind::ERROR_TOKEN)
    return makeToken(res);

  Error::error(&token, "Unexpected character: " + std::string(1, c), *this);
  return makeToken(TokenKind::ERROR_TOKEN);
}
