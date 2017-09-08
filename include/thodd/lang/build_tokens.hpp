#ifndef __THODD_LANG_LEXICAL_ANALYSE_HPP__
#  define __THODD_LANG_LEXICAL_ANALYSE_HPP__

#  include <array>
#  include <list>
#  include <utility>

#  include <thodd/functional.hpp>
#  include <thodd/lang/pod.hpp>

namespace 
thodd::lang
{
    inline auto
    build_tokens (
        auto __begin, 
        auto const __end,
        auto const & __error,
        auto const & __ignored, 
        auto const & __first,
        auto const & ... __word)
    {
        std::list<token<decltype(meta::id(__first)), decltype(__begin)>> __tokens ;

        auto __cursor = __begin ;
        
        while (__cursor != __end)
        {
            auto&& __each_tokens = 
                    std::array 
                    { std::move(matches(__ignored, *$0, __cursor, __end)),   
                      std::move(matches(__first, *$0, __cursor, __end)), 
                      std::move(matches(__word, *$0, __cursor, __end)) ... } ;
      
            auto&& __greater = 
                std::max_element(
                    __each_tokens.begin(), __each_tokens.end(), 
                    [] (auto const & __l, auto const & __r) 
                    { return __l.size() < __r.size() ; }) ;

            if (__cursor == (*__greater).end())
               __tokens.push_back(token { std::pair { __cursor, ++__cursor }, meta::id(__error) }) ;  
            else 
            {
                __cursor = (*__greater).end() ;
                constexpr auto __ignored_id = meta::id(__ignored) ;
                
                if ((*__greater).id != __ignored_id) 
                    __tokens.push_back(std::move(*__greater)) ;
            }
        }

        return __tokens ;
    }
}

#endif