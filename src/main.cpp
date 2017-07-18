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

    std::string __input3 {"11111"} ;
    __begin = __input3.begin() ;
    __end = __input3.end() ;

    std::cout << std::boolalpha
              << matches((*chr('1'))(2, 5), __begin, __end)
              << std::endl ;
        
    std::string __input4 { "my_name_is_bryan." } ;
    __begin = __input4.begin() ;
    __end = __input4.end() ;

    constexpr auto __odod = +(chr('a') - chr('z') | chr('_')) | chr('.') ;
    std::cout << std::boolalpha 
              << matches(
                  __odod, 
                  __begin, 
                  __end) 
              << std::endl ;


    namespace stx = thodd::syntax ;
    
    constexpr auto __23 = stx::wrd(chr('2') > chr('3')) ;
    constexpr auto __or = __23 | __23 | __23 ;
    constexpr auto __and = __23 > __23 > __23 ;
    constexpr auto __some = *__23 ;
    constexpr auto __comb = +__and | __and ;
    

} 