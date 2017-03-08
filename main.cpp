#include <iostream>
#include "my_any.h"

int main(){
    my_any a(2);
    std::cout<<a.cast<int>()<<'\n';
    return 0;
}