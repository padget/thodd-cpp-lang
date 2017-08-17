#ifndef __THODD_LANG_LEXICAL_ANALYSE_HPP__
#  define __THODD_LANG_LEXICAL_ANALYSE_HPP__

#  include <array>
#  include <list>
#  include <utility>

#  include <thodd/lang/pod.hpp>

namespace 
thodd::lang
{
    inline auto
    build_tokens (
        auto __begin, 
        auto const __end, 
        auto const & __first,
        auto const & ... __word)
    {
        std::list<token<std::decay_t<decltype(__first.id)>, decltype(__begin)>> __tokens ;

        constexpr auto __each = 
        [](auto __begin, auto const & __end, auto const & __word)
        {
            auto&& [__matched, __it] = matches (__word.item, __begin, __end) ;
            return 
            token { std::pair { __begin, __it }, __word.id } ;
        } ;

        auto __cursor = __begin ;
        
        while (__cursor != __end)
        {
            auto&& __each_tokens = 
                    std::array 
                    { std::move(__each(__cursor, __end, __first)), 
                      std::move(__each(__cursor, __end, __word))... } ;
                      
            auto&& __greater = 
                std::max_element(
                    __each_tokens.begin(), __each_tokens.end(), 
                    [] (auto const & __l, auto const & __r) 
                    { return __l.size() < __r.size() ; }) ;

            __cursor = (*__greater).end() ;
            __tokens.push_back(std::move(*__greater)) ;
        }

        return __tokens ;
    }
}

#endif