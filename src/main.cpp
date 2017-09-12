#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <functional>
#include <type_traits>
#include <typeinfo>


#include <cxxabi.h>


#include <thodd/lang.hpp>
#include <thodd/tuple/indexof.hpp>


template<typename T>
std::string type_name()
{
    int status;
    std::string tname = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    
    if(status == 0) {
        tname = demangled_name;
        std::free(demangled_name);
    } 

    return tname;
}

enum class
calc : int
{
    digit = 0,
    pair, // 1 
    unpair,// 2
    sub_symbol,// 3
    add_symbol,// 4
    mult_symbol,// 5
    div_symbol,// 6
    left_symbol,// 7
    right_symbol,// 8
    error, // 9
    ignored,// 10

    term, // 11
    expression,// 12 
    number, // 13
    factor,// 14
    parens// 15
} ;

/*

 expression, 
        number     <= (+digit) ,
        parens     <= (left_symbol > expression > right_symbol) ,
        factor     <= (number | parens) ,
        term       <= (factor > *(mult_symbol | div_symbol)) ,
        expression <= (term > *((add_symbol | sub_symbol) > term))

*/


int main() 
{
    using namespace thodd::lang ;

    constexpr auto digit        = term <calc::number> (+(chr<'0'> {} - chr<'9'>{}) > (~(chr<'.'>{} > +(chr<'0'> {} - chr<'9'> {})))(bounds<0, 1> {})) ;
    constexpr auto sub_symbol   = term <calc::sub_symbol> ( chr<'-'> {} ) ;
    constexpr auto add_symbol   = term <calc::add_symbol> ( chr<'+'> {} ) ;
    constexpr auto mult_symbol  = term <calc::mult_symbol> (  chr<'*'> {} ) ;
    constexpr auto div_symbol   = term <calc::div_symbol> ( chr<'/'> {} ) ;
    constexpr auto left_symbol  = term <calc::left_symbol> ( chr<'('> {} ) ;
    constexpr auto right_symbol = term <calc::right_symbol> ( chr<')'> {} ) ;

    constexpr auto error = error_term <calc::error> () ;
    constexpr auto ignored = term <calc::ignored> ( chr<' '> {} ) ;

    std::string_view __expression = "45.12" ;
    auto && __tokens = build_tokens (
                        __expression.begin(), 
                        __expression.end(), 
                        error, 
                        ignored,
                        digit , 
                        left_symbol , 
                        right_symbol) ;

    for(auto const & __token : __tokens)
        std::cout << (int) __token.id << " value : " <<std::endl ;

    constexpr auto number = non_term <calc::number> () ;
    constexpr auto expression = non_term <calc::expression> () ;
    constexpr auto parens = non_term <calc::parens> ()  ;
    constexpr auto factor = non_term <calc::factor> () ;
    constexpr auto term = non_term <calc::term> () ;
    
    constexpr auto calc_grammar = 
    grammar (
        expression, 
        number     <= (+digit) ,
        parens     <= (left_symbol > expression > right_symbol) ,
        factor     <= (number | parens) ,
        term       <= (factor > *(mult_symbol | div_symbol)) ,
        expression <= (term > *((add_symbol | sub_symbol) > term))) ;

    auto && [__matched, __it] = check (calc_grammar, __tokens.begin(), __tokens.end()) ;
    // std::cout << std::boolalpha << __matched << std::endl ;
}