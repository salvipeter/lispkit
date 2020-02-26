#pragma once

#include <string>

// Section 11.1

struct Data;

extern Data *nil;
extern Data *bool_t, *bool_f; // Section 11.8

Data *symbol(std::string s);
Data *number(int n);
Data *cons(Data *a, Data *b);

bool issymbol(Data *d);
bool isnumber(Data *d);
bool iscons(Data *d);

std::string svalue(Data *d);
int ivalue(Data *d);
Data *car(Data *d);
Data *cdr(Data *d);
void rplaca(Data *c, Data *d);

void dispose(Data *d);
