CXX = g++
CXXFLAGS = -O2 -Wall

SRC = main.cpp keccak.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = scanner

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
