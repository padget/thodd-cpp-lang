#ifndef __THODD_LANG_REGEX_HPP__
#  define __THODD_LANG_REGEX_HPP__

#  include <tuple>
#  include <type_traits>

namespace 
thodd::lang
{
   




    template <
        typename ... choice_t>
    struct or_view
    {
        using regex_marker = void ;
        using 

        pod::or_<choices_t...> or_ ; 
        
        constexpr auto
        choices() const 
        -> decltype(auto)
        { return or_.choices ; }
    } ;

    template <
        typename ... item_t>
    struct and_regex 
    {
        using regex_marker = void ;

        pod::and_<item_t...> and_ ;

        constexpr auto 
        items () const 
        -> decltype(auto)
        { return and_.items ; }    
    } ;




    constexpr auto
    operator | (
        auto const & __lregex, 
        auto const & __rregex)
    requires all_regex_marked<decltype(__lregex), decltype(__rregex)>
    {        
        return
        or_<
            std::decay_t<decltype(__lregex)>, 
            std::decay_t<decltype(__rregex)>>
        { std::tuple(
            std::forward<decltype(__lregex)>(__lregex), 
            std::forward<decltype(__rregex)>(__rregex)) } ;
    }


    template<
        typename ... regexs_t>
    constexpr auto
    operator | (
        or_<regexs_t...> const & __or,
        auto const & __rregex)
    requires regex_based<decltype(__rregex), regexs_t...>
    {
        return 
        or_<regexs_t..., std::decay_t<decltype(__rregex)>>
        { std::tuple_cat(
            __or.choices, 
            std::tuple(std::forward<decltype(__rregex)>(__rregex))) } ;
    }

    template<
        typename ... regexs_t>
    constexpr auto
    operator | (
        auto const & __rregex, 
        or_<regexs_t...> const & __or)
    requires regex_based<decltype(__rregex), regexs_t...>
    {
        return 
        or_<std::decay_t<decltype(__rregex)>, regexs_t...>
        { std::tuple_cat(
            std::tuple(std::forward<decltype(__rregex)>(__rregex)),
            __or.choices) } ;
    }

    template<
        typename ... regexs_t, 
        typename ... regexs2_t>
    constexpr auto
    operator | (
        or_<regexs_t...> const & __or,
        or_<regexs2_t...> const & __or2)
    requires regex_based<regexs2_t..., regexs_t...>
    {
        return 
        or_<regexs_t..., regexs2_t...>
        { std::tuple_cat(__or.choices, __or2.choices) } ;
    }
}

#endif