#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <functional>
#include <type_traits>
#include <typeinfo>

#include <thodd/lang.hpp>
#include <cxxabi.h>


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
calc
{
    digit,
    sub_symbol,
    add_symbol,
    mult_symbol,
    div_symbol,
    left_symbol,
    right_symbol
};

int main() 
{
    using namespace thodd::lang::syntax ;

    struct digit        : leaf<calc::digit> {} ;
    struct sub_symbol   : leaf<calc::sub_symbol> {} ;
    struct add_symbol   : leaf<calc::add_symbol> {} ;
    struct mult_symbol  : leaf<calc::mult_symbol> {} ;
    struct div_symbol   : leaf<calc::div_symbol> {} ;
    struct left_symbol  : leaf<calc::left_symbol> {} ;
    struct right_symbol : leaf<calc::right_symbol> {} ;
    struct number       : item {} ; 
    struct parens       : item {} ; 
    struct factor       : item {} ; 
    struct term         : item {} ; 
    struct expression   : item {} ;


    constexpr auto calc_grammar = 
        grammar(
            number{}     <= (+digit{}) ,
            parens{}     <= (left_symbol{} > expression{} > right_symbol{}) ,
            factor{}     <= (number{} | parens{}) ,
            term{}       <= (factor{} > *((mult_symbol{} | div_symbol{}) > factor{})) ,
            expression{} <= (term{} > *((add_symbol{} | sub_symbol{}) > term{}))) ;
 
    std::cout << type_name<std::decay_t<decltype(calc_grammar)>>() << std::endl ;
    std::cout << read(calc_grammar) ;
}

/* grammar 
    rule 
        item 
        some 
            leaf 
    rule 
        item 
        and leaf item leaf rule item or item item rule item and item some and or leaf leaf item rule item and item some and or leaf leaf item */