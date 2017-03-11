#include <iostream>
#include "my_any.h"
#include "vector"


struct str {
    int a;
    int b;
    int c;
    double d;

    str() : a(10), b(20), c(30), d(40.0) {};
    str(const str& other) : a(other.a), b(other.b), c(other.c), d(other.d) {};
    str(str&& other) : a(std::move(other.a)), b(std::move(other.b)), c(std::move(other.c)), d(std::move(other.d)) {};

};

struct other_str : str {};
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

    my_any c1((long long int) 10);
    std::cout<< any_cast<long long int>(c1);
    try {
        std::cout<< any_cast< double>(c1);
    }catch (std::bad_cast& e){
        std::cout<<"OK";
    }
    // 3
    big d(220);
    my_any c(d);
    std::cout << any_cast<big>(c).field << '\n';

    my_any e(20);
    my_any e1(std::move(e));
    std::cout<<any_cast<int>(e1)<<'\n';
    std::vector<int> vectorCheck;
    vectorCheck.push_back(23);
    my_any eqwd(vectorCheck);
    std::string stringCheck = "qweqw";
    my_any dds(stringCheck);
    dds.swap(eqwd);
    std::cout<<any_cast<std::string>(eqwd)<<"\n";
    std::cout<<(any_cast<std::vector<int>>(dds))[0]<<'\n';


    // 4
    for (int i = 0; i < 500; i++) {
        big cc(i);
        my_any temp1(cc);
        my_any temp2(temp1);
        std::cout << any_cast<big>(temp2).field << '\n';

    }


    return 0;
}