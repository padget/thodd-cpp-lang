#ifndef __THODD_LANG_SYNTAX_WORD_HPP__
#  define __THODD_LANG_SYNTAX_WORD_HPP__

#  include <type_traits>
#  include <utility>
#  include <optional>

#  include <thodd/lang/syntax/rule.hpp>
#  include <thodd/lang/syntax/token.hpp>

namespace 
thodd::syntax
{
    template<
        typename reg_t, 
        typename lang_t>
    struct word : rule
    {
        reg_t reg ;
        lang_t id ;

        constexpr word(
            decltype(reg) && __reg, 
            decltype(id) const & __id) :
            reg { std::move(__reg) }, 
            id { __id } {}

        constexpr word(
            decltype(reg) const & __reg, 
            decltype(id) const & __id) :
            reg { __reg }, id { __id } {}

        constexpr word(word const&) = default ;
        constexpr word(word&&) = default ;
        constexpr word& operator = (word const&) = default ;
        constexpr word& operator = (word&&) = default ;
    } ;

    constexpr auto 
    wrd (
        auto&& __reg, 
        auto&& __id)
    {
        return 
        word<
            std::decay_t<decltype(__reg)>, 
            std::decay_t<decltype(__id)>>
        { std::forward<decltype(__reg)>(__reg), std::forward<decltype(__id)>(__id) } ;
    }


    inline void
    read (
        word<auto, auto> const& __word,
        auto & __cursor,
        auto const & __end, 
        auto & __tokens)
    {
        auto __save = __cursor ;
    
        using token_t = decltype(make_token(__word.id, __save, __cursor)) ;

        if(matches(__word.reg, __cursor, __end))
            __tokens.push_back(make_token(__word.id, __save, __cursor)) ;
        else
        {
            __tokens.push_back(make_invalid_token(__word.id, __save, __cursor)) ;
            __cursor = __save ;
        }
    }
}

#endif