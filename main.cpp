#include <iostream>
#include <filesystem>
#include <fstream>
#include <string> 
#include "iniParse.h"
#include "dataReducer.h"

/*Eric Larson 10/22/2020
I assert that this work is entirely my own. I did not work with another student/person
on it.
I actually enjoyed this project quite a bit because the output is a concrete practical use
function. I could use this to examine many different data sets if I wanted. This project 
also kicked my butt in trying to keep track of all the requirements that were briefly mentioned. 
I feel like the project spec could benefit from organizing related information.
I think I finally learned what lamdas are, and how to use them. Though I find them less clear
to read later just because it usually makes long lines of code. This project forced me to 
entirely switch over to running gdb for debugging. Though it was tedious at first, I think I will
continue to use it in the future. I even wrote some make file scripts for outputting and running
and also outputting a debug version. In the future I would like to learn some other useful
code maintenance scripts/tips. In attempting to use the algorithms library, I learned several of them
didn't behave exactly how I needed them to. Maybe there were other options, but I ended up rewriting
them in a way that worked for how I wanted to use them. Perhaps if I had more time I could come up 
with a solution that used more of std::algorithm. But as of right now, I think I only successfully used
count_if.*/

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
				ifstream file(path);
				if (file.is_open()) {
				//dr.findPulses(); called on each file in directory with .dat ending
					DataReducer dr(ip, path);
					try {
						dr.findPulses(file);
					} 
					catch (std::runtime_error& e) {
						cout << e.what() << endl;
					}
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

