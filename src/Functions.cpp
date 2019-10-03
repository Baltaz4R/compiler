#include <sstream>
#include <iomanip>

#include "Functions.h"

using namespace std;

bool isNumber(string number) {
	for (unsigned int j = 0; j < number.length(); j++)
		if (!isdigit(number.at(j)))
			return false;

	return true;
}


string decimalToHexadecimal(int decimal, char size) {
	if (size != 'b' && size != 'w')
		throw "Must be w or b!";

	if (decimal == 0)
		if (size == 'b')
			return "0x00";
		else
			return "0x0000";

	stringstream stream;
	if (size == 'b')
		stream << "0x" << setfill('0') << setw(2) << hex << decimal;
	else
		stream << "0x" << setfill('0') << setw(4) << hex << decimal;

	string hexadecimal;
	hexadecimal = stream.str();
	stream.clear();

	return hexadecimal;
}


int hexadecimalTodecimal(string hexadecimal) {

	if (hexadecimal.length() < 3 && hexadecimal.at(0) != '0' && hexadecimal.at(1) != 'x')
		return -1;

	int base = 1;
	int value = 0;
	for (int i = hexadecimal.length() - 1; i > 1; i--) {
		switch (hexadecimal.at(i)) {
		case '0': break;
		case '1': value += 1 * base; break;
		case '2': value += 2 * base; break;
		case '3': value += 3 * base; break;
		case '4': value += 4 * base; break;
		case '5': value += 5 * base; break;
		case '6': value += 6 * base; break;
		case '7': value += 7 * base; break;
		case '8': value += 8 * base; break;
		case '9': value += 9 * base; break;
		case 'A': case 'a': value += 10 * base; break;
		case 'B': case 'b': value += 11 * base; break;
		case 'C': case 'c': value += 12 * base; break;
		case 'D': case 'd': value += 13 * base; break;
		case 'E': case 'e': value += 14 * base; break;
		case 'F': case 'f': value += 15 * base; break;
		default: return -1;
		}

		base *= 16;
	}
	return value;
}