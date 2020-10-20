#ifndef _iniParse_h_
#define _iniParse_h_

#include <stdexcept>
#include <bitset>
#include <string>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include "dataReducer.h"

class IniParse {
public:
	iniParams parseIni(std::ifstream& ifs, std::string fileName);

private:
	std::string fileName;
	bool allSet(std::bitset<5>& b);
	template <typename T>
	void evaluateKey(std::string& key, std::bitset<5>& b, size_t bitIndex, std::ifstream& ifs, std::string compareMe, T &data);
	std::string readNext(std::ifstream& ifs);
};

#endif // !_iniParse_h_

