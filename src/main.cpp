#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <functional>
#include <type_traits>
#include <typeinfo>


#include <cxxabi.h>


#include <thodd/lang.hpp>


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


enum struct lisp
{
    ignored=0, 
    error=2,
    left=1, 
    right=3, 
    identifiant=6,
    number=10,
    expression=7, 
    expressions=8, 
    parens_expression=9
} ;

template <
    typename language_t, 
    typename iterator_t>
void 
print_tree (
    thodd::lang::tree<language_t, iterator_t> const & tree, 
    size_t dec = 0)
{
    std::cout << std::string(dec, ' ') << (int) tree.id << " : '" ;

    std::for_each(tree.data.first, tree.data.second, [] (auto && c) { std::cout << c ; } ) ;

    std::cout << "'" << std::endl ;

    for (auto && tchild : tree.childs)
        print_tree (tchild, dec + 4) ;
}

THODD_LANG_OPERATOR_FOR(lisp)

int main() 
{
    using namespace thodd ;
    using namespace thodd::lang ;

    auto input_stream = std::string("(add(neg1)a)");

    auto && tokens = build_tokens (
                        input_stream.begin(), 
                        input_stream.end(), 
                        terminal<lisp::error, void>{}, 
                        term<lisp::ignored>(chr<' '>{}),
                        term<lisp::left>(chr<'('> {}), 
                        term<lisp::right>(chr<')'> {}), 
                        term<lisp::identifiant>(+(chr<'a'> {} - chr<'z'> {})), 
                        term<lisp::number>(+(chr<'0'> {} - chr<'9'> {}))) ;

    auto lisp_grammar = 
    grammar <lisp> (
        lisp::expression, 
        lisp::expression        <= ( lisp::number | lisp::identifiant | lisp::parens_expression ) ,
        lisp::parens_expression <= ( lisp::left > lisp::identifiant > lisp::expressions > lisp::right ) ,
        lisp::expressions       <= ( *lisp::expression )    
    ) ;

    std::vector<lisp> lisp_stream ;
    std::transform (
        tokens.begin(), 
        tokens.end(), 
        std::back_inserter(lisp_stream), 
        [] (auto&& token) { return token.id ; } ) ;
    
    auto && [checked, cursor] = check (lisp_grammar, lisp_stream.begin(), lisp_stream.end()) ;

    std::for_each (
        lisp_stream.begin(), 
        lisp_stream.end(), 
        [] (auto && id) { std::cout << (int) id << std::endl ; } ) ;
    
    std::cout << std::boolalpha << checked << std::endl ;
    std::cout << std::boolalpha << (cursor == lisp_stream.end()) << std::endl ;

    if (checked)
    {
        auto && [tree, it] = 
            build_tree (lisp_grammar, tokens.begin(), tokens.end()) ;

        if (tree.id != lisp::error) ;
            print_tree (tree) ;

        std::cout << std::boolalpha << (it == tokens.end()) << std::endl ;
        
        purge_tree (tree, lisp_grammar) ;
        print_tree (tree) ;

        /* interpret (tree, 
            rule (lisp::identifiant, 
                  [] (auto const & data_begin, auto const & data_end) 
                  { std::for_each(data_begin, data_end, cout_ << $0) ; } ), 
            rule (lisp::number, 
                  [] (auto && data_begin, auto && data_end)
                  { std::for_each(data_begin, data_end, cout_ << $0) })) ;*/
    }
}
