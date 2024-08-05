#include "lexer.hpp"

#include <climits>
#include <iostream>
#include <memory>
#include <unordered_map>

void Lexer::initMaps() {
  skipWhiteSpaceFunctions = {
      {' ', [](Lexer &lexer) { lexer.advance(); }},
      {'\r', [](Lexer &lexer) { lexer.advance(); }},
      {'\t', [](Lexer &lexer) { lexer.advance(); }},
      {'\n',
       [](Lexer &lexer) {
         lexer.advance();
         lexer.scanner.line++;
         lexer.scanner.column = 1;
       }},
      {'#',
       [](Lexer &lexer) {
         while (lexer.peek() != '\n' && !lexer.isAtEnd()) {
           lexer.advance();
         }
       }},
  };
  keyboards = {
      {"fn", TokenKind::FN},         {"let", TokenKind::VAR},
      {"loop", TokenKind::LOOP},     {"local", TokenKind::LOCAL},
      {"global", TokenKind::GLOBAL}, {"print", TokenKind::PRINT},
      {"if", TokenKind::IF},         {"else", TokenKind::ELSE},
      {"exit", TokenKind::EXIT},     {"return", TokenKind::RETURN},
      {"const", TokenKind::CONST},   {"class", TokenKind::CLASS},
      {"switch", TokenKind::SWITCH}, {"case", TokenKind::CASE},
      {"break", TokenKind::BREAK},   {"this", TokenKind::THIS},
  };
  dcMap = {
      {"==", TokenKind::EQUAL_EQUAL}, {"!=", TokenKind::BANG_EQUAL},
      {"<=", TokenKind::LESS_EQUAL},  {">=", TokenKind::GREATER_EQUAL},
      {"++", TokenKind::INC},         {"--", TokenKind::DEC},
  };
  scMap = {
      {'(', TokenKind::LPAREN},   {')', TokenKind::RPAREN},
      {'{', TokenKind::LBRACE},   {'}', TokenKind::RBRACE},
      {'[', TokenKind::LBRACKET}, {']', TokenKind::RBRACKET},
      {',', TokenKind::COMMA},    {'+', TokenKind::PLUS},
      {'-', TokenKind::MINUS},    {'*', TokenKind::STAR},
      {'/', TokenKind::SLASH},    {';', TokenKind::SEMICOLON},
      {':', TokenKind::COLON},    {'.', TokenKind::DOT},
      {'!', TokenKind::BANG},     {'=', TokenKind::EQUAL},
      {'<', TokenKind::LESS},     {'>', TokenKind::GREATER},
  };
}

TokenKind Lexer::checkKeyword(std::string identifier) {
  auto it = keyboards.find(identifier);
  if (it != keyboards.end()) {
    return it->second;
  }
  return TokenKind::IDNET;
}

TokenKind Lexer::sc_dcLookup(std::string dc, char sc) {
    if (dc.size() == 2) {
        auto it = dcMap.find(dc);
        if (it != dcMap.end()) {
        return it->second;
        }
    }
    auto it = scMap.find(sc);
    if (it != scMap.end()) {
        return it->second;
    }
    return TokenKind::ERROR_TOKEN; 
}