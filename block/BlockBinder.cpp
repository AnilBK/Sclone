#include "BlockBinder.hpp"

void BlockBinder::bind_function(
    const std::string bind_string,
    const block_generator_fn_ptr fn_that_returns_the_block) {
  for (const auto &b_block : bound_blocks) {
    auto b_bind_string = b_block.first;
    ERR_FAIL_COND_CRASH(b_bind_string == bind_string,
                        "[Debug] String \"" + bind_string +
                            "\" Previously bound to some function.");
  }

  bound_blocks.push_back({bind_string, fn_that_returns_the_block});
}

std::optional<block_generator_fn_ptr>
BlockBinder::get_bound_block_gen_fn(const std::string &query) {
  for (const auto &block : bound_blocks) {
    auto name = block.first;
    if (name == query) {
      return block.second;
    }
  }

  auto unbound_msg_str = "[Debug] Function \"" + query + "\" Possibly Unbound.";
  ERR_FAIL_COND_CRASH(true, unbound_msg_str);

  return {};
}