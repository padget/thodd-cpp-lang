#ifndef __THODD_LANG_LEXICAL_ANALYSE_HPP__
#  define __THODD_LANG_LEXICAL_ANALYSE_HPP__

#  include <array>
#  include <list>
#  include <utility>

#  include <thodd/lang/pod.hpp>

namespace 
thodd::lang
{

    namespace
    meta
    {
        template <   
            auto id_c>
        constexpr auto 
        id (terminal<id_c, auto> const &) 
        { return id_c ; }

        template <auto id_c, typename regex_t>
        constexpr auto
        item (terminal<id_c, regex_t> const &)
        -> regex_t
        { return {} ; }


        template <   
            auto id_c>
        constexpr auto 
        id (ignored_terminal<id_c, auto> const &) 
        { return id_c ; }

        template <auto id_c, typename regex_t>
        constexpr auto
        item (ignored_terminal<id_c, regex_t> const &)
        -> regex_t
        { return {} ; }


        template <   
            auto id_c>
        constexpr auto 
        id (error_terminal<id_c> const &) 
        { return id_c ; }
    }

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

        constexpr auto __each = 
        [] (auto __begin, auto const & __end, auto const & __word)
        {
            auto && [__matched, __it] = matches (meta::item(__word), __begin, __end) ;
            return 
            token { std::pair { __begin, __it }, meta::id(__word) } ;
        } ;

        auto __cursor = __begin ;
        
        while (__cursor != __end)
        {
            auto&& __each_tokens = 
                    std::array 
                    { std::move(__each(__cursor, __end, __ignored)),   
                      std::move(__each(__cursor, __end, __first)), 
                      std::move(__each(__cursor, __end, __word))... } ;
      
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