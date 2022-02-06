#ifndef TRUE_FALSE_H_
#define TRUE_FALSE_H_

#include <vector>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <conio.h>
#include <math.h>
#include <list>
#include <queue>
#include <cstdlib>
#include <ctype.h>
#define string_length strlen

using namespace std;

class logical_expression	
{  
public:
	char symbol[20]; // null if sentence is a more complex expression
	char connective[20]; // null if sentice is a symbol
	vector<logical_expression *> subexpressions;

	logical_expression();
	~logical_expression();
};


void print_expression(logical_expression * expression, char * separator);

logical_expression * read_expression(char * input_string);
logical_expression * read_expression(char * input_string, long & counter);

long read_subexpressions(char * input_string, long & counter, 
                         vector <logical_expression*> & subexpressions);

void read_word(char * input_string, long & counter, char * connective);
long valid_expression(logical_expression * expression);
long valid_symbol(char * symbol);
int exit_function(int value);

void check_true_false(logical_expression * knowledge_base, 
                      logical_expression * statement);


logical_expression::logical_expression()
{
  symbol[0] = 0;
  connective[0] = 0;
}

logical_expression::~logical_expression()
{
  unsigned long counter;
  for (counter = 0; counter < subexpressions.size(); counter++)
  {
    delete subexpressions[counter];
  }
}

void print_expression(logical_expression * expression, char * separator)
{
  if (expression == 0)
  {
    printf("\nINVALID\n");
  }
  else if (strcmp(expression->symbol, "") != 0)
  {
    printf("%s", expression->symbol);
  }
  else
  {
    printf("(%s", expression->connective);
    unsigned long counter;
    for (counter = 0; counter < expression->subexpressions.size(); counter++)
    {
      printf(" ");
      print_expression(expression->subexpressions[counter], "");
      printf(separator);
    }
    printf(")");
  }
}

logical_expression * read_expression(char * input_string)
{
  long counter = 0;
  return read_expression(input_string, counter);
}


logical_expression * read_expression(char * input_string, long & counter)
{
  logical_expression * result = new logical_expression();
  long length =  string_length(input_string);
  while(1)
  {
    if (counter >= length)
    {
      break;
    }
    if (isspace(input_string[counter]))    // skip whitespace
    {
      counter++;
      continue;
    }
    else if (input_string[counter] == '(')
    {
      counter++;
      read_word(input_string, counter, result->connective);
      read_subexpressions(input_string, counter, result->subexpressions);
      break;
    }
    else
    {
      read_word(input_string, counter, result->symbol);
      break;
    }
  }

  return result;
}


long read_subexpressions(char * input_string, long & counter, 
                         vector <logical_expression*> & subexpressions)
{
  long length =  string_length(input_string);
  while(1)
  {
    if (counter >= length)
    {
      printf("\nunexpected end of input\n");
      return 0;
    }
    if (isspace(input_string[counter]))    // skip whitespace
    {
      counter++;
      continue;
    }
    if (input_string[counter] == ')') // we are done
    {
      counter++;
      return 1;
    }
    else
    {
      logical_expression * expression = read_expression(input_string, counter);
      subexpressions.push_back(expression);
    }
  }
}

void read_word(char * input_string, long & counter, char * target)
{
  unsigned long second_counter = 0;
  while (1)
  {
    if (counter >= (long) string_length(input_string))
    {
      break;
    }
    if ((isalpha(input_string[counter])) || (input_string[counter] == '_') ||
        (isdigit(input_string[counter])))
    {
      target[second_counter] = input_string[counter];
      counter++;
      second_counter++;
    }
    else if ((input_string[counter] == ')') || (isspace(input_string[counter])))
    {
      break;
    }
    else
    {
      printf("unexpected character %c\n", input_string[counter]);
      exit_function(0);
    } 
  }

  target[second_counter] = 0;
}


long valid_expression(logical_expression *  expression)
{
  if (strcmp(expression->symbol, "") != 0)
  {
    return valid_symbol(expression->symbol);
  }

  if ((strcmp(expression->connective, "if") == 0) ||
      (strcmp(expression->connective, "iff") == 0))
  {
    if (expression->subexpressions.size() != 2)
    {
      printf("error: connective \"%s\" with %li arguments\n", 
             expression->connective, expression->subexpressions.size());
      return 0;
      return 0;
    }
  }
  else   if (strcmp(expression->connective, "not") == 0)
  {
    if (expression->subexpressions.size() != 1)
    {
      printf("error: connective \"%s\" with %li arguments\n", 
             expression->connective, expression->subexpressions.size());
      return 0;
    }
  }
  else if ((strcmp(expression->connective, "and") != 0) &&
           (strcmp(expression->connective, "or") != 0) &&
           (strcmp(expression->connective, "xor") != 0))
  {
    printf("error: unknown connective %s\n", expression->connective);
    return 0;
  }

  unsigned long counter;
  for (counter = 0; counter < expression->subexpressions.size(); counter++)
  {
    if (valid_expression(expression->subexpressions[counter]) == 0)
    {
      return 0;
    }
  }

  return 1;
}


long valid_symbol(char * symbol)
{
  if ((symbol == 0) || (strlen(symbol) == 0))
  {
    return 0;
  }
  unsigned long counter;
  for (counter = 0; counter < strlen(symbol);  counter++)
  {
    if ((symbol[counter] != '_') && (! (isalpha(symbol[counter]))) && (! (isdigit(symbol[counter]))))
    {
      return 0;
    }       
  }

  return 1;
}


int exit_function(int value)
{
//	char str_buf[200]; 
//	printf("Press <ENTER> to exit.\n");
//	gets(str_buf);

  exit(value);
  return value;
}


void check_true_false(logical_expression * knowledge_base, 
                      logical_expression * statement)
{
  // your code goes here
  printf("\nI don't know if the statement is definitely true or definitely false.\n");

  FILE * fp = fopen("result.txt", "wb");
  if (fp == 0)
  {
    printf("something is wrong, cannot open result.txt for writing\n");
  }
  else
  {
    fprintf(fp, "result unknown");
    fclose(fp);
  }
}
