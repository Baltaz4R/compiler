#ifndef SECTION_H_
#define SECTION_H_

#include <string>
#include <vector>

#include "Relocation.h"

using namespace std;

struct Section {
	string name;
	vector<uint8_t> instruction;
	vector<Realocation*> relocation;

	Section(string Name) : name(Name) {}
	friend ostream& operator<<(ostream& stream, const Section& section);
	~Section();
};

#endif
