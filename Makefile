CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -fopenmp
TARGET = sim_regla110
OBJS = main.o TuringMachine.o Rule110.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean