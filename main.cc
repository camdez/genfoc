#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "variable.h"
#include "expression.h"

using std::cout;
using std::cerr;
using std::string;


string evaluateString(string ex_str);
string lookupVariable(string key);
void parseData();
void dump_stack();

std::list<expression*> glb_exp_list;
std::list<variable> var_stack;


int main()
{
  srand(time(NULL));

  parseData();
  cout << evaluateString("[root]") << std::endl;

  return 0;
}


// Output the stack. Useful for debugging.
void dump_stack()
{
  std::cerr << "Stack dump:\n  ";

  for (std::list<variable>::iterator stack_iter = var_stack.begin();
       stack_iter != var_stack.end(); stack_iter++)
    {
      std::cout << stack_iter->key << "=>" << stack_iter->value << ", ";
    }

  std::cerr << "\n";
}


// Parse the data, build expression table
void parseData()
{
  char buffer[1024];
  bool in_def = false;          // Are we inside of a definition statement
  bool open_bracket = false;    // Is there a currently open brace?
  expression* exp;

  while(true)
    {
      // Eat whitespace
      while(true)
        {
          char a = std::cin.peek();

          if ((a == ' ' || a == '\t'))
            std::cin.get();
          else
            break;
        }

      // Get the statement
      if (!std::cin.getline(buffer, 1024))
        break;

      // Comment line
      if (buffer[0] == '#')
        continue;

      // Blank line
      if (std::cin.gcount() == 1) // 1 because of the \n character
        continue;

      if (!in_def)
        {
          // TODO: make sure that the expression name doesn't have invalid characters

          // Expression definition just started
          in_def = true;
          exp = new expression;
          exp->setId(string(buffer));
        }
      else
        {
          if (buffer[0] == '{') // gobble it up
            {
              if (open_bracket)
                assert(false); // Parse error

              open_bracket = true;
              continue;
            }

          if (buffer[0] == '}')
            {
              if (!open_bracket)
                assert(false); // Parse error

              in_def = false;
              open_bracket = false;
              glb_exp_list.push_back(exp);
              continue;
            }

          // Check for variable definitions
          if (buffer[0] == '~')
            {
              // TODO: Check for valid variable names, make sure they
              // give a definition, etc.

              unsigned int space_location = 1;
              while (true)
                {
                  if (buffer[space_location] == ' ')
                    break;
                  else
                    space_location++;
                }


              // Make sure the first character after the ~ was not a space
              assert(space_location > 1);

              string buffer_str(buffer);

              // TODO: A space is not a decent criterion.  We should
              // be agnostic towards all varieties of whitespace
              string key = buffer_str.substr(1, space_location-1);
              string value = buffer_str.substr(space_location+1, (buffer_str.size()-space_location-1));

              exp->add_var(key, value);
              continue;
            }

          exp->newSub(string(buffer));
        }
    } // EOF

  if (open_bracket)
    assert(false); // we should not be in an expression definition at the end. One was left unterminated
}


// Evaluate a given string, resolving all subexpressions and variables
// down to the lowest level.
string evaluateString(string ex_str)
{
  // parse string
  std::istringstream ex_stream(ex_str);

  string buffer;
  string output = "";

  while (ex_stream >> buffer)
    {
      int end = buffer.size() - 1;

      // Allow punctuation at the end of tags and variables
      if ((buffer[end] == '.') || (buffer[end] == ',')
          || (buffer[end] == '!') || (buffer[end] == '?'))
        end--;

      // If this word is a tag
      if ((buffer[0] == '[') && (buffer[end] == ']'))
        {
          string findId = buffer.substr(1, end-1);

          bool wasFound = false;

          // find it in the table
          for (std::list<expression*>::iterator exp_iter = glb_exp_list.begin();
               exp_iter != glb_exp_list.end();
               exp_iter++)
            {
              if ((*exp_iter)->getId() == findId)
                {
                  wasFound = true;

                  // Put variables on the stack
                  (*exp_iter)->add_vars_to_stack(var_stack);

                  // Recursively call for evaluation
                  output += evaluateString((*exp_iter)->getValue());

                  // Take variables off the stack
                  for (int s = 0; s < (*exp_iter)->get_var_count(); s++)
                    var_stack.pop_back();

                  if (end != (buffer.size() - 1))
                    output += buffer[buffer.size()-1];

                  output += ' ';

                  goto CHECKED;
                }
            }

          if (!wasFound)
            {
              cout << std::endl << findId << " is not a valid lookup expression!" << std::endl;
              assert(false); // no definition for this string
            }
        }
      else if (buffer[0] == '$') // variable
        {
          output += evaluateString(lookupVariable(buffer.substr(1, end)));

          // Put back any punctuation
          if (end != (buffer.size() - 1))
            output += buffer[buffer.size()-1];

          output += ' ';
        }
      else // normal word
        {
          output += buffer + " ";
        }

    CHECKED:
      ;
    }

  // Don't output the space at the end of the last word because that
  // spacing is taken care of one level up
  output = output.substr(0, output.size()-1);
  return output;
}


// Find the most in-scope value for a variable. Does not resolve tags
// and sub-variables, we leave that up to evaluateString()
string lookupVariable(string key)
{
  string value;

  for (std::list<variable>::reverse_iterator stack_iter = var_stack.rbegin();
       stack_iter != var_stack.rend(); stack_iter++)
    {
      if (stack_iter->key == key)
        {
          // When we find the variable we are looking for on the
          // stack, resolve it if it is unresolved and replace the
          // version on the stack with the completely parsed version.
          if (!stack_iter->resolved)
            {
              stack_iter->resolved = true;
              stack_iter->value = evaluateString(stack_iter->value);
            }

          value = stack_iter->value;
          goto FOUND;
        }
    }

  cerr << "Variable \"" << key << "\" does not exist (at least not in this scope).\n";
  dump_stack();
  assert(false);

 FOUND:
  return value;
}
