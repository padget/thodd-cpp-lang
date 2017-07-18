#ifndef __THODD_LANG_CORE_TOKEN_HPP__
#  define __THODD_LANG_CORE_TOKEN_HPP__

#  include <utility>
#  include <memory>
#  include <type_traits>

namespace 
thodd
{
    template<
        typename lang_t,
        typename iterator_t>
    struct token
    {
        lang_t tid ;
        std::pair<iterator_t, iterator_t> data ;
        std::unique_ptr<token> next ;
    } ;



    inline auto 
    make_token(
        auto && __tid, 
        auto && __begin, 
        auto && __end)
    {
        return 
        token<
            std::decay_t<decltype(__tid)>, 
            std::decay_t<decltype(__begin)>>
        { static_cast<decltype(__tid)&&>(__tid), 
          { static_cast<decltype(__begin)&&>(__begin), 
            static_cast<decltype(__end)&&>(__end) } } ;
    }



    inline auto 
    append(
        token<auto, auto> & __token, 
        token<auto, auto> && __next)
    -> decltype(auto)
    {
        return 
        (__token.next = std::move(std::make_unique(__next))) ;
    }
}

#endif