#ifndef __THODD_LANG_LEXICAL_ANALYSE_HPP__
#  define __THODD_LANG_LEXICAL_ANALYSE_HPP__

#  include <array>
#  include <list>
#  include <utility>

#  include <thodd/lang/token.hpp>
#  include <thodd/lang/matches.hpp>

namespace 
thodd::lang
{
    template <
        auto error_id, 
        auto ignored_id>
    inline auto
    build_tokens (
        auto __begin, 
        auto const __end,
        terminal<error_id, void>,
        terminal<ignored_id, auto> __ignored, 
        auto __first,
        auto ... __next)
    {
        std::list<token<decltype(ignored_id), decltype(__begin)>> __tokens ;

        auto __cursor = __begin ;
        
        while (__cursor != __end)
        {
            auto&& __each_tokens = 
                    std::array 
                    { std::move(matches(__ignored, __cursor, __end)),   
                      std::move(matches(__first, __cursor, __end)), 
                      std::move(matches(__next, __cursor, __end)) ... } ;
      
            auto&& __greater = 
                std::max_element(
                    __each_tokens.begin(), __each_tokens.end(), 
                    [] (auto const & __l, auto const & __r) 
                    { return __l.size() < __r.size() ; }) ;

            if (__cursor == (*__greater).end())
               __tokens.push_back(token { std::pair { __cursor, ++__cursor }, error_id }) ;  
            else 
            {
                __cursor = (*__greater).end() ;
                
                if ((*__greater).id != ignored_id) 
                    __tokens.push_back(std::move(*__greater)) ;
            }
        }

        return __tokens ;
    }
}

#endif