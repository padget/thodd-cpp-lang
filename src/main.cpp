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



enum struct math
{ 
    digit, 
    add, 
    sub,
    mult, 
    divi,
    left, 
    right,

    number, 
    mult_or_div, 
    add_or_sub,

    expression, 
    expression_tail,
    factor, 
    term, 
    term_tail,
    parens
} ;

enum struct lisp
{
    left, 
    right, 
    identifiant,
    expression, 
    expressions, 
    parens_expression
} ;

THODD_LANG_OPERATOR_FOR(lisp)

int main() 
{
    using namespace thodd::lang ;

    auto lisp_grammar = 
    grammar <lisp> (
        lisp::expression, 
        lisp::expression  <= ( lisp::identifiant | lisp::parens_expression ) ,
        lisp::parens_expression <= ( lisp::left > lisp::identifiant > lisp::expressions > lisp::right ) ,
        lisp::expressions <= ( *lisp::expression )    
    ) ;

    auto lisp_stream = std::list { lisp::left, lisp::identifiant, lisp::left, lisp::identifiant, lisp::identifiant, lisp::right, lisp::right } ;
    
    auto && [checked, cursor] = check(lisp_grammar, lisp_stream.begin(),  lisp_stream.end()) ;

    std::cout << std::boolalpha << checked << std::endl ;
    std::cout << std::boolalpha << (cursor == lisp_stream.end()) << std::endl ;
}