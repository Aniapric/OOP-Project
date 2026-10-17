# compiler settings
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = restaurant
SRC = restaurant.cpp

# default build target
all: $(TARGET)

# compile target executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# run application
run: $(TARGET)
	./$(TARGET)

# remove build artifacts
clean:
	rm -f $(TARGET) *.out *.o

.PHONY: all run clean
