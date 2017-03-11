#include <iostream>
//#include "any.h"
#include "my_any.h"


struct alignas(128) big {
    int field;

    big(int i) {
        field = i;
    }

    big(const big& other) {
        field = other.field;
    }

    big(big&& other) {
        field = other.field;
    }
    int operator()(size_t qw){

        return field;
    }

};

int main() {
    // 1
    my_any a(3.1415);
    std::cout << any_cast<double>(a) << '\n';

    // 2
    my_any b(4.20);
    b = 1;
    std::cout << any_cast<int>(b) << '\n';

    a = 4;

    my_any c((long long int) 10);
    std::cout<< any_cast<long long int>(c);
    // 3
    /*big d(220);
    my_any c(d);
    std::cout << any_cast<big>(c).field << '\n';*/


    // 4
    for (int i = 0; i < 100000; i++) {
        big cc(i);
        my_any temp1(cc);
        my_any temp2(temp1);
        std::cout << any_cast<big>(temp2).field << '\n';

    }
    return 0;
}