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


template<
    typename T>
std::string type_name()
{
    int status;
    std::string tname = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    
    if(status == 0) 
    {
        tname = demangled_name;
        std::free(demangled_name);
    } 

    return tname;
}


template <
    typename language_t>
struct trace
{
    language_t id ; 
    size_t index ;
    thodd::lang::production_operator op ;
} ;

constexpr auto 
is_terminal (
    auto id, 
    auto const & grammar)
{
    return 
    grammar.dictionary.count(id) == 0 ;
}





enum struct math
{ digit, number, add, sub, expression, addition, substraction } ;

THODD_LANG_OPERATOR_FOR(math)

int main() 
{
    using namespace thodd::lang ;

    auto calc_grammar = 
    grammar<math> (
        math::expression,
        math::number <= (*math::digit),
        math::addition <= (math::expression > math::add > math::expression),
        math::substraction <= (math::expression > math::sub > math::expression),
        math::expression <= (math::number | math::addition | math::substraction)) ;

    auto number = { math::digit, math::digit, math::add, math::digit } ;
    
    //check (calc_grammar, number.begin(), number.end()) ;
    
    
    
}