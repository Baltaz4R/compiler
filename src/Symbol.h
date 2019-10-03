#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <string>

using namespace std;

struct Symbol {
	unsigned int number;
	string value; // address(LABEL), value(SYMBOL)
	string size; // only foe SECTION
	string type;
	string bind;
	string name; // label or name
	string section;
	string rwx;

	static unsigned int ID;

	Symbol(string Name, string Value, string Size, string Type, string Bind, string Section, string RWX) : number(ID++), name(Name), value(Value), size(Size), type(Type), bind(Bind), section(Section), rwx(RWX) {}
};

#endif