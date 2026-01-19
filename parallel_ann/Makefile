CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -fopenmp

SRC = main.cpp nn.cpp
OUT = out

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
