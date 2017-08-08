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
    using namespace thodd::regex ;
    

    constexpr auto __unpair = chr('1') | chr('3') | chr('5') | chr('7') | chr('9') ;
    constexpr auto __pair   = chr('0') | chr('2') | chr('4') | chr('6') | chr('8') ;
    
    std::string __input2 { "2" } ;
    auto [__matched, __it] = matches(__unpair | __pair, __input2.begin(), __input2.end());
    
    std::cout << std::boolalpha << (__matched && __it == __input2.end()) << std::endl ;
    std::cout << type_name<decltype(__unpair | __pair)>() << std::endl ;

    constexpr auto __hello  = chrs("hello") ;
    std::string __input3 { "hello" } ;
    auto [__matched2, __it2] = matches(__hello, __input3.begin(), __input3.end()) ;
    std::cout << std::boolalpha << __matched2 << std::endl ;
    std::cout << type_name<decltype(__hello)>() << std::endl ;



    using namespace thodd::lexical ;

    auto __pair_w = word { calc::pair, __pair } ;
    auto __unpair_w = word { calc::unpair, __unpair } ;
    
    std::string __input4 { "123456789" } ;
    
    auto&& __res = analyse (__input4.begin(), __input4.end(), __unpair_w, __pair_w) ;
    std::cout << type_name<decltype(__res)>() << std::endl ;
    
    for(auto const& __r : __res)
    {    
        std::cout << std::boolalpha << !__r.invalid() << " size " << __r.size() << std::endl ;
        std::cout << (int) __r.id << std::endl ;
    }



    using namespace thodd::syntax ;

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


    constexpr auto calc_grammar = 
        grammar (
            expression{}, 
            number{}     <= (+digit{}) ,
            parens{}     <= (left_symbol{} > expression{} > right_symbol{}) ,
            factor{}     <= (number{} | parens{}) ,
            term{}       <= (factor{} > *((mult_symbol{} | div_symbol{}) > factor{})) ,
            expression{} <= (term{} > *((add_symbol{} | sub_symbol{}) > term{}))) ;
 
    std::cout << type_name<std::decay_t<decltype(calc_grammar)>>() << std::endl ;
    get_definition(calc_grammar, expression{}) ; 

}
