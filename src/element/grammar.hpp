#ifndef __thodd_element_grammar_hpp__
#  define __thodd_element_grammar_hpp__

namespace thodd::element {

  inline auto either (auto func) {
    return [func] (auto const & pred, auto end) {return detail::if_has_not(pred, end, func) ;} ;
  }

  inline auto one (auto func) {
    return [=] (auto basicone) {
      return [=] (auto const & pred, auto e) {
        return basicone(pred, e, func) ;
      } ;
    } ;
  }

  inline auto many (auto func) { 
    return [=] (auto basicmany) {
      return [=] (auto const & pred, auto e) {
        return basicmany(pred, e, func) ;
      } ; 
    } ;
  }

  inline auto many (auto func, auto sep) {
   return [=] (auto basicbasicone) {
      return [=] (auto const & pred, auto e) {
        auto && rx_res     = if_has(res, end, rx) ;
        auto && rx_sep_res = if_has(rx_res, end, rx_sep) ;
        return has_all(res, rx_res, rx_sep_res) ? 
          has_elements(rx_sep_res, end, rx, rx_sep) 
        : make_positive_has_response(std::get<cursor_idx>(rx_res)) ;
      } ; 
    } ;
  }
  inline auto explore (auto first, auto ... next) {
    return [=] (auto const & pred, auto end) {
      auto && res = first(pred, end) ;

      if constexpr (sizeof...(next) > 0)
        return explore(next...)(res, end) ;
      else 
        return res ; 
    } ;
  }

}


#endif 