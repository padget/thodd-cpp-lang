#ifndef __THODD_LANG_LEXICAL_ANALYSE_HPP__
#  define __THODD_LANG_LEXICAL_ANALYSE_HPP__

#  include <array>
#  include <list>
#  include <utility>

#  include <thodd/lang/token.hpp>

namespace 
thodd::lang
{
    template <
        typename language_t>
    inline auto
    build_tokens (
        auto __begin, 
        auto const end,
        auto ... terminal)
    {
        std::list<token<language_t, decltype(__begin)>> __tokens ;

        auto cursor = __begin ;
        
        while (cursor != end)
        {
            auto&& __each_tokens = 
                    std::array {std::move(terminal(cursor, end)) ... } ;
      
            auto&& __greater = 
                std::max_element(
                    __each_tokens.begin(), __each_tokens.end(), 
                    [] (auto const & __l, auto const & __r) 
                    { return __l.size() < __r.size() ; }) ;

            if (cursor == (*__greater).end())
               __tokens.push_back(token { std::pair { cursor, ++cursor }, language_t::error }) ;  
            else 
            {
                cursor = (*__greater).end() ;
                
                if ((*__greater).id != language_t::ignored) 
                    __tokens.push_back(std::move(*__greater)) ;
            }
        }

        return __tokens ;
    }
}

#endif