#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "Functions.h"
#include "SymbolTable.h"
#include "Section.h"
#include "Shift.h"

using namespace std;


int numberOfOperands(string instruction) {

	if (instruction == "halt" || instruction == "HALT" ||
		instruction == "ret" || instruction == "RET" ||
		instruction == "iret" || instruction == "IRET")
		return 0;
	
	if (instruction == "int" || instruction == "INT" ||
		instruction == "not" || instruction == "NOT" || instruction == "notb" || instruction == "NOTB" || instruction == "notw" || instruction == "NOTW" ||
		instruction == "push" || instruction == "PUSH" ||
		instruction == "pop" || instruction == "POP" ||
		instruction == "jmp" || instruction == "JMP" ||
		instruction == "jeq" || instruction == "JEQ" ||
		instruction == "jne" || instruction == "JNE" ||
		instruction == "jgt" || instruction == "JGT" ||
		instruction == "call" || instruction == "CALL")
		return 1;

	if (instruction == "xchg" || instruction == "XCHG" || instruction == "xchgb" || instruction == "XCHGB" || instruction == "xchgw" || instruction == "XCHGW" ||
		instruction == "mov" || instruction == "MOV" || instruction == "movb" || instruction == "MOVB" || instruction == "movw" || instruction == "MOVW" ||
		instruction == "add" || instruction == "ADD" || instruction == "addb" || instruction == "ADDB" || instruction == "addw" || instruction == "ADDW" ||
		instruction == "sub" || instruction == "SUB" || instruction == "subb" || instruction == "SUBB" || instruction == "subw" || instruction == "SUBW" ||
		instruction == "mul" || instruction == "MUL" || instruction == "mulb" || instruction == "MULB" || instruction == "mulw" || instruction == "MULW" ||
		instruction == "div" || instruction == "DIV" || instruction == "divb" || instruction == "DIVB" || instruction == "divw" || instruction == "DIVW" ||
		instruction == "cmp" || instruction == "CMP" || instruction == "cmpb" || instruction == "CMPB" || instruction == "cmpw" || instruction == "CMPW" ||
		instruction == "and" || instruction == "AND" || instruction == "andb" || instruction == "ANDB" || instruction == "andw" || instruction == "ANDW" ||
		instruction == "or" || instruction == "OR" || instruction == "orb" || instruction == "ORB" || instruction == "orw" || instruction == "ORW" ||
		instruction == "xor" || instruction == "XOR" || instruction == "xorb" || instruction == "XORB" || instruction == "xorw" || instruction == "XORW" ||
		instruction == "test" || instruction == "TEST" || instruction == "testb" || instruction == "TESTB" || instruction == "testw" || instruction == "TESTW" ||
		instruction == "shl" || instruction == "SHL" || instruction == "shlb" || instruction == "SHLB" || instruction == "shlw" || instruction == "SHLW" ||
		instruction == "shr" || instruction == "SHR" || instruction == "shrb" || instruction == "SHRB" || instruction == "shrw" || instruction == "SHRW")
		return 2;

	return -1;
}


bool isInstruction(string instruction) {
	if (numberOfOperands(instruction) != -1)
		return true;
	else
		return false;
}


bool isJumpInstruction(string instruction) {
	char first = instruction.at(0);
	if (first == 'j' || first == 'J' ||
		instruction == "call" || instruction == "CALL")
		return true;
	else
		return false;
}


int sizeOfOperand(string instruction) {
	if (instruction == "sub" || instruction == "SUB")
		return 2;

	char last = instruction.at(instruction.length() - 1);
	if (last == 'b' || last == 'B')
		return 1;
	else
		return 2;
}


bool isRegister(string reg) {
	if (((reg.at(0) == 'r' || reg.at(0) == 'R') && (reg.at(1) == '0' || reg.at(1) == '1' || reg.at(1) == '2' || reg.at(1) == '3' || reg.at(1) == '4' || reg.at(1) == '5' || reg.at(1) == '6' || reg.at(1) == '7')) ||
		reg.substr(0, 2) == "pc" || reg.substr(0, 2) == "PC" ||
		reg.substr(0, 2) == "sp" || reg.substr(0, 2) == "SP")
		return true;
	else
		return false;
}


void getRegister(const string operand, string &reg, string &offset) {
	char* iterator = strtok((char*)operand.c_str(), "[]");	// razdvoj

	if (iterator != NULL) {
		reg = iterator;
		iterator = strtok(NULL, "[]");
		if (iterator != NULL) {
			offset = iterator;
			iterator = strtok(NULL, "[]");
			if (iterator != NULL)
				throw "Greska prilikom registarskog adresiranja!";
		}
	} else
		throw "Greska prilikom registarskog adresiranja!";
}


int registerNumber(string reg) {
	if (isRegister(reg)) {
		if ((reg.at(0) == 'p' && reg.at(1) == 'c') || (reg.at(0) == 'P' && reg.at(1) == 'C'))
			return 7;
		if ((reg.at(0) == 's' && reg.at(1) == 'p') || (reg.at(0) == 'S' && reg.at(1) == 'P'))
			return 6;

		return (int)(reg.at(1) - '0');
	} else
		return -1;
}


uint8_t instructionID(string instruction) {

	uint8_t id;

	if (instruction == "halt" || instruction == "HALT")
		id = 1;
	else if (instruction.substr(0, 4) == "xchg" || instruction.substr(0, 4) == "XCHG")
		id = 2;
	else if (instruction == "int" || instruction == "INT")
		id = 3;
	else if (instruction.substr(0, 3) == "mov" || instruction.substr(0, 3) == "MOV")
		id = 4;
	else if (instruction.substr(0, 3) == "add" || instruction.substr(0, 3) == "ADD")
		id = 5;
	else if (instruction.substr(0, 3) == "sub" || instruction.substr(0, 3) == "SUB")
		id = 6;
	else if (instruction.substr(0, 3) == "mul" || instruction.substr(0, 3) == "MUL")
		id = 7;
	else if (instruction.substr(0, 3) == "div"|| instruction.substr(0, 3) == "DIB")
		id = 8;
	else if (instruction.substr(0, 3) == "cmp" || instruction.substr(0, 3) == "CMP")
		id = 9;
	else if (instruction.substr(0, 3) == "not" || instruction.substr(0, 3) == "NOT")
		id = 10;
	else if (instruction.substr(0, 3) == "and" || instruction.substr(0, 3) == "AND")
		id = 11;
	else if (instruction.substr(0, 2) == "or" || instruction.substr(0, 2) == "OR")
		id = 12;
	else if (instruction.substr(0, 3) == "xor" || instruction.substr(0, 3) == "XOR")
		id = 13;
	else if (instruction.substr(0, 4) == "test" || instruction.substr(0, 4) == "TEST")
		id = 14;
	else if (instruction.substr(0, 3) == "shl" || instruction.substr(0, 3) == "SHL")
		id = 15;
	else if (instruction.substr(0, 3) == "shr" || instruction.substr(0, 3) == "SHR")
		id = 16;
	else if (instruction == "push" || instruction == "PUSH")
		id = 17;
	else if (instruction == "pop" || instruction == "POP")
		id = 18;
	else if (instruction == "jmp" || instruction == "JMP")
		id = 19;
	else if (instruction == "jeq" || instruction == "JEQ")
		id = 20;
	else if (instruction == "jne" || instruction == "JNE")
		id = 21;
	else if (instruction == "jgt" || instruction == "JGT")
		id = 22;
	else if (instruction == "call"|| instruction == "CALL")
		id = 23;
	else if (instruction == "ret" || instruction == "RET")
		id = 24;
	else if (instruction == "iret" || instruction == "IRET")
		id = 25;

	id *= 8;
	if (sizeOfOperand(instruction) == 2)
		id += 4;

	return id;
}


int sizeOfOperand2(string operand2, int size, SymbolTable &symbolTable) {

	if (isNumber(operand2))
		return 2 + size;

	else if (operand2.at(0) == '&')
		return 2 + size;

	//registarsko
	else if (isRegister(operand2)) {
		char* iterator = strtok((char*)operand2.c_str(), "[]");	// razdvoj

		string reg;
		string offset = "";
		if (iterator != NULL) {
			reg = iterator;
			iterator = strtok(NULL, "[]");
			if (iterator != NULL)
				offset = iterator;
		} else
			throw "Ovde nikada ne bi smeo da udje!";

		// direktno r<num>
		if (offset == "") {
			return 2;

			// indirektno
		} else {
			if (isNumber(offset)) {
				int value = stoi(offset);

				// bez pomeraja sp[0]
				if (value == 0)
					return 2;

				// sa pomerajem, r<num>[<val>]
				else if (value < -128 || value > 127)
					return 4;
				else
					return 3;

				// sa simbolom kao pomerajem, r<num>[<symbol_name>]
			} else {
				Symbol* symbol = symbolTable.getSymbol(offset);

				if (symbol && symbol->type == "SYMBOL") {
					int value = hexadecimalTodecimal(symbol->value);

					if (value < -128 || value > 127)
						return 4;
					else
						return 3;
				} else
					throw "Pomeraj nije definisan!";
			}
		}

		//pcrelsym
	} else if (operand2.at(0) == '$')
		return 3;

	//memdir
	else if (operand2.at(0) == '*')
		return 4;

	//abssym	
	else
		return 4;
}


void instruction(const vector<string> text, SymbolTable &symbolTable, Section* section, int &locationCounter, const string operand, const int size) {

	// immed <val>
	if (isNumber(operand)) {
		int value = stoi(operand);

		section->instruction.push_back(0);	// immed je 0
		section->instruction.push_back((uint8_t)(255 & value));	// 0xFF & value, da bi uzeli samo 8bita

		if (sizeOfOperand(text[0]) == 2)
			section->instruction.push_back((uint8_t)((65280 & value) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita

		locationCounter++;
		locationCounter += sizeOfOperand(text[0]);

		// immed &<symbol_name>
	} else if (operand.at(0) == '&') {
		string symbolName = operand.substr(1);
		Symbol* symbol = symbolTable.getSymbol(symbolName);

		locationCounter++;
		section->instruction.push_back(0);	// immed je 0

		if (symbol->value == "?" || symbol->bind == "GLOBAL") {

			Realocation* relocation = new Realocation(symbol->number, locationCounter, "R_386_16");
			section->relocation.push_back(relocation);

			section->instruction.push_back(0); // simbol nije dodeljen

			if (sizeOfOperand(text[0]) == 2)
				section->instruction.push_back(0); // visa polovina

		} else {

			if (section->name != symbol->section) {	// ako nije u istoj sekciji	// TODO: sta je ovo?
				Symbol* sectionSymbol = symbolTable.getSymbol(symbol->section);
				Realocation* relocation = new Realocation(sectionSymbol->number, locationCounter, "R_386_16");
				section->relocation.push_back(relocation);
			}

			int value = hexadecimalTodecimal(symbol->value);
			section->instruction.push_back((uint8_t)(255 & value));	// 0xFF & value, da bi uzeli samo 8bita	

			if (sizeOfOperand(text[0]) == 2)
				section->instruction.push_back((uint8_t)((65280 & value) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita
		}

		locationCounter += sizeOfOperand(text[0]);

		// reg
	} else if (isRegister(operand)) {
		char* iterator = strtok((char*)operand.c_str(), "[]");	// razdvoj

		string reg;
		string offset = "";
		if (iterator != NULL) {
			reg = iterator;
			iterator = strtok(NULL, "[]");
			if (iterator != NULL)
				offset = iterator;
		} else
			throw "Ovde nikada ne bi smeo da udje!";

		int regNumber = registerNumber(reg);

		// direktno r<num> AM AM AM R R R R L/H
		if (offset == "") {
			uint8_t regCode = (uint8_t)(32 | (regNumber * 2)); // 0010 0000 zato sto je direktno 0x01

			if (sizeOfOperand(text[0]) == 1 && (operand.at(2) == 'h' || operand.at(2) == 'H'))
				regCode |= 1;	// 0000 0001 zbog L/H

			section->instruction.push_back(regCode);
			locationCounter++;

			// indirektno AM AM AM R R R R L/H
		} else {
			if (isNumber(offset)) {
				int value = stoi(offset);

				// bez pomeraja sp[0]
				if (value == 0) {
					section->instruction.push_back((uint8_t)(64 | (regNumber * 2)));  // 0100 0000 zato sto je ind bez pom 0x02
					locationCounter++;

					// sa pomerajem, r<num>[<val>]
				} else if (value < -128 || value > 127) {
					section->instruction.push_back((uint8_t)(128 | (regNumber * 2)));  // 1000 0000 zato sto je ind sa pom od 16bita 0x04
					section->instruction.push_back((uint8_t)(255 & value));	// 0xFF & value, da bi uzeli samo 8bita	
					section->instruction.push_back((uint8_t)((65280 & value) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita
					locationCounter += 3;
				} else {
					section->instruction.push_back((uint8_t)(96 | (regNumber * 2)));  // 0110 0000 zato sto je ind sa pom od 8bita 0x03
					section->instruction.push_back((uint8_t)(255 & value));	// 0xFF & value, da bi uzeli samo 8bita	
					locationCounter += 2;
				}

				// sa simbolom kao pomerajem, r<num>[<symbol_name>]
			} else {
				Symbol* symbol = symbolTable.getSymbol(offset);
				int value = hexadecimalTodecimal(symbol->value);
				locationCounter++;

				if (value < -128 || value > 127)
					section->instruction.push_back((uint8_t)(128 | (regNumber * 2)));  // 1000 0000 zato sto je ind sa pom od 16bita 0x04
				else
					section->instruction.push_back((uint8_t)(96 | (regNumber * 2)));  // 0110 0000 zato sto je ind sa pom od 8bita 0x03

				if (symbol->bind == "GLOBAL") {
					Realocation* relocation = new Realocation(symbol->number, locationCounter, "R_386_16");
					section->relocation.push_back(relocation);

					section->instruction.push_back(0); // simbol nije dodeljen
					if (value < -128 || value > 127)
						section->instruction.push_back(0); // visa polovina	

				} else {
					section->instruction.push_back((uint8_t)(255 & value));	// 0xFF & value, da bi uzeli samo 8bita
					if (value < -128 || value > 127)
						section->instruction.push_back((uint8_t)((65280 & value) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita
				}

				if (value < -128 || value > 127)
					locationCounter += 2;
				else
					locationCounter++;
			}
		}

		// pcrel $<symbol_name>
	} else if (operand.at(0) == '$') {
		string symbolName = operand.substr(1);
		Symbol* symbol = symbolTable.getSymbol(symbolName);

		locationCounter++;

		if (symbol->value == "?" || symbol->bind == "GLOBAL") {
			char value = -2;

			section->instruction.push_back((uint8_t)(128 | (7 * 2)));  // pc je 7 // 1000 0000 zato sto je ind sa pom od 16bita 0x04

			Realocation* relocation = new Realocation(symbol->number, locationCounter, "R_386_PC16");
			section->relocation.push_back(relocation);

			section->instruction.push_back((uint8_t)(255 & value));	// 0xFF & value, da bi uzeli samo 8bita	
			section->instruction.push_back((uint8_t)((65280 & value) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita

			locationCounter += 2;

		} else if (section->name != symbol->section) {
			int value = hexadecimalTodecimal(symbol->value);
			int diff = value - 2;

			section->instruction.push_back((uint8_t)(128 | (7 * 2)));  // pc je 7 // 1000 0000 zato sto je ind sa pom od 16bita 0x04

			Symbol* sectionSymbol = symbolTable.getSymbol(symbol->section);
			Realocation* relocation = new Realocation(sectionSymbol->number, locationCounter, "R_386_PC16");
			section->relocation.push_back(relocation);

			section->instruction.push_back((uint8_t)(255 & diff));	// 0xFF & value, da bi uzeli samo 8bita	
			section->instruction.push_back((uint8_t)((65280 & diff) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita

			locationCounter += 2;

		} else {
			int value = hexadecimalTodecimal(symbol->value);
			int PC = locationCounter + 2 + size;
			int diff = value - PC;

			if ((value - PC) < -128 || (value - PC) > 127) {
				section->instruction.push_back((uint8_t)(128 | (7 * 2)));  // pc je 7 // 1000 0000 zato sto je ind sa pom od 16bita 0x04
				section->instruction.push_back((uint8_t)(255 & diff));	// 0xFF & value, da bi uzeli samo 8bita	
				section->instruction.push_back((uint8_t)((65280 & diff) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita
				locationCounter += 2;
			} else {
				section->instruction.push_back((uint8_t)(96 | (7 * 2)));  // pc je 7 // 0110 0000 zato sto je ind sa pom od 8bita 0x03
				section->instruction.push_back((uint8_t)(255 & diff));	// 0xFF & value, da bi uzeli samo 8bita
				locationCounter++;
			}
		}

		//memdir *<val>
	} else if (operand.at(0) == '*') {
		int value = stoi(operand.substr(1));

		section->instruction.push_back(160);	// mem je 5
		section->instruction.push_back((uint8_t)(255 & value));	// 0xFF & value, da bi uzeli samo 8bita	
		section->instruction.push_back((uint8_t)((65280 & value) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita
		locationCounter += 3;

		//absolutno adresiranje simbola, <symbol_name>
	} else {
		Symbol* symbol = symbolTable.getSymbol(operand);

		locationCounter++;
		section->instruction.push_back(160); // mem je 5

		if (symbol->value == "?" || symbol->bind == "GLOBAL") {

			Realocation* relocation = new Realocation(symbol->number, locationCounter, "R_386_16");
			section->relocation.push_back(relocation);

			section->instruction.push_back(0); // simbol nije dodeljen
			section->instruction.push_back(0); // visa polovina

		} else {

			if (section->name != symbol->section) {	// ako nije u istoj sekciji	// TODO: sta je ovo?
				Symbol* sectionSymbol = symbolTable.getSymbol(symbol->section);
				Realocation* relocation = new Realocation(sectionSymbol->number, locationCounter, "R_386_16");
				section->relocation.push_back(relocation);
			}

			int value = hexadecimalTodecimal(symbol->value);
			section->instruction.push_back((uint8_t)(255 & value));	// 0xFF & value, da bi uzeli samo 8bita
			section->instruction.push_back((uint8_t)((65280 & value) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita
		}
		locationCounter += 2;
	}
}


void firstPass(const vector<vector<string>> text, SymbolTable &symbolTable) {

	string currentSection = "";
	int locationCounter = 0;

	ShiftTable* shiftTable = new ShiftTable();

	for (unsigned int i = 0; i < text.size(); i++) {

		// labela
		if (text[i][0].at(text[i][0].length() - 1) == ':') {
			if (currentSection == "")
				throw "Labela je definisana van sekcije!";

			string name = text[i][0].substr(0, text[i][0].length() - 1);

			if (!symbolTable.addSymbol(name, decimalToHexadecimal(locationCounter, 'w'), "/", "LABEL", "LOCAL", currentSection)) {
				Symbol* exist = symbolTable.getSymbol(name);

				if (exist->section == "UND") {
					exist->section = currentSection;
					exist->value = decimalToHexadecimal(locationCounter, 'w');
				} else
					throw "Labela je vec definisana!";
			}

		// .global <ime_simbola>
		} else if (text[i][0] == ".global") {

			for (unsigned int j = 1; j < text[i].size(); j++) {

				for (unsigned int k = j + 1; k < text[i].size(); k++)
					if (text[i][j] == text[i][k])
						throw "Simbol je vise puta definisan!";

				if (!symbolTable.addSymbol(text[i][j], "?", "/", "LABEL", "GLOBAL", "UND"))
					symbolTable.getSymbol(text[i][j])->bind = "GLOBAL";
			}

		// .extern <ime_simbola>
		} else if (text[i][0] == ".extern") {

			for (unsigned int j = 1; j < text[i].size(); j++) {

				for (unsigned int k = j + 1; k < text[i].size(); k++)
					if (text[i][j] == text[i][k])
						throw "Simbol je vise puta definisan!";

				if (!symbolTable.addSymbol(text[i][j], "?", "/", "LABEL", "GLOBAL", "?"))
					throw "Simbol je vec definisan!";
			}

		// .equ
		} else if (text[i][0] == ".equ") {
			if (text[i].size() != 3)
				throw "Definicija direktive .equ nije ispravna!";

			if (!isNumber(text[i][2]))
				throw "Ocekujem cifru!";

			int valueInt = stoi(text[i][2]);
			string value;
			if (valueInt < -128 || valueInt > 127) {
				value = decimalToHexadecimal(valueInt, 'w');
			} else {
				value = decimalToHexadecimal(valueInt, 'b');
			}

			if (!symbolTable.addSymbol(text[i][1], value, "/", "SYMBOL", "LOCAL", "?")) {
				Symbol* exist = symbolTable.getSymbol(text[i][1]);

				if (exist->section == "UND") {
					exist->value = value;
					exist->type = "SYMBOL";
					exist->section = "?";
				} else {
					throw "Simbol je vec definisan!";
				}
			}

		// .text, .data i .bss
		} else if (text[i][0] == ".text" || text[i][0] == ".data" || text[i][0] == ".bss") {

			if (text[i].size() > 1)
				throw "Definicija sekcije mora da se sastoji samo od jedne reci!";

			string rwx;
			if (text[i][0] == ".text")
				rwx = "r-x";
			else
				rwx = "rw-";

			if (symbolTable.addSymbol(text[i][0].substr(1), "0x00", "0x00", "SECTION", "LOCAL", text[i][0].substr(1), rwx)) {

				if (currentSection != "")
					symbolTable.getSymbol(currentSection)->size = decimalToHexadecimal(locationCounter, 'w');

				currentSection = text[i][0].substr(1);
				locationCounter = 0;

			} else {
				throw "Sekcija je vec definisana!";
			}

		// .section <ime_sekcije> [, "<flegovi>"]
		} else if (text[i][0] == ".section") {

			if (text[i].size() < 2 || text[i].size() > 3)
				throw "Definicija sekcije nije ispravna!";

			string rwx = "rwx";
			if (text[i].size() == 3) {
				rwx = text[i][2];

				if (rwx.size() != 3)
					throw "Definicija flegova nije ispravna!";
				else if (rwx[0] != 'r' && rwx[0] != '-' &&
					rwx[1] != 'w' && rwx[1] != '-' &&
					rwx[2] != 'x' && rwx[2] != '-')
					throw "Definicija flegova nije ispravna!";
			}

			if (symbolTable.addSymbol(text[i][1].substr(1), "0x00", "0x00", "SECTION", "LOCAL", text[i][1].substr(1), rwx)) {

				if (currentSection != "")
					symbolTable.getSymbol(currentSection)->size = decimalToHexadecimal(locationCounter, 'w');

				currentSection = text[i][1].substr(1);
				locationCounter = 0;

			} else {
				throw "Sekcija je vec definisana!";
			}

		// .end
		} else if (text[i][0] == ".end") {
			if (currentSection != "")
				symbolTable.getSymbol(currentSection)->size = decimalToHexadecimal(locationCounter, 'w');

			locationCounter = 0;
			break;

		// .byte, .word
		} else if (text[i][0] == ".byte" || text[i][0] == ".word") {
			if (currentSection == "text")
				throw "Direktiva .byte i .word ne smeju biti u .text sekciji!";

			else if (currentSection == "bss") {
				if (text[i].size() > 1)
					throw "U .bss sekciji se ne smeju inicijalizovati podatci!";

				if (text[i][0] == ".byte")
					locationCounter += 1;
				else	// .word 
					locationCounter += 2;

			} else {
				if (text[i].size() <= 1)
					throw "Nakon direktiva .byte i .word moraju stajati vrednosti!";

				int numberOfData = text[i].size() - 1;
				if (text[i][0] == ".byte")
					locationCounter += numberOfData;
				else	// .word 
					locationCounter += 2 * numberOfData;
			}

		// .align
		} else if (text[i][0] == ".align") {
			if (text[i].size() != 2)
				throw "Nakon direktive .align mora stajati vrednost!";

			if (!isNumber(text[i][1]))
				throw "Ocekujem cifru!";

			int align = stoi(text[i][1]);

			if (align == 0 || (align & (align - 1)))
				throw "Nakon direktive .align mora stajati broj koji je stepen dvojke!";

			int mod = locationCounter % align;
			if (mod != 0) {
				locationCounter -= mod;
				locationCounter += align;
			}

		// .skip
		} else if (text[i][0] == ".skip") {
			if (text[i].size() != 2)
				throw "Nakon direktive .skip mora stajati vrednost!";

			if (!isNumber(text[i][1]))
				throw "Ocekujem cifru!";

			int skip = stoi(text[i][1]);

			locationCounter += skip;
		
		// instruction
		} else if (isInstruction(text[i][0])) {
			if (text[i].size() > 3)
				throw "Nije moguce imati vise od tri operanata!";

			Symbol* sym = symbolTable.getSymbol(currentSection);
			if (sym->rwx != "rwx" && sym->rwx != "r-x" && sym->rwx != "-wx" && sym->rwx != "--x")
				throw "Instrukcija je u sekciji koja ne moze da se izvrsi (rwx)!";

			locationCounter++;

			int operands = numberOfOperands(text[i][0]);

			// no-operands
			if (operands == 0) {
				if (text[i].size() != 1)
					throw "Instrukcija ne sme da ima operande!";

			// one-operand
			} else if (operands == 1) {
				if (text[i].size() != 2)
					throw "Instrukcija mora da ima tacno jedan operand!";

				string operand = text[i][1];

				// immed <val>, &<symbol_name>
				if (isNumber(operand) || operand.at(0) == '&') {
					if (text[i][0] == "push" || text[i][0] == "PUSH") {

						if (operand.at(0) == '&') {
							string symbolName = operand.substr(1);
							symbolTable.addSymbol(symbolName, "?", "/", "LABEL", "LOCAL", "UND");
						}

						locationCounter++;
						locationCounter += sizeOfOperand(text[i][0]);
					} else
						throw "Nije dozvoljeno neposredno adresiranje za dst operand!";

				// reg
				} else if (isRegister(operand)) {

					string reg;
					string offset = "";
					getRegister(operand, reg, offset);

					// direktno r<num>
					if (offset == "") {
						if (isJumpInstruction(text[i][0]))
							throw "Za datu instrukciju nije dozvoljeno registarsko direktno adresiranje!";

						if (sizeOfOperand(text[i][0]) == 1)
							if (reg.length() != 3)
								throw "Greska prilikom registarskog adresiranja!";
							else if (reg.at(2) != 'l' && reg.at(2) != 'L' && reg.at(2) != 'h' && reg.at(2) != 'H')
								throw "Greska prilikom registarskog adresiranja!";

						locationCounter++;

					// indirektno
					} else {
						if (isNumber(offset)) {
							int value = stoi(offset);

							// bez pomeraja sp[0]
							if (value == 0)
								locationCounter++;

							// sa pomerajem, r<num>[<val>]
							else if (value < -128 || value > 127)
								locationCounter += 3;
							else
								locationCounter += 2;

						// sa simbolom kao pomerajem, r<num>[<symbol_name>]
						} else {
							Symbol* symbol = symbolTable.getSymbol(offset);

							if (symbol && symbol->type == "SYMBOL") {
								int value = hexadecimalTodecimal(symbol->value);

								if (value < -128 || value > 127)
									locationCounter += 3;
								else
									locationCounter += 2;
							} else
								throw "Pomeraj nije definisan!";
						}
					}

				// pcrel $<symbol_name>
				} else if (operand.at(0) == '$') {
					string symbolName = operand.substr(1);

					symbolTable.addSymbol(symbolName, "?", "/", "LABEL", "LOCAL", "UND");

					locationCounter += 2;

					shiftTable->addShift(locationCounter, currentSection, symbolTable.getSymbol(symbolName));

				//memdir *<val>
				} else if (operand.at(0) == '*') {

					if (isNumber(operand.substr(1)))
						locationCounter += 3;
					else 
						throw "Ocekujem cifru!";

				//absolutno adresiranje simbola, <symbol_name>
				} else {

					symbolTable.addSymbol(operand, "?", "/", "LABEL", "LOCAL", "UND");
					locationCounter += 3;
				}
			// END one-operand

			// two-operands
			} else if (operands == 2) {
				if (text[i].size() != 3)
					throw "Instrukcija mora da ima tacno dva operanda!";

				string operand1 = text[i][1];
				string operand2 = text[i][2];

				// operand1
				// immed <val>, &<symbol_name>
				if (isNumber(operand1) || operand1.at(0) == '&')
					throw "Nije dozvoljeno neposredno adresiranje za dst operand!";

				// reg
				else if (isRegister(operand1)) {

					string reg;
					string offset = "";
					getRegister(operand1, reg, offset);

					// direktno r<num>
					if (offset == "") {

						if (sizeOfOperand(text[i][0]) == 1)
							if (reg.length() != 3)
								throw "Greska prilikom registarskog adresiranja!";
							else if (reg.at(2) != 'l' && reg.at(2) != 'L' && reg.at(2) != 'h' && reg.at(2) != 'H')
								throw "Greska prilikom registarskog adresiranja!";

						locationCounter++;

					// indirektno
					} else {
						if (isNumber(offset)) {
							int value = stoi(offset);

							// bez pomeraja sp[0]
							if (value == 0)
								locationCounter ++;

							// sa pomerajem, r<num>[<val>]
							else if (value < -128 || value > 127)
								locationCounter += 3;
							else
								locationCounter += 2;

						// sa simbolom kao pomerajem, r<num>[<symbol_name>]
						} else {
							Symbol* symbol = symbolTable.getSymbol(offset);

							if (symbol && symbol->type == "SYMBOL") {
								int value = hexadecimalTodecimal(symbol->value);

								if (value < -128 || value > 127)
									locationCounter += 3;
								else
									locationCounter += 2;
							} else
								throw "Pomeraj nije definisan!";
						}
					}

				// pcrel $<symbol_name>
				} else if (operand1.at(0) == '$') {
					string symbolName = operand1.substr(1);

					symbolTable.addSymbol(symbolName, "?", "/", "LABEL", "LOCAL", "UND");

					locationCounter += 2;

					shiftTable->addShift(locationCounter + sizeOfOperand2(operand2, sizeOfOperand(text[i][0]), symbolTable), currentSection, symbolTable.getSymbol(symbolName));

				//memdir *<val>
				} else if (operand1.at(0) == '*') {

					if (isNumber(operand1.substr(1)))
						locationCounter += 3;
					else
						throw "Ocekujem cifru!";

				//absolutno adresiranje simbola, <symbol_name>
				} else {

					symbolTable.addSymbol(operand1, "?", "/", "LABEL", "LOCAL", "UND");
					locationCounter += 3;
				}
				// END operand1

				// operand2
				// immed <val>, &<symbol_name>
				if (isNumber(operand2) || operand2.at(0) == '&') {
					if (text[i][0].substr(0, 4) != "xchg" && text[i][0].substr(0, 4) != "XCHG") {

						if (operand2.at(0) == '&') {
							string symbolName = operand2.substr(1);
							symbolTable.addSymbol(symbolName, "?", "/", "LABEL", "LOCAL", "UND");
						}

						locationCounter++;
						locationCounter += sizeOfOperand(text[i][0]);
					} else
						throw "Nije dozvoljeno neposredno adresiranje za dst operand!";

				// reg
				} else if (isRegister(operand2)) {

					string reg;
					string offset = "";
					getRegister(operand2, reg, offset);

					// direktno r<num>
					if (offset == "") {

						if (sizeOfOperand(text[i][0]) == 1)
							if (reg.length() != 3)
								throw "Greska prilikom registarskog adresiranja!";
							else if (reg.at(2) != 'l' && reg.at(2) != 'L' &&
								reg.at(2) != 'h' && reg.at(2) != 'H')
								throw "Greska prilikom registarskog adresiranja!";

						locationCounter++;

					// indirektno
					} else {
						if (isNumber(offset)) {
							int value = stoi(offset);

							// bez pomeraja sp[0]
							if (value == 0)
								locationCounter++;

							// sa pomerajem, r<num>[<val>]
							else if (value < -128 || value > 127)
								locationCounter += 3;
							else
								locationCounter += 2;

						// sa simbolom kao pomerajem, r<num>[<symbol_name>]
						} else {
							Symbol* symbol = symbolTable.getSymbol(offset);

							if (symbol && symbol->type == "SYMBOL") {
								int value = hexadecimalTodecimal(symbol->value);

								if (value < -128 || value > 127)
									locationCounter += 3;
								else
									locationCounter += 2;
							} else
								throw "Pomeraj nije definisan!";
						}
					}

				// pcrel $<symbol_name>
				} else if (operand2.at(0) == '$') {
					string symbolName = operand2.substr(1);

					symbolTable.addSymbol(symbolName, "?", "/", "LABEL", "LOCAL", "UND");

					locationCounter += 2;

					shiftTable->addShift(locationCounter, currentSection, symbolTable.getSymbol(symbolName));

				//memdir *<val>
				} else if (operand2.at(0) == '*') {

					if (isNumber(operand2.substr(1)))
						locationCounter += 3;
					else
						throw "Ocekujem cifru!";

				//absolutno adresiranje simbola, <symbol_name>
				} else {

					symbolTable.addSymbol(operand2, "?", "/", "LABEL", "LOCAL", "UND");
					locationCounter += 3;
				}
				// END operand2
			}
			// END two-operands
		} else
			throw "Operacija ne postoji!";
	}

	if (!symbolTable.isDefine())
		throw "Ima nedefinisan simbol!";

	shiftTable->shift(symbolTable);
	
}	


vector<Section*> secondPass(const vector<vector<string>> text, SymbolTable &symbolTable) {

	vector<Section*> sections;
	Section* section = new Section("");
	int locationCounter = 0;

	for (unsigned int i = 0; i < text.size(); i++) {

		// labela, .global, .extern, .equ
		if (text[i][0].at(text[i][0].length() - 1) == ':' || text[i][0] == ".global" || text[i][0] == ".extern" || text[i][0] == ".equ")
			continue;

		// .text, .data i .bss
		else  if (text[i][0] == ".text" || text[i][0] == ".data" || text[i][0] == ".bss") {
			if (section->name != "")
				sections.push_back(section);

			section = new Section(text[i][0].substr(1));

			locationCounter = 0;

		// .section <ime_sekcije> [, "<flegovi>"]
		} else if (text[i][0] == ".section") {
			if (section->name != "")
				sections.push_back(section);

			section = new Section(text[i][1].substr(1));
			locationCounter = 0;

		// .end
		} else if (text[i][0] == ".end") {
			if (section->name != "")
				sections.push_back(section);

			locationCounter = 0;
			break;

		// .byte, .word
		} else if (text[i][0] == ".byte" || text[i][0] == ".word") {

			if (section->name == "bss") {
				section->instruction.push_back(0);

				if (text[i][0] == ".byte")
					locationCounter += 1;
				else {	// .word 
					section->instruction.push_back(0);
					locationCounter += 2;
				}

			} else {
				for (unsigned int k = 1; k < text[i].size(); k++) {
					if (isNumber(text[i][k])) {
						int value = stoi(text[i][k]);
						section->instruction.push_back((uint8_t)(255 & value));	// 0xFF & value, da bi uzeli samo 8bita
						locationCounter++;

						if (text[i][0] == ".word") {
							section->instruction.push_back((uint8_t)((65280 & value) >> 8)); // 0xFF00 & value, da bi uzeli samo visih 8bita
							locationCounter++;
						}	

					} else
						throw "Ne ispravan simbol u .byte/.word!";
				}
			}

		//.align 
		} else if (text[i][0] == ".align") {
			int align = stoi(text[i][1]);

			for (int k = 0; k < align; k++) {
				if (locationCounter % align == 0)
					break;

				section->instruction.push_back(0);
				locationCounter++;
			}

		// .skip
		} else if (text[i][0] == ".skip") {

			int skip = stoi(text[i][1]);

			for (int k = 0; k < skip; k++)
				section->instruction.push_back(0);
				
			locationCounter += skip;
			
		// instruction
		} else if (isInstruction(text[i][0])) {

			section->instruction.push_back(instructionID(text[i][0]));

			locationCounter++;

			int operands = numberOfOperands(text[i][0]);

			if (operands == 1)
				instruction(text[i], symbolTable, section, locationCounter, text[i][1], 0);

			else if (operands == 2) {
				instruction(text[i], symbolTable, section, locationCounter, text[i][1], sizeOfOperand(text[i][0]));
				instruction(text[i], symbolTable, section, locationCounter, text[i][2], 0);
			}
		}
	}

	return sections;
}


int main(int argc, char* argv[]) {

	try {
		if (argc < 3)
			throw "Fali argument!";
		else if (argc > 3)
			throw "Imate viska argumenata!";

		string in = argv[1];
		string out = argv[2];

		ifstream input;
		input.open(in);

		if (!input.is_open())
			throw "Ulazni fajl ne moze da se otvori!";

		vector<vector<string>> text;

		int i = -1;
		for (char line[256]; input.getline(line, 256);) {

			// comment?
			int j = 0;
			while (j < 256 && (isblank(line[j]) || line[j] == '\t'))
				j++;

			if (j < 256 && line[j] == '@')
				continue;
			// END comment?

			char* clearLine = new char[256];
			char* word = new char[256];

			clearLine = strtok(line, "@");
			word = strtok(clearLine, " ,	\r");

			if (word != NULL) {
				text.push_back(vector<string>());
				i++;
			}

			// ako je labela predji u novi red u text
			if (word != NULL && word[strlen(word) - 1] == ':') {
				text[i].push_back(word);
				word = strtok(NULL, " ,	\r");
				if (word == NULL)
					continue;
				text.push_back(vector<string>());
				i++;
			}

			while (word != NULL) {
				text[i].push_back(word);
				word = strtok(NULL, " ,	\r");
			}
		}

		input.close();

		/* for (unsigned int i = 0; i < text.size(); i++) {
			for (unsigned int j = 0; j < text[i].size(); j++)
				cout << text[i][j];
			cout << "\n";
		} */

		ofstream output;
		output.open(out);

		if (!output.is_open())
			throw "Izlazni fajl ne moze da se otvori!";

		SymbolTable symbolTable;

		firstPass(text, symbolTable);

		output << symbolTable;

		vector<Section*> sections = secondPass(text, symbolTable);

		for (unsigned int i = 0; i < sections.size(); i++)
			output << *sections[i];

		output.close();

	} catch (const char* exception) {
		cout << exception << endl; 
	}
}