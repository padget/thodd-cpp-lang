#include <iostream>
#include <thodd/range.hpp>
#include <vector>
#include <string>

int main(int argc, char** argv)
{
    std::vector<std::string> args { argv, argv + argc } ;
    thodd::for_each(
        args, [](auto const & item){std::cout << item << std::endl ; }) ;

    thodd::for_each(
        thodd::filter(
            args, [] (auto && item) { return item == "--source"; }), 
        [] (auto && item) { std::cout << item << std::endl ; }) ;
}