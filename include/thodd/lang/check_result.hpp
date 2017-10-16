#ifndef __THODD_LANG_CHECK_RESULT_HPP__
#  define __THODD_LANG_CHECK_RESULT_HPP__

namespace 
thodd::lang
{
    template<
        typename iterator_t>
    struct check_result
    {
        bool checked ; 
        iterator_t local_cursor ; 
    } ;
    
    template <typename iterator_t>
    check_result (bool, iterator_t) -> check_result<iterator_t> ;
}

#endif