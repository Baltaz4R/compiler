#include <iostream>
#include <iomanip>

#include "SymbolTable.h"

using namespace std;

SymbolTable::SymbolTable() {
	/* Symbol* add = new Symbol("?", "0x00", "0x00", "SECTION", "LOCAL", "?", "/");
	symbolTable.push_back(add); */
}

Symbol* SymbolTable::getSymbol(string name) {

	for (unsigned int i = 0; i < symbolTable.size(); i++)
		if (symbolTable[i]->name == name) 
			return symbolTable[i];

	return nullptr;
}

bool SymbolTable::addSymbol(string name, string value, string size, string type, string bind, string section, string rwx) {

	if (getSymbol(name) == nullptr) {

		Symbol* add = new Symbol(name, value, size, type, bind, section, rwx);
		symbolTable.push_back(add);

		return true;

	} else {
		return false;
	}

	return false;
}

bool SymbolTable::isDefine() {
	for (unsigned int i = 0; i < symbolTable.size(); i++)
		if (symbolTable[i]->section == "UND")
			return false;
	return true;;
}

void SymbolTable::shift(Shift* sh) {
	for (unsigned int i = 0; i < symbolTable.size(); i++) {
		Symbol* symbol = symbolTable[i];

		if (symbol->type == "LABEL" && symbol->value != "?") {
			int value = hexadecimalTodecimal(symbol->value);
			if (sh->section == symbol->section && value >= sh->location)
				symbol->value = decimalToHexadecimal(++value, 'w');

		} else if (symbol->type == "SECTION") {
			int size = hexadecimalTodecimal(symbol->size);
			if (sh->section == symbol->section && size >= sh->location)
				symbol->size = decimalToHexadecimal(++size, 'w');
		}
	}
}

ostream& operator<<(ostream& stream, const SymbolTable& symbolTable) {
	for (unsigned int i = 0; i < symbolTable.symbolTable.size(); i++)
		if (symbolTable.symbolTable[i]->type == "SECTION")
			stream << setw(7) << symbolTable.symbolTable[i]->type << "  "
				   << setw(7) << symbolTable.symbolTable[i]->number << "  "
				   << setw(7) << symbolTable.symbolTable[i]->name << "  "
				   << setw(7) << symbolTable.symbolTable[i]->section << "  "
				   << setw(7) << symbolTable.symbolTable[i]->value << "  "
				   << setw(7) << symbolTable.symbolTable[i]->bind << "  "
				   << setw(7) << symbolTable.symbolTable[i]->rwx << "  "
				   << setw(7) << symbolTable.symbolTable[i]->size << "\n";

	for (unsigned int i = 0; i < symbolTable.symbolTable.size(); i++)
		if (symbolTable.symbolTable[i]->type == "LABEL")
			stream << setw(7) << symbolTable.symbolTable[i]->type << "  "
				   << setw(7) << symbolTable.symbolTable[i]->number << "  "
				   << setw(7) << symbolTable.symbolTable[i]->name << "  "
				   << setw(7) << symbolTable.symbolTable[i]->section << "  "
				   << setw(7) << symbolTable.symbolTable[i]->value << "  "
				   << setw(7) << symbolTable.symbolTable[i]->bind << "  "
				   << setw(7) << symbolTable.symbolTable[i]->rwx << "  "
				   << setw(7) << symbolTable.symbolTable[i]->size << "\n";
	
	for (unsigned int i = 0; i < symbolTable.symbolTable.size(); i++)
		if (symbolTable.symbolTable[i]->type == "SYMBOL")
			stream << setw(7) << symbolTable.symbolTable[i]->type << "  "
				   << setw(7) << symbolTable.symbolTable[i]->number << "  "
				   << setw(7) << symbolTable.symbolTable[i]->name << "  "
				   << setw(7) << symbolTable.symbolTable[i]->section << "  "
				   << setw(7) << symbolTable.symbolTable[i]->value << "  "
				   << setw(7) << symbolTable.symbolTable[i]->bind << "  "
				   << setw(7) << symbolTable.symbolTable[i]->rwx << "  "
				   << setw(7) << symbolTable.symbolTable[i]->size << "\n";

	stream << endl;
	return stream;
}
