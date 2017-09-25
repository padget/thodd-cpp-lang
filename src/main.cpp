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


constexpr auto 
check (
    auto const & grammar,
    auto begin, auto end)
{
    using id_t = decltype(grammar.start) ;
    using po_t = thodd::lang::production_operator ;
        
    bool checked = true ;
 
    auto id_index = 0u ;
    std::stack<trace<id_t>> grammar_explorer ;
    grammar_explorer.push({ grammar.start, id_index }) ;

    while (begin != end && checked)
    {
        while (!is_terminal(grammar_explorer.top().id, grammar))
        {
            auto const & production = grammar.dictionary.at(grammar_explorer.top().id) ;

            grammar_explorer.push( { production.ids[id_index], id_index, production.op } ) ; 
            std::cout << (int) grammar_explorer.top().id << std::endl ;
        }

        bool const local_checked { *begin == grammar_explorer.top().id } ;
        bool must_up { true } ;
        
        if (local_checked)
            ++ begin ; 
        
        while (must_up && !grammar_explorer.empty())
        {
            grammar_explorer.pop() ;
            
            switch (grammar_explorer.top().op)
            {
                case po_t::some : 
                { 
                    must_up = ! local_checked ; 
                    break ;
                }
                case po_t::and_ : 
                { 
                    must_up = (id_index == grammar_explorer.top().id.size()) ; 
                    break ;
                }
                case po_t::or_  :
                { 
                    ; 
                    break ; 
                }
                default : break ;
            }
        }
        

        
    }

    return 
    checked ;
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

    auto number = { math::digit, math::digit } ;
    
    check (calc_grammar, number.begin(), number.end()) ;
    
    
    
}