#pragma once

#include <iostream>

#include "types.hh"

// Section 11.3

enum class TokenType { ALPHANUMERIC, NUMERIC, DELIMITER, ENDFILE };

struct Token {
	TokenType type;
	std::string value;
};

Data *getexp(const Token &token);
Data *getexp(std::istream &is);
Data *getexplist(std::istream &is, Data *next_car = nullptr);
void putexp(Data *e, std::ostream &os);

Token gettoken(std::istream &is);
