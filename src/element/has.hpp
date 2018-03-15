#ifndef __thodd_element_has_hpp__
#  define __thodd_element_has_hpp__

#  include "../stream/utils.hpp"
#  include <vector>
#  include <tuple>
#  include "../lexer/lexem.hpp"


#  define stringerize(s) #s 

#  define THODD_HAS_XXX(name, type)                                      \
template<has_type type_c>                                                \
auto has_##name (auto begin, auto end) {                                 \
  auto && res = has_element(begin, end, {lexer::lexem::type_::type}) ;   \
  return make_has_res<type_c>(std::get<has_idx>(res), res, begin) ;      \
}                                                                        \

#  define THODD_LAMBDA_IT(name)                       \
[] (auto begin, auto end) {return name(begin, end) ;} \


namespace thodd::element {
  constexpr auto has_idx = 0u ;
  constexpr auto cursor_idx = 1u ;
  constexpr auto errors_idx = 2u ;

  namespace detail {
    auto lexem_type_comparator () { 
      return [] (auto const & lexem, auto const & type) {
        std::cout << (int) lexem.type << " vs " << (int) type << std::endl ;
        return lexem.type == type ;
      } ; 
    }
    
   auto if_has (auto const & res, auto end, auto && has_predicate) {
      return std::get<has_idx>(res) ? has_predicate(std::get<cursor_idx>(res), end) : res ;
    }
  }

  enum class has_type { 
    with_errors, no_errors
  } ;

  template<has_type type_c> 
  auto make_has_res (bool has, auto const & last_res, auto begin) {
    std::cout << std::boolalpha << has << std::endl ; 
    
    if constexpr (type_c == has_type::no_errors)                           
      return std::make_tuple(has, has ? std::get<cursor_idx>(last_res) : begin) ;
    else if constexpr (type_c == has_type::with_errors)                    
      return std::make_tuple(has, has ? std::get<cursor_idx>(last_res) : begin) ;
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

  template<has_type type_c>
  std::tuple<bool, auto> has_expression (auto begin, auto end) ;

  template<has_type type_c>
  std::tuple<bool, auto> has_const_instruction(auto begin, auto end) {
    auto && type = has_identifier<type_c>(begin, end) ;
    auto && name = detail::if_has(type, end, THODD_LAMBDA_IT(has_identifier<type_c>)) ;
    auto && expr = detail::if_has(name, end, THODD_LAMBDA_IT(has_expression<type_c>)) ;
    auto && scol = detail::if_has(expr, end, THODD_LAMBDA_IT(has_semi_colon<type_c>)) ;
    auto && has = std::get<has_idx>(type) && 
                  std::get<has_idx>(name) && 
                  std::get<has_idx>(expr) &&
                  std::get<has_idx>(scol) ;
    return make_has_res<type_c>(has, scol, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_return_instruction(auto begin, auto end) {
    auto && return_ = has_return_kw<type_c>(begin, end) ;
    auto && expr    = detail::if_has(return_, end, THODD_LAMBDA_IT(has_expression<type_c>)) ;
    auto && scol    = detail::if_has(expr, end, THODD_LAMBDA_IT(has_semi_colon<type_c>)) ;
    auto && has = std::get<has_idx>(return_) && 
                  std::get<has_idx>(expr)    &&
                  std::get<has_idx>(scol) ;
    return make_has_res<type_c>(has, scol, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_access (auto begin, auto end) {
    auto && name    = has_identifier<type_c>(begin, end) ;
    auto && point   = detail::if_has(name, end, THODD_LAMBDA_IT(has_point<type_c>)) ;
    auto && members = has_elements(std::get<cursor_idx>(point), end, 
                                   [] (auto begin, auto end) {return has_identifier<type_c>(begin, end) ;}, 
                                   [] (auto begin, auto end) {return has_point<type_c>(begin, end) ;});
    auto && has = std::get<has_idx>(name)  &&
                  std::get<has_idx>(point) && 
                  std::get<has_idx>(members) ;
    return make_has_res<type_c>(has, members, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_function_call (auto begin, auto end) {
    auto && name     = has_identifier<type_c>(begin, end) ;
    auto && lbracket = detail::if_has(name, end, THODD_LAMBDA_IT(has_lbracket<type_c>)) ;
    auto && args     = std::get<has_idx>(lbracket) ? has_elements(std::get<cursor_idx>(lbracket), end, 
                                    [] (auto begin, auto end) {return has_expression<type_c>(begin, end) ;}, 
                                    [] (auto begin, auto end) {return has_comma<type_c>(begin, end) ;}) : lbracket ;
    auto && rbracket = detail::if_has(name, end, THODD_LAMBDA_IT(has_rbracket<type_c>)) ;
    auto && has = std::get<has_idx>(name)     &&
                  std::get<has_idx>(lbracket) &&
                  std::get<has_idx>(args)     &&
                  std::get<has_idx>(rbracket) ;
    return make_has_res<type_c>(has, rbracket, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_expression (auto begin, auto end) {
    auto && fcall = has_function_call<type_c>(begin, end) ; 
    if (std::get<has_idx>(fcall)) {
      return fcall ;
    }
    
    auto && acces = has_access<type_c>(begin, end) ; 
    if (std::get<has_idx>(acces)) {
       return acces ;
    }

    auto && ident = has_identifier<type_c>(begin, end) ;
    if (std::get<has_idx>(ident)) {
      return ident ;
    }
    
    auto && numbr = has_number<type_c>(begin, end) ;
    if (std::get<has_idx>(numbr)) {
      return numbr ;
    }
    
    auto && strin = has_string<type_c>(begin, end) ;
    if (std::get<has_idx>(strin)) {
       return strin ;
    }

    return make_has_res<type_c>(false, strin, begin) ;
  } 

  template<has_type type_c>
  std::tuple<bool, auto> has_member (auto begin, auto end) {
    auto && type = has_identifier<type_c>(begin, end) ;
    auto && name = detail::if_has(type, end, THODD_LAMBDA_IT(has_identifier<type_c>)) ;
    auto && scol = detail::if_has(name, end, THODD_LAMBDA_IT(has_semi_colon<type_c>)) ;
    auto && has = std::get<has_idx>(type) &&
                  std::get<has_idx>(name) &&
                  std::get<has_idx>(scol) ;
    return make_has_res<type_c>(has, scol, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_pod (auto begin, auto end) {
    auto && pod_kw  = has_pod_kw<type_c>(begin, end) ;
    auto && name    = detail::if_has(pod_kw, end, THODD_LAMBDA_IT(has_identifier<type_c>)) ;
    auto && lbrace  = detail::if_has(name, end, THODD_LAMBDA_IT(has_lbrace<type_c>)) ;
    auto && members = has_elements(std::get<cursor_idx>(lbrace), end, 
                                   [] (auto begin, auto end) {return has_member<type_c>(begin, end) ;}) ;
    auto && rbrace  = detail::if_has(members, end, THODD_LAMBDA_IT(has_rbrace<type_c>)) ;
    auto && has = std::get<has_idx>(pod_kw) && 
                  std::get<has_idx>(name) && 
                  std::get<has_idx>(lbrace) && 
                  std::get<has_idx>(members) && 
                  std::get<has_idx>(rbrace) ;
    return make_has_res<type_c>(has, rbrace, begin) ;  
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_parameter (auto begin, auto end) {
    auto && type = has_identifier<type_c>(begin, end) ;
    auto && name = detail::if_has(type, end, THODD_LAMBDA_IT(has_identifier<type_c>)) ;
    auto && has = std::get<has_idx>(type) && 
                  std::get<has_idx>(name) ;
    return make_has_res<type_c>(has, name, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_function (auto begin, auto end) {
    auto && name     = has_identifier<type_c>(begin, end) ;
    auto && lbracket = detail::if_has(name, end, THODD_LAMBDA_IT(has_lbracket<type_c>)) ;
    auto && params   = has_elements(std::get<cursor_idx>(lbracket), end, 
                                    [] (auto begin, auto end) {return has_parameter<type_c>(begin, end) ;}, 
                                    [] (auto begin, auto end) {return has_comma<type_c>(begin, end) ;}) ;
    std::cout << "search for has_rbracket" << std::endl ;
    auto && rbracket = detail::if_has(params, end, THODD_LAMBDA_IT(has_rbracket<type_c>)) ;
    auto && colon    = detail::if_has(rbracket, end, THODD_LAMBDA_IT(has_colon<type_c>)) ;
    auto && type     = detail::if_has(colon, end, THODD_LAMBDA_IT(has_identifier<type_c>)) ;
    auto && lbrace   = detail::if_has(type, end, THODD_LAMBDA_IT(has_lbrace<type_c>)) ;
    auto && consts   = has_elements(std::get<cursor_idx>(lbrace), end, THODD_LAMBDA_IT(has_const_instruction<type_c>)) ;
    auto && return_  = detail::if_has(consts, end, THODD_LAMBDA_IT(has_return_instruction<type_c>)) ;
    auto && has = std::get<has_idx>(name) &&
                  std::get<has_idx>(lbracket) &&
                  std::get<has_idx>(params) &&
                  std::get<has_idx>(rbracket) &&
                  std::get<has_idx>(colon) &&
                  std::get<has_idx>(type) &&
                  std::get<has_idx>(lbrace) &&
                  std::get<has_idx>(consts) && 
                  std::get<has_idx>(return_) ;
                  
    return make_has_res<type_c>(has, return_, begin) ;  
  }
  template<has_type type_c>
  std::tuple<bool, auto> has_reader (auto begin, auto end) {
    auto && reader_kw = has_reader_kw<type_c>(begin, end) ;
    auto && func      = detail::if_has(reader_kw, end, THODD_LAMBDA_IT(has_function<type_c>)) ;
    auto && has = std::get<has_idx>(reader_kw) && 
                  std::get<has_idx>(func) ;
    return make_has_res<type_c>(has, func, begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_listener (auto begin, auto end) {
    auto && listener_kw = has_listener_kw<type_c>(begin, end) ;
    auto && func        = detail::if_has(listener_kw, end, THODD_LAMBDA_IT(has_function<type_c>)) ;
    auto && has = std::get<has_idx>(listener_kw) && 
                  std::get<has_idx>(func) ;
    return make_has_res<type_c>(has, func, begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_processor (auto begin, auto end) {
    auto && processor_kw = has_processor_kw<type_c>(begin, end) ;
    auto && func         = detail::if_has(processor_kw, end, THODD_LAMBDA_IT(has_function<type_c>)) ;
    auto && has = std::get<has_idx>(processor_kw) && 
                  std::get<has_idx>(func) ;
    return make_has_res<type_c>(has, func, begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_builder (auto begin, auto end) {
    auto && builder_kw = has_builder_kw<type_c>(begin, end) ;
    auto && func       = detail::if_has(builder_kw, end, THODD_LAMBDA_IT(has_function<type_c>)) ;
    auto && has = std::get<has_idx>(builder_kw) && 
                  std::get<has_idx>(func) ;
    return make_has_res<type_c>(has, func, begin) ;  
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_writer (auto begin, auto end) {
    auto && writer_kw = has_writer_kw<type_c>(begin, end) ;
    auto && func      = detail::if_has(writer_kw, end, THODD_LAMBDA_IT(has_function<type_c>)) ;
    auto && has = std::get<has_idx>(writer_kw) && 
                  std::get<has_idx>(func) ;
    return make_has_res<type_c>(has, func, begin) ;  
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_flow (auto begin, auto end) {
    auto && has = true ;
    return make_has_res<type_c>(has,  begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_main_flow (auto begin, auto end) {
    auto && main_kw = has_main_kw<type_c>(begin, end) ;
    auto && flow    = detail::if_has(main_kw, end, THODD_LAMBDA_IT(has_flow<type_c>)) ;
    auto && has = std::get<has_idx>(main_kw) &&   
                  std::get<has_idx>(flow) ;
    return make_has_res<type_c>(has, flow, begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_thodd (auto begin, auto end) {
  }
}

#endif 