#ifndef __thodd_element_has_hpp__
#  define __thodd_element_has_hpp__

#  include <vector>
#  include <tuple>

#  include "../stream/utils.hpp"
#  include "../lexer/lexem.hpp"

#  define stringerize(s) #s 

#  define THODD_HAS_XXX(name, type)                                              \
auto has_##name (auto begin, auto end) {                                         \
  auto && res = detail::has_element(begin, end, lexer::lexem::type_::type) ;     \
  return detail::make_has_res(res, begin) ;                                      \
}                                                                                \

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
    inline bool has_all (auto const & ... has_res) {
      return (... && std::get<has_idx>(has_res)) ;
    }

    auto if_has (auto const & res, auto end, auto && has_predicate) {
      return has_all(res) ? has_predicate(std::get<cursor_idx>(res), end) : res ;
    }

    auto if_has_not (auto const & res, auto end, auto && has_predicate) {
      return !has_all(res) ? has_predicate(std::get<cursor_idx>(res), end) : res ; 
    }
   
    inline auto make_positive_has_response(auto cursor) -> decltype(std::make_tuple(true, cursor)) {
      return std::make_tuple(true, cursor) ;
    }

    inline auto make_negative_has_response(auto cursor) -> decltype(std::make_tuple(false, cursor)) {
      return std::make_tuple(false, cursor) ;
    }

    inline auto make_has_res (auto const & last_res, auto begin) {   
      auto && has = std::get<has_idx>(last_res) ;                    
      return std::make_tuple(has, has ? std::get<cursor_idx>(last_res) : begin) ;
    }

    inline auto has_element (auto begin, auto end, lexer::lexem::type_ id) -> decltype(auto) {
      auto && has = begin != end && container::get(begin).type == id ;
      return std::make_tuple(has, has ? container::next(begin) : begin) ;
    }

    inline auto has_elements (auto const & res, auto end, auto rx, auto rx_sep) -> decltype(rx(end, end)) {
      auto && rx_res     = if_has(res, end, rx) ;
      auto && rx_sep_res = if_has(rx_res, end, rx_sep) ;
      return has_all(res, rx_res, rx_sep_res) ? 
          has_elements(rx_sep_res, end, rx, rx_sep) 
        : make_positive_has_response(std::get<cursor_idx>(rx_res)) ;
    }

    inline auto has_elements (auto const & res, auto end, auto rx) -> decltype(rx(end, end)) {
      auto && rx_res = if_has(res, end, rx) ; 
      return has_all(res, rx_res) ? 
          has_elements(rx_res, end, rx) 
        : make_positive_has_response(std::get<cursor_idx>(rx_res))  ;
    }
  }

  inline auto init(auto cursor) {
    return detail::make_positive_has_response(cursor) ;
  }

  inline auto init_negative(auto cursor) {
    return detail::make_negative_has_response(cursor) ;
  }

  inline auto either (auto func) {
    return [func] (auto const & pred, auto end) {return detail::if_has_not(pred, end, func) ;} ;
  }

  inline auto one (auto func) {
    return [func] (auto const & pred, auto e) {return detail::if_has(pred, e, func) ;} ;
  }

  inline auto many (auto func) {
    return [func] (auto const & pred, auto e) {return detail::has_elements(pred, e, func) ;} ; 
  }

  inline auto many (auto func, auto sep) {
    return [func, sep] (auto const & pred, auto e) {return detail::has_elements(pred, e, func, sep) ;} ; 
  }

  inline auto explore (auto first, auto ... next) {
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
  THODD_HAS_XXX(flow_kw, flow_kw)

  auto has_expression (auto begin, auto end) -> decltype(detail::make_positive_has_response(begin)) ;

  std::tuple<bool, auto> has_const_instruction(auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_identifier)), 
                   one(THODD_LAMBDA_IT(has_identifier)), 
                   one(THODD_LAMBDA_IT(has_expression)), 
                   one(THODD_LAMBDA_IT(has_semi_colon))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_return_instruction(auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_return_kw)), 
                   one(THODD_LAMBDA_IT(has_expression)), 
                   one(THODD_LAMBDA_IT(has_semi_colon))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_access (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_identifier)), 
                   one(THODD_LAMBDA_IT(has_point)), 
                   many(THODD_LAMBDA_IT(has_identifier), 
                        THODD_LAMBDA_IT(has_point))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_function_call (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_identifier)), 
                   one(THODD_LAMBDA_IT(has_lbracket)), 
                   many(THODD_LAMBDA_IT(has_expression), 
                        THODD_LAMBDA_IT(has_comma)), 
                   one(THODD_LAMBDA_IT(has_rbracket))) (init(begin), end) ;
  }

  auto has_expression (auto begin, auto end) -> decltype(detail::make_positive_has_response(begin)) {
    return explore(either(THODD_LAMBDA_IT(has_function_call)), 
                   either(THODD_LAMBDA_IT(has_access)), 
                   either(THODD_LAMBDA_IT(has_identifier)), 
                   either(THODD_LAMBDA_IT(has_number)), 
                   either(THODD_LAMBDA_IT(has_string))) (init_negative(begin), end) ;
  } 

  std::tuple<bool, auto> has_member (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_identifier)), 
                   one(THODD_LAMBDA_IT(has_identifier)), 
                   one(THODD_LAMBDA_IT(has_semi_colon))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_pod (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_pod_kw)), 
                   one(THODD_LAMBDA_IT(has_identifier)), 
                   one(THODD_LAMBDA_IT(has_lbrace)), 
                   many(THODD_LAMBDA_IT(has_member)), 
                   one(THODD_LAMBDA_IT(has_rbrace))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_parameter (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_identifier)), 
                   one(THODD_LAMBDA_IT(has_identifier))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_function (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_identifier)),
                   one(THODD_LAMBDA_IT(has_lbracket)), 
                   many(THODD_LAMBDA_IT(has_parameter), 
                        THODD_LAMBDA_IT(has_comma)), 
                   one(THODD_LAMBDA_IT(has_rbracket)), 
                   one(THODD_LAMBDA_IT(has_colon)), 
                   one(THODD_LAMBDA_IT(has_identifier)), 
                   one(THODD_LAMBDA_IT(has_lbrace)), 
                   many(THODD_LAMBDA_IT(has_const_instruction)), 
                   one(THODD_LAMBDA_IT(has_return_instruction))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_reader (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_reader_kw)), 
                   one(THODD_LAMBDA_IT(has_function))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_listener (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_listener_kw)), 
                   one(THODD_LAMBDA_IT(has_function))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_processor (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_processor_kw)), 
                   one(THODD_LAMBDA_IT(has_function))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_builder (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_builder_kw)), 
                   one(THODD_LAMBDA_IT(has_function))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_writer (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_writer_kw)), 
                   one(THODD_LAMBDA_IT(has_function))) (init(begin), end) ;
  }

  // TODO define has_flow function
  std::tuple<bool, auto> has_flow (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_flow_kw))) (init(begin), end) ;
  }

  std::tuple<bool, auto> has_main_flow (auto begin, auto end) {
    return explore(one(THODD_LAMBDA_IT(has_main_kw)), 
                   one(THODD_LAMBDA_IT(has_flow))) (init(begin), end) ;
  }

  // TODO define has_thodd function
  std::tuple<bool, auto> has_thodd (auto begin, auto end) {
  }
}

#endif 