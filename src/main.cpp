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



int main() 
{
    using namespace thodd::lang::syntax ;

    struct digit        : item {} ;
    struct sub_symbol   : item {} ;
    struct add_symbol   : item {} ;
    struct mult_symbol  : item {} ;
    struct div_symbol   : item {} ;
    struct left_symbol  : item {} ;
    struct right_symbol : item {} ;
    struct number       : item {} ; 
    struct parens       : item {} ; 
    struct factor       : item {} ; 
    struct term         : item {} ; 
    struct expression   : item {} ;


    constexpr auto calc = 
        grammar(
            number{}     <= +digit{} ,
            parens{}     <= left_symbol{} > expression{} > right_symbol{} ,
            factor{}     <= number{} | parens{} ,
            term{}       <= factor{} > *((mult_symbol{} | div_symbol{}) > factor{}) ,
            expression{} <= term{} > *((add_symbol{} | sub_symbol{}) > term{})) ;
 
    std::cout << type_name<std::decay_t<decltype(calc)>>() << std::endl ;

    /*
    thodd::lang::syntax::grammar_item<
        thodd::lang::syntax::rule<
            main::number, 
            thodd::lang::syntax::some<main::digit> >, 
        thodd::lang::syntax::and_<
            thodd::lang::syntax::rule<
                main::parens, 
                main::left_symbol>, 
            main::expression, 
            main::right_symbol>, 
        thodd::lang::syntax::or_<
            thodd::lang::syntax::rule<
                main::factor, main::number>, main::parens>, thodd::lang::syntax::and_<thodd::lang::syntax::rule<main::term, main::factor>, thodd::lang::syntax::some<thodd::lang::syntax::and_<thodd::lang::syntax::or_<main::mult_symbol, main::div_symbol>, main::factor> > >, thodd::lang::syntax::and_<thodd::lang::syntax::rule<main::expression, main::term>, thodd::lang::syntax::some<thodd::lang::syntax::and_<thodd::lang::syntax::or_<main::add_symbol, main::sub_symbol>, main::term> > > >
    
    
    
    
    */
}