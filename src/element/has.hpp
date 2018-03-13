#ifndef __thodd_element_has_hpp__
#  define __thodd_element_has_hpp__

#  include "../stream/utils.hpp"
#  include <vector>
#  include <tuple>
#  include "../lexer/lexem.hpp"


#  define stringerize(s) #s 

#  define THODD_HAS_XXX(name, type)                            \
  std::tuple<bool, auto> has_##name (auto begin, auto end) {   \
  std::cout << stringerize(has_##name) << std::endl ; \
    return has_element(begin, end, {lexer::lexem::type_::type}) ;   \
  }                                                            \


namespace thodd::element {
  constexpr auto has_idx = 0u ;
  constexpr auto cursor_idx = 1u ;

  namespace detail {
    auto lexem_type_comparator () { 
      return [] (auto const & lexem, auto const & type) { 
        return lexem.type == type ;
      } ; 
    }
    
    std::tuple<bool, auto> if_has (auto const & res, auto end, auto && has_predicate) {
      return std::get<has_idx>(res) ? 
        has_predicate(std::get<cursor_idx>(res), end) 
      : res ;
    }
  }

  std::tuple<bool, auto> has_element (auto begin, auto end, std::vector<lexer::lexem::type_> ids) {
    auto && has = stream::start_with(begin, end, ids.begin(), ids.end(), detail::lexem_type_comparator()) ;
    return std::make_tuple(has, has ? std::next(begin, ids.size()) : begin) ;
  }

  std::tuple<bool, auto> has_elements (auto begin, auto end, auto rx, auto rx_sep) {
    auto && rx_res = rx(begin, end) ;
    auto && rx_sep_res = detail::if_has(rx_res, end, rx_sep) ;
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

  THODD_HAS_XXX(pod_kw, pod_kw)
  THODD_HAS_XXX(main_kw, main_kw)
  THODD_HAS_XXX(reader_kw, reader_kw)
  THODD_HAS_XXX(writer_kw, writer_kw)
  THODD_HAS_XXX(listener_kw, listener_kw)
  THODD_HAS_XXX(processor_kw, processor_kw)
  THODD_HAS_XXX(builder_kw, builder_kw)
  THODD_HAS_XXX(return_kw, return_kw)


  std::tuple<bool, auto> has_expression (auto begin, auto end) ;

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
    auto && return_ = has_return_kw(begin, end) ;
    auto && expr    = has_expression(std::get<cursor_idx>(return_), end) ;
    auto && scol    = has_semi_colon(std::get<cursor_idx>(expr), end) ;
    auto && has = std::get<has_idx>(return_) && 
                  std::get<has_idx>(expr)    &&
                  std::get<has_idx>(scol) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(scol) : begin) ;
  }

  std::tuple<bool, auto> has_access (auto begin, auto end) {
    auto && name    = has_identifier(begin, end) ;
    auto && point   = has_point(std::get<cursor_idx>(name), end) ;
    auto && members = has_elements(std::get<cursor_idx>(point), end, 
                                   [] (auto begin, auto end) {return has_identifier(begin, end) ;}, 
                                   [] (auto begin, auto end) {return has_point(begin, end) ;});
    auto && has = std::get<has_idx>(name)  &&
                  std::get<has_idx>(point) && 
                  std::get<has_idx>(members) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(members) : begin) ;
  }

  std::tuple<bool, auto> has_function_call (auto begin, auto end) {
    auto && name     = has_identifier(begin, end) ;
    auto && lbracket = has_lbracket(std::get<cursor_idx>(name), end) ;
    auto && args     = has_elements(std::get<cursor_idx>(lbracket), end, 
                                    [] (auto begin, auto end) {return has_expression(begin, end) ;}, 
                                    [] (auto begin, auto end) {return has_comma(begin, end) ;}) ;
    auto && rbracket = has_rbracket(std::get<cursor_idx>(args), end) ;
    auto && has = std::get<has_idx>(name)     &&
                  std::get<has_idx>(lbracket) &&
                  std::get<has_idx>(args)     &&
                  std::get<has_idx>(rbracket) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(rbracket) : begin) ;
  }

  std::tuple<bool, auto> has_expression (auto begin, auto end) {
    auto && fcall = has_function_call(begin, end) ; 
    if (std::get<has_idx>(fcall)) return fcall ;

    auto && acces = has_access(begin, end) ; 
    if (std::get<has_idx>(acces)) return acces ;
    
    auto && ident = has_identifier(begin, end) ;
    if (std::get<has_idx>(ident)) return ident ;
    
    auto && numbr = has_number(begin, end) ;
    if (std::get<has_idx>(numbr)) return numbr ;
    
    auto && strin = has_string(begin, end) ;
    if (std::get<has_idx>(strin)) return strin ;
    
    return std::make_tuple(false, begin) ;
  } 

  std::tuple<bool, auto> has_member (auto begin, auto end) {
    auto && type = has_identifier(begin, end) ;
    auto && name = has_identifier(std::get<cursor_idx>(type), end) ;
    auto && scol = has_semi_colon(std::get<cursor_idx>(name), end) ;
    auto && has = std::get<has_idx>(type) &&
                  std::get<has_idx>(name) &&
                  std::get<has_idx>(scol) ;
    return std::make_tuple(has, has ? std::get<has_idx>(scol) : begin) ;
  }

  std::tuple<bool, auto> has_pod (auto begin, auto end) {
    auto && pod_kw  = has_pod_kw(begin, end) ;
    auto && name    = has_identifier(std::get<cursor_idx>(pod_kw), end) ;
    auto && lbrace  = has_lbrace(std::get<cursor_idx>(name), end) ;
    auto && members = has_elements(std::get<cursor_idx>(lbrace), end, 
                                   [] (auto begin, auto end) {return has_member(begin, end) ;}) ;
    auto && rbrace  = has_rbrace(std::get<cursor_idx>(members), end) ;
    auto && has = std::get<has_idx>(pod_kw) && 
                  std::get<has_idx>(name) && 
                  std::get<has_idx>(lbrace) && 
                  std::get<has_idx>(members) && 
                  std::get<has_idx>(rbrace) ;
    return std::make_tuple(has, has ? std::get<has_idx>(rbrace) : begin) ;
  }

  std::tuple<bool, auto> has_parameter (auto begin, auto end) {
    auto && type = has_identifier(begin, end) ;
    auto && name = has_identifier(std::get<cursor_idx>(type), end) ;
    auto && has = std::get<has_idx>(type) && 
                  std::get<has_idx>(name) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(name) : begin) ;
  }

  std::tuple<bool, auto> has_function (auto begin, auto end) {
    auto && name     = has_identifier(begin, end) ;
    auto && lbracket = has_lbracket(std::get<cursor_idx>(name), end) ;
    // auto && params   = has_elements(std::get<cursor_idx>(lbracket), end, 
    //                                 [] (auto begin, auto end) {return has_parameter(begin, end) ;}, 
    //                                 [] (auto begin, auto end) {return has_comma(begin, end) ;}) ;
    auto && rbracket = has_rbracket(std::get<cursor_idx>(lbracket), end) ;
    auto && colon    = has_colon(std::get<cursor_idx>(rbracket), end) ;
    auto && lbrace   = has_lbrace(std::get<cursor_idx>(colon), end) ;
    // auto && consts   = has_elements(std::get<cursor_idx>(lbrace), end, 
    //                                 [] (auto begin, auto end) {return has_const_instruction(begin, end) ;}) ;
    auto && return_  = has_return_instruction(std::get<cursor_idx>(lbrace), end) ;
    auto && has = std::get<has_idx>(name) &&
                  std::get<has_idx>(lbracket) &&
                  // std::get<has_idx>(params) &&
                  std::get<has_idx>(rbracket) &&
                  std::get<has_idx>(colon) &&
                  std::get<has_idx>(lbrace) &&
                  // std::get<has_idx>(consts) && 
                  std::get<has_idx>(return_) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(return_) : begin) ;  
  }

  std::tuple<bool, auto> has_reader (auto begin, auto end) {
    auto && reader_kw = has_reader_kw(begin, end) ;
    auto && func = has_function(std::get<cursor_idx>(reader_kw), end) ;
    auto && has = std::get<has_idx>(reader_kw) && 
                  std::get<has_idx>(func) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(func) : begin) ; 
  }

  std::tuple<bool, auto> has_listener (auto begin, auto end) {
    auto && listener_kw = has_listener_kw(begin, end) ;
    auto && func        = has_function(std::get<cursor_idx>(listener_kw), end) ;
    auto && has = std::get<has_idx>(listener_kw) && 
                  std::get<has_idx>(func) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(func) : begin) ; 
  }

  std::tuple<bool, auto> has_processor (auto begin, auto end) {
    auto && processor_kw = has_processor_kw(begin, end) ;
    auto && func         = has_function(std::get<cursor_idx>(processor_kw), end) ;
    auto && has = std::get<has_idx>(processor_kw) && 
                  std::get<has_idx>(func) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(func) : begin) ; 
  }

  std::tuple<bool, auto> has_builder (auto begin, auto end) {
    auto && builder_kw = has_builder_kw(begin, end) ;
    auto && func       = has_function(std::get<cursor_idx>(builder_kw), end) ;
    auto && has = std::get<has_idx>(builder_kw) && 
                  std::get<has_idx>(func) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(func) : begin) ; 
  }

  std::tuple<bool, auto> has_writer (auto begin, auto end) {
    auto && writer_kw = has_writer_kw(begin, end) ;
    auto && func      = has_function(std::get<cursor_idx>(writer_kw), end) ;
    auto && has = std::get<has_idx>(writer_kw) && 
                  std::get<has_idx>(func) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(func) : begin) ; 
  }

  std::tuple<bool, auto> has_flow (auto begin, auto end) {
    auto && has = true ;
    return std::make_tuple(has, begin) ;
  }

  std::tuple<bool, auto> has_main_flow (auto begin, auto end) {
    auto && main_kw = has_main_kw(begin, end) ;
    auto && flow    = has_flow(std::get<cursor_idx>(main_kw), end) ;
    auto && has = std::get<has_idx>(main_kw) &&   
                  std::get<has_idx>(flow) ;
    return std::make_tuple(has, has ? std::get<cursor_idx>(flow) : begin) ;
  }

  std::tuple<bool, auto> has_thodd (auto begin, auto end) {
  }



  

}

#endif 