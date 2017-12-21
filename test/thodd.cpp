#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <list>

enum class thodd : int {
  identifier, 
  lbrace, rbrace, 
  lbracket, rbracket,
  lsbracket, rsbracket,
  colon, semi_colon, 
  point, comma 
} ;

auto 
operator > (thodd l, thodd r) {
  std::cout << (int) l << (int) r << std::endl ;
  return 2 ;
}

struct rx {
  std::regex reg ; 
  thodd id ;
} ;

struct lexem {
  std::string str ;
  thodd id ; 
} ;

int main(int argc, char** argv) {
  auto stream = std::string ("__add(a:int,b:int):int;");
  
  // Bibliothèque des regex.
  auto const identifier_rx = rx { std::regex("^[a-z_]+"), thodd::identifier } ;
  auto const lbracket_rx = rx { std::regex ("^\\("), thodd::lbracket } ;
  auto const rbracket_rx = rx { std::regex ("^\\)"), thodd::rbracket } ;
  auto const colon_rx = rx { std::regex ("^:"), thodd::colon } ;
  auto const semi_colon_rx = rx { std::regex ("^;"), thodd::semi_colon } ;
  auto const comma_rx = rx { std::regex ("^,"), thodd::comma } ;

  // Constitution du stream de lexem.
  auto begin = stream.cbegin() ;
  auto end = stream.cend() ;

  std::smatch matched ; 
  std::list<lexem> lexems ;

  while (begin != end) {
    std::vector<lexem> matchs ;
    auto rxs = { identifier_rx, lbracket_rx, rbracket_rx, colon_rx, comma_rx, semi_colon_rx } ;

    std::transform (
      rxs.begin(), rxs.end(), std::back_inserter(matchs), 
      [&] (auto && rx) { 
        std::regex_search(begin, end, matched, rx.reg) ; 
        return lexem { matched.str(), rx.id } ; 
      }) ;

    constexpr auto lexem_cpr = 
      [] (auto const & l, auto const & r) { 
        return l.str.size() < r.str.size() ; 
      } ;

    auto max = std::max_element (matchs.cbegin(), matchs.cend(), lexem_cpr) ;
    std::advance (begin, (*max).str.size() == 0 ? 1 : (*max).str.size()) ;
    
    if ((*max).str.size() != 0)
      lexems.push_back(*max) ;
  } 

  std::for_each (
    lexems.cbegin(), lexems.cend(), 
    [] (auto const & lex) {std::cout << (int) lex.id << ':' << lex.str << std::endl ; }) ;

  const auto is_param = [] (auto & lexem, auto const & end) {
    if ((*lexem).id == thodd::identifier)
      if ((*(++lexem)).id == thodd::colon)
        if ((*(++lexem)).id == thodd::identifier)
          return true ;
    
    return false;
  } ;

  const auto is_signature = [is_param] (auto lexem, auto const end) {
    if ((*lexem).id == thodd::identifier)
      if ((*(++lexem)).id == thodd::lbracket)
        while (is_param(++lexem, end)) 
          if ((*(++lexem)).id == thodd::comma) ; 
    
    return lexem == (--end) ;
  } ;

  std::cout << std::boolalpha << is_signature (lexems.cbegin(), lexems.cend()) << std::endl ;
}