#include <cassert>
#include <cstdio>

using Triple = float[3];

bool associative(float a, float b, float c) {
    return a + (b+c) == (a+b) + c;
}

int main() {
    Triple  triples[] = {
        {1, 2, 3},
        {.1, 20, 30},
        {1e-127, 2, 3},
        {1e-10, 2e10, 3e10},
    };
    for(auto t: triples){
        assert( associative(t[0], t[1], t[2]) );
    }
}