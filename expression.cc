#include "expression.h"

expression::expression()
{
  num_variables = 0;
}

void expression::increment_var_count()
{
  num_variables++;
}

int expression::get_var_count()
{
  return num_variables;
}

void expression::add_var(string key, string value)
{
  // TODO: Check to see if a variable of this name has already been
  // declared in this expression.  If it has, our var_count will get
  // all jacked up and then we will start popping the wrong values off
  // the stack.  That will get ugly.
  variables[key] = value;
  increment_var_count();
}

void expression::add_vars_to_stack(std::list<variable>& vstack)
{
  if (num_variables == 0)
    return;

  variable* new_var;

  for (std::map<string, string>::iterator var_iter = variables.begin();
       var_iter != variables.end(); var_iter++)
    {
      new_var = new variable;
      new_var->key = var_iter->first;
      new_var->value = var_iter->second;
      new_var->resolved = false;

      vstack.push_back(*new_var);
    }
}

void expression::setId(string newId)
{
  id = newId;
}

void expression::newSub(string newSub)
{
  sub_expressions.push_back(newSub);
}

string expression::getId()
{
  return id;
}

string expression::getValue()
{
  int number = rand() % sub_expressions.size();

  int i = 0;

  for (std::list<string>::iterator sub_iter = sub_expressions.begin();
       sub_iter != sub_expressions.end(); sub_iter++)
    {
      if (i == number)
        return *sub_iter;

      i++;
    }

  assert(false); // should NEVER get here
}
