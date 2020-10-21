#include "dataReducer.h"

void DataReducer::piggyback(std::vector<int>::iterator i) {
	std::cout << "Found piggyback at " << i - smooth.begin() << std::endl;
}

void DataReducer::pulse(std::vector<int>::iterator i) {
	std::cout << i - smooth.begin() << " ";
}

bool DataReducer::testPiggyback(std::vector<int>::iterator i) {
	// i is start of pulse
	for (auto j = i; i < j + ip.pulse_delta; ++j) {
		if (ip.vt < *(j + 2) - *j) {
			const int pulse1 = *i;
			int numBelow = count_if_below_dr(i, j, [this, &pulse1](std::vector<int>::iterator iter) {return (*iter < (ip.drop_ratio* pulse1)); });
			if (numBelow > ip.below_drop_ratio) return true;
		}
	}
	return false;
}

int DataReducer::count_if_below_dr(std::vector<int>::iterator start, std::vector<int>::iterator end, std::function<bool(std::vector<int>::iterator&)> fn) {
	int count = 0;
	while (start != end) {
		if (fn(start)) {
			++count;
		}
		++start;
	}
	return count;
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

	this->getPulses(smooth.begin(), smooth.end(), [this](std::vector<int>::iterator i) {return (ip.vt < *(i + 2) - *i); });
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
	if (ifs.peek() == '\r') { // might not be needed
		ifs.get();
	}
	return -1 * (std::atoi(sBuilder.str().c_str()));
}



void DataReducer::copyToSmooth()
{
	for (auto i = normal.begin(); i != normal.begin() + 3; ++i) {
		smooth.push_back(*i);
	}
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
				for (int i = ip.pulse_delta; i != 0; --i) { // increment till outside the pulse_delta
					++start;
				}
			}
			else {
				pulse(start); // print, calculate area
				std::cout << "(" << std::accumulate(start, start + ip.width, 0) << ")" << std::endl;
				while (*start <= *(start + 1)) { // increment after pulse till data starts dropping
					++start;
				}
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
