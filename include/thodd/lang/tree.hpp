#ifndef __THODD_LANG_TREE_HPP__
#  define __THODD_LANG_TREE_HPP__

#  include <utility>
#  include <list>

namespace 
thodd::lang
{
    template <
        typename language_t, 
        typename iterator_t>
    struct tree 
    {
        language_t id ;
        std::pair<iterator_t, iterator_t> data ;
        std::list<tree<language_t, iterator_t>> childs ;
    } ;
}

#endif