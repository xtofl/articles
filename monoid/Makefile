
CXXFLAGS=-std=c++17
LDFLAGS=-lgtest -lpthread

test_groceries.exe: groceries.cc test_groceries.cpp
	$(CXX) $(CXXFLAGS) test_groceries.cpp $(LDFLAGS) -o $@


test: test_groceries.exe
	./test_groceries.exe