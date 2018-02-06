#ifndef __thodd_optimise_function_hpp__
#  define __thodd_optimise_function_hpp__


/**
 foo(name: string, firstname: string) : person {
   return make_person(upper(name), lower(firstname)) ;
 }
                    <=> 
 foo(name: string, firstname: string) : person {
   dlkajzd : [@]string upper(name) ;
   qlksjdl : [@]string lower(firstname) ;
   return make_person(dlkajzd, qlksjdl) ;
 }
 */

function optimise_function (function const & f) {
  auto && name = f.name ;
  auto && type = f.type ;
  auto && paramters = f.paramters ;
  auto && consts = f.consts ;
  auto && return_ = f.return_ ;

  return function{name, type, paramters, consts, return_} ;
}


#endif