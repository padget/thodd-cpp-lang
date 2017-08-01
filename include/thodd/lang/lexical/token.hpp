#ifndef __THODD_LANG_LEXICAL_TOKEN_HPP__
#  define __THODD_LANG_LEXICAL_TOKEN_HPP__

#  include <utility>
#  include <optional>
#  include <type_traits>

namespace 
thodd::lexical
{
    template<
        typename lang_t,
        typename iterator_t>
    struct token
    {
        std::pair<iterator_t, iterator_t> data ;
        std::optional<lang_t> id ;

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
        invalid () const
        {
            return 
            !id.has_value() ;
        }
    } ;
}

#endif