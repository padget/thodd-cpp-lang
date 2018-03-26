#ifndef __thodd_element_extract_hpp__
#  define __thodd_element_extract_hpp__

#  include "element.hpp"
#  include "has.hpp"
#  include "../stream/utils.hpp"
#  include <optional>

#  define THODD_EXTRACTED(name, it) extracted<name, decltype(it)>

namespace thodd::element {

  template <typename ext_t, 
            typename it_t>
  struct extracted {
    std::optional<ext_t> opt ;
    it_t it ;
  } ;

  auto make_extracted(auto ext, auto it) {
    return extracted<decltype(ext), decltype(it)>{ext, it} ;
  }


  auto extracts (auto begin, auto end, auto extractor, auto has_sep) -> std::vector<decltype(extractor(begin, end))> {    
    auto && ext_res = extractor(begin, end) ;
    auto && sep_res = has_sep(ext_res.it, end) ;

    if (ext_res.has_value() && std::get<has_idx>(sep_res)) 
      return stream::concat(extracts(std::get<cursor_idx>(sep_res), end, extractor, has_sep), std::vector<decltype(ext_res)>{ext_res});
    else 
      return {ext_res} ;
  }

  auto extract_identifier(auto begin, auto end) -> THODD_EXTRACTED(identifier, begin) {
    auto && ident = has_identifier(begin, end) ;
    return std::get<has_idx>(ident) ?
      make_extracted(identifier{(*begin).data}, std::get<cursor_idx>(ident)) :
      make_extracted({}, begin) ;
  }

  auto extract_number(auto begin, auto end) -> THODD_EXTRACTED(number, begin) {
    auto && nb = has_number(begin, end) ;
    return std::get<has_idx>(nb) ?
      make_extracted(number{(*begin).data}, std::get<cursor_idx>(nb)) :
      make_extracted({}, begin) ;
  }
  
  auto extract_string(auto begin, auto end) -> THODD_EXTRACTED(string, begin) {
    auto && str = has_identifier(begin, end) ;
    return std::get<has_idx>(str) ?
      make_extracted(string{(*begin).data}, std::get<cursor_idx>(str)) :
      make_extracted({}, begin) ;
  }

  auto extract_expression(auto begin, auto end) -> THODD_EXTRACTED(expression, begin) ;

  auto extract_function_call(auto begin, auto end) -> THODD_EXTRACTED(function_call, begin) {
    auto && fcall = has_function_call(begin, end) ;

    auto && name     = extract_identifier(begin, std::get<cursor_idx>(fcall)) ;
    auto && lbracket = has_lbracket(std::get<cursor_idx>(name), std::get<cursor_idx>(fcall)) ;
    auto && args     = extracts(std::get<cursor_idx>(lbracket), std::get<cursor_idx>(fcall), 
                                [] (auto begin, auto end) {return extract_expression(begin, end) ;}, 
                                [] (auto begin, auto end) {return has_comma(begin, end) ;}) ;
    auto && rbracket = has_rbracket(std::get<cursor_idx>(args), std::get<cursor_idx>(fcall)) ;
    return std::get<has_idx>(fcall) ?
      make_extracted(function_call{name, args}, std::get<cursor_idx>(fcall)) :
      make_extracted({}, begin) ;
  }

  auto extract_expression(auto begin, auto end) -> THODD_EXTRACTED(expression, begin) {

  }
}

#endif