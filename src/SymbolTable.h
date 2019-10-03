#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <vector>

#include "Functions.h"
#include "Symbol.h"
#include "Shift.h"

using namespace std;

class SymbolTable {
private:

	vector<Symbol*> symbolTable;

public:
	SymbolTable();
	Symbol * getSymbol(string name);
	bool addSymbol(string name, string value, string size, string type, string bind, string section, string rwx = "/");
	bool isDefine();
	void shift(Shift* sh);
	friend ostream& operator<<(ostream& stream, const SymbolTable& symbolTable);
};

#endif