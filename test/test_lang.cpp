#include <iostream>
#include <string>
#include <thodd/lang2.hpp>
#include <thodd/range.hpp>

int main() 
try
{
    auto input = thodd::make_string("cc");
    constexpr auto a = thodd::lang::regex::chr('a') ;
    constexpr auto b = thodd::lang::regex::chr('b') ;
    constexpr auto c = thodd::lang::regex::chr('c') ;
    
    thodd::if_exists(
        c(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "c exists : " << get(it) << std::endl ; });

    constexpr auto bd = thodd::lang::regex::range('b', 'd') ;
    thodd::if_exists(
        bd(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "bd exists : " << get(it) << std::endl ; });

    constexpr auto somec = thodd::lang::regex::some(c)(0, 1);
    thodd::if_exists(
        somec(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "somec exists : " << get(it) << std::endl ; });

    constexpr auto one_ofabc = thodd::lang::regex::one_of(a, b, c);
    thodd::if_exists(
        one_ofabc(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "one_ofabc exists : " << get(it) << std::endl ; });

    constexpr auto sequence_ofcc = thodd::lang::regex::sequence_of(c, c);
    thodd::if_exists(
        sequence_ofcc(thodd::begin(input), thodd::end(input)), 
        [] (auto && it) {std::cout << "sequence_ofcc exists : " << get(it) << std::endl ; });

    
    namespace rx = thodd::lang::regex ;

    auto input2 = thodd::make_string("Je commence par une majuscule et termine par un point.") ;
    constexpr auto is_phrase = rx::sequence_of(rx::range('A', 'Z'), rx::zero_more(rx::one_of(rx::range('a', 'z'), rx::chr(' '))), rx::chr('.')) ;
    thodd::if_exists(
        is_phrase(thodd::begin(input2), thodd::end(input2)), 
        [] (auto && ignored) { std::cout << "je suis une phrase" << std::endl ; }) ;  


} 
catch (...) 
{

}