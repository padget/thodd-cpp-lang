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
    bool dive = true ; // to dive or contrary if false ;

    std::stack<trace<id_t>> grammar_explorer ;
    grammar_explorer.push({grammar.start, 0u, grammar.dictionary.at(grammar.start).op}) ;    

    while (begin != end && checked && !grammar_explorer.empty())
    {
        std::cout << "id to check " << (int) *begin << ", explorer top id " << (int) grammar_explorer.top().id << ", explorer top operator " << (int) grammar_explorer.top().op << ", dived " << std::boolalpha << dive <<std::endl ; 

        auto const & top_id = grammar_explorer.top().id ;

        if (is_terminal (top_id, grammar))
        {    
            std::cout << "is terminal" << std::endl ;
            dive = false ;
            checked &= *begin == grammar_explorer.top().id ;
            
            if (checked) ++ begin ; 
        }
        
        if (dive)
        {
            std::cout << "i push" << std::endl ;
            auto const & production = grammar.dictionary.at (grammar_explorer.top().id);
            grammar_explorer.push ({production.ids[0u], 0u, production.op}) ;
        }     
        else
        {
            std::cout << "i pop" << std::endl ;
            grammar_explorer.pop () ;

            if (!grammar_explorer.empty())
                switch (grammar_explorer.top().op)
                {
                    case thodd::lang::production_operator::some : std::cout << "i am some" << std::endl ; dive = checked ; break ;
                    case thodd::lang::production_operator::and_ : std::cout << "i am and" << std::endl ; break ;
                    default: break ;
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
        math::expression <= (math::number > math::add > math::number)/*, 
        math::addition <= (math::expression > math::add > math::expression),
        math::substraction <= (math::expression > math::sub > math::expression),
        math::expression <= (math::number | math::addition | math::substraction)*/) ;

    auto number = { math::digit, math::digit, math::add, math::digit } ;
    
    check (calc_grammar, number.begin(), number.end()) ;
    
    
    
}