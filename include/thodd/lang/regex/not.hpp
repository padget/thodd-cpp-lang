#ifndef __THODD_LANG_REGEX_NOT_HPP__
#  define __THODD_LANG_REGEX_NOT_HPP__

#  include <thodd/lang/regex/regex.hpp>
#  include <utility>
#  include <type_traits> 

namespace 
thodd::regex 
{
    template<
        typename regex_t>
    struct not_
    {
        using regex_marker = not_ ;
        
        regex_t reg ;
    } ;

    
   
}

#endif