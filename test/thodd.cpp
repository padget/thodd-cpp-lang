#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <list>
#include <optional>

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
    [] (auto const & lex) { 
      std::cout << (int) lex.id << ':' << lex.str << std::endl ; 
    }) ;

  auto const is_not_end = 
    [end = lexems.cend()] (auto lexem) {
      return lexem != end ;
    } ;

  auto const to_ids = [] (auto lexem, size_t nb) {
    std::vector<thodd> ids ;
    std::transform (
      lexem, std::next(lexem, nb), 
      std::back_inserter(ids), 
      [] (auto lexem) { return lexem.id ; }) ;

    return ids ; 
  } ;

  auto const is_param = [to_ids] (auto lexem, auto end) {
    auto const param_ids = { thodd::identifier, thodd::colon, thodd::identifier } ;
    auto const lexem_ids = to_ids (lexem, param_ids.size()) ;

    return 
    std::equal(lexem_ids.cbegin(), lexem_ids.cend(), 
               param_ids.begin(), param_ids.end()) ?
      std::make_optional(std::next(lexem, param_ids.size())) : 
      std::make_optional<decltype(lexem)>() ;
  } ;

  auto const is_signature = [is_param, is_not_end] (auto lexem, auto end) {
    auto tmp = lexem ;
    
    auto const begin_signature_ids = { thodd::identifier, thodd::lbracket } ;

    if (is_not_end(lexem) && (*lexem).id == thodd::identifier) {
      std::cout << (int) (*lexem).id << std::endl ;

      if (is_not_end(lexem) && (*(tmp = std::next(lexem))).id == thodd::lbracket) {
        std::swap(lexem, tmp) ;
        std::cout << (int) (*lexem).id << std::endl ;
        decltype(is_param(lexem, end)) opt_lexem ;

        while (opt_lexem = is_param(std::next(lexem), end)) {
          lexem = opt_lexem.value_or(lexem) ;
          std::cout << (int) (*lexem).id << std::endl ;
          
          if (is_not_end(lexem) && (*(tmp = std::next(lexem))).id == thodd::comma) 
            std::swap(lexem, tmp) ;
        }

        if (is_not_end(lexem) && (*(tmp = std::next(lexem))).id == thodd::rbracket) {
          std::swap (lexem, tmp) ;
          std::cout << (int) (*lexem).id << std::endl ;
          
          if (is_not_end(lexem) && (*(tmp = std::next(lexem))).id == thodd::colon) {
            std::swap (lexem, tmp) ;
            std::cout << (int) (*lexem).id << std::endl ;

            if (is_not_end(lexem) && (*(tmp = std::next(lexem))).id == thodd::identifier) { 
              std::swap (lexem, tmp) ;
              std::cout << (int) (*lexem).id << std::endl ;
          
              if (is_not_end(lexem) && (*(tmp = std::next(lexem))).id == thodd::semi_colon) { 
                std::swap (lexem, tmp) ;
                std::cout << (int) (*lexem).id << std::endl ;
                
                return 
                std::make_optional (lexem) ;
              }
            }
          }
        }
      }
    }
    
    return 
    std::make_optional<decltype(lexem)>() ;
  } ;

  std::cout << std::boolalpha 
            << is_signature (lexems.begin(), lexems.end()).has_value() 
            << std::endl ;
}