#ifndef __THODD_LANG_REGEX_SOME_HPP__
#  define __THODD_LANG_REGEX_SOME_HPP__

#  include <type_traits>
#  include <utility>
#  include <limits>

#  include <thodd/lang/regex/regex.hpp>

namespace 
thodd::regex
{
    template<
        typename regex_t>
    struct some
    {
        using regex_marker = some ;
        
        regex_t reg ;

        std::size_t min { 0 } ;
        std::size_t max { 1 } ; 

        constexpr some
        operator () (
            std::size_t const& __min, 
            std::size_t const& __max) const
        {
            return 
            { reg, __min, __max } ;
        }
    } ;


    constexpr auto
    operator ~ (
        auto&& __regex)
    requires regex_based<decltype(__regex)>
    {   
        return
        some<std::decay_t<decltype(__regex)>>
        { std::forward<decltype(__regex)>(__regex) } ;
    }


    constexpr auto
    operator + (
        auto&& __regex)
    requires regex_based<decltype(__regex)>
    {   
        return
        (~std::forward<decltype(__regex)>(__regex))(1, std::numeric_limits<size_t>::max()) ;
    }


    constexpr auto
    operator * (
        auto&& __regex) 
    requires regex_based<decltype(__regex)>
    {   
        return
        (~std::forward<decltype(__regex)>(__regex))(0, std::numeric_limits<size_t>::max()) ;
    }
}


#endif