#ifndef BLOCK_BINDER_HPP
#define BLOCK_BINDER_HPP

#include "Block.hpp"
#include <functional>
#include <optional>
#include <string>
#include <vector>

using block_generator_fn_ptr = std::function<void(Block *)>;

class BlockBinder {
  std::vector<std::pair<std::string, block_generator_fn_ptr>> bound_blocks;

public:
  /**
   * @brief Bind a function pointer to the given string.
   *
   * @param bind_string The string that identifies the generator function.
   * @param fn_that_returns_the_block The function pointer used to spawn a
   * block.
   * \note 'Bind String' should be same as 'Function Identifiers' contained in
   * the 'block' returned by that function. We use this identifiers to query the
   * function at runtime.
   */
  void bind_function(const std::string &bind_string,
                     const block_generator_fn_ptr &fn_that_returns_the_block);

  /// @brief Return the function pointer bound with the given string.
  /// @param query The string to search for.
  /// @return An optional containing a function pointer.
  std::optional<block_generator_fn_ptr>
  get_bound_block_gen_fn(const std::string &query);

  BlockBinder() { bound_blocks.reserve(25); }
};

#endif // BLOCK_BINDER_HPP