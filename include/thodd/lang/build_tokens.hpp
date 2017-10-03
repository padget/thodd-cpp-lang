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
    inline constexpr auto
    token_builder = 
    [] (auto ... terminal) 
    {
        return
        [terminal...] (auto begin, auto const end)
        {
            constexpr auto greater_token = 
            [] (auto const & l, auto const & r) { return l.size() < r.size() ; } ;
            constexpr auto make_token = 
            [] (auto begin, auto end, auto id) { return token{std::pair{begin, end}, id} ;} ;
            
            std::list<token<language_t, decltype(begin)>> tokens ;
            auto cursor = begin ;
            
            while (cursor != end)
            {
                auto&& each_tokens = std::array {std::move(terminal(cursor, end)) ... } ;
                auto&& greater = std::max_element(each_tokens.begin(), each_tokens.end(), greater_token) ;

                if (cursor == (*greater).end())
                   tokens.push_back(make_token(cursor, ++cursor, language_t::error)) ;  
                else 
                {
                    cursor = (*greater).end() ;

                    if ((*greater).id != language_t::ignored) 
                        tokens.push_back(std::move(*greater)) ;
                }
            }

            return tokens ;
        } ;
    } ;
}

#endif
