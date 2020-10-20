#ifndef _dataReducer_h_
#define _dataReducer_h_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <functional>

struct iniParams {
	iniParams() : vt(0), width(0), pulse_delta(0), drop_ratio(0.0), below_drop_ratio(0) {}
	int vt; // voltage threshold
	int width;
	int pulse_delta;
	double drop_ratio;
	int below_drop_ratio;
};

class DataReducer {

public:
	DataReducer(iniParams param);

	void findPulses(std::ifstream &ifs);

private:
	iniParams ip;
	std::vector<int> normal;
	std::vector<int> smooth;
	int getNext(std::ifstream &ifs);
	int negate(int);

	void copyToSmooth();
	
	void smoothIt(std::vector<int>::iterator& i);
	
	void getPulses(std::vector<int>::iterator start, std::vector<int>::iterator end, std::function<bool(std::vector<int>::iterator)>);

	void myForEach(std::vector<int>::iterator start, std::vector<int>::iterator end);
	bool testPiggyback(std::vector<int>::iterator i);
	
	void piggyback(std::vector<int>::iterator i);
	void pulse(std::vector<int>::iterator i);
};


#endif // !_dataReducer_h_