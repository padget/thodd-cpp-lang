#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

#include "../src/has_element.hpp"
#include "../src/extract_lexems.hpp"

using thd = lexem::type_ ;

struct test_result {
  std::string name ;
  bool passed ;
} ;

test_result 
make_result (bool res, std::string const & name) {
  return {name, res} ;
}

std::vector<lexem>
make_lexems(std::vector<thd> const & thds) {
  std::vector<lexem> lxms ;
  std::transform(
    thds.begin(), thds.end(), 
    std::back_inserter(lxms), 
    [] (thd const & th) {return lexem{th} ;}) ;
  
  return lxms ;
}

/// //////// ///
/// TEST HAS ///
/// //////// /// 

test_result test_has_lbracket () {
  std::vector<lexem> lxs = make_lexems({thd::lbracket}) ;
  return make_result(has_lbracket(lxs.begin(), lxs.end()), "test_has_lbracket") ;
}

test_result test_has_rbracket () {
  std::vector<lexem> lxs = make_lexems({thd::rbracket}) ;
  return make_result(has_rbracket(lxs.begin(), lxs.end()), "test_has_rbracket") ;
}

test_result test_has_lbrace () {
  std::vector<lexem> lxs = make_lexems({thd::lbrace}) ;
  return make_result(has_lbrace(lxs.begin(), lxs.end()), "test_has_lbrace") ;
}

test_result test_has_rbrace () {
  std::vector<lexem> lxs = make_lexems({thd::rbrace}) ;
  return make_result(has_rbrace(lxs.begin(), lxs.end()), "test_has_rbrace") ;
}

test_result test_has_return_kw () {
  std::vector<lexem> lxs = make_lexems({thd::return_kw}) ;
  return make_result(has_return_kw(lxs.begin(), lxs.end()), "test_has_return_kw") ;
}

test_result test_has_lambda_kw () {
  std::vector<lexem> lxs = make_lexems({thd::lambda_kw}) ;
  return make_result(has_lambda_kw(lxs.begin(), lxs.end()), "test_has_lambda_kw") ;
}

test_result test_has_colon () {
  std::vector<lexem> lxs = make_lexems({thd::colon}) ;
  return make_result(has_colon(lxs.begin(), lxs.end()), "test_has_colon") ;
}

test_result test_has_comma () {
  std::vector<lexem> lxs = make_lexems({thd::comma}) ;
  return make_result(has_comma(lxs.begin(), lxs.end()), "test_has_comma") ;
}

test_result test_has_pod_kw () {
  std::vector<lexem> lxs = make_lexems({thd::pod_kw}) ;
  return make_result(has_pod_kw(lxs.begin(), lxs.end()), "test_has_pod_kw") ;
}

test_result test_has_semi_colon () {
  std::vector<lexem> lxs = make_lexems({thd::semi_colon}) ;
  return make_result(has_semi_colon(lxs.begin(), lxs.end()), "test_has_semi_colon") ;
}

test_result test_has_string () {
  std::vector<lexem> lxs = make_lexems({thd::string}) ;
  return make_result(has_string(lxs.begin(), lxs.end()), "test_has_string") ;
}

test_result test_has_identifier () {
  std::vector<lexem> lxs = make_lexems({thd::identifier}) ;
  return make_result(has_identifier(lxs.begin(), lxs.end()), "test_has_identifier") ;
}

test_result test_has_number () {
  std::vector<lexem> lxs = make_lexems({thd::number}) ;
  return make_result(has_number(lxs.begin(), lxs.end()), "test_has_number") ;
}

test_result test_has_function_call_expression_empty () {
  std::vector<lexem> lxs = make_lexems({
    thd::identifier, thd::lbracket, thd::rbracket}) ;
  return make_result(has_function_call_expression(lxs.begin(), lxs.end()), "test_has_function_call_expression_empty") ;
}

test_result test_has_function_call_expression_nb_ident () {
  std::vector<lexem> lxs = make_lexems({
    thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::rbracket}) ;
  return make_result(has_function_call_expression(lxs.begin(), lxs.end()), "test_has_function_call_expression_nb_ident") ;
}

test_result test_has_function_call_expression_end_with_comma () {
  std::vector<lexem> lxs = make_lexems({
    thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::comma, thd::rbracket}) ;
  return make_result(!has_function_call_expression(lxs.begin(), lxs.end()), "test_has_function_call_expression_end_with_comma") ;
}

test_result test_has_function_call_expression_end_with_func_call () {
  std::vector<lexem> lxs = make_lexems({
    thd::identifier, thd::lbracket, 
      thd::identifier, thd::comma, 
      thd::number, thd::comma, 
      thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::rbracket,
    thd::rbracket}) ;
  return make_result(has_function_call_expression(lxs.begin(), lxs.end()), "test_has_function_call_expression_end_with_func_call") ;
}

test_result test_has_parameter () {
  std::vector<lexem> lxs = make_lexems({thd::identifier, thd::colon, thd::identifier}) ;
  return make_result(has_parameter(lxs.begin(), lxs.end()), "test_has_parameter") ;
}

test_result test_has_lambda_expression () {
  std::vector<lexem> lxs = make_lexems({
    thd::lambda_kw, thd::lbracket, thd::rbracket, 
      thd::colon, thd::identifier, 
      thd::lbrace, 
        thd::return_kw, thd::identifier, thd::semi_colon,
      thd::rbrace}) ;
  return make_result(has_lambda_expression(lxs.begin(), lxs.end()), "test_has_lambda_expression") ;
}

test_result test_has_lambda_expression_with_parameters () {
  std::vector<lexem> lxs = make_lexems({
    thd::lambda_kw, thd::lbracket, 
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, 
    thd::rbracket, 
      thd::colon, thd::identifier, 
      thd::lbrace, 
        thd::return_kw, thd::identifier, thd::semi_colon,
      thd::rbrace}) ;
  return make_result(has_lambda_expression(lxs.begin(), lxs.end()), "test_has_lambda_expression_with_parameters") ;
}

test_result test_has_lambda_expression_with_parameters_and_const_instructions () {
  std::vector<lexem> lxs = make_lexems({
    thd::lambda_kw, thd::lbracket, 
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, 
    thd::rbracket, 
      thd::colon, thd::identifier, 
      thd::lbrace, 
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::return_kw, thd::identifier, thd::semi_colon,
      thd::rbrace}) ;
  return make_result(has_lambda_expression(lxs.begin(), lxs.end()), "test_has_lambda_expression_with_parameters_and_const_instructions") ;
}

test_result test_has_const_instruction () {
  std::vector<lexem> lxs = make_lexems({thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon}) ;
  return make_result(has_const_instruction(lxs.begin(), lxs.end()), "test_has_const_instruction") ;
}

test_result test_has_return_instruction () {
  std::vector<lexem> lxs = make_lexems({thd::return_kw, thd::identifier, thd::semi_colon}) ;
  return make_result(has_return_instruction(lxs.begin(), lxs.end()), "test_has_return_instruction") ;
}

test_result test_has_expression_identifier () {
  std::vector<lexem> lxs = make_lexems({thd::identifier}) ;
  return make_result(has_expression(lxs.begin(), lxs.end()), "test_has_expression_identifier") ;
}

test_result test_has_expression_number () {
  std::vector<lexem> lxs = make_lexems({thd::number}) ;
  return make_result(has_expression(lxs.begin(), lxs.end()), "test_has_expression_number") ;
}

test_result test_has_expression_function_call () {
  std::vector<lexem> lxs = make_lexems({
    thd::identifier, thd::lbracket, 
      thd::identifier, thd::comma, 
      thd::number, thd::comma, 
      thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::rbracket,
    thd::rbracket}) ;
  return make_result(has_expression(lxs.begin(), lxs.end()), "test_has_expression_function_call") ;
}

test_result test_has_expression_lambda () {
  std::vector<lexem> lxs = make_lexems({
    thd::lambda_kw, thd::lbracket, 
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, 
    thd::rbracket, 
      thd::colon, thd::identifier, 
      thd::lbrace, 
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::return_kw, thd::identifier, thd::semi_colon,
      thd::rbrace}) ;
  return make_result(has_expression(lxs.begin(), lxs.end()), "test_has_expression_lambda") ;
}

test_result test_has_pod_member() {
  std::vector<lexem> lxs = make_lexems({}) ;
  return make_result(has_pod_member(lxs.begin(), lxs.end()), "test_has_pod_member") ;
}

test_result test_has_pod_declaration() {
  std::vector<lexem> lxs = make_lexems({}) ;
  return make_result(has_pod_declaration(lxs.begin(), lxs.end()), "test_has_pod_member") ;
}

/// ///////// ///
/// TEST NEXT ///
/// ///////// ///


test_result test_next_lbracket () {
  std::vector<lexem> lxs = make_lexems({thd::lbracket}) ;
  return make_result(next_lbracket(lxs.begin(), lxs.end()) == lxs.end(), "test_next_lbracket") ;
}

test_result test_next_rbracket () {
  std::vector<lexem> lxs = make_lexems({thd::rbracket}) ;
  return make_result(next_rbracket(lxs.begin(), lxs.end()) == lxs.end(), "test_next_rbracket") ;
}

test_result test_next_lbrace () {
  std::vector<lexem> lxs = make_lexems({thd::lbrace}) ;
  return make_result(next_lbrace(lxs.begin(), lxs.end()) == lxs.end(), "test_next_lbrace") ;
}

test_result test_next_rbrace () {
  std::vector<lexem> lxs = make_lexems({thd::rbrace}) ;
  return make_result(next_rbrace(lxs.begin(), lxs.end()) == lxs.end(), "test_next_rbrace") ;
}

test_result test_next_return_kw () {
  std::vector<lexem> lxs = make_lexems({thd::return_kw}) ;
  return make_result(next_return_kw(lxs.begin(), lxs.end()) == lxs.end(), "test_next_return_kw") ;
}

test_result test_next_lambda_kw () {
  std::vector<lexem> lxs = make_lexems({thd::lambda_kw}) ;
  return make_result(next_lambda_kw(lxs.begin(), lxs.end()) == lxs.end(), "test_next_lambda_kw") ;
}

test_result test_next_colon () {
  std::vector<lexem> lxs = make_lexems({thd::colon}) ;
  return make_result(next_colon(lxs.begin(), lxs.end()) == lxs.end(), "test_next_colon") ;
}

test_result test_next_comma () {
  std::vector<lexem> lxs = make_lexems({thd::comma}) ;
  return make_result(next_comma(lxs.begin(), lxs.end()) == lxs.end(), "test_next_comma") ;
}

test_result test_next_pod_kw () {
  std::vector<lexem> lxs = make_lexems({thd::pod_kw}) ;
  return make_result(next_pod_kw(lxs.begin(), lxs.end()) == lxs.end(), "test_next_pod_kw") ;
}

test_result test_next_semi_colon () {
  std::vector<lexem> lxs = make_lexems({thd::semi_colon}) ;
  return make_result(next_semi_colon(lxs.begin(), lxs.end()) == lxs.end(), "test_next_semi_colon") ;
}

test_result test_next_string () {
  std::vector<lexem> lxs = make_lexems({thd::string}) ;
  return make_result(next_string(lxs.begin(), lxs.end()) == lxs.end(), "test_next_string") ;
}

test_result test_next_identifier () {
  std::vector<lexem> lxs = make_lexems({thd::identifier}) ;
  return make_result(next_identifier(lxs.begin(), lxs.end()) == lxs.end(), "test_next_identifier") ;
}

test_result test_next_number () {
  std::vector<lexem> lxs = make_lexems({thd::number}) ;
  return make_result(next_number(lxs.begin(), lxs.end()) == lxs.end(), "test_next_number") ;
}

test_result test_next_function_call_expression_empty () {
  std::vector<lexem> lxs = make_lexems({
    thd::identifier, thd::lbracket, thd::rbracket}) ;
  return make_result(next_function_call_expression(lxs.begin(), lxs.end()) == lxs.end(), "test_next_function_call_expression_empty") ;
}

test_result test_next_function_call_expression_end_with_func_call () {
  std::vector<lexem> lxs = make_lexems({
    thd::identifier, thd::lbracket, 
      thd::identifier, thd::comma, 
      thd::number, thd::comma, 
      thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::rbracket,
    thd::rbracket}) ;
  return make_result(next_function_call_expression(lxs.begin(), lxs.end()) == lxs.end(), "test_next_function_call_expression_end_with_func_call") ;
}

test_result test_next_parameter () {
  std::vector<lexem> lxs = make_lexems({thd::identifier, thd::colon, thd::identifier}) ;
  return make_result(next_parameter(lxs.begin(), lxs.end()) == lxs.end(), "test_next_parameter") ;
}

test_result test_next_lambda_expression () {
  std::vector<lexem> lxs = make_lexems({
    thd::lambda_kw, thd::lbracket, thd::rbracket, 
      thd::colon, thd::identifier, 
      thd::lbrace, 
        thd::return_kw, thd::identifier, thd::semi_colon,
      thd::rbrace}) ;
  return make_result(next_lambda_expression(lxs.begin(), lxs.end()) == lxs.end(), "test_next_lambda_expression") ;
}

test_result test_next_lambda_expression_with_parameters () {
  std::vector<lexem> lxs = make_lexems({
    thd::lambda_kw, thd::lbracket, 
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, 
    thd::rbracket, 
      thd::colon, thd::identifier, 
      thd::lbrace, 
        thd::return_kw, thd::identifier, thd::semi_colon,
      thd::rbrace}) ;
  return make_result(next_lambda_expression(lxs.begin(), lxs.end()) == lxs.end(), "test_next_lambda_expression_with_parameters") ;
}

test_result test_next_lambda_expression_with_parameters_and_const_instructions () {
  std::vector<lexem> lxs = make_lexems({
    thd::lambda_kw, thd::lbracket, 
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, 
    thd::rbracket, 
      thd::colon, thd::identifier, 
      thd::lbrace, 
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::return_kw, thd::identifier, thd::semi_colon,
      thd::rbrace}) ;
  return make_result(next_lambda_expression(lxs.begin(), lxs.end()) == lxs.end(), "test_next_lambda_expression_with_parameters_and_const_instructions") ;
}

test_result test_next_const_instruction () {
  std::vector<lexem> lxs = make_lexems({thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon}) ;
  return make_result(next_const_instruction(lxs.begin(), lxs.end()) == lxs.end(), "test_next_const_instruction") ;
}

test_result test_next_return_instruction () {
  std::vector<lexem> lxs = make_lexems({thd::return_kw, thd::identifier, thd::semi_colon}) ;
  return make_result(next_return_instruction(lxs.begin(), lxs.end()) == lxs.end(), "test_next_return_instruction") ;
}

test_result test_next_expression_identifier () {
  std::vector<lexem> lxs = make_lexems({thd::identifier}) ;
  return make_result(next_expression(lxs.begin(), lxs.end()) == lxs.end(), "test_next_expression_identifier") ;
}

test_result test_next_expression_number () {
  std::vector<lexem> lxs = make_lexems({thd::number}) ;
  return make_result(next_expression(lxs.begin(), lxs.end()) == lxs.end(), "test_next_expression_number") ;
}

test_result test_next_expression_function_call () {
  std::vector<lexem> lxs = make_lexems({
    thd::identifier, thd::lbracket, 
      thd::identifier, thd::comma, 
      thd::number, thd::comma, 
      thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::rbracket,
    thd::rbracket}) ;
  return make_result(next_expression(lxs.begin(), lxs.end()) == lxs.end(), "test_next_expression_function_call") ;
}

test_result test_next_expression_lambda () {
  std::vector<lexem> lxs = make_lexems({
    thd::lambda_kw, thd::lbracket, 
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, thd::comma,
      thd::identifier, thd::colon, thd::identifier, 
    thd::rbracket, 
      thd::colon, thd::identifier, 
      thd::lbrace, 
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon,
        thd::return_kw, thd::identifier, thd::semi_colon,
      thd::rbrace}) ;
  return make_result(next_expression(lxs.begin(), lxs.end()) == lxs.end(), "test_next_expression_lambda") ;
}

test_result test_next_pod_member () {
  std::vector<lexem> lxs = make_lexems({}) ;
  return make_result(false/*next_pod_member(lxs.begin(), lxs.end()) == lxs.end()*/, "next_pod_member") ;
}

test_result test_next_pod_declaration () {
  std::vector<lexem> lxs = make_lexems({}) ;
  return make_result(false/*next_pod_declaration(lxs.begin(), lxs.end()) == lxs.end()*/, "next_pod_declaration") ;
}


int main() {
  std::vector<std::function<test_result()>> tests = {
    test_has_lbracket, test_has_rbracket, test_has_lbrace, test_has_rbrace, 
    test_has_return_kw, test_has_lambda_kw, test_has_colon, test_has_comma, 
    test_has_pod_kw, test_has_semi_colon, test_has_string, test_has_identifier, 
    test_has_number, test_has_function_call_expression_empty, 
    test_has_function_call_expression_end_with_comma, 
    test_has_function_call_expression_nb_ident,
    test_has_function_call_expression_end_with_func_call, test_has_parameter,
    test_has_lambda_expression, test_has_lambda_expression_with_parameters,
    test_has_lambda_expression_with_parameters_and_const_instructions, 
    test_has_const_instruction, test_has_return_instruction,  
    test_has_expression_identifier, test_has_expression_number, 
    test_has_expression_lambda, test_has_expression_function_call,
    test_has_pod_member, test_has_pod_declaration,

    test_next_lbracket, test_next_rbracket, test_next_lbrace, test_next_rbrace, 
    test_next_return_kw, test_next_lambda_kw, test_next_colon, test_next_comma, 
    test_next_pod_kw, test_next_semi_colon, test_next_string, test_next_identifier, 
    test_next_number, test_next_function_call_expression_empty,
    test_next_function_call_expression_end_with_func_call, test_next_parameter,
    test_next_lambda_expression, test_next_lambda_expression_with_parameters,
    test_next_lambda_expression_with_parameters_and_const_instructions, 
    test_next_const_instruction, test_next_return_instruction, 
    test_next_expression_identifier, test_next_expression_number, 
    test_next_expression_lambda, test_next_expression_function_call, 
    test_next_pod_declaration, test_next_pod_member
  } ;
  
  bool passed = true ;

  std::for_each (tests.begin(), tests.end(), [&passed] (auto && test) {
    auto && result = test() ;
    std::cout << result.name << (result.passed ? " ---- OK" : " ---- KO /!\\") << std::endl ;
    passed = passed && result.passed ;
  }) ;
  
  
  if (passed)
    return EXIT_SUCCESS ;
  
  std::cout << "une erreur est survenue" << std::endl ; 
  return EXIT_FAILURE ;
}