#include "build_context.hpp"
#include "extract_element.hpp"
#include <algorithm>

std::vector<std::string>
build_context(std::string const & parent_context, thodd const & tdd) {
  std::vector<std::string> ctx_identifiers {tdd.filename} ;
  auto const & tdd_ctx = *ctx_identifiers.begin() ;
 
  for (auto const & decl : tdd.declarations) {
    auto && child_identifiers = build_context(tdd_ctx, decl) ;
    std::move(child_identifiers.begin(), child_identifiers.end(), 
              ctx_identifiers.end()) ;
  }

  return ctx_identifiers ;
}

std::vector<std::string>
build_context(std::string const & parent_context, declaration const & decl) {
  switch (decl.type) {
    case declaration::type_::function : 
      return build_context(
        parent_context, 
        extract_pod_declaration(
          decl.data.begin(), 
          decl.data.end()).ext) ;
    case declaration::type_::pod : 
      return build_context(
        parent_context, 
        extract_function_declaration(
          decl.data.begin(), 
          decl.data.end()).ext) ; ;
    default : return {} ;
  }
}

std::vector<std::string>
build_context(std::string const & parent_context, function_declaration const & func_decl) {
  std::vector<std::string> ctx_identifiers {build_context(parent_context, func_decl.name)} ;
  auto const & func_ctx = (*ctx_identifiers.begin()) ;

  for (auto const & param : func_decl.parameters) {
    auto && child_identifiers = build_context(func_ctx, param) ;
    std::move(child_identifiers.begin(), child_identifiers.end(), 
              ctx_identifiers.end()) ;
  }

  return ctx_identifiers ;
}

std::vector<std::string>
build_context(std::string const & parent_context, parameter const & param) {
  return build_context(parent_context, param.name) ;
}

std::vector<std::string>
build_context(std::string const & parent_context, pod_declaration const & pod_decl) {
  std::vector<std::string> ctx_identifiers {build_context(parent_context, pod_decl.name)} ;
  auto const & pod_ctx = (*ctx_identifiers.begin()) ;

  for (auto const & mbr : pod_decl.members) {
    auto && child_identifiers = build_context(pod_ctx, mbr) ;
    std::move(child_identifiers.begin(), child_identifiers.end(), 
              ctx_identifiers.end()) ;
  }

  return ctx_identifiers ;  
}

std::vector<std::string>
build_context(std::string const & parent_context, pod_member const & mbr) {
  return build_context(parent_context, mbr.name) ;
}

std::vector<std::string>
build_context(std::string const & parent_context, instruction const & inst) {
  switch (inst.type) {
    case instruction::type_::if_statement : 
      return build_context(parent_context, extract_if_statement(inst.data.begin(), inst.data.end()).ext) ;
    case instruction::type_::while_statement : 
      return build_context(parent_context, extract_while_statement(inst.data.begin(), inst.data.end()).ext) ;
    case instruction::type_::const_declaration : 
      return build_context(parent_context, extract_const_declaration(inst.data.begin(), inst.data.end()).ext) ;
    default : return {} ;
  }
}

std::vector<std::string>
build_context(std::string const & parent_context, if_statement const & if_stt) {
  // TODO generate id for if context
  std::vector<std::string> ctx_identifiers {parent_context + "::if_xxx"} ; 
  auto const & if_ctx = (*ctx_identifiers.begin()) ;
  
  for (auto const & inst : if_stt.instructions) {
    auto && child_identifiers = build_context(if_ctx, inst) ;
    std::move(child_identifiers.begin(), child_identifiers.end(), 
              ctx_identifiers.end()) ;
  }

  return ctx_identifiers ;
}

std::vector<std::string>
build_context(std::string const & parent_context, while_statement const & while_stt) {
  // TODO generate id for while context
  std::vector<std::string> ctx_identifiers {parent_context + "::while_xxx"} ;
  auto const & while_ctx = (*ctx_identifiers.begin()) ;
  
  for (auto const & inst : while_stt.instructions) {
    auto && child_identifiers = build_context(while_ctx, inst) ;
    std::move(child_identifiers.begin(), child_identifiers.end(), 
              ctx_identifiers.end()) ;
  }

  return ctx_identifiers ;
}

std::vector<std::string>
build_context(std::string const & parent_context, const_declaration const & cstdecl) {
  return {parent_context + "::" + cstdecl.name.data} ;
}

std::vector<std::string>
build_context(std::string const & parent_context, identifier const & ident) {
  return {parent_context + "::" + ident.data} ;
}


// std::chrono::now
// => {{std, [chrono]}, {std::chrono, [now]}}
std::map<std::string, std::vector<std::string>>
merge_context(std::vector<std::string> const & ctx_names) {}
