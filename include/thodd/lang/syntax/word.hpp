#ifndef __THODD_LANG_SYNTAX_WORD_HPP__
#  define __THODD_LANG_SYNTAX_WORD_HPP__

#  include <type_traits>
#  include <utility>

#  include <thodd/lang/syntax/rule.hpp>

namespace 
thodd::syntax
{
    template<
        typename regex_t>
    struct word : rule
    {
        regex_t regex ;

        constexpr word(
            decltype(regex) && __regex) :
            regex { std::move(__regex) } {}

        constexpr word(
            decltype(regex) const& __regex) :
            regex { __regex } {}

        constexpr word(word const&) = default ;
        constexpr word(word&&) = default ;
        constexpr word& operator = (word const&) = default ;
        constexpr word& operator = (word&&) = default ;
    } ;

    constexpr auto 
    wrd(
        auto&& __regex)
    {
        return 
        word<std::decay_t<decltype(__regex)>>
        { std::forward<decltype(__regex)>(__regex) } ;
    }
}

#endif