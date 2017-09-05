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
    pair, 
    unpair,
    sub_symbol,
    add_symbol,
    mult_symbol,
    div_symbol,
    left_symbol,
    right_symbol,
    error, 
    ignored
} ;


int main() 
{
    using namespace thodd::lang ;

    constexpr auto digit        = terminal { calc::digit, chr('0') - chr('9') } ;
    constexpr auto sub_symbol   = terminal { calc::sub_symbol, chr('-') } ;
    constexpr auto add_symbol   = terminal { calc::add_symbol, chr('+') } ;
    constexpr auto mult_symbol  = terminal { calc::mult_symbol, chr('*') } ;
    constexpr auto div_symbol   = terminal { calc::div_symbol, chr('/') } ;
    constexpr auto left_symbol  = terminal { calc::left_symbol, chr('(') } ;
    constexpr auto right_symbol = terminal { calc::right_symbol, chr(')') } ;

    constexpr auto error = error_terminal { calc::error } ;
    constexpr auto ignored = ignored_terminal { calc::ignored, chr(' ') } ;

    std::string_view __expression = "*    45" ;
    auto && __tokens = build_tokens (
                        __expression.begin(), 
                        __expression.end(), 
                        error, ignored,
                        digit , 
                        left_symbol , 
                        right_symbol) ;

    for(auto const & __token : __tokens)
        std::cout << (int) __token.id << std::endl ;

    
    // struct number       : non_terminal<number> {} ; 
    // struct parens       : non_terminal<parens> {} ; 
    // struct factor       : non_terminal<factor> {} ; 
    // struct term         : non_terminal<term> {} ; 
    // struct expression   : non_terminal<expression> {} ;
    
    // constexpr auto calc_grammar = 
    // grammar (
    //     expression{}, 
    //     number{}     <= (+digit) ,
    //     parens{}     <= (left_symbol > expression{} > right_symbol) ,
    //     factor{}     <= (number{} | parens{}) ,
    //     term{}       <= (factor{} > *((mult_symbol | div_symbol) > factor{})) ,
    //     expression{} <= (term{} > *((add_symbol | sub_symbol) > term{}))) ;


    
    
}
