#include <iostream>
#include <string>
#include <thodd/lang2.hpp>
#include <thodd/range.hpp>



int main() 
try
{
    auto input = thodd::make_string("cc");

    namespace rx = thodd::lang::regex ;

    constexpr auto a = rx::chr('a') ;
    constexpr auto b = rx::chr('b') ;
    constexpr auto c = rx::chr('c') ;
    
    thodd::if_exists(
        c(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "c exists : " << get(it) << std::endl ; });

    constexpr auto bd = rx::range('b', 'd') ;
    thodd::if_exists(
        bd(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "bd exists : " << get(it) << std::endl ; });

    constexpr auto somec = rx::zero_more(c) ;
    thodd::if_exists(
        somec(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "somec exists : " << get(it) << std::endl ; });

    constexpr auto one_ofabc = rx::one_of(a, b, c);
    thodd::if_exists(
        one_ofabc(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "one_ofabc exists : " << get(it) << std::endl ; });

    constexpr auto sequence_ofcc = rx::sequence_of(c, c);
    thodd::if_exists(
        sequence_ofcc(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "sequence_ofcc exists : " << get(it) << std::endl ; });

    
    
    
    auto input2 = thodd::make_string("Je commence par une majuscule et termine par un point.") ;
    constexpr auto is_phrase = rx::sequence_of(rx::range('A', 'Z'), rx::zero_more(rx::one_of(rx::range('a', 'z'), rx::chr(' '))), rx::chr('.')) ;
    thodd::if_exists(
        is_phrase(thodd::begin(input2), thodd::end(input2)), 
        [] (auto && ignored) { std::cout << "je suis une phrase" << std::endl ; }) ;  

    constexpr auto print_token = 
    [] (auto idprinter)
    {
        return 
        [idprinter] (auto && t) 
        {
            thodd::for_each_inifinally(
                t.data, 
                [] (auto const & c) { std::cout << c ; }, 
                [&] { std::cout << idprinter(t.id) << " : \"" ; },
                [] { std::cout << '"' << std::endl ; }) ;
        } ;
    } ;

    thodd::if_exists(
        thodd::lang::terminal(0, is_phrase)(thodd::begin(input2), thodd::end(input2)),
        print_token(thodd::$0)) ;
    
    enum class phrase_lang 
    { maj, word, point, ignored, error } ;

    constexpr auto phrase_lang2string = 
    [] (phrase_lang id) 
    {
        switch (id) 
        {
            case phrase_lang::word : return "word" ;
            case phrase_lang::error : return "error" ;
            case phrase_lang::ignored : return "ignored" ;
            case phrase_lang::maj : return "maj" ;
            case phrase_lang::point : return "point" ;
        }   
    } ;
    auto && tokens = 
        thodd::lang::build_tokens(
            input2,
            thodd::lang::terminal(phrase_lang::ignored, rx::chr(' ')),
            thodd::lang::terminal(phrase_lang::maj, rx::range('A', 'Z')),
            thodd::lang::terminal(phrase_lang::word, rx::one_more(rx::range('a', 'z'))), 
            thodd::lang::terminal(phrase_lang::point, rx::chr('.'))) ;


    // thodd::for_each(
    //     tokens,
    //     print_token(phrase_lang2string)) ;
} 
catch (...) 
{

}