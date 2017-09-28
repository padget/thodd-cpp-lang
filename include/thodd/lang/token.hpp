#ifndef __THODD_LANG_TOKEN_HPP__
#  define __THODD_LANG_TOKEN_HPP__

#  include <utility>
#  include <algorithm>

namespace 
thodd::lang
{
    template <
        typename language_t,
        typename iterator_t>
    struct token
    {
        std::pair<iterator_t, iterator_t> data ;
        language_t id ;

        inline auto 
        begin () 
        {
            return data.first ;
        }

        inline auto const
        begin () const 
        {
            return data.first ;
        }

        inline auto 
        end () 
        {
            return data.second ;
        }

        inline auto const
        end () const
        {
            return data.second ;
        }

        inline auto const
        size() const 
        {
            return 
            std::distance(begin(), end()) ;
        }

        inline auto const
        invalid () const
        {
            return 
            size() == 0 ;
        }
    } ;


    template<
        typename language_t, 
        typename iterator_t>
    token (std::pair<iterator_t, iterator_t> const &, language_t) -> token<language_t, iterator_t> ;
}

#endif 