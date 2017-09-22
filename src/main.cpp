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
check (
    auto const & grammar,
    auto begin, auto end)
{
    using id_t = decltype(grammar.start) ;
    std::stack<trace<id_t>> id_stack ;
    
    id_stack.push ( { grammar.start, 0 } ) ;

    while (begin != end)
    {
        while (grammar.dictionary.count(id_stack.top().id) != 0)
        {
            auto definition = grammar.dictionary.at(id_stack.top().id) ;
            id_stack.push( { definition.ids[0], 0, definition.op } ) ; 
        }
    }
}



enum struct math
{ digit, number, add, sub, expression, addition, substraction } ;

THODD_LANG_OPERATOR_FOR(math)

int main() 
{
    using namespace thodd::lang ;

    auto calc_grammar = 
    grammar<math> (
        math::number,
        math::number <= (*math::digit), 
        math::addition <= (math::expression > math::add > math::expression),
        math::substraction <= (math::expression > math::sub > math::expression),
        math::expression <= (math::number | math::addition | math::substraction)) ;

    auto number = { math::digit, math::digit } ;
    
    check (calc_grammar, number.begin(), number.end()) ;
    
    
    
}