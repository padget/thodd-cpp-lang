#ifndef __THODD_LANG_POD_HPP__
#  define __THODD_LANG_POD_HPP__

#  include <tuple>
#  include <limits>
#  include <utility>
#  include <type_traits>


namespace
thodd::lang::pod
{
    template <
        typename char_t >
    struct char_
    {
        using regex_marker = void ;
        
        char_t c ;
    } ;


    template<
        typename item_t>
    struct not_
    {
        using regex_marker = void ;
        
        item_t item ;
    } ;


    template<
        typename min_t, 
        typename max_t>
    struct between
    {
        using regex_marker = void ;

        char_<min_t> min ;  
        char_<max_t> max ;
    } ;


    template <
        typename ... choice_t>
    struct or_  
    {
        using regex_marker = void ;
        using syntax_marker = void ;
        
        std::tuple<choice_t...> choices ;        
    } ;

    template <
        typename ... item_t>
    struct and_ 
    {
        using regex_marker = void ;
        using syntax_marker = void ;
        
        std::tuple<item_t...> items ;
    } ;

    template <
        typename item_t>
    struct some 
    {
        using regex_marker = void ;
        using syntax_marker = void ;
        
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
        between<std::decay_t<decltype(__min)>, std::decay_t<decltype(__max)>> 
        { __min, __max } ;
    }

    
    
    
    constexpr auto
    operator ! (
        auto const & __right)
    requires regex_marked<decltype(__right)>
    {
        return
        not_<std::decay_t<decltype(__right)>> { __right } ; 
    }







    constexpr auto
    operator | (
        auto const & __left, 
        auto const & __right)
    requires 
        all_regex_marked<decltype(__left), decltype(__right)>
     or all_syntax_marked<decltype(__left), decltype(__right)>
    {        
        return
        or_<std::decay_t<decltype(__left)>, std::decay_t<decltype(__right)>> 
        { { __left, __right } } ;
    }

    template<
        typename ... choice_t>
    constexpr auto
    operator | (
        or_<choice_t...> const & __or,
        auto const & __right)
    requires 
        all_regex_marked<decltype(__right), choice_t...>
     or all_syntax_marked<decltype(__right), choice_t...>
    {
        return 
        or_<choice_t..., std::decay_t<decltype(__right)>>
        { std::tuple_cat(__or.choices, std::tuple(__right)) } ;
    }

    template<
        typename ... choice_t>
    constexpr auto
    operator | (
        auto const & __left, 
        or_<choice_t...> const & __or)
    requires 
        all_regex_marked<decltype(__left), choice_t...>
     or all_syntax_marked<decltype(__left), choice_t...>
    {
        return 
        or_<std::decay_t<decltype(__left)>, choice_t...>
        { std::tuple_cat(std::tuple(__left), __or.choices) } ;
    }

    template<
        typename ... choice_t, 
        typename ... choice2_t>
    constexpr auto
    operator | (
        or_<choice_t...> const & __or,
        or_<choice2_t...> const & __or2)
    requires 
        all_regex_marked<choice_t..., choice2_t...>
     or all_syntax_marked<choice_t..., choice2_t...>
    {
        return 
        or_<choice_t..., choice2_t...>
        { std::tuple_cat(__or.choices, __or2.choices) } ;
    }






    constexpr auto
    operator > (
        auto const & __left, 
        auto const & __right)
    requires 
        all_regex_marked<decltype(__left), decltype(__right)>
     or all_syntax_marked<decltype(__left), decltype(__right)>
    {        
        return
        and_<std::decay_t<decltype(__left)>, std::decay_t<decltype(__right)>> 
        { { __left, __right } } ;
    }

    template<
        typename ... item_t>
    constexpr auto
    operator > (
        and_<item_t...> const & __and,
        auto const & __right)
    requires 
        all_regex_marked<decltype(__right), item_t...>
     or all_syntax_marked<decltype(__right), item_t...>
    {
        return 
        and_<item_t..., std::decay_t<decltype(__right)>>
        { std::tuple_cat(__and.choices, std::tuple(__right)) } ;
    }

    template<
        typename ... item_t>
    constexpr auto
    operator > (
        auto const & __left, 
        and_<item_t...> const & __and)
    requires 
        all_regex_marked<decltype(__left), item_t...>
     or all_syntax_marked<decltype(__left), item_t...>
    {
        return 
        and_<std::decay_t<decltype(__left)>, item_t...>
        { std::tuple_cat(std::tuple(__left), __and.choices) } ;
    }

    template<
        typename ... item_t, 
        typename ... item2_t>
    constexpr auto
    operator > (
        and_<item_t...> const & __and,
        and_<item2_t...> const & __and2)
    requires 
        all_regex_marked<item_t..., item2_t...>
     or all_syntax_marked<item_t..., item2_t...>
    {
        return 
        and_<item_t..., item2_t...>
        { std::tuple_cat(__and.choices, __and2.choices) } ;
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
        some<std::decay_t<decltype(__right)>> { __right } ;
    }


    constexpr auto
    operator + (
        auto const & __right)
    requires regex_or_syntax_marked<std::decay_t<decltype(__right)>>
    {   
        return
        some<std::decay_t<decltype(__right)>> 
        { __right, 1, std::numeric_limits<size_t>::max() } ;
    }


    constexpr auto
    operator * (
        auto const & __right)
    requires regex_or_syntax_marked<std::decay_t<decltype(__right)>>
    {   
        return
        some<std::decay_t<decltype(__right)>> 
        { __right, 0, std::numeric_limits<size_t>::max() } ;
    }



    constexpr auto
    chr (
        auto const & __c)
    {        
        return 
        char_<std::decay_t<decltype(__c)>> { __c } ;
    }




    template <
        typename declaration_t, 
        typename definition_t>
    struct rule  
    {
        declaration_t declaration ;
        definition_t definition ;
    } ; 

    constexpr auto
    operator <= (
        auto const & __declaration, 
        auto const & __production) 
    requires all_syntax_marked<decltype(__declaration), decltype(__production)>
    {
        return 
        rule<std::decay_t<decltype(__declaration)>, std::decay_t<decltype(__production)>> 
        { __declaration, __production } ;
    }



    template <
        typename start_t, 
        typename ... rules_t> 
    struct grammar_rules 
    {
        start_t start;
        std::tuple<rules_t...> rules ;
    } ;

    
    constexpr auto 
    grammar (
        auto const & __start,
        rule<auto, auto> const & ... __rules)
    {
        return 
        grammar_rules<std::decay_t<decltype(__start)>, std::decay_t<decltype(__rules)>...> 
        { __start, { __rules... } } ;
    }
    
    
    template<
        auto id_c>
    struct leaf 
    { 
        using syntax_marked = void ;
        using regex_marked = void ;
        static const decltype(id_c) id { id_c } ; 
    } ;    

    template<
        typename declaration_t>
    struct node
    {
        using regex_marked = void ;
        using syntax_marked = void ;
    } ;
}


#endif