
CXXFLAGS=-std=c++17
LDFLAGS=-lgtest -lpthread

test_groceries.exe: groceries.cc test_groceries.cpp
	$(CXX) $(CXXFLAGS) test_groceries.cpp $(LDFLAGS) -o $@


test: test_groceries.exe
	./test_groceries.exe

float_monoid: float_monoid.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test_float: float_monoid
	./float_monoid