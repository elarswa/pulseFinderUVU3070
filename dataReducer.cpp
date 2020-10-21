#include "dataReducer.h"

void DataReducer::piggyback(size_t index) {
	std::cout << "Found piggyback at " << index << std::endl;
}

void DataReducer::pulse(Pulse p, int area) {
	std::cout << p.index << " (" << area << ")" << std::endl;
}

void DataReducer::testPiggyback() {

	for (auto i = pList.begin(); i != pList.end(); ++i) {
		std::vector<int>::iterator start = smooth.begin() + (*i).index;
		int peak = *start;
		int peakInd = 0;
		while (start != smooth.begin() + (*i).index + ip.pulse_delta && start != smooth.begin() + (*(i + 1)).index) {
			if (*start > peak) {
				peak = *start;
				peakInd = start - smooth.begin();
			}
			++start;
		}
		start = smooth.begin() + peakInd; // start at peak of first pulse

		int startNextPulseIndex = (*(i + 1)).index;
		int delta = (startNextPulseIndex - peakInd < ip.pulse_delta) ? startNextPulseIndex - peakInd : ip.pulse_delta;
		if (i != pList.end() - 1 && count_if_below_dr(start, start + delta,[this, &peak](std::vector<int>::iterator iter) {return (*iter < (ip.drop_ratio* peak)); }) > ip.below_drop_ratio) {
			piggyback((*i).index);
		}
		else {
			int startNextPulseIndex = (*(i + 1)).index;
			int endWidthIndex = (*i).index + ip.width;
			std::vector<int>::iterator end = (startNextPulseIndex < endWidthIndex && i != pList.end() - 1) ? smooth.begin() + startNextPulseIndex : smooth.begin() + endWidthIndex;
			int area = std::accumulate(smooth.begin() + (*i).index, end, 0);

			pulse(*i, area);
		}
	}
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

	if (pList.size() > 0) {
	std::cout << fileName.substr(2) << ":" << std::endl;
	}
	testPiggyback();
}

//[this](auto i) { return (this->vt < *(i + 2) - *i); }
int DataReducer::getNext(std::ifstream& ifs)
{
	std::ostringstream sBuilder;
	char c = ifs.get();
	while (ifs.good() && c != '\n' && c != '\r') {
		if (std::isdigit(c) || c == '-' || c == '\r' || c == '\n' || std::isspace(c)) {
			if (!std::isspace(c)) {
			sBuilder << c;
			c = ifs.get();
			}
		} 
		else {
			throw std::runtime_error("Invalid file: " + fileName.substr(2));
		}
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
			pList.push_back(Pulse(start - smooth.begin()));
			do { // increment after pulse till data starts dropping
				++start;
			} while (*start <= *(start + 1));
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
