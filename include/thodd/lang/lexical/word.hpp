#ifndef __THODD_LANG_LEXICAL_HPP__
#  define __THODD_LANG_LEXICAL_HPP__

#  include <type_traits>

namespace 
thodd::lang::lexical
{
    template<
        typename id_t,
        typename regex_t>
    struct word
    {
        id_t id ;
        regex_t reg ;
    } ;

    template<
        typename id_t, 
        typename regex_t>
    word(id_t, regex_t&&) -> word<id_t, std::decay_t<regex_t>> ;
}

#endif