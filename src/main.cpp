#include "common.hpp"
#include "helper/flags.hpp"
#include "lexer/lexer.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[]) {
  flags(argc, argv);

  const char* filePath = argv[1];
  std::unique_ptr<char[]> src = readfile(filePath);
  Lexer tokens(src.get(), filePath);
  std::unique_ptr<Lexer::Token> token = tokens.scanToken();
  
  while (token->kind != TokenKind::END_OF_FILE) {
    std::cout << token->kind << std::endl;
    token = tokens.scanToken();
  }
  return 0;
}
