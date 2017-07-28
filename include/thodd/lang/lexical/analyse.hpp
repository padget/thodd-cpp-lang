#ifndef __THODD_LANG_LEXICAL_ANALYSE_HPP__
#  define __THODD_LANG_LEXICAL_ANALYSE_HPP__

#  include <array>

#  include <thodd/lang/lexical/token.hpp>

namespace 
thodd::lang::lexical
{
    inline auto
    analyse(
        auto __begin, 
        auto __end,
        auto const & __first, 
        auto const & ... __word)
    {
        auto __token = make_token(__first.id, __end, __end) ;

        for(auto&& __tmp : std::array  { 
                                __analyse(__begin, __end, __first), 
                                __analyse(__begin, __end, __word)... })
        {
            auto&& __token_size = std::distance(__token.data.first, __token.data.second) ;
            auto&& __tmp_size = std::distance(__tmp.data.first, __tmp.data.second) ;
            
            if(__token_size < __tmp_size)
                __token = __tmp ;
        }

    }

    inline auto 
    __analyse (
        auto __begin, 
        auto __end, 
        auto const & __word)
    {
        auto __save = __begin, 
        return 
        matches(__begin, __end, __reg) ?
        make_token(__word.id, __save, __begin) :
        make_invalid_token(__word.id, __save, __begin) ;
    }
}

#endif