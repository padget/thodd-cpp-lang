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
    right_symbol
};


int main() 
{
    using namespace thodd::lang ;
    

    constexpr auto __digit = word{calc::digit, chr('0') - chr('9')} ;
    constexpr auto __add = word{calc::add_symbol, chr('+')} ;
    constexpr auto __sub = word{calc::sub_symbol, chr('-')} ;
    constexpr auto __mlt = word{calc::mult_symbol, chr('*')} ;
    constexpr auto __div = word{calc::div_symbol, chr('/')} ;
    constexpr auto __left = word{calc::left_symbol, chr('(')} ;
    constexpr auto __right = word{calc::right_symbol, chr(')')} ;


   
    

    struct digit        : leaf<calc::digit> {} ;
    struct sub_symbol   : leaf<calc::sub_symbol> {} ;
    struct add_symbol   : leaf<calc::add_symbol> {} ;
    struct mult_symbol  : leaf<calc::mult_symbol> {} ;
    struct div_symbol   : leaf<calc::div_symbol> {} ;
    struct left_symbol  : leaf<calc::left_symbol> {} ;
    struct right_symbol : leaf<calc::right_symbol> {} ;
    struct number       : node<number> {} ; 
    struct parens       : node<parens> {} ; 
    struct factor       : node<factor> {} ; 
    struct term         : node<term> {} ; 
    struct expression   : node<expression> {} ;

    // //constexpr auto __test = digit{} | digit{} ;

    constexpr auto calc_grammar = 
        grammar (
            expression{}, 
            number{}     <= (+digit{}) ,
            parens{}     <= (left_symbol{} > expression{} > right_symbol{}) ,
            factor{}     <= (number{} | parens{}) ,
            term{}       <= (factor{} > *((mult_symbol{} | div_symbol{}) > factor{})) ,
            expression{} <= (term{} > *((add_symbol{} | sub_symbol{}) > term{}))) ;
 

    std::string __expression = "1-1-(12)" ;
    auto && __tokens = build_tokens (
                        __expression.begin(), 
                        __expression.end(), 
                        __digit, __add, __sub, __mlt, __div, __left, __right) ;

    for(auto const & __t : __tokens)
        std::cout << (int)__t.id << std::endl ;


    // /*check (
    //     __tokens.begin(), 
    //     __tokens.end(), 
    //     calc_grammar) ;*/

    // std::cout << type_name<std::decay_t<decltype(calc_grammar)>>() << std::endl ;
    // //get_definition(calc_grammar, number{}) ; 

}
