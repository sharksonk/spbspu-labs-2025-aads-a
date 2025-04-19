#include <iostream>
#include <functional>
#include <cstring>
#include "bitree.hpp"

int main()
{
  karnauhova::BiTree< size_t, std::string, std::less< std::pair< size_t, std::string > > > meow;
  meow.push(1, "meow");
  meow.push(2, "meow meow");
  meow.push(3, "meow meow meow");
  meow.push(4, "lol");
  std::cout  << meow.get_value(4) << "\n";

}
