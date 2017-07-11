#include <iostream>

#include <thodd/lang.hpp>

int main() 
{
    using namespace thodd::regex ;

    constexpr auto __reg = ! (chr('2') - chr('0')) | chr('7') > chr('0') ;
} 