#include <iostream>
#include <iomanip>

#include "Functions.h"
#include "Section.h"

using namespace std;

Section::~Section() {
	instruction.clear();
	relocation.clear();
}

ostream& operator<<(ostream& stream, const Section& section) {

	stream << "-------------------------------------------------" << endl;

	if (section.relocation.size() > 0) {
		stream << "***********" << endl;
		stream << section.name << " relocation table" << endl;

		for (unsigned int j = 0; j < section.relocation.size(); j++) {
			stream << decimalToHexadecimal(section.relocation[j]->offset, 'w') << " ";
			stream << setw(10) << section.relocation[j]->type << " ";
			stream << setw(3) << section.relocation[j]->number << endl;
		}
		stream << "***********" << endl;
	}
	stream << section.name << endl;

	for (unsigned int j = 0; j < section.instruction.size(); j++) {
		stream << decimalToHexadecimal(section.instruction[j], 'b') << " ";

		if ((j + 1) % 10 == 0)
			stream << endl;
	}
	stream << endl;
	stream << "-------------------------------------------------" << endl;

	return stream;
}