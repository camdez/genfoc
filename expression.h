#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <iostream>
#include <list>
#include <map>
#include <assert.h>

#include "variable.h"


using std::string;

class expression
{
private:
  string id;
  std::list<string> sub_expressions;
  std::map<string, string> variables;
  // Keep track of how many variables an expression declares so that
  // we can pop that many off the variable stack when it goes out of
  // scope.
  unsigned int num_variables;

  void increment_var_count();

public:
  expression();
  string getId();
  void setId(string newId);
  void newSub(string newSub);
  string getValue();
  void add_var(string key, string value);
  int get_var_count();
  void add_vars_to_stack(std::list<variable>& vstack);
};

#endif // __EXPRESSION_H__
