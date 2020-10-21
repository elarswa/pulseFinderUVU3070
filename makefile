make:
	g++ -o algorithms -Wall -Werror --std=c++17 *.cpp
clean:
	rm algorithms
run:
	g++ -o algorithms -Wall -Werror --std=c++17 *.cpp && ./algorithms gage2scope.ini
deb:
	g++ -o algorithms -g -Wall -Werror --std=c++17 *.cpp