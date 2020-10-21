#include <iostream>
#include <filesystem>
#include <fstream>
#include <string> 
#include "iniParse.h"
#include "dataReducer.h"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

	if (argc == 2) {
		// read ini
		ifstream iniFile(argv[1]);
		iniParams ip;
		if (iniFile.is_open()) {
			//cout << "iniFile is open for input" << endl;
			try {
				IniParse IP;
				ip = IP.parseIni(iniFile, argv[1]);
				
			}
			catch (std::runtime_error& e) {
				cout << e.what() << endl;
				return 1;
			}
		}
		else {
			cout << "Failed to open INI file. Terminating program." << endl;
			return 1;
		}

		//process data
		string path = "./";
		for (const auto & entry : fs::directory_iterator(path)) {
			//cout << entry.path() << endl;
			string path = entry.path().string();
			if (path.length() > 4 && path.substr(path.length() - 4) == ".dat") {
				cout << path << ":" << endl;
				ifstream file(path);
				if (file.is_open()) {
				//dr.findPulses(); called on each file in directory with .dat ending
					DataReducer dr(ip, path);
					dr.findPulses(file);
					cout << endl;
				}
				else {
					cout << "Failed to open" + path << endl;
				}
			}
		}
	}
	else {
		cout << "please specify a single INI file" << endl;
	}

	return 0;
}

