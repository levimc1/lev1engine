#include <iostream>

#include <engine/core/eventh/eventh.hpp>

struct MyEvent {
  MyEvent(std::string data) : data(data) {}
  std::string data;
};

int main() {

  std::cout << "Hello, world!" << std::endl;

  auto ctx1 = eventh::init();
  eventh::subscribe<MyEvent>([](MyEvent& event){
    std::cout << event.data << std::endl;
  });
  eventh::subscribe<MyEvent>([](MyEvent& event){
    std::cout << event.data << " - Másolat (1.-es context) " << std::endl;
  });
  
  eventh::emit<MyEvent>("Esemény1 - 1 db");
  eventh::poll();
  eventh::emit<MyEvent>("Esemény2 - 2db"); // dupla
  eventh::emit<MyEvent>("Esemény3 - 2db");
  eventh::poll();
  eventh::emit<MyEvent>("Esemény4 - ctx váltás"); // ctx1-ben, váltás
  auto ctx2 = eventh::EventhContext();
  eventh::use(ctx2);
  eventh::emit<MyEvent>("Esemény5 - ctx váltva");
  eventh::subscribe<MyEvent>([](MyEvent& event){
    std::cout << event.data << " - ÚJ ctx " << std::endl;
  });
  eventh::poll();
  eventh::use(ctx1);
  eventh::emit<MyEvent>("Esemény6 - ctx visszaváltva");
  eventh::poll();

  return 0;
}