#include <iostream>
#include "my_any.h"
#include "vector"


struct str {
    int a;
    int b;
    int c;
    double d;

    str() : a(10), b(20), c(30), d(40.0) {};

    str(const str &other) : a(other.a), b(other.b), c(other.c), d(other.d) {};

    str(str &&other) : a(std::move(other.a)), b(std::move(other.b)), c(std::move(other.c)), d(std::move(other.d)) {};

};

struct other_str : str {
};

struct alignas(128) big {
    int field;

    big(int i) {
        field = i;
    }

    big(const big &other) {
        field = other.field;
    }

    big(big &&other) {
        field = other.field;
    }

    int operator()(size_t qw) {

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
    std::cout << any_cast<long long int>(c1);
    try {
        std::cout << any_cast<double>(c1);
    } catch (std::bad_cast &e) {
        std::cout << "OK";
    }
    // 3
    big d(220);
    my_any c(d);
    std::cout << any_cast<big>(c).field << '\n';

    my_any e(20);
    my_any e1(std::move(e));
    std::cout << any_cast<int>(e1) << '\n';

    std::vector<int> vectorCheck;
    vectorCheck.push_back(23);
    my_any eqwd(vectorCheck);
    std::string stringCheck = "qweqw";
    my_any dds(stringCheck);
    dds.swap(eqwd);
    std::cout << any_cast<std::string>(eqwd) << "\n";
    std::cout << (any_cast<std::vector<int>>(dds))[0] << '\n';

    std::vector<int> asa;
    int asb = 73;
    asa.push_back(68);

    my_any sa(asa), sb(asb);
    sa.swap(sb);
    std::cout << any_cast<std::vector<int>>(sb)[0] << '\n';
    std::cout << any_cast<int>(sa) << '\n';

    double smalla = 1234.0;
    int smallb(1);

    my_any SmallA(smalla), SmallB(smallb);
    SmallA.swap(SmallB);
    std::cout << any_cast<double>(SmallB) << '\n';
    std::cout << any_cast<int>(SmallA) << '\n';

    my_any self(111110);
    self.swap(self);
    std::cout<<any_cast<int>(self)<<'\n';

    // 4
    for (int i = 0; i < 1000; i++) {
         big cc(i);
         my_any temp1(cc);
         my_any temp2(temp1);
         std::cout << any_cast<big>(temp2).field << '\n';

     }


    int x = 10;
    const int cx = 10;
    str y;
    const str cy;


    my_any a0;

    my_any a_T_small0(x);
    my_any a_T_small1(cx);
    my_any a_T_small2(7);

    my_any a_T_big0(y);
    my_any a_T_big1(cy);
    my_any a_T_big2(std::move(y));


    my_any a_copy(a0);

    const my_any a_copy_small0(a_T_small0);
    my_any a_copy_small1(a_copy_small0);
    my_any a_copy_small2(std::move(a_copy_small1));

    const my_any a_copy_big0(a_T_big0);
    my_any a_copy_big1(a_copy_big0);
    my_any a_copy_big2(std::move(a_copy_big1));


    a_T_small0 = a_T_small1;
    a_T_small2 = a_T_big2;
    a_copy_big2 = a_copy_small2;
    a_T_big1 = a_T_big0;


    std::cout << "good casts (small): ";

    std::cout << *any_cast<int>(&a_T_small0) << " ";
    std::cout << *any_cast<int>(&a_copy_small0) << " ";
    std::cout << any_cast<int>(a_T_small0) << " ";
    std::cout << any_cast<int>(a_copy_small0) << " ";
    std::cout << any_cast<int>(std::move(a_T_small0)) << " ";

    std::cout << "\ngood casts (big): ";

    std::cout << any_cast<str>(&a_T_big0)->a << " ";
    std::cout << any_cast<str>(&a_copy_big0)->a << " ";
    std::cout << any_cast<str>(a_T_big0).a << " ";
    std::cout << any_cast<str>(a_copy_big0).a << " ";
    std::cout << any_cast<str>(std::move(a_T_big0)).a << " ";


    std::cout << "\nbad casts: ";

        std::cout << (any_cast<double>(&a_T_small0) == nullptr) << " ";

        std::cout << (any_cast<other_str>(&a_copy_big0) == nullptr) << " ";

    try {
        any_cast<other_str>(a_T_big0);
    } catch (std::bad_cast &e) {
        std::cout << "1 ";
    }
    try {
        any_cast<double>(a_copy_small0);
    } catch (std::bad_cast &e) {
        std::cout << "1 ";
    }
    try {
        any_cast<int>(std::move(a0));
    } catch (std::bad_cast &e) {
        std::cout << "1 ";
    }
    return 0;
}