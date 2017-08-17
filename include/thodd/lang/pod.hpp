#ifndef __THODD_LANG_POD_HPP__
#  define __THODD_LANG_POD_HPP__

#  include <tuple>
#  include <limits>
#  include <utility>
#  include <type_traits>
#  include <algorithm>


namespace
thodd::lang
{
    /// CHAR_
    template <
        typename char_t >
    struct char_
    {
        using regex_marker = void ;
        using char_marker = void ;
        
        char_t c ;
    } ;

    template <typename type_t>
    concept bool char_marked = requires { typename std::decay_t<type_t>::char_marker ; } ;

    template<typename char_t>
    char_(char_t const&) -> char_<std::decay_t<char_t>> ;


    /// NOT_
    template<
        typename item_t>
    struct not_
    {
        using regex_marker = void ;
        using not_marker = void ;

        item_t item ;
    } ;

    template <typename type_t>
    concept bool not_marked = requires { typename std::decay_t<type_t>::not_marker ; } ;

    template<typename item_t>
    not_(item_t const&) -> not_<item_t> ;


    /// BETWEEN
    template<
        typename min_t, 
        typename max_t>
    struct between
    {
        using regex_marker = void ;
        using between_marker = void ;

        char_<min_t> min ;  
        char_<max_t> max ;
    } ;

    template<typename min_t, typename max_t>
    between (char_<min_t> const &, char_<max_t> const &) -> between<min_t, max_t> ;

    template <typename type_t>
    concept bool between_marked = requires { typename std::decay_t<type_t>::between_marker ; } ;

    
    /// OR_
    template <
        typename ... choice_t>
    struct or_  
    {
        using regex_marker = void ;
        using syntax_marker = void ;
        using or_marker = void ;

        std::tuple<choice_t...> choices ;        
    } ;

    template<typename ... choice_t>
    or_ (std::tuple<choice_t...> const &) -> or_<choice_t...> ;

    template <typename type_t>
    concept bool or_marked = requires { typename std::decay_t<type_t>::or_marker ; } ;


    // AND_
    template <
        typename ... item_t>
    struct and_ 
    {
        using regex_marker = void ;
        using syntax_marker = void ;
        using and_marker = void ;

        std::tuple<item_t...> items ;
    } ;

    template<typename ... item_t>
    and_ (std::tuple<item_t...> const &) -> and_<item_t...> ;

    template <typename type_t>
    concept bool and_marked = requires { typename std::decay_t<type_t>::and_marker ; } ;


    /// SOME
    template <
        typename item_t>
    struct some 
    {
        using regex_marker = void ;
        using syntax_marker = void ;
        using some_marker = void ;

        item_t item ;

        std::size_t min { 0 } ; 
        std::size_t max { std::numeric_limits<size_t>::max() } ;
    
        constexpr some
        operator () (
            std::size_t const& __min, 
            std::size_t const& __max) const
        {
            return 
            { item, __min, __max } ;
        }
    } ;

    template <typename type_t>
    concept bool some_marked = requires { typename std::decay_t<type_t>::some_marker ; } ;

    template<typename item_t>
    some(item_t const&) -> some<std::decay_t<item_t>> ;




    template <typename regex_t>
    concept bool regex_marked = requires { typename std::decay_t<regex_t>::regex_marker ; } ;

    template <typename ... regex_t>
    concept bool all_regex_marked = (regex_marked<regex_t> && ...) ;



    template <typename syntax_t>
    concept bool syntax_marked = requires { typename std::decay_t<syntax_t>::syntax_marker ; } ;

    template <typename ... syntax_t>
    concept bool all_syntax_marked = (syntax_marked<syntax_t> && ...) ;




    constexpr auto
    operator - (
        char_<auto> const & __min, 
        char_<auto> const & __max)
    {
        return
        between { __min, __max } ;
    }

    
    constexpr auto
    operator ! (
        auto const & __right)
    requires regex_marked<decltype(__right)>
    {
        return
        not_ { __right } ; 
    }


    constexpr auto
    operator | (
        auto const & __left, 
        auto const & __right)
    requires 
        all_regex_marked<decltype(__left), decltype(__right)>
     or all_syntax_marked<decltype(__left), decltype(__right)>
    {        
        if constexpr (!or_marked<decltype(__left)> && !or_marked<decltype(__right)>)
            return or_ { std::tuple { __left, __right } } ;
        else 
        if constexpr (or_marked<decltype(__left)> && !or_marked<decltype(__right)>)
            return  or_ { std::tuple_cat(__left.choices, std::tuple { __right }) } ;
        else 
        if constexpr (!or_marked<decltype(__left)> && or_marked<decltype(__right)>)
            return  or_ { std::tuple_cat(std::tuple { __left }, __right.choices) } ;    
        else 
        if constexpr (or_marked<decltype(__left)> && or_marked<decltype(__right)>)
            return  or_ { std::tuple_cat(__left.choices, __right.choices) } ;
    }

    
    constexpr auto
    operator > (
        auto const & __left, 
        auto const & __right)
    requires 
        all_regex_marked<decltype(__left), decltype(__right)>
     or all_syntax_marked<decltype(__left), decltype(__right)>
    {        
        if constexpr (!and_marked<decltype(__left)> && !and_marked<decltype(__right)>)
            return and_ { std::tuple { __left, __right } } ;
        else 
        if constexpr (and_marked<decltype(__left)> && !and_marked<decltype(__right)>)
            return  and_ { std::tuple_cat(__left.items, std::tuple { __right }) } ;
        else 
        if constexpr (!and_marked<decltype(__left)> && and_marked<decltype(__right)>)
            return  and_ { std::tuple_cat(std::tuple { __left }, __right.items) } ;    
        else 
        if constexpr (and_marked<decltype(__left)> && and_marked<decltype(__right)>)
            return  and_ { std::tuple_cat(__left.items, __right.items) } ;
    }



    template<typename type_t>
    concept bool regex_or_syntax_marked =
        regex_marked<type_t> or syntax_marked<type_t> ; 




    constexpr auto
    operator ~ (
        auto const & __right)
    requires regex_or_syntax_marked<std::decay_t<decltype(__right)>>
    {   
        return
        some { __right } ;
    }


    constexpr auto
    operator + (
        auto const & __right)
    requires regex_or_syntax_marked<std::decay_t<decltype(__right)>>
    {   
        return
        (~ __right)(1, std::numeric_limits<size_t>::max()) ;
    }


    constexpr auto
    operator * (
        auto const & __right)
    requires regex_or_syntax_marked<std::decay_t<decltype(__right)>>
    {   
        return
        (~ __right)(0, std::numeric_limits<size_t>::max()) ;
    }



    constexpr auto
    chr (
        auto const & __c)
    {        
        return 
        char_ { __c } ;
    }


    template <
        typename char_t, 
        size_t size_c,
        template <
            typename, 
            size_t...> 
        typename sequence_t,
        size_t ... idx_c>
    constexpr auto 
    __chrs_impl (
        char_t const (&__chrs) [size_c], 
        sequence_t<size_t, idx_c...>)
    {
        return 
        ( chr(__chrs[idx_c]) > ... ) ;
    }

    template<
        typename char_t, 
        size_t size_c>
    constexpr auto
    chrs (
    char_t const (&__chrs) [size_c])
    {
        return 
        __chrs_impl (
            __chrs, 
            std::make_index_sequence<size_c - 1>{}) ;
    }




    template <
        typename declaration_t, 
        typename definition_t>
    struct rule  
    {
        declaration_t declaration ;
        definition_t definition ;
    } ; 

    template <typename declaration_t, typename definition_t>
    rule (declaration_t const&, definition_t const&) -> rule<std::decay_t<declaration_t>, std::decay_t<definition_t>> ;  



    constexpr auto
    operator <= (
        auto const & __declaration, 
        auto const & __production) 
    requires all_syntax_marked<decltype(__declaration), decltype(__production)>
    {
        return 
        rule { __declaration, __production } ;
    }


    template <
        typename start_t, 
        typename ... rule_t> 
    struct grammar_rules 
    {
        start_t start;
        std::tuple<rule_t...> rules ;
    } ;

    template <typename start_t, typename ... rule_t> 
    grammar_rules (start_t const &, std::tuple<rule_t...> const &) -> grammar_rules<std::decay_t<start_t>, rule_t...> ;

    constexpr auto 
    grammar (
        auto const & __start,
        rule<auto, auto> const & ... __rules)
    {
        return 
        grammar_rules { __start, std::tuple { __rules... } } ;
    }
    
    
    template <
        auto id_c>
    struct leaf 
    { 
        using syntax_marker = void ;
        using regex_marker = void ;

        decltype(id_c) id { id_c } ; 
    } ;   

    template<
        typename declaration_t>
    struct node
    {
        using regex_marker = void ;
        using syntax_marker = void ;
    } ;


    template<
        typename id_t,
        typename regex_t>
    struct word
    {
        id_t id ;
        regex_t reg ;
    } ;

    template<
        typename id_t, 
        typename regex_t>
    word (id_t, regex_t&&) -> word<id_t, std::decay_t<regex_t>> ;



    template <
        typename lang_t,
        typename iterator_t>
    struct token
    {
        std::pair<iterator_t, iterator_t> data ;
        lang_t id ;

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
        typename lang_t, 
        typename iterator_t>
    token (std::pair<iterator_t, iterator_t> const &, lang_t) -> token<lang_t, iterator_t> ;
}


#endif