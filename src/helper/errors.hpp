#pragma once

#include "../../inc/colorize.hpp"
#include "../lexer/lexer.hpp"
#include "../common.hpp"
#include <iostream>

class Error {
private:
    inline static int errorCount = 0;

public:
    static void error(const Lexer::Token* token, const std::string& msg, Lexer& lexer) {
        printErrorMessage(token, msg);
        printErrorLocation(token, lexer);

        if (errorCount == 5)
            Exit(ExitValue::_ERROR);
        errorCount++;
    }

    static void memError(const std::string& msg) {
        std::cout << termcolor::red << "Error" << termcolor::reset
                  << ": " << termcolor::yellow << msg
                  << termcolor::reset << std::endl;
        Exit(ExitValue::MEMORY_ALLOCATION_FAILURE);
    }

private:
    static void printErrorMessage(const Lexer::Token* token, const std::string& msg) {
        std::cout << termcolor::red << "Error" << termcolor::reset
                  << ": [line: " << termcolor::bright_blue << token->line << termcolor::reset
                  << ", column: " << termcolor::bright_blue << token->column
                  << termcolor::reset << "] " << termcolor::yellow << msg
                  << termcolor::reset << std::endl;
    }

    static void printErrorLocation(const Lexer::Token* token, Lexer& lexer) {
        std::string lineContent = lexer.lineStart(token->line);

        size_t lineEnd = lineContent.find('\n');
        if (lineEnd == std::string::npos)
            lineEnd = lineContent.size();

        std::cout << "    " << token->line << " | " << lineContent.substr(0, lineEnd) << std::endl;
        std::cout << "      |" << std::string(token->column, ' ') << termcolor::red << "^" << termcolor::reset << std::endl;
    }
};
