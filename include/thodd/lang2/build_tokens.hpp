#ifndef __thodd_lang_build_tokens_hpp__
#  define __thodd_lang_build_tokens_hpp__

#  include <utility>

#  include <thodd/range.hpp>
#  include <thodd/optional.hpp>
#  include <thodd/lang2/token.hpp>

#  include <cxxabi.h>

template<
    typename T>
std::string type_name()
{
    int status;
    std::string tname = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    
    if(status == 0) 
    {
        tname = demangled_name;
        std::free(demangled_name);
    } 

    return tname;
}


namespace 
thodd::lang
{

  constexpr auto token_greater = 
  [] (auto && ltoken, auto && rtoken) 
  { 
    return has_value(ltoken) && has_value(rtoken) ?
      size(value_of(ltoken).data) > size(value_of(ltoken).data) : 
      has_value(ltoken) && !has_value(rtoken) ;
  } ;
  
  constexpr auto 
  build_tokens (
    auto && stream,
    auto && first_term, 
    auto && ... term)
  -> decltype(auto)
  {
    using language_t = std::decay_t<decltype(value_of(first_term(begin(stream), end(stream))).id)> ;
    language_t captured_id {} ;
    
    return 
    filter(
      project(
        step(
          std::forward<decltype(stream)>(stream),
          [&captured_id, &first_term, &term...] 
          (auto && cursor_it, auto const & end_it) 
          {
            auto && res = make_array(first_term(cursor_it, end_it), term(cursor_it, end_it)...) ;

            if_exists (
              max(res, token_greater), 
              [&captured_id, &cursor_it] (auto && max_opt_token) 
              { 
                if_exists (
                  max_opt_token,
                  [&captured_id, &cursor_it] (auto && token) 
                  { cursor_it = end(token.data) ; 
                    captured_id = token.id ; }, 
                  [&captured_id, &cursor_it] 
                  { next(cursor_it) ; 
                    captured_id = language_t::error ; }) ;
              }) ;
          }), 
        [&captured_id] (auto && step_it) 
        { std::cout << "coucou" ; return make_token(captured_id, step_it.begin_it, step_it.it) ; 
      }), 
      [] (auto && t) { return t.id != language_t::ignored ; });
  }
}

#endif