#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <functional>

#include <thodd/tuple/algorithm.hpp>
#include <thodd/lang.hpp>

enum class test { foo, bar, fii } ;

enum class calc { integer, add_op, mult } ;

enum class lexword {digit, point, float_, exp} ;

constexpr auto 
predict (
    thodd::syntax::word<auto, auto> const & ... __word)
{
    __word.reg
} 

constexpr auto lex = 
    [] ( auto&& __begin, 
         auto&& __end, 
         thodd::syntax::word<auto, auto> const & ... __word) 
    {

    } ;

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
    
    std::tie(__begin, __end) = std::tuple(__input2.begin(), __input2.end()) ;

    std::cout << std::boolalpha 
              << matches(chr('2') > chr('3') > chr('1'), __begin, __end)
              << std::endl ;

    std::string __input3 {"11111"} ;
    __begin = __input3.begin() ;
    __end = __input3.end() ;

    std::cout << std::boolalpha
              << matches((*chr('1'))(2, 5), __begin, __end)
              << std::endl ;

    std::string __input4 { "my_name_is_bryan..." } ;
    __begin = __input4.begin() ;
    __end = __input4.end() ;

    constexpr auto __odod = +(chr('a') - chr('z') | chr('_')) > chr('.') > chr('.') > chr('.');
    std::cout << std::boolalpha 
              << matches(
                  __odod, 
                  __begin, 
                  __end) 
              << std::endl ;


    namespace syntax = thodd::syntax ;
    
    constexpr auto __23 = syntax::wrd(__odod, test::fii) ;
    constexpr auto __or = __23 | __23 | __23 ;
    constexpr auto __and = __23 > __23 > __23 ;
    constexpr auto __some = *__23 ;
    constexpr auto __comb = +__and | __and ;

    __begin = __input4.begin() ;
    __end = __input4.end() ;
    std::list<syntax::token<test, decltype(__begin)>> __tokens ;
    
    read(__23, __begin, __end, __tokens) ;
    
    if (!__tokens.empty())
        for (auto&& __token : __tokens)
            for (auto&& __item : __token)
                std::cout << __item ;


    auto __tuple = std::tuple(1, 2, 3) ;
    auto __accumulator = [] (auto&& __state, auto&& __step) {return __state + __step ; } ;
    auto __func = [](auto&& __item) -> decltype(auto) { return __item ; } ;

    ::thodd::tuple::iterate(
        [] (auto && __item) -> auto&& {return (std::cout << __item) ; }, 
        __tuple) ;

    ::thodd::tuple::iterate_if(
        [] (auto && __item) { return __item % 2 == 0 ; } ,  
        [] (auto && __item) { std::cout << __item ; }    ,
        std::tuple(1, 2, 3, 4, 5, 6, 7, 8, 9)) ;
    
    std::cout << ::thodd::tuple::accumulate(__func, __accumulator, 0u, __tuple) << std::endl ;
    std::cout << ::thodd::tuple::accumulate_if([](auto && __state, auto && __item) { return __item % 2 == 0 ; } , __func, __accumulator, 0u, __tuple) << std::endl ;

    std::string __input5 {"my_name_is_bryan..."} ;
    __begin = __input5.begin() ;
    __end = __input5.end() ;
    std::list<syntax::token<test, decltype(__begin)>> __tokensbad ;
    
    
    read(__23, __begin, __end, __tokensbad) ;
    
    if (!__tokensbad.empty())
    {
        for (auto&& __token : __tokensbad)
            if (!__token.invalid())
                for (auto&& __item : __token)
                    std::cout << __item ;
            else std::cout << "token invalid" << std::endl ;
    } 

    std::cout << "next test" << std::endl ;

    constexpr auto __and23 = __23 > __23 > __23 ;

    std::string __input6 {"my_name_is_bryan...my_name_is_bryan...my_name_is_bryan.."} ;
    __begin = __input6.begin() ;
    __end = __input6.end() ;
    std::list<syntax::token<test, decltype(__begin)>> __tokensgood ;

    read(__and23, __begin, __end, __tokensgood) ;

    for (auto&& __token : __tokensgood)
        if (!__token.invalid())
            for (auto&& __item : __token)
                std::cout << __item ;
        else std::cout << "token invalid" << std::endl ;

    

    lex(__begin, __end, 
        syntax::wrd(chr('0') - chr('9')                                                  , lexword::digit), 
        syntax::wrd(chr('.')                                                             , lexword::point), 
        syntax::wrd(+(chr('0') - chr('9')) > (~(chr('.') > +(chr('0') - chr('9'))))(0, 1), lexword::float_)) ;

    
}