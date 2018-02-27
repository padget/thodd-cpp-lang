#ifndef __thodd_element_has_hpp__
#  define __thodd_element_has_hpp__

#  include "../stream/utils.hpp"
#  include <vector>
#  include <tuple>
#  include "../lexer/lexem.hpp"


#  define THODD_HAS_XXX(name, type) \
  std::tuple<bool, auto> has_##name (auto begin, auto end) {\
    return has_element(begin, end, {lexem::type_::##type}) ;\
  } \




namespace thodd::element {
  constexpr auto has_idx = 0u ;
  constexpr auto cursor_idx = 1u ;

  namespace detail {
    auto lexem_type_comparator () { 
      return [] (auto const & lexem, auto const & type) { 
        return lexem.type == type ;
      } ; 
    }
  }

  std::tuple<bool, auto> has_element (auto begin, auto end, std::vector<lexem::type_> ids) {
    auto && has = stream::start_with(begin, end, ids.begin(), ids.end(), lexem_type_comparator()) ;
    return std::make_tuple(has, has ? std::next(begin, ids.size()) : begin) ;
  }

  std::tuple<bool, auto> has_elements (auto begin, auto end, auto rx, auto rx_sep) {
    auto && rx_res = rx(begin, end) ;
    auto && rx_sep_res = rx_sep(std::get<cursor_idx>(rx_res), end) ;
    auto && has = std::get<has_idx>(rx_res) && std::get<has_idx>(rx_sep_res) ; 
    return has ? has_elements(std::get<cursor_idx>(rx_sep_res), end, rx, rx_sep) : rx_res ;
  }

  std::tuple<bool, auto> has_elements (auto begin, auto end, auto rx) {
    auto && rx_res = rx(begin, end) ; 
    return std::get<has_idx>(rx_res) ? has_elements(std::get<cursor_idx>(rx_res), end, rx) : rx_res ;
  }

  THODD_HAS_XXX(lbracket, lbracket)
  THODD_HAS_XXX(rbracket, rbracket)
  THODD_HAS_XXX(lbrace, lbrace)
  THODD_HAS_XXX(rbrace, rbrace)
  THODD_HAS_XXX(colon, colon)
  THODD_HAS_XXX(semi_colon, semi_colon)
  THODD_HAS_XXX(comma, comma)
  THODD_HAS_XXX(point, point)
  THODD_HAS_XXX(identifier, identifier)
  THODD_HAS_XXX(string, string)
  THODD_HAS_XXX(number, number)

  THODD_HAS_XXX(pod, pod_kw)
  THODD_HAS_XXX(main, main_kw)
  THODD_HAS_XXX(reader, reader_kw)
  THODD_HAS_XXX(writer, writer_kw)
  THODD_HAS_XXX(listener, listener_kw)
  THODD_HAS_XXX(processor, processor_kw)
  THODD_HAS_XXX(builder, builder_kw)
  THODD_HAS_XXX(return, return_kw)

  std::tuple<bool, auto> has_const_instruction(auto begin, auto end) {
    auto && type = has_identifier(begin, end) ;
    auto && name = has_identifier(std::get<cursor_idx>(type), end) ;
    auto && expr = has_expression(std::get<cursor_idx>(name), end) ;
    auto && scol = has_semi_colon(std::get<cursor_idx>(expr), end) ;
    auto && has = std::get<has_idx>(type) && 
                  std::get<has_idx>(name) && 
                  std::get<has_idx>(expr) &&
                  std::get<has_idx>(scol) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(scol) : begin) ;
  }

  std::tuple<bool, auto> has_return_instruction(auto begin, auto end) {
    auto && return_ = has_return(begin, end) ;
    auto && expr = has_expression(std::get<cursor_idx>(return_), end) ;
    auto && scol = has_semi_colon(std::get<cursor_idx>(expr), end) ;
    auto && has = std::get<has_idx>(return_) && 
                  std::get<has_idx>(expr) &&
                  std::get<has_idx>(scol) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(scol) : begin) ;
  }

  std::tuple<bool, auto> has_access (auto begin, auto end) {
    auto && name = has_identifier(begin, end) ;
    auto && point = has_point(std::get<cursor_idx>) ;
    auto && members = has_elements(std::get<cursor_idx>(point), end, 
                                   [] (auto begin, auto end) {return has_identifier(begin, end) ;}, 
                                   [] (auto begin, auto end) {return has_point(begin, end) ;});
    auto && has = std::get<has_idx>(name) &&
                  std::get<has_idx>(point) && 
                  std::get<has_idx>(members) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(members) : begin) ;
  }

  std::tuple<bool, auto> has_function_call (auto begin, auto end) {

  }

  std::tuple<bool, auto> has_expression (auto begin, auto end) {

  } 

  std::tuple<bool, auto> has_member (auto begin, auto end) {

  }

  std::tuple<bool, auto> has_pod (auto begin, auto end) {

  }

  std::tuple<bool, auto> has_function (auto begin, auto end) {

  }

  std::tuple<bool, auto> has_flow (auto begin, auto end) {

  }

  

}

#endif 