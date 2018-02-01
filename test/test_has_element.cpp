#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

#include "../src/has_element.hpp"
#include "../src/extract_lexems.hpp"

#include "jasmine.hpp"

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

int main() {
  suite(
    "has_xxx",
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_lbracket", 
        it("should detect a lbracket", expect(has_lbracket(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_lbracket(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::rbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_rbracket", 
        it("should detect a rbracket", expect(has_rbracket(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a rbracket", expect(has_rbracket(lxs2.begin(), lxs2.end()), be_false()))) ; 
    },  
    [] {
      std::vector<lexem> const lxs_identifier = make_lexems({thd::identifier}) ;
      
      std::vector<lexem> const lxs_number = make_lexems({thd::number}) ;
      
      std::vector<lexem> const lxs_fcall = make_lexems({
        thd::identifier, thd::lbracket, 
          thd::identifier, thd::comma, 
          thd::number, thd::comma, 
          thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::rbracket,
        thd::rbracket}) ;

      std::vector<lexem> const lxs_lambda = make_lexems({
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
          thd::rbrace
      }) ;

      describe("has_expression", 
        it("should detect a identifier", 
          expect(has_expression(lxs_identifier.begin(), lxs_identifier.end()), be_true())),
        it("should detect a number", 
          expect(has_expression(lxs_number.begin(), lxs_number.end()), be_true())),
        it("should detect a function call", 
          expect(has_expression(lxs_fcall.begin(), lxs_fcall.end()), be_true())),
        it("should detect a lambda", 
          expect(has_expression(lxs_lambda.begin(), lxs_lambda.end()), be_true()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbrace}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_lbrace", 
        it("should detect a lbrace", expect(has_lbrace(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbrace", expect(has_lbrace(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
   [] {
      std::vector<lexem> const lxs = make_lexems({thd::rbrace}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_rbrace", 
        it("should detect a rbrace", expect(has_rbrace(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a rbrace", expect(has_rbrace(lxs2.begin(), lxs2.end()), be_false()))) ;
    },
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::return_kw}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_return_kw", 
        it("should detect a return_kw", expect(has_return_kw(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a return_kw", expect(has_return_kw(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lambda_kw}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_lambda_kw", 
        it("should detect a lambda_kw", expect(has_lambda_kw(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lambda_kw", expect(has_lambda_kw(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::colon}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_colon", 
        it("should detect a colon", expect(has_colon(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a colon", expect(has_colon(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_comma", 
        it("should detect a lbracket", expect(has_comma(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_comma(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_pod_kw", 
        it("should detect a lbracket", expect(has_pod_kw(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_pod_kw(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_semi_colon", 
        it("should detect a lbracket", expect(has_semi_colon(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_semi_colon(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_string", 
        it("should detect a lbracket", expect(has_string(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_string(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_identifier", 
        it("should detect a lbracket", expect(has_identifier(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_identifier(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_number", 
        it("should detect a lbracket", expect(has_number(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_number(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_function_call_expression", 
        it("should detect a lbracket", expect(has_function_call_expression(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_function_call_expression(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_parameter", 
        it("should detect a lbracket", expect(has_parameter(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_parameter(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_lambda_expression", 
        it("should detect a lbracket", expect(has_lambda_expression(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_lambda_expression(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_const_instruction", 
        it("should detect a lbracket", expect(has_const_instruction(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_const_instruction(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_return_instruction", 
        it("should detect a lbracket", expect(has_return_instruction(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_return_instruction(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::lbracket}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_pod_member", 
        it("should detect a lbracket", expect(has_pod_member(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a lbracket", expect(has_pod_member(lxs2.begin(), lxs2.end()), be_false()))) ;
    }) ;
  
  return EXIT_SUCCESS ;
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


test_result test_has_pod_member () {
  std::vector<lexem> lxs = make_lexems({thd::identifier, thd::colon, thd::identifier, thd::semi_colon}) ;
  return make_result(has_pod_member(lxs.begin(), lxs.end()), "test_has_pod_member") ;
}

test_result test_has_pod_declaration_empty () {
  std::vector<lexem> lxs = make_lexems({thd::pod_kw, thd::identifier, thd::lbrace, thd::rbrace}) ;
  return make_result(has_pod_declaration(lxs.begin(), lxs.end()), "test_has_pod_member") ;
}

test_result test_has_pod_declaration_with_members () {
  std::vector<lexem> lxs = make_lexems({
    thd::pod_kw, thd::identifier, thd::lbrace, 
      thd::identifier, thd::colon, thd::identifier, thd::semi_colon, 
      thd::identifier, thd::colon, thd::identifier, thd::semi_colon, 
      thd::identifier, thd::colon, thd::identifier, thd::semi_colon,
    thd::rbrace}) ;
  return make_result(has_pod_declaration(lxs.begin(), lxs.end()), "test_has_pod_declaration_with_members") ;
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
  std::vector<lexem> lxs = make_lexems({{thd::identifier, thd::colon, thd::identifier, thd::semi_colon}}) ;
  return make_result(next_pod_member(lxs.begin(), lxs.end()) == lxs.end(), "next_pod_member") ;
}

test_result test_next_pod_declaration () {
  std::vector<lexem> lxs = make_lexems({thd::pod_kw, thd::identifier, thd::lbrace, thd::rbrace}) ;
  return make_result(next_pod_declaration(lxs.begin(), lxs.end()) == lxs.end(), "next_pod_declaration") ;
}


/// //////// ///
/// E2E TEST ///
/// //////// ///
#include "../src/from_file.hpp"

test_result test_has_thodd_by_stream () {
  auto const & stream   = from_file("main.thodd") ;
  auto const & lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs(0)) ;
  auto const & filtered = filter_lexems(lexems.begin(), lexems.end()) ;

  return make_result(has_thodd(filtered.begin(), filtered.end()), "test_has_thodd_by_stream") ;
}

