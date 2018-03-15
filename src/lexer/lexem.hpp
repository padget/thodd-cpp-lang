#ifndef __thodd_lexer_lexem_hpp__
#  define __thodd_lexer_lexem_hpp__

#  include <string>

namespace thodd::lexer {

  struct lexem {
    enum class type_ : int {
      // Characters
      identifier, // 0 
      number, // 1
      string, // 2
      lbrace, // 3
      rbrace, // 4
      lbracket, // 5
      rbracket,// 6
      lsbracket, // 7
      rsbracket,// 8
      colon, // 9
      semi_colon,// 10 
      point, // 11
      comma, // 12
      ignored, // 13
      error,// 14
      // Keywords
      flow_kw, // 15
      reader_kw, // 16
      writer_kw, // 17
      processor_kw, // 18
      builder_kw, // 19
      listener_kw, // 20
      pod_kw, // 21
      return_kw, // 22
      main_kw// 23
    } ;

    type_ type ;
    std::string data ;
  } ;

}

#endif