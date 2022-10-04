#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <functional>
#include <iostream>
#include <string>
#include <vector>

class Subject {
public:
  std::string message;
  std::vector<std::function<void()>> observers;

  void add_observer(std::function<void()> fn) { observers.push_back(fn); }
  void notify_all();
};

class Object {
public:
  std::vector<Subject> signals;

  void connect(const std::string &message, std::function<void()> fn);
  void emit_signal(const std::string &emitted_message);
};

/*
Usage:

class Button : public Object {
public:
  void pressed() {
    emit_signal("Pressed");
    std::cout << "Button Pressed.\n";
  }
};

int main() {
  Button btn;

  auto test = []() { std::cout << "Hi from the message receiver.\n"; };
  auto test2 = []() { std::cout << "Hi from the other message receiver.\n"; };

  btn.connect("Pressed", test);
  btn.connect("Pressed", test2);

  btn.pressed();

  return 0;
}
*/

#endif // OBJECT_HPP