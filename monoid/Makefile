
CXXFLAGS=-std=c++17
LDFLAGS=-lgtest -lpthread

HEADERS=monoid.hpp monoid_impl.hpp groceries.h groceries_monoid.hpp
OBJECTS=groceries.o test_groceries.o

$(OBJECTS): $(HEADERS) $(patsubst %.o,%.cpp,$(OBJECTS))

test_groceries.exe: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@


test: test_groceries.exe
	./test_groceries.exe

float_monoid: float_monoid.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test_float: float_monoid
	./float_monoid