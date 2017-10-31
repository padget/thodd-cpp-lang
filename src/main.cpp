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


inline constexpr auto
lisp_token_builder = 
[] () 
{
    using namespace thodd ;
    using namespace thodd::lang ;

    return 
    token_builder <lisp> (
        regex::term (lisp::ignored, regex::chr(' ')),
        regex::term (lisp::left, regex::chr('(')), 
        regex::term (lisp::right, regex::chr(')')), 
        regex::term (lisp::identifiant, (regex::one_more(regex::range('a', 'z')))), 
        regex::term (lisp::number, (regex::one_more(regex::range('0', '9'))))) ;
} ;

#  include <array>

int main() 
{
    using namespace thodd ;
    using namespace thodd::lang ;
    
    auto stream = std::string("(add (neg 1 12 12 a) a)") ;
    auto end = stream.end() ;

    constexpr auto space = regex::chr(' ') ;
    constexpr auto left = regex::chr('(') ;
    constexpr auto number = regex::range(regex::chr('0'), regex::chr('9')) ;
    constexpr auto rxs = std::make_tuple(space, left, number) ; 
    
    constexpr auto group_by = 
    [] (auto && container, auto && grouper) 
    {

    } ;
    
    group_by(
        stream, 
        [rxs, end] (auto && it) 
        {
            return
            std::apply(
                [it, end] (auto && ... rx) 
                { return min(std::array(rx(it, end)...), lower($0, $1)) ; }, 
                rxs) ;
        }
    ) ;

    for(auto && it : indirect(stream)) 
        std::cout << *it << std::endl ;

    auto && s = {0,1,2,3} ;
    auto && min_ = min(s, lower($0, $1)) ;

    if_exists(min(s, greater($0, $1)), [] (auto && item) { std::cout << item << std::endl ; });

    //auto && tokens = lisp_token_builder () (input_stream.begin(), input_stream.end()) ;

    /*constexpr auto lisp_grammar = 
        syntax::grammar (
            lisp::expression,
            syntax::is (lisp::expression, syntax::one_of (lisp::number, lisp::identifiant, lisp::parens_expression)), 
            syntax::is (lisp::parens_expression, syntax::sequence_of (lisp::left, lisp::expressions, lisp::right)), 
            syntax::is (lisp::expressions, syntax::zero_more (lisp::expression))) ;
        
    std::cout << std::boolalpha << syntax::is_terminal (lisp::number, lisp_grammar) << std::endl ;
    std::cout << (int) syntax::get_operator_by_id (lisp::parens_expression, lisp_grammar) << std::endl ; 


    
    std::vector<lisp> lisp_stream ;
    std::transform (
        tokens.begin(), 
        tokens.end(), 
        std::back_inserter(lisp_stream), 
        [] (auto&& token) { return token.id ; } ) ;

    //std::cout << std::boolalpha << inside(2, tuple_map(tuple(0, "coucou"), tuple(1, 12))) << std::endl  ;
    //std::cout << at(1, tuple_map(tuple(0, "coucou"), tuple(1, ""))) << std::endl ;
    auto && mres = syntax::check_builder (lisp_grammar) (lisp_stream.begin(), lisp_stream.end()) ;

    if (matched (mres))
    {
        auto && [tree, it] = 
            build_tree (lisp_grammar, tokens.begin(), tokens.end()) ;

        if (tree.id != lisp::error) ;
            print_tree (tree) ;

        std::cout << std::boolalpha << (it == tokens.end()) << std::endl ;
        
        purge_tree (tree, lisp_grammar) ;
        print_tree (tree) ;
    }*/
    constexpr auto lisp_grammar = 
    syntax::grammar (
        lisp::expression,
        syntax::one_of (lisp::expression, lisp::number, lisp::identifiant, lisp::parens_expression), 
        syntax::sequence_of (lisp::parens_expression, lisp::left, lisp::expressions, lisp::right), 
        syntax::zero_more (lisp::expressions, lisp::expression)) ;

    std::cout << std::boolalpha << syntax::is_terminal (lisp::number, lisp_grammar) << std::endl ;

  
}
