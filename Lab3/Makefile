CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = lab3
SOURCES = Lab3.cpp
OBJECTS = $(SOURCES:.cpp=.o)
all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f $(TARGET) $(OBJECTS)
.PHONY: all clean
