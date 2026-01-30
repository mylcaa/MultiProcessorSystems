CXX = g++
CXXFLAGS = -O2 -std=c++17 -Iinc -fopenmp
OPENCV = `pkg-config --cflags --libs opencv4`

SRC_DIR  := src
SRC = main.cpp $(wildcard $(SRC_DIR)/*.cpp)
EXEC = space_invaders

all: $(EXEC)

$(EXEC):
	$(CXX) $(CXXFLAGS) $(SRC) $(OPENCV) -o $@

clean:
	rm -f $(EXEC)
