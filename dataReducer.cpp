#include "dataReducer.h"

void DataReducer::piggyback(std::vector<int>::iterator i) {
	std::cout << "Found piggyback at " << i - smooth.begin() << std::endl;
}

void DataReducer::pulse(std::vector<int>::iterator i) {
	std::cout << i - smooth.begin() << " ";
}

bool DataReducer::testPiggyback(std::vector<int>::iterator i) {
	for (int j = ip.pulse_delta; j > 0; --j) { // start at end of pulse delta
		if (ip.vt < *((i + j) + 2) - *(i + j)) { // if another pulse is inside pulse_delta
			int count_below_drop_ratio = 0;
			for (int k = 1; k < j; ++k) { // j is the start of the second pulse
				if (*(i + k) < (ip.drop_ratio * (*i))) {// if point of interest is less than first pulse times the drop_ratio
					++count_below_drop_ratio;
				}
			}
			if (count_below_drop_ratio > ip.below_drop_ratio) return true;
		}

	}
	return false;
}


DataReducer::DataReducer(iniParams param, std::string file)
{
	ip = param;
	fileName = file;
}

void DataReducer::findPulses(std::ifstream& ifs)
{
	//read in file One time
	while (ifs.good()) {
		normal.push_back(getNext(ifs));
	}
	// normal is full of the negated data, no smoothing
	this->copyToSmooth();
	//Find the pulses

	this->getPulses(smooth.begin(), smooth.end(), [this](std::vector<int>::iterator i) {return (ip.vt < *(i + 2) - *i);});
}

//[this](auto i) { return (this->vt < *(i + 2) - *i); }
int DataReducer::getNext(std::ifstream& ifs)
{
	std::ostringstream sBuilder;
	char c = ifs.get();
	while (ifs.good() && c != '\n' && c != '\r') {
		sBuilder << c;
		c = ifs.get();
	}
	if (ifs.peek() == '\n') {
		ifs.get();
	}
	if (ifs.peek() == '\r') { // might not be neededg
		ifs.get();
	}
	return -1 * (std::atoi(sBuilder.str().c_str()));
}



void DataReducer::copyToSmooth()
{
	for (auto i = normal.begin(); i != normal.begin() + 3; ++i) {
		smooth.push_back(*i);
	}
	//smooth.resize(3);
	//std::copy(normal.begin(), normal.begin() + 2, smooth); compiler doesn't like this for some reason
	//smooth.resize(normal.size());
	//std::copy(normal.begin(), normal.end(), smooth); // copy whole range
	myForEach(normal.begin() + 3, normal.end() - 4);
	for (auto i = normal.end() - 4; i != normal.end(); ++i) {
		smooth.push_back(*i);
	}
	/*std::ofstream ofs("smooth.txt", std::ios_base::trunc); // print to file to check
	auto i = smooth.begin();
	while (i != smooth.end()) {
		ofs << *i << '\n';
		++i;
	}
	ofs.close();*/
}


void DataReducer::smoothIt(std::vector<int>::iterator& iter) {
	int temp = (iter[-3] + 2 * iter[-2] + 3 * iter[-1] + 3 * iter[0] + 3 * iter[1] + 2 * iter[2] + iter[3]) / 15;
	smooth.push_back(temp);
}

void DataReducer::getPulses(std::vector<int>::iterator start, std::vector<int>::iterator end, std::function<bool(std::vector<int>::iterator)> testPulse)
{
	while (start != end) {
		if (testPulse(start)) {
			if (testPiggyback(start)) {
				piggyback(start); // just prints what i is
			}
			else {
				pulse(start); // print, calculate area
				std::cout << "(" << std::accumulate(start, start + ip.width, 0) << ")" << std::endl;
			}
			while (*start <= *(start + 1)) {
				++start;
			}
		}
		++start;
	}
}


void DataReducer::myForEach(std::vector<int>::iterator start, std::vector<int>::iterator end)
{
	{
		while (start != end) {
			smoothIt(start);
			++start;
		}
	}
}
