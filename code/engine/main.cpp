#include <iostream>

#include <engine/core/eventh/eventh.hpp>

struct MyEvent {
  MyEvent(std::string data) : data(data) {}
  std::string data;
};

int main() {

  std::cout << "What the sigma" << std::endl;

  eventh::init();
  eventh::subscribe<MyEvent>([](MyEvent& event){
    std::cout << event.data << std::endl;
    return;
  });
  eventh::emit<MyEvent>("erm");
  eventh::poll();

  return 0;
}