#ifndef __THODD_LANG_REGEX_CHARS_HPP__
#  define __THODD_LANG_REGEX_CHARS_HPP__

#  include <utility>

#  include <thodd/lang/regex/char.hpp>
#  include <thodd/lang/regex/and.hpp>

namespace 
thodd::regex 
{
    template <
        typename char_t, 
        size_t size_c,
        template <
            typename, 
            size_t...> 
        typename sequence_t,
        size_t ... idx_c>
    constexpr auto 
    __chrs_impl (
        char_t const (&__chrs) [size_c], 
        sequence_t<size_t, idx_c...>)
    {
        return 
        ( chr(__chrs[idx_c]) > ... ) ;
    }

    template<
        typename char_t, 
        size_t size_c>
    constexpr auto
    chrs (
       char_t const (&__chrs) [size_c])
    {
        return 
        __chrs_impl (
            __chrs, 
            std::make_index_sequence<size_c - 1>{}) ;
    }
}

#endif