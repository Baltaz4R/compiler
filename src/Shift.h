#ifndef SHIFT_H_
#define SHIFT_H_

#include <string>
#include <vector>

#include "Symbol.h"

class SymbolTable;

using namespace std;

struct Shift {
	int location;	// = locationCounter
	string section;	// = currentSection
	bool move;
	Symbol* symbol;

	Shift(int Location, string Section, Symbol* Symbol) : location(Location), section(Section), move(true), symbol(Symbol) {}
};

class ShiftTable {
private:

	vector<Shift*> shiftTable;

public:
	Shift* getShift(int i);
	void addShift(int location, string section, Symbol* symbol);
	void shift(SymbolTable &symbolTable);
};

#endif
