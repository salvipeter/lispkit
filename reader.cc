#include <exception>

#include "reader.hh"

Data *getexp(const Token &token, std::istream &is) {
	switch (token.type) {
	case TokenType::NUMERIC:
		return new Number(std::atoi(token.value.c_str()));
	case TokenType::ALPHANUMERIC:
		return new Symbol(token.value);
	case TokenType::DELIMITER:		// should be "("
		return getexplist(is);
	case TokenType::ENDFILE:
		throw std::runtime_error("premature end of file");
	}
	return nullptr;								// cannot reach this
}

Data *getexp(std::istream &is) {
	auto token = gettoken(is);
	return getexp(token, is);
}

Data *getexplist(std::istream &is, Data *next_car) {
	Data *car, *cdr;
	if (next_car)
		car = next_car;
	else
		car = getexp(is);
	auto token = gettoken(is);
	if (token.value == ".")
		cdr = getexp(is);
	else if (token.value == ")")
		cdr = &nil;
	else
		cdr = getexplist(is, getexp(token, is));
	return new Cons(car, cdr);
}


// Section 11.4

Token gettoken(std::istream &is) {
	is >> std::ws;
	if (is.eof())
		return { TokenType::ENDFILE, "" };
	char c;
	is.get(c);
	if (std::isdigit(c) || c == '-') {
		std::string s(1, c);
		while (!is.eof()) {
			c = is.peek();
			if (c == std::char_traits<char>::eof() || !std::isdigit(c))
				break;
			is.get(c);
			s += c;
		}
		return { TokenType::NUMERIC, s };
	}
	if (std::isalpha(c)) {
		std::string s(1, c);
		while (!is.eof()) {
			c = is.peek();
			if (c == std::char_traits<char>::eof() || !(std::isalpha(c) || std::isdigit(c)))
				break;
			is.get(c);
			s += c;
		}
		return { TokenType::ALPHANUMERIC, s };
	}
	return { TokenType::DELIMITER, { c } };
}


// Section 11.5

// There are only 3 cases when we do not want space before printing something:
// - it is the first thing we print
// - it is immediately after a '('
// - it is a ')'
void putexp(Data *e, std::ostream &os) {
	if (e->issymbol())
		os << dynamic_cast<Symbol*>(e)->svalue;
	else if (e->isnumber())
		os << dynamic_cast<Number*>(e)->ivalue;
	else {
		os << '(';
		auto p = e;
		while (p->iscons()) {
			putexp(dynamic_cast<Cons*>(p)->car, os);
			p = dynamic_cast<Cons*>(p)->cdr;
			if (p->iscons())
				os << ' ';
		}
		if (p != &nil) {
			os << " . ";
			putexp(p, os);
		}
		os << ')';
	}
}
