#ifndef __thodd_child_ctx_hpp__
#  define __thodd_child_ctx_hpp__

namespace detail {
  auto child_ctx(auto const & ctx, auto const & child) {
    return ctx + "::" + child ;
  }
}

#endif