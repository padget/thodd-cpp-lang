#ifndef __THODD_LANG_REGEX_REGEX_HPP__
#  define __THODD_LANG_REGEX_REGEX_HPP__

#  include <type_traits>

namespace 
thodd::regex
{
    template<
        typename type_t>
    concept bool regex_based_one = 
    requires { typename type_t::regex_marker ; } ;

    template<
        typename ... types_t>
    concept bool 
    regex_based = (regex_based_one<std::decay_t<types_t>> && ...) ;
}

#endif