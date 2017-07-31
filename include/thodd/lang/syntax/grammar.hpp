#ifndef __THODD_LANG_SYNTAX_GRAMMAR_HPP__
#  define __THODD_LANG_SYNTAX_GRAMMAR_HPP__

#  include <type_traits>

namespace 
thodd::syntax
{
    template <typename...> 
    struct grammar_item {} ;

    constexpr auto
    grammar(
        auto&& ... __rule)
    {
        return 
        grammar_item<
            std::decay_t<decltype(__rule)>... > {} ;
    }

    template <
        typename ... rule_t>
    std::string 
    read(
        grammar_item<rule_t ...> const & __grammar)
    {
        return 
        std::string("grammar ")
        + (read(rule_t{}) + ...) ;
    }

    template<
        typename def_t, 
        typename production_t>
    std::string
    read(
        rule<def_t, production_t> const & __rule)
    {
        return 
        std::string("rule ")
        + (read(def_t{}) + read(production_t{}))  ;
    }

    template<
        typename ... item_t>
    std::string
    read(
        and_<item_t...> const & __and)
    {
        return 
        std::string("and ")
        + (read(item_t{}) + ...) ;
    }
    

    template<
        typename ... item_t>
    std::string
    read(
        or_<item_t...> const & __or)
    {
        return 
        std::string("or ")
        + (read(item_t{}) + ...) ;
    }
    
    template<
        typename item_t>
    std::string
    read(
        some<item_t> const & __some)
    {
        return 
        std::string("some ")
        + read(item_t{}) ;
    }
    
    std::string 
    read(
        auto const & __leaf)
    requires item_based<decltype(__leaf)>
          && has_static_id<decltype(__leaf)>
    {
        return 
        "leaf " ; 
    }

    std::string 
    read(
        item const&)
    {
        return 
        "item " ;
    }
}

#endif