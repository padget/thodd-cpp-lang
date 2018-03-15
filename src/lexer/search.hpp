#ifndef __thodd_lexer_search_hpp__
#  define __thodd_lexer_search_hpp__

#  include "../stream/utils.hpp"


namespace thodd::lexer {
  auto search_for(auto cursor, auto end, std::string_view searched, lexem::type_ type) {
    auto && is_start_with = stream::start_with(cursor, end, searched.begin(), searched.end()) ;
    
    return std::make_tuple(
      is_start_with ? std::next(cursor, searched.size()) : cursor, 
      is_start_with ? type : lexem::type_::error) ;
  }

  auto search_for_identifier_rx (auto begin, auto end) {
    auto cursor = begin ;
    
    constexpr auto is_letter = [] (auto const & cursor) {
      return ('a' <= *cursor && *cursor <= 'z') || *cursor == '_' ;
    } ;

    constexpr auto is_digit = [] (auto  const & cursor) {
      return ('0' <= *cursor && *cursor <= '9') ;
    } ;

    if (is_letter(cursor)) {
      cursor = std::next(cursor) ;

      while (is_letter(cursor) || is_digit(cursor))
        cursor = std::next(cursor) ;
    }

    return std::make_tuple(cursor, cursor != begin ? lexem::type_::identifier : lexem::type_::error) ;
  }


  auto search_for_number_rx (auto begin, auto end) {
    auto cursor = begin ;
    
    while (('0' <= *cursor && *cursor <= '9'))
      cursor = std::next(cursor) ;

    if (cursor == begin)
      return std::make_tuple(begin, lexem::type_::error) ;

    if (*cursor == '.') {
      auto save = cursor ;
      cursor = std::next(cursor) ;

      while (('0' <= *cursor && *cursor <= '9'))
        cursor = std::next(cursor) ;

      if (cursor == save)
        return std::make_tuple(begin, lexem::type_::error) ;
    }

    return std::make_tuple(cursor, cursor != begin ? lexem::type_::number : lexem::type_::error) ;
  }

  auto search_for_string_rx (auto begin, auto end) {
    auto cursor = begin ;
    
    if (*cursor != '"') 
      return std::make_tuple(begin, lexem::type_::error) ;

    cursor = std::next(cursor) ;

    while (cursor != end && *cursor != '"') {
      cursor = std::next (cursor) ;
      // TODO To end.
    }

    return std::make_tuple(cursor, cursor != begin ? lexem::type_::string : lexem::type_::error) ;
  }

  auto search_for_ignored_rx (auto begin, auto end) {
    auto cursor = begin ;
    
    while (*cursor == ' ' || *cursor == '\t' || *cursor == '\n')
      cursor = std::next(cursor) ;

    return std::make_tuple(cursor, lexem::type_::ignored) ;
  }

  auto thodd_keywords_rxs (auto) {
    return std::make_tuple (
      [] (auto b, auto e) { return search_for(b, e, "pod",       lexem::type_::pod_kw) ;},  
      [] (auto b, auto e) { return search_for(b, e, "flow",      lexem::type_::flow_kw) ;}, 
      [] (auto b, auto e) { return search_for(b, e, "reader",    lexem::type_::reader_kw) ;}, 
      [] (auto b, auto e) { return search_for(b, e, "listener",  lexem::type_::listener_kw) ;}, 
      [] (auto b, auto e) { return search_for(b, e, "processor", lexem::type_::processor_kw) ;}, 
      [] (auto b, auto e) { return search_for(b, e, "builder",   lexem::type_::builder_kw) ;}, 
      [] (auto b, auto e) { return search_for(b, e, "writer",    lexem::type_::writer_kw) ;}, 
      [] (auto b, auto e) { return search_for(b, e, "main",      lexem::type_::main_kw) ;}, 
      [] (auto b, auto e) { return search_for(b, e, "return",    lexem::type_::return_kw) ;}) ;
  }

  auto thodd_simple_rxs (auto) {
    return std::make_tuple(
      [] (auto b, auto e) { return search_for_identifier_rx(b, e) ;}, 
      [] (auto b, auto e) { return search_for_number_rx(b, e) ;},
      [] (auto b, auto e) { return search_for_string_rx(b, e) ;}, 
      [] (auto b, auto e) { return search_for_ignored_rx(b, e) ;},
      [] (auto b, auto e) { return search_for(b, e, "(", lexem::type_::lbracket) ;}, 
      [] (auto b, auto e) { return search_for(b, e, ")", lexem::type_::rbracket) ;}, 
      [] (auto b, auto e) { return search_for(b, e, "{", lexem::type_::lbrace) ;},
      [] (auto b, auto e) { return search_for(b, e, "}", lexem::type_::rbrace) ;}, 
      [] (auto b, auto e) { return search_for(b, e, ".", lexem::type_::point) ;}, 
      [] (auto b, auto e) { return search_for(b, e, ":", lexem::type_::colon) ;}, 
      [] (auto b, auto e) { return search_for(b, e, ";", lexem::type_::semi_colon) ;},
      [] (auto b, auto e) { return search_for(b, e, ",", lexem::type_::comma) ;}) ;
  }

  auto thodd_rxs (auto) {
    auto && keywords_rxs = thodd_keywords_rxs(0) ;
    auto && simple_rxs = thodd_simple_rxs(0) ;

    return std::tuple_cat(keywords_rxs, simple_rxs) ;
  } 

}


#endif