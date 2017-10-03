#ifndef __THODD_LANG_LEXICAL_ANALYSE_HPP__
#  define __THODD_LANG_LEXICAL_ANALYSE_HPP__

#  include <array>
#  include <list>
#  include <utility>

#  include <thodd/lang/token.hpp>

namespace 
thodd::lang
{
    inline constexpr auto
    token_builder = 
    [] (auto ... terminal) 
    {
        return
        [terminal...] (auto begin, auto const end)
        {
            constexpr auto greater_token = 
            [] (auto const & __l, auto const & __r) { return __l.size() < __r.size() ; } ;
                
            std::list<token<language_t, decltype(__begin)>> __tokens ;
            auto cursor = __begin ;
            

            while (cursor != end)
            {
                auto&& __each_tokens = std::array {std::move(terminal(cursor, end)) ... } ;
                auto&& __greater = std::max_element(__each_tokens.begin(), __each_tokens.end(), greater_token) ;

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
        } ;
    } ;
}

#endif
