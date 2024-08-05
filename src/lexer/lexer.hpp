#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

enum TokenKind {
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  RBRACKET,
  LBRACKET,
  COMMA,
  PLUS,
  MINUS,
  STAR,
  SLASH,
  SEMICOLON,
  COLON,
  DOT,
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  INC,
  DEC,
  IDNET,
  NUMBER,
  STRING,
  FN,
  VAR,
  RETURN,
  LOOP,
  PRINT,
  EXIT,
  LOCAL,
  GLOBAL,
  IF,
  ELSE,
  CLASS,
  CONST,
  DEFAULT,
  BREAK,
  SWITCH,
  CASE,
  THIS,
  NEW,
  ERROR_TOKEN,
  END_OF_FILE
};

class Lexer {
public:
  Lexer(const std::string &source, const std::string &filename);

  struct Token {
    std::string lexem; // this is the actual token in string form
    TokenKind kind;
    size_t length;
    int current;
    int column;
    int line;
  };

  std::unique_ptr<Token> scanToken();
  std::unique_ptr<Token> errorToken(const std::string &message);
  Token token;
  const char *lineStart(int line);
  void reset();

  // Note: We are switching to use maps for fatser lookups and nicer to mangage
  // and add too later on.
  using SkipWhiteSpaceFunction = std::function<void(Lexer &)>;
  std::unordered_map<char, SkipWhiteSpaceFunction> skipWhiteSpaceFunctions;
  std::unordered_map<std::string, TokenKind> keyboards;
  std::unordered_map<std::string, TokenKind> dcMap; // Double char map
  std::unordered_map<char, TokenKind> scMap;        // Single char map
  TokenKind sc_dcLookup(std::string dc, char sc);
  void initMaps(); // This is used to initialize the maps at compile time to
                   // avoid runtime overhead

private:
  struct Scanner {
    const char *current;
    const char *source;
    const char *start;
    int column;
    int line;
  };

  Scanner scanner;

  char advance();
  char peek();

  bool match(char expected);
  bool isAtEnd();

  std::unique_ptr<Token> makeToken(TokenKind kind);
  std::unique_ptr<Token> identifier();
  std::unique_ptr<Token> number();
  std::unique_ptr<Token> string();

  TokenKind checkKeyword(std::string indentifer);

  void skipWhitespace();
};
