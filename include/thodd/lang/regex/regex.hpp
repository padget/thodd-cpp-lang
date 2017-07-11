#ifndef __THODD_LANG_REGEX_REGEX_HPP__
#  define __THODD_LANG_REGEX_REGEX_HPP__

#  include <type_traits>

namespace 
thodd::lang::regex
{
    struct regex {} ;


    template<
        typename ... types_t>
    concept bool 
    regex_based (
        types_t&&...) 
    {
        return 
        std::conjunction_v<
            std::is_base_of<regex, std::decay_t<types_t>...> ;
    } ;
}

#endif