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


template<typename T>
std::string type_name()
{
    int status;
    std::string tname = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    
    if(status == 0) {
        tname = demangled_name;
        std::free(demangled_name);
    } 

    return tname;
}




enum struct 
math
{ digit, number, add, sub, expression, addition, substraction } ;

THODD_LANG_OPERATOR_FOR(math)

int main() 
{
    using namespace thodd::lang ;

    constexpr auto number =       math::number <= *math::digit ;
    constexpr auto addition =     math::addition <= and_op (math::expression, math::add, math::expression) ;
    constexpr auto substraction = math::substraction <= and_op (math::expression, math::sub, math::expression) ;
    constexpr auto expression =   math::expression <= or_op (math::number, math::addition, math::substraction) ;

    std::cout << type_name<decltype(number)> () << std::endl ;
}