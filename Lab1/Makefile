CUDDDIR = /home/ivesliu/EDA_2024_Spring/cudd-3.0.0
CUDD_INCLUDE = $(CUDDDIR)/cudd
UTIL_INCLUDE = $(CUDDDIR)/util
CUDD_LIB = $(CUDDDIR)/cudd/.libs/libcudd.a

CXX = g++
CXXFLAGS = -I $(CUDD_INCLUDE) -I $(UTIL_INCLUDE) -lm -O3

EXEC = Lab1

SRCS = Lab1.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(CUDD_LIB) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)