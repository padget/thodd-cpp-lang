#ifndef __thodd_lexer_lexem_hpp__
#  define __thodd_lexer_lexem_hpp__

#  include <string>

namespace thodd::lexer {

  struct lexem {
    enum class type_ : int {
      // Characters
      identifier, 
      number, string, 
      lbrace, rbrace, 
      lbracket, rbracket,
      lsbracket, rsbracket,
      colon, semi_colon, 
      point, comma, ignored, error,
      // Keywords
      flow_kw, reader_kw, writer_kw, 
      processor_kw, builder_kw, listener_kw, 
      pod_kw, return_kw, main_kw
    } ;

    type_ type ;
    std::string data ;
  } ;

}

#endif