#ifndef __thodd_element_extract_hpp__
#  define __thodd_element_extract_hpp__

#  include "element.hpp"
#  include "has.hpp"

#  define THODD_EXTRACTED(name, it) extracted<name, decltype(it)>

namespace thodd::element {

  template <
    typename ext_t, 
    typename it_t>
  struct extracted {
    std::optional<ext_t> ext ;
    it_t it ;
  } ;

  auto make_extracted(auto ext, auto it) {
    return extracted<decltype(ext), decltype(it)>{ext, it} ;
  }

  template<typename ret_t>
  std::vector<ret_t> extracts (auto begin, auto end, auto has_sep) {
    auto && rx_res = extract<ret_t> (begin, end) ;
    auto && rx_sep_res = rx_sep(std::get<cursor_idx>(rx_res), end) ;
    auto && has = std::get<has_idx>(rx_res) && std::get<has_idx>(rx_sep_res) ; 
    return has ? has_elements(std::get<cursor_idx>(rx_sep_res), end, rx, rx_sep) : rx_res ;
  }

  std::tuple<bool, auto> has_elements (auto begin, auto end, auto rx) {
    auto && rx_res = rx(begin, end) ; 
    return std::get<has_idx>(rx_res) ? has_elements(std::get<cursor_idx>(rx_res), end, rx) : rx_res ;
  }

  template <typename ret_t>
  auto extract(auto begin, auto end) -> THODD_EXTRACTED(ret_t, begin) ;

  template <>
  auto extract<identifier>(auto begin, auto end) -> THODD_EXTRACTED(identifier, begin) {
    auto && ident = has_identifier(begin, end) ;
    return std::get<has_idx>(ident) ?
      make_extracted(identifier{(*begin).data}, std::get<cursor_idx>(ident)) :
      make_extracted({}, begin) ;
  }

  template <>
  auto extract<number>(auto begin, auto end) -> THODD_EXTRACTED(number, begin) {
    auto && nb = has_number(begin, end) ;
    return std::get<has_idx>(nb) ?
      make_extracted(number{(*begin).data}, std::get<cursor_idx>(nb)) :
      make_extracted({}, begin) ;
  }
  
  template <>
  auto extract<string>(auto begin, auto end) -> THODD_EXTRACTED(string, begin) {
    auto && str = has_identifier(begin, end) ;
    return std::get<has_idx>(str) ?
      make_extracted(string{(*begin).data}, std::get<cursor_idx>(str)) :
      make_extracted({}, begin) ;
  }
  /*
  struct function_call { 
    identifier name ;
    std::vector<expression> args ;
  } ;
  */
  template <>
  auto extract<function_call>(auto begin, auto end) -> THODD_EXTRACTED(function_call, begin) {
    auto && fcall = has_function_call(begin, end) ;

    auto && name     = extract<identifier>(begin, std::get<cursor_idx>(fcall)) ;
    auto && lbracket = has_lbracket(std::get<cursor_idx>(name), std::get<cursor_idx>(fcall)) ;
    auto && args     = extracts<expression>(std::get<cursor_idx>(lbracket), std::get<cursor_idx>(fcall), 
                                    [] (auto begin, auto end) {return has_expression(begin, end) ;}, 
                                    [] (auto begin, auto end) {return has_comma(begin, end) ;}) ;
    auto && rbracket = has_rbracket(std::get<cursor_idx>(args), std::get<cursor_idx>(fcall)) ;
    return std::get<has_idx>(fcall) ?

      make_extracted(string{(*begin).data}, std::get<cursor_idx>(str)) :
      make_extracted({}, begin) ;
  }
}

#endif