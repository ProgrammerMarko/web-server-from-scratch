CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I include

SRC = $(wildcard src/*.cpp)

web_server: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@

clean:
	rm -f web_server

.PHONY: clean
