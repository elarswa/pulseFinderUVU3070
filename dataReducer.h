#ifndef _dataReducer_h_
#define _dataReducer_h_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>

struct iniParams {
	iniParams() : vt(0), width(0), pulse_delta(0), drop_ratio(0.0), below_drop_ratio(0) {}
	int vt; // voltage threshold
	int width;
	int pulse_delta;
	double drop_ratio;
	int below_drop_ratio;
};

struct Pulse {
	Pulse(size_t i) : index(i) {}
	size_t index;
};

class DataReducer {

public:
	DataReducer(iniParams param, std::string file);

	void findPulses(std::ifstream &ifs);

private:
	iniParams ip;
	std::string fileName;
	std::vector<int> normal;
	std::vector<int> smooth;
	std::vector<Pulse> pList;
	
	int getNext(std::ifstream &ifs);
	void copyToSmooth();
	void smoothIt(std::vector<int>::iterator& i);
	void getPulses(std::vector<int>::iterator start, std::vector<int>::iterator end, std::function<bool(std::vector<int>::iterator)>);
	void myForEach(std::vector<int>::iterator start, std::vector<int>::iterator end);
	void testPiggyback();
	
	void piggyback(size_t index);
	void pulse(Pulse p, int area);
	//int count_if_below_dr(std::vector<int>::iterator start, std::vector<int>::iterator end, std::function<bool(std::vector<int>::iterator&)> fn);
};


#endif // !_dataReducer_h_