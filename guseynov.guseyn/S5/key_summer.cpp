#include "key_summer.hpp"

guseynov::KeySummer::KeySummer():
  result_(0),
  values_("")
{}

void guseynov::KeySummer::operator()(const std::pair< const long long, std::string >& key_value)
{
  if (key_value.first > 0 && result_ > std::numeric_limits< long long >::max() - key_value.first)
  {
    throw std::overflow_error("Key sum overflow");
  }
  if (key_value.first < 0 && result_ < std::numeric_limits< long long >::min() - key_value.first)
  {
    throw std::underflow_error("Key sum underflow");
  }
  result_ += key_value.first;
  if (!values_.empty())
  {
    values_ += " ";
  }
  values_ += key_value.second;
}

void guseynov::KeySummer::reset()
{
  result_ = 0;
  values_.clear();
}
