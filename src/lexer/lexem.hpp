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
      new_line, // 15
      // Keywords
      flow_kw, // 16
      reader_kw, // 17
      writer_kw, // 18
      processor_kw, // 19
      builder_kw, // 20
      listener_kw, // 21
      pod_kw, // 22
      return_kw, // 23
      main_kw// 24
    } ;

    type_ type ;
    std::string data ;
    size_t line ;
  } ;

}

#endif