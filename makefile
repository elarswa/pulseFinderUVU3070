make:
	g++ -o algorithms -Wall -Werror --std=c++17 *.cpp
clean:
	rm algorithms