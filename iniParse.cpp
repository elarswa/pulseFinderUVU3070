#include "iniParse.h"


iniParams IniParse::parseIni(std::ifstream& ifs, std::string fileName) {
	this->fileName = fileName;
	iniParams ip;
	std::bitset<5> b;
	while (ifs.good()) {
		if (ifs.peek() == '#' || ifs.peek() == '\n') { // comment or unfinished line read
			while (ifs.peek() != '\n' && ifs.good()) {
				ifs.get();
			}
			ifs.get(); // consume \n
		}
		else if (ifs.peek() == 'v') {
			std::string vt = readNext(ifs);
			try {
				evaluateKey(vt, b, 0, ifs, "vt", ip.vt);
			}
			catch (std::runtime_error& e) {
				throw e; // throw again to outside caller
			}
		}
		else if (ifs.peek() == 'w') {
			std::string width = readNext(ifs);
			try {
				evaluateKey(width , b, 1, ifs, "width", ip.width);
			}
			catch (std::runtime_error& e) {
				throw e; // throw again to outside caller
			}
		}
		else if (ifs.peek() == 'p') {
			std::string pulse_delta = readNext(ifs);
			try {
				evaluateKey(pulse_delta, b, 2, ifs, "pulse_delta", ip.pulse_delta);
			}
			catch (std::runtime_error& e) {
				throw e; // throw again to outside caller
			}
		}
		else if (ifs.peek() == 'd') {
			std::string drop_ratio = readNext(ifs);
			try {
				evaluateKey(drop_ratio, b, 3, ifs, "drop_ratio", ip.drop_ratio);
			}
			catch (std::runtime_error& e) {
				throw e; // throw again to outside caller
			}
		}
		else if (ifs.peek() == 'b') {
			std::string below_drop_ratio = readNext(ifs);
			try {
				evaluateKey(below_drop_ratio, b, 4, ifs, "below_drop_ratio", ip.below_drop_ratio);
			}
			catch (std::runtime_error& e) {
				throw e; // throw again to outside caller
			}
		}
		else {
			std::string invalidStr = readNext(ifs);
			if (invalidStr.length() > 0) {
				throw std::runtime_error("Invalid or duplicate INI key " + invalidStr);
			}
		}
	}
	if (ifs.bad()) {
		throw std::runtime_error("Filestream failed: ending program");
	}
	if (!allSet(b)) {
		throw std::runtime_error("Invalid INI file: missing one or more keys");
	}
	return ip;
}
template <typename T>
void IniParse::evaluateKey(std::string& key, std::bitset<5>& b, size_t bitIndex, std::ifstream& ifs, std::string compareMe, T &data) {
	if (key.compare(compareMe) == 0 && !b[bitIndex]) {
		std::string val = readNext(ifs);
		if (typeid(data) == typeid(int)) {
			try {
				data = std::stoi(val);
			}
			catch (std::invalid_argument& e) {
				throw std::runtime_error("Invalid value for " + key + " in " + this->fileName);
			}
		}
		else if (typeid(data) == typeid(double)) {
			try {
				data = std::stod(val);
			}
			catch (std::invalid_argument& e) {
				throw std::runtime_error("Invalid value for " + key + " in " + this->fileName);
			}
		}
		else {
			throw std::runtime_error("Invalid type evaluation at evaluateKey"); // should not get here
		}
		b[bitIndex] = 1;
		return; // set the variable already by ref
	}
	else {
		throw std::runtime_error("Invalid or duplicate INI key: " + key);
	}
}

std::string IniParse::readNext(std::ifstream& ifs) {
	// also return if a \n is read or eof
	std::ostringstream sBuilder;
	char c = ifs.get();
	// if c is '<' we good
	while (c != '=' && ifs.good() && (std::isalnum(c) || c == '_' || c == '.')) {
		// just grab whatever it is until = or \n or eof

		sBuilder << c;
		c = ifs.get();
	}
	return sBuilder.str();
}


bool IniParse::allSet(std::bitset<5>& b) {
	for (size_t i = 0; i < 5; ++i) {
		if (!b[i]) return false;
	}
	return true;
}

