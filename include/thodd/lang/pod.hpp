#ifndef __THODD_LANG_POD_HPP__
#  define __THODD_LANG_POD_HPP__

#  include <tuple>
#  include <limits>
#  include <utility>
#  include <type_traits>
#  include <algorithm>

#  include <thodd/tuple/indexof.hpp>


namespace
thodd::lang
{
    /// CHAR_
    template <
        auto char_c>
    struct chr
    {
        using regex_marker = void ;
    } ;
    

    /// NOT_
    template<
        typename item_t>
    struct not_
    {
        using regex_marker = void ;
    } ;



    /// BETWEEN
    template<
        auto min_c, 
        auto max_c>
    struct between
    {
        using regex_marker = void ;
    } ;


    
    /// OR_
    template <
        typename ... choice_t>
    struct or_  
    {
        using regex_marker = void ;
        using syntax_marker = void ;
        using or_marker = void ;
    } ;
    


    // AND_
    template <
        typename ... item_t>
    struct and_ 
    {
        using regex_marker = void ;
        using syntax_marker = void ;
        using and_marker = void ;
    } ;

    
    template <
        auto min_c, 
        auto max_c>
    struct bounds {} ;


    /// SOME
    template <
        typename item_t, 
        auto min_c = 0, 
        auto max_c = std::numeric_limits<size_t>::max()>
    struct some 
    {
        using regex_marker = void ;
        using syntax_marker = void ;
    
        template <
            auto omin_c, 
            auto omax_c>
        constexpr some<item_t, omin_c, omax_c>
        operator () (
            bounds<omin_c, omax_c> const &) const
        { return {} ; }
    } ;


    template <typename regex_t>
    concept bool or_marked = requires { typename std::decay_t<regex_t>::or_marker ; } ;

    template <typename regex_t>
    concept bool and_marked = requires { typename std::decay_t<regex_t>::and_marker ; } ;

    template <typename regex_t>
    concept bool regex_marked = requires { typename std::decay_t<regex_t>::regex_marker ; } ;

    template <typename ... regex_t>
    concept bool all_regex_marked = (regex_marked<regex_t> && ...) ;



    template <typename syntax_t>
    concept bool syntax_marked = requires { typename std::decay_t<syntax_t>::syntax_marker ; } ;

    template <typename ... syntax_t>
    concept bool all_syntax_marked = (syntax_marked<syntax_t> && ...) ;


    template<typename type_t>
    concept bool regex_or_syntax_marked =
        regex_marked<type_t> or syntax_marked<type_t> ; 


    template<auto min_c, auto max_c>
    constexpr auto
    operator - (
        chr<min_c> const & __min, 
        chr<max_c> const & __max)
    -> between<min_c, max_c>
    { return {} ; }

    
    constexpr auto
    operator ! (
        auto const & __right)
    -> not_<std::decay_t<decltype(__right)>>
    requires regex_marked<decltype(__right)>
    
    { return {} ; }

    
    namespace 
    meta 
    {
        template<
            typename added_t,
            template <typename ... > typename pack_t,
            typename ... type_t>
        constexpr pack_t <type_t..., added_t>
        push_back (pack_t<type_t...> const &)
        { return {} ; } 


        template<
            typename added_t,
            template <typename ... > typename pack_t, 
            typename ... type_t>
        constexpr pack_t <added_t, type_t...>
        push_front (pack_t<type_t...> const &)
        { return {} ; } 

        template<
            template <typename ... > typename pack_t, 
            typename ... left_t, 
            typename ... right_t>
        constexpr pack_t <left_t..., right_t...>
        concat (pack_t<left_t...> const &, pack_t<right_t...> const &)
        { return {} ; } 
    }


    constexpr auto
    operator | (
        auto const & __left, 
        auto const & __right)
    requires 
        all_regex_marked<decltype(__left), decltype(__right)>
     or all_syntax_marked<decltype(__left), decltype(__right)>
    {        
        using left_t = decltype(__left) ;
        using right_t = decltype(__right) ;

        if constexpr (!or_marked<left_t> && !or_marked<right_t>)
            return or_ <
                    std::decay_t<decltype(__left)>, 
                    std::decay_t<right_t>> {} ;
        else if constexpr (or_marked<decltype(__left)> && !or_marked<right_t>)
            return meta::push_back <std::decay_t<right_t>> (__left) ;
        else if constexpr (!or_marked<decltype(__left)> && or_marked<right_t>)
            return meta::push_front <std::decay_t<decltype(__left)>> (__right) ;
        else if constexpr (or_marked<decltype(__left)> && or_marked<right_t>)
            return meta::concat (__left, __right) ;
    }

    
    constexpr auto
    operator > (
        auto const & __left, 
        auto const & __right)
    requires 
        all_regex_marked<decltype(__left), decltype(__right)>
     or all_syntax_marked<decltype(__left), decltype(__right)>
    {        
        using left_t = decltype(__left) ;
        using right_t = decltype(__right) ;

        if constexpr (!and_marked<left_t> && !and_marked<right_t>)
            return and_ <
                    std::decay_t<decltype(__left)>, 
                    std::decay_t<right_t>> {} ;
        else if constexpr (and_marked<decltype(__left)> && !and_marked<right_t>)
            return meta::push_back <std::decay_t<right_t>> (__left) ;
        else if constexpr (!and_marked<decltype(__left)> && and_marked<right_t>)
            return meta::push_front <std::decay_t<decltype(__left)>> (__right) ;
        else if constexpr (and_marked<decltype(__left)> && and_marked<right_t>)
            return meta::concat (__left, __right) ;
    }



    




    constexpr auto
    operator ~ (
        auto const & __right)
    -> some<std::decay_t<decltype(__right)>>
    requires regex_or_syntax_marked<std::decay_t<decltype(__right)>>
    { return {} ; }


    constexpr auto
    operator * (
        auto const & __right)
    requires regex_or_syntax_marked<std::decay_t<decltype(__right)>>
    { return (~__right)(bounds<0, std::numeric_limits<size_t>::max()>{}) ; }

    
    constexpr auto
    operator + (
        auto const & __right)
    requires regex_or_syntax_marked<std::decay_t<decltype(__right)>>
    { return (~__right)(bounds<1, std::numeric_limits<size_t>::max()>{}) ; }


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
    struct rule { } ; 


    constexpr auto
    operator <= (
        auto const & __declaration, 
        auto const & __definition) 
    -> rule<std::decay_t<decltype(__declaration)>, std::decay_t<decltype(__definition)>>
    requires all_syntax_marked<decltype(__declaration), decltype(__definition)>
    { return {} ; }


    
    template <
        auto id_t, 
        typename regex_t>
    struct terminal 
    { 
        using syntax_marker = void ;
        using regex_marker = void ;
    } ;   

    template <
        auto id_c>
    constexpr auto
    term (
        auto const & __item)
    -> terminal<id_c, std::decay_t<decltype(__item)>>
    { return {} ; }


    template <
        auto id_t>
    struct error_terminal 
    { 
        using syntax_marker = void ;
        using regex_marker = void ; 
    } ;   


    template <
        auto id_c>
    constexpr auto
    error_term ()
    -> error_terminal<id_c>
    { return {} ; }
    

    template<
        auto id_t>
    struct non_terminal
    {
        using regex_marker = void ;
        using syntax_marker = void ;
    } ;


    template <
        auto id_c>
    constexpr auto
    non_term ()
    -> non_terminal<id_c>
    { return {} ; }


   template <
        typename start_t, 
        typename ... rule_t> 
    struct grammar_rules {} ;


    constexpr auto 
    grammar (
        auto const & __start,
        rule<auto, auto> const & ... __rules)
    {
        return 
        grammar_rules <
            std::decay_t <decltype(__start)>, 
            std::decay_t <decltype(__rules)> ... > {} ;
    }
        

    
    

    namespace 
    meta
    {
        template <   
            auto id_c>
        constexpr auto 
        id (terminal<id_c, auto> const &) 
        { return id_c ; }

        template <auto id_c, typename regex_t>
        constexpr auto
        item (terminal<id_c, regex_t> const &)
        -> regex_t
        { return {} ; }


        template <   
            auto id_c>
        constexpr auto 
        id (error_terminal<id_c> const &) 
        { return id_c ; }

        template <   
            auto id_c>
        constexpr auto 
        id (non_terminal<id_c> const &) 
        { return id_c ; }

        template < 
            typename declaration_t>
        constexpr declaration_t 
        declaration (rule<declaration_t, auto> const &)
        { return {} ; }

        template < 
            typename definition_t>
        constexpr definition_t 
        definition (rule<auto, definition_t> const &)
        { return {} ; }
    

        template <
            typename start_t, 
            typename ... rule_t>
        constexpr start_t
        start (
            grammar_rules<start_t, rule_t...> const & __grammar)
        { return {} ; }


        constexpr auto
        extract_if (
            auto const & __predicate,
            auto const & __first,
            auto const & ... __next)
        {
            if constexpr (__predicate (__first))
                return __first ;
            else 
                return extract_if (__predicate, __next...) ;
        }
    }
        

    template <
        typename ... declaration_t, 
        typename ... definition_t>
    constexpr auto 
    extract_definition (
        auto const & __declaration,
        grammar_rules<auto, rule<declaration_t, definition_t>...> const & __grammar)
    -> decltype(auto)
    { 
        return
        meta::definition (
            meta::extract_if (
                [] (auto const & __rule) constexpr
                { return meta::id (std::decay_t<decltype(__declaration)>{}) == meta::id(meta::declaration(__rule)); },
                rule<declaration_t, definition_t> {} ... ));
    }


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