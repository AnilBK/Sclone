#include "Object.hpp"

void Subject::notify_all() {
  for (const auto &fn_ptr : observers) {
    if (fn_ptr) {
      fn_ptr();
    }
  }
}

void Object::connect(const std::string &message, std::function<void()> fn) {
  // If there's existing message, then append the new fn ptr as the observer
  // for the given message.
  for (auto &signal : signals) {
    if (signal.message == message) {
      std::cout << "\"" << message
                << "\" Found. Binding a new observer to it.\n";
      signal.add_observer(fn);
      return;
    }
  }

  // If no existing messages,then append one.
  Subject new_signal;
  new_signal.message = message;
  new_signal.add_observer(fn);

  signals.push_back(new_signal);
  std::cout << "Adding new Signal with message: \"" << message << "\"\n";
}

void Object::emit_signal(const std::string &emitted_message) {
  std::cout << "Message Broadcasted: " << emitted_message << "\n";
  for (auto &signal : signals) {
    if (signal.message == emitted_message) {
      signal.notify_all();
      return;
    }
  }
}
