#ifndef __THODD_LANG_MATCH_RESULT_HPP__
#  define __THODD_LANG_MATCH_RESULT_HPP__

namespace 
thodd::lang
{
    template<
        typename iterator_t>
    struct match_result
    {
        bool matched ; 
        iterator_t local_cursor ; 
    } ; 

    template <typename iterator_t>
    match_result (bool, iterator_t) -> match_result<iterator_t> ;
}

#endif