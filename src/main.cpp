#include <iostream>
#include <string>
#include <thodd/lang.hpp>

int main() 
{
    using namespace thodd::regex ;

    std::string __input("1") ;
    auto __begin = __input.begin() ;
    auto __end = __input.end() ;

    std::cout << std::boolalpha 
              << matches(chr('0') - chr('9'), __begin, __end)
              << std::endl ;

    __begin = __input.begin() ;

    std::cout << std::boolalpha 
              << matches(!chr('2'), __begin, __end)
              << std::endl ;

    __begin = __input.begin() ;

    std::cout << std::boolalpha 
              << matches(chr('2') | chr('3') | chr('1'), __begin, __end)
              << std::endl ;

    std::string __input2 = "231" ;
    
    std::tie(__begin, __end) = std::make_tuple(__input2.begin(), __input2.end()) ;

    std::cout << std::boolalpha 
              << matches(chr('2') > chr('3') > chr('1'), __begin, __end)
              << std::endl ;


} 