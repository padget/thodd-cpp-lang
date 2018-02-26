#ifndef __thodd_lexer_lexem_hpp__
#  define __thodd_lexer_lexem_hpp__

#  include <string>

namespace tdd::lexer {

  struct lexem {
    enum class type_ : int {
      identifier, 
      number, string, 
      lbrace, rbrace, 
      lbracket, rbracket,
      lsbracket, rsbracket,
      colon, semi_colon, 
      point, comma, ignored, 
      pure_kw, impure_kw, pod_kw, error,
      weak, strengh, alias, 
      switch_kw, case_kw, return_kw, 
      iterate_kw, iterate_if_kw
    } ;

    type_ type ;
    std::string data ;
  } ;

}

#endif