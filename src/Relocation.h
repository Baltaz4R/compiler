#ifndef RELOCATION_H_
#define RELOCATION_H_

#include <string>

using namespace std;

struct Realocation {
	int number;
	int offset;
	string type;

	Realocation(int Number, int Offset, string Type) : number(Number), offset(Offset), type(Type) {}
};

#endif
