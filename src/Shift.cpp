#include <iostream>

#include "Shift.h"
#include "Functions.h"
#include "SymbolTable.h"

using namespace std;

Shift * ShiftTable::getShift(int i) {
	return shiftTable[i];
}

void ShiftTable::addShift(int location, string section, Symbol * symbol) {
	Shift* shift = new Shift(location, section, symbol);
	shiftTable.push_back(shift);
}

void ShiftTable::shift(SymbolTable & symbolTable) {
	bool flag = true;
	while (flag) {
		flag = false;
		for (unsigned int i = 0; i < shiftTable.size(); i++) {

			Shift* shift = shiftTable[i];
			Symbol* symbol = shift->symbol;

			int value = hexadecimalTodecimal(symbol->value);
			if (shift->move && ((shift->section == symbol->section && ((value - shift->location) < -128 || (value - shift->location) > 127)) ||
				shift->section != symbol->section)) {	// ako je pomeraj veci od byte
				flag = true;
				shift->move = false;
				for (unsigned int i = 0; i < shiftTable.size(); i++) {	// uvecaj sve koji su iznad i u istoj sekciji
					Shift* s = shiftTable[i];
					if (shift->section == s->section && s->location > shift->location)
						s->location++;
				}
				symbolTable.shift(shift);
			}
		}
	}
}
