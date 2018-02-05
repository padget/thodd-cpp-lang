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
      std::vector<lexem> const lxs = make_lexems({thd::comma}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_comma", 
        it("should detect a comma", expect(has_comma(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a comma", expect(has_comma(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::pod_kw}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_pod_kw", 
        it("should detect a pod_kw", expect(has_pod_kw(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a pod_kw", expect(has_pod_kw(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::semi_colon}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_semi_colon", 
        it("should detect a semi_colon", expect(has_semi_colon(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a semi_colon", expect(has_semi_colon(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::string}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_string", 
        it("should detect a string", expect(has_string(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a string", expect(has_string(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::identifier}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_identifier", 
        it("should detect a identifier", expect(has_identifier(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a identifier", expect(has_identifier(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::number}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::alias}) ;

      describe("has_number", 
        it("should detect a number", expect(has_number(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a number", expect(has_number(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs_fcall_empty = make_lexems({thd::identifier, thd::lbracket, thd::rbracket}) ;
      std::vector<lexem> const lxs_fcall_nb_ident = make_lexems({thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::rbracket}) ;
      std::vector<lexem> const lxs_fcall_end_comma = make_lexems({thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::comma, thd::rbracket}) ;
      std::vector<lexem> const lxs_fcall_fcall = make_lexems({ 
        thd::identifier, thd::lbracket, 
          thd::identifier, thd::comma, 
          thd::number, thd::comma, 
          thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::rbracket,
        thd::rbracket}) ;

      describe("has_function_call", 
        it("should detect an empty function_call expression", 
           expect(has_function_call(lxs_fcall_empty.begin(), lxs_fcall_empty.end()), be_true())),
        it("should detect a function_call expression with nb and identifier", 
           expect(has_function_call(lxs_fcall_nb_ident.begin(), lxs_fcall_nb_ident.end()), be_true())),
        it("shouldn't detect a function_call expression (ends with comma)", 
           expect(has_function_call(lxs_fcall_end_comma.begin(), lxs_fcall_end_comma.end()), be_false())),
        it("should detect a function_call expression (fcall as parameter)", 
           expect(has_function_call(lxs_fcall_fcall.begin(), lxs_fcall_fcall.end()), be_true()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::identifier, thd::colon, thd::identifier}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::identifier, thd::colon}) ;

      describe("has_parameter", 
        it("should detect a parameter", expect(has_parameter(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a parameter", expect(has_parameter(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs_lambda_empty = make_lexems({
        thd::lambda_kw, thd::lbracket, thd::rbracket, 
          thd::colon, thd::identifier, 
          thd::lbrace, 
            thd::return_kw, thd::identifier, thd::semi_colon,
          thd::rbrace}) ;

      std::vector<lexem> const lxs_lambda_params = make_lexems({
        thd::lambda_kw, thd::lbracket, 
          thd::identifier, thd::colon, thd::identifier, thd::comma,
          thd::identifier, thd::colon, thd::identifier, thd::comma,
          thd::identifier, thd::colon, thd::identifier, 
        thd::rbracket, thd::colon, thd::identifier, 
        thd::lbrace, 
          thd::return_kw, thd::identifier, thd::semi_colon,
        thd::rbrace}) ;

      std::vector<lexem> const lxs_lambda_full = make_lexems({
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

      describe("has_lambda", 
        it("should detect a lambda_expression with no parameter", 
           expect(has_lambda(lxs_lambda_empty.begin(), lxs_lambda_empty.end()), be_true())),
        it("should detect a lambda_expression with parameter", 
           expect(has_lambda(lxs_lambda_params.begin(), lxs_lambda_params.end()), be_true())),
        it("should detect a lambda_expression with parameters and const expression", 
           expect(has_lambda(lxs_lambda_full.begin(), lxs_lambda_full.end()), be_true()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::identifier, thd::colon, thd::identifier, thd::number, thd::semi_colon}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::identifier, thd::identifier, thd::number, thd::semi_colon}) ;

      describe("has_const_instruction", 
        it("should detect a const_instruction", expect(has_const_instruction(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a const_instruction", expect(has_const_instruction(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::return_kw, thd::identifier, thd::semi_colon}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::return_kw, thd::identifier, }) ;

      describe("has_return_instruction", 
        it("should detect a return_instruction", expect(has_return_instruction(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a return_instruction", expect(has_return_instruction(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs = make_lexems({thd::identifier, thd::colon, thd::identifier, thd::semi_colon}) ;
      std::vector<lexem> const lxs2 = make_lexems({thd::identifier, thd::colon, thd::identifier}) ;

      describe("has_member", 
        it("should detect a member", expect(has_member(lxs.begin(), lxs.end()), be_true())),
        it("shouldn't detect a member", expect(has_member(lxs2.begin(), lxs2.end()), be_false()))) ;
    }, 
    [] {
      std::vector<lexem> const lxs_pod_empty = make_lexems({thd::pod_kw, thd::identifier, thd::lbrace, thd::rbrace}) ;
      std::vector<lexem> const lxs_pod_members = make_lexems({
        thd::pod_kw, thd::identifier, thd::lbrace, 
          thd::identifier, thd::colon, thd::identifier, thd::semi_colon, 
          thd::identifier, thd::colon, thd::identifier, thd::semi_colon, 
          thd::identifier, thd::colon, thd::identifier, thd::semi_colon,
        thd::rbrace}) ;

      describe("has_pod", 
        it("should detect a pod_declaration with no member", 
           expect(has_pod(lxs_pod_empty.begin(), lxs_pod_empty.end()), be_true())),
        it("should detect a pod_declaration with members", 
           expect(has_pod(lxs_pod_members.begin(), lxs_pod_members.end()), be_true()))) ;
    }) ;

    suite(
      "next_xxx", 
      [] {
        describe("", it("", expect(true, be_true()))) ;
      }) ;
  
  return EXIT_SUCCESS ;
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
  return make_result(next_function_call(lxs.begin(), lxs.end()) == lxs.end(), "test_next_function_call_expression_empty") ;
}

test_result test_next_function_call_expression_end_with_func_call () {
  std::vector<lexem> lxs = make_lexems({
    thd::identifier, thd::lbracket, 
      thd::identifier, thd::comma, 
      thd::number, thd::comma, 
      thd::identifier, thd::lbracket, thd::identifier, thd::comma, thd::number, thd::rbracket,
    thd::rbracket}) ;
  return make_result(next_function_call(lxs.begin(), lxs.end()) == lxs.end(), "test_next_function_call_expression_end_with_func_call") ;
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
  return make_result(next_lambda(lxs.begin(), lxs.end()) == lxs.end(), "test_next_lambda_expression") ;
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
  return make_result(next_lambda(lxs.begin(), lxs.end()) == lxs.end(), "test_next_lambda_expression_with_parameters") ;
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
  return make_result(next_lambda(lxs.begin(), lxs.end()) == lxs.end(), "test_next_lambda_expression_with_parameters_and_const_instructions") ;
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
  return make_result(next_member(lxs.begin(), lxs.end()) == lxs.end(), "next_pod_member") ;
}

test_result test_next_pod_declaration () {
  std::vector<lexem> lxs = make_lexems({thd::pod_kw, thd::identifier, thd::lbrace, thd::rbrace}) ;
  return make_result(next_pod(lxs.begin(), lxs.end()) == lxs.end(), "next_pod_declaration") ;
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

