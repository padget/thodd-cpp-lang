#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <functional>

#include <thodd/functional.hpp>

enum class test { foo, bar, fii } ;

enum class calc { integer, add_op, mult } ;

enum class lexword {digit, point, float_, exp} ;




template<typename char_t>
struct bin_op
{
    char_t left, 
    char_t op, 
    char_t right,
} ;



int main() 
{
    using namespace thodd ;

    auto is_between_chars = tern($0 <= *$2 && *$2 <= $1)[cout_<<*$2, ++$2][$2] ;
    auto is_particular_char = tern($0 == *$1)[cout_<<*$1, ++$1][$1] ;

    auto is_digit = bind(is_between_chars, val('0'), val('9'), $0) ;

    auto is_add  = bind(is_particular_char, val('+'), $0) ;
    auto is_sub  = bind(is_particular_char, val('-'), $0) ;
    auto is_mult = bind(is_particular_char, val('*'), $0) ;
    auto is_div  = bind(is_particular_char, val('/'), $0) ;

    auto is_open  = bind(is_particular_char, val('('), $0) ;
    auto is_close = bind(is_particular_char, val(')'), $0) ;

    
    auto is_add_operation = compose(is_digit, compose(is_add, is_digit)) ;
    auto is_sub_operation = compose(is_digit, compose(is_sub, is_digit)) ;
    auto is_mult_operation = compose(is_digit, compose(is_mult, is_digit)) ;
    auto is_div_operation = compose(is_digit, compose(is_div, is_digit)) ;

    auto is_expression = compose(
                            is_add_operation, 
                            compose(
                                is_sub_operation, 
                                compose(
                                    is_mult_operation, 
                                    is_div_operation))) ;

    auto char_to_int = bind(static_cast_<int>, *$0 + val('0')) ;
    auto on_add_operation = compose(char_to_int, ) ;

    std::string __str {"1/1="}; 
    std::cout << *is_expression(__str.begin()) ;

  

    
}