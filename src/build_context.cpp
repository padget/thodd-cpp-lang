#include "build_context.hpp"
#include "extract_element.hpp"
#include <algorithm>
#include <iostream>

std::vector<context_identifier>
build_context(std::string const & parent_context, thodd const & tdd) {
  std::vector<context_identifier> ctx_identifiers {{parent_context}} ;
  
  for (auto const & decl : tdd.declarations) {
    auto && child_identifiers = build_context(parent_context, decl) ;
    
    // for (auto && child : child_identifiers)
    //   ctx_identifiers.push_back(child) ;
  }

  return ctx_identifiers ;
}

std::vector<context_identifier>
build_context(std::string const & parent_context, declaration const & decl) {
  switch (decl.type) {
    case declaration::type_::function : 
      std::cout << "je suis un function" << std::endl ;
      return build_context(
        parent_context, 
        extract_function_declaration(
          decl.data.begin(), 
          decl.data.end()).ext) ;
    case declaration::type_::pod : 
      std::cout << "je suis un pod" << std::endl ;
      return build_context(
        parent_context, 
        extract_pod_declaration(
          decl.data.begin(), 
          decl.data.end()).ext) ; ;
    default : return {} ;
  }
}

std::vector<context_identifier>
build_context(std::string const & parent_context, function_declaration const & func_decl) {
  std::vector<context_identifier> ctx_identifiers ;
  //auto const & current_context = (*ctx_identifiers.begin()).name ;

  for (auto const & param : func_decl.parameters) 
    ctx_identifiers.push_back(build_context(parent_context, param)) ;

  return ctx_identifiers ;
}

context_identifier
build_context(std::string const & parent_context, parameter const & param) {
  return {parent_context + "::" + param.name.data, param.type.data} ;
}

std::vector<context_identifier>
build_context(std::string const & parent_context, pod_declaration const & pod_decl) {
  std::vector<context_identifier> ctx_identifiers {{parent_context + "::" + pod_decl.name.data}} ;
  auto const & pod_ctx = (*ctx_identifiers.begin()).name ;
  
  for (auto const & mbr : pod_decl.members)
    ctx_identifiers.push_back(build_context(pod_ctx, mbr)) ;
  
  std::cout << "prout" << std::endl ;

  return ctx_identifiers ;  
}

context_identifier
build_context(std::string const & parent_context, pod_member const & mbr) {
  return {parent_context + "::" + mbr.name.data, mbr.type.data} ;
}

std::vector<context_identifier>
build_context(std::string const & parent_context, instruction const & inst) {
  switch (inst.type) {
    case instruction::type_::if_statement : 
      return build_context(parent_context, extract_if_statement(inst.data.begin(), inst.data.end()).ext) ;
    case instruction::type_::while_statement : 
      return build_context(parent_context, extract_while_statement(inst.data.begin(), inst.data.end()).ext) ;
    case instruction::type_::const_declaration : 
      return {build_context(parent_context, extract_const_declaration(inst.data.begin(), inst.data.end()).ext)} ;
    default : return {} ;
  }
}

std::vector<context_identifier>
build_context(std::string const & parent_context, if_statement const & if_stt) {
  // TODO generate id for if context
  std::vector<context_identifier> ctx_identifiers {{parent_context + "::if_xxx"/*, TODO faire le type d'un if*/}} ; 
  auto const & if_ctx = (*ctx_identifiers.begin()).name ;
  
  for (auto const & inst : if_stt.instructions) {
    auto && child_identifiers = build_context(if_ctx, inst) ;
    std::copy(child_identifiers.begin(), child_identifiers.end(), 
              std::back_inserter(ctx_identifiers)) ;
  }

  return ctx_identifiers ;
}

std::vector<context_identifier>
build_context(std::string const & parent_context, while_statement const & while_stt) {
  // TODO generate id for while context
  std::vector<context_identifier> ctx_identifiers {{parent_context + "::while_xxx"/*, TODO faire le type d'un while*/}} ;
  auto const & while_ctx = (*ctx_identifiers.begin()).name ;
  
  for (auto const & inst : while_stt.instructions) {
    auto && child_identifiers = build_context(while_ctx, inst) ;
    std::copy(child_identifiers.begin(), child_identifiers.end(), 
              std::back_inserter(ctx_identifiers)) ;
  }

  return ctx_identifiers ;
}

context_identifier
build_context(std::string const & parent_context, const_declaration const & cstdecl) {
  return {parent_context + "::" + cstdecl.name.data, cstdecl.type.data} ;
}



// std::chrono::now
// => {{std, [chrono]}, {std::chrono, [now]}}
std::map<std::string, std::vector<std::string>>
merge_context(std::vector<std::string> const & ctx_names) {

}
