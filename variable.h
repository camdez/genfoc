#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>

struct variable
{
  std::string key;
  std::string value;
  bool resolved; // has the variable been completely parsed?
};

#endif // __VARIABLE_H__
