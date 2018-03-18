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
  auto && res = detail::has_element(begin, end, {lexer::lexem::type_::type}) ;   \
  return detail::make_has_res<type_c>(res, begin) ;      \
}                                                                        \

#  define THODD_LAMBDA_IT(name)                       \
[] (auto begin, auto end) {return name(begin, end) ;} \


namespace thodd::element {
  constexpr auto has_idx = 0u ;
  constexpr auto cursor_idx = 1u ;
  constexpr auto errors_idx = 2u ;

  enum class has_type { 
    with_errors, no_errors
  } ;

  namespace detail {
    auto lexem_type_comparator () { 
      return [] (auto const & lexem, auto const & type) {
        return lexem.type == type ;
      } ; 
    }
    
    inline bool has_all (auto const & ... has_res) {
      return (... && std::get<has_idx>(has_res)) ;
    }

    auto if_has (auto const & res, auto end, auto && has_predicate) {
      return has_all(res) ? has_predicate(std::get<cursor_idx>(res), end) : res ;
    }
   
    inline auto make_positive_has_response(auto cursor) -> decltype(std::make_tuple(true, cursor)) {
      return std::make_tuple(true, cursor) ;
    }

    template<has_type type_c> 
    auto make_has_res (auto const & last_res, auto begin) -> std::tuple<bool, auto> {   
      auto && has = std::get<has_idx>(last_res) ;

      if constexpr (type_c == has_type::no_errors)                           
        return std::make_tuple(has, has ? std::get<cursor_idx>(last_res) : begin) ;
      else if constexpr (type_c == has_type::with_errors)                    
        return std::make_tuple(has, has ? std::get<cursor_idx>(last_res) : begin) ;
    }

    auto has_element (auto begin, auto end, std::vector<lexer::lexem::type_> ids) -> decltype(auto) {
      auto && has = stream::start_with(begin, end, ids.begin(), ids.end(), detail::lexem_type_comparator()) ;
      return std::make_tuple(has, has ? std::next(begin, ids.size()) : begin) ;
    }

    auto has_elements (auto const & res, auto end, auto rx, auto rx_sep) -> decltype(rx(end, end)) {
      auto && rx_res     = if_has(res, end, rx) ;
      auto && rx_sep_res = if_has(rx_res, end, rx_sep) ;
      return has_all(res, rx_res, rx_sep_res) ? 
          has_elements(rx_sep_res, end, rx, rx_sep) 
        : make_positive_has_response(std::get<cursor_idx>(rx_res)) ;
    }

    auto has_elements (auto const & res, auto end, auto rx) -> decltype(rx(end, end)) {
      auto && rx_res = if_has(res, end, rx) ; 
      return has_all(res, rx_res) ? 
          has_elements(rx_res, end, rx) 
        : make_positive_has_response(std::get<cursor_idx>(rx_res))  ;
    }
  }

  inline init(auto cursor) {
    return detail::make_positive_has_response(cursor) ;
  }

  inline auto one(auto func)  {
    return [func] (auto const & pred, auto e) {return detail::if_has(pred, e, func) ;} ;
  }

  inline auto many(auto func) {
    return [func] (auto const & pred, auto e) {return detail::has_elements(pred, e, func) ;} ; 
  }

  inline auto many(auto func, auto sep) {
    return [func, sep] (auto const & pred, auto e) {return detail::has_elements(pred, e, func, sep) ;} ; 
  }

  auto explore (auto first, auto ... next) {
    return [first, next...] (auto const & pred, auto end) {
      auto && res = first(pred, end) ;

      if constexpr (sizeof...(next) > 0)
        return explore(next...)(res, end) ;
      else 
        return res ; 
    } ;
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
  auto has_expression (auto begin, auto end) -> decltype(detail::make_positive_has_response(begin)) ;

  template<has_type type_c>
  std::tuple<bool, auto> has_const_instruction(auto begin, auto end) {
    auto && type = has_identifier<type_c>(begin, end) ;
    auto && name = detail::if_has(type, end, THODD_LAMBDA_IT(has_identifier<type_c>)) ;
    auto && expr = detail::if_has(name, end, THODD_LAMBDA_IT(has_expression<type_c>)) ;
    auto && scol = detail::if_has(expr, end, THODD_LAMBDA_IT(has_semi_colon<type_c>)) ;
    auto && has  = detail::has_all(type, name , expr, scol) ;
    return detail::make_has_res<type_c>( scol, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_return_instruction(auto begin, auto end) {
    auto && return_ = has_return_kw<type_c>(begin, end) ;
    auto && expr    = detail::if_has(return_, end, THODD_LAMBDA_IT(has_expression<type_c>)) ;
    auto && scol    = detail::if_has(expr, end, THODD_LAMBDA_IT(has_semi_colon<type_c>)) ;
    auto && has     = detail::has_all(return_, expr, scol) ;
    return detail::make_has_res<type_c>(scol, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_access (auto begin, auto end) {
    auto && name    = has_identifier<type_c>(begin, end) ;
    auto && point   = detail::if_has(name, end, THODD_LAMBDA_IT(has_point<type_c>)) ;
    auto && members = detail::has_elements(point, end, THODD_LAMBDA_IT(has_identifier<type_c>), THODD_LAMBDA_IT(has_point<type_c>)) ;
    return detail::make_has_res<type_c>(members, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_function_call (auto begin, auto end) {
    auto && name     = has_identifier<type_c>(begin, end) ;
    auto && lbracket = detail::if_has(name, end, THODD_LAMBDA_IT(has_lbracket<type_c>)) ;
    auto && args     = detail::has_elements(lbracket, end, THODD_LAMBDA_IT(has_expression<type_c>), THODD_LAMBDA_IT(has_comma<type_c>)) ;
    auto && rbracket = detail::if_has(name, end, THODD_LAMBDA_IT(has_rbracket<type_c>)) ;
    return detail::make_has_res<type_c>(rbracket, begin) ;
  }

  template<has_type type_c>
  auto has_expression (auto begin, auto end) -> decltype(detail::make_positive_has_response(begin))  {
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

    return detail::make_has_res<type_c>(strin, begin) ;
  } 

  template<has_type type_c>
  std::tuple<bool, auto> has_member (auto begin, auto end) {
    auto && type = has_identifier<type_c>(begin, end) ;
    auto && name = detail::if_has(type, end, THODD_LAMBDA_IT(has_identifier<type_c>)) ;
    auto && scol = detail::if_has(name, end, THODD_LAMBDA_IT(has_semi_colon<type_c>)) ;
    return detail::make_has_res<type_c>(scol, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_pod (auto begin, auto end) {
    auto && pod_kw  = has_pod_kw<type_c>(begin, end) ;
    auto && name    = detail::if_has(pod_kw, end, THODD_LAMBDA_IT(has_identifier<type_c>)) ;
    auto && lbrace  = detail::if_has(name, end, THODD_LAMBDA_IT(has_lbrace<type_c>)) ;
    auto && members = detail::has_elements(lbrace, end, THODD_LAMBDA_IT(has_member<type_c>)) ;
    auto && rbrace  = detail::if_has(members, end, THODD_LAMBDA_IT(has_rbrace<type_c>)) ;
    return detail::make_has_res<type_c>(rbrace, begin) ;  
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_parameter (auto begin, auto end) {
    auto && type = has_identifier<type_c>(begin, end) ;
    auto && name = detail::if_has(type, end, THODD_LAMBDA_IT(has_identifier<type_c>)) ;
    return detail::make_has_res<type_c>(name, begin) ;
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_function (auto begin, auto end) {
    auto && explorer = 
      explore(one(THODD_LAMBDA_IT(has_identifier<type_c>)),
              one(THODD_LAMBDA_IT(has_lbracket<type_c>)), 
              many(THODD_LAMBDA_IT(has_parameter<type_c>), THODD_LAMBDA_IT(has_comma<type_c>)), 
              one(THODD_LAMBDA_IT(has_rbracket<type_c>)), 
              one(THODD_LAMBDA_IT(has_colon<type_c>)), 
              one(THODD_LAMBDA_IT(has_identifier<type_c>)), 
              one(THODD_LAMBDA_IT(has_lbrace<type_c>)), 
              many(THODD_LAMBDA_IT(has_const_instruction<type_c>)), 
              one(THODD_LAMBDA_IT(has_return_instruction<type_c>)));
    auto && res = explorer(init(begin), end) ;
    
    return detail::make_has_res<type_c>(res, begin) ;  
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_reader (auto begin, auto end) {
    explore(one(THODD_LAMBDA_IT(has_reader_kw<type_c>)), 
            one(THODD_LAMBDA_IT(has_function<type_c>)));
    return detail::make_has_res<type_c>( 
      explore(one(THODD_LAMBDA_IT(has_reader_kw<type_c>)), 
              one(THODD_LAMBDA_IT(has_function<type_c>)))(begin, end), begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_listener (auto begin, auto end) {
    auto && listener_kw = has_listener_kw<type_c>(begin, end) ;
    auto && func        = detail::if_has(listener_kw, end, THODD_LAMBDA_IT(has_function<type_c>)) ;
    auto && has         = detail::has_all(listener_kw, func) ;
    return detail::make_has_res<type_c>(func, begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_processor (auto begin, auto end) {
    auto && processor_kw = has_processor_kw<type_c>(begin, end) ;
    auto && func         = detail::if_has(processor_kw, end, THODD_LAMBDA_IT(has_function<type_c>)) ;
    auto && has          = detail::has_all(processor_kw, func) ;
    return detail::make_has_res<type_c>(func, begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_builder (auto begin, auto end) {
    auto && builder_kw = has_builder_kw<type_c>(begin, end) ;
    auto && func       = detail::if_has(builder_kw, end, THODD_LAMBDA_IT(has_function<type_c>)) ;
    auto && has        = detail::has_all(builder_kw, func) ;
    return detail::make_has_res<type_c>(func, begin) ;  
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_writer (auto begin, auto end) {
    auto && writer_kw = has_writer_kw<type_c>(begin, end) ;
    auto && func      = detail::if_has(writer_kw, end, THODD_LAMBDA_IT(has_function<type_c>)) ;
    auto && has       = detail::has_all(writer_kw, func) ;
    return detail::make_has_res<type_c>(func, begin) ;  
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_flow (auto begin, auto end) {
    auto && has = true ;
    //return detail::make_has_res<type_c>(begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_main_flow (auto begin, auto end) {
    auto && main_kw = has_main_kw<type_c>(begin, end) ;
    auto && flow    = detail::if_has(main_kw, end, THODD_LAMBDA_IT(has_flow<type_c>)) ;
    auto && has     = detail::has_all(main_kw, flow) ;
    return detail::make_has_res(flow, begin) ; 
  }

  template<has_type type_c>
  std::tuple<bool, auto> has_thodd (auto begin, auto end) {
  }
}

#endif 