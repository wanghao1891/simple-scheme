/*
 * gcc scheme.c -o scheme
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct ELEMENT {
  int type; //1, number; 2, node
  int number;
  struct NODE *node;
  struct ELEMENT *next;
} ELEMENT;

typedef struct NODE {
  char operator; //+ - * /
  ELEMENT *element;
} NODE;

int add_element(ELEMENT *element);

//(+ 1 (+ 2 3))
NODE *parse(FILE *in, ELEMENT *previous_element) {
  int c;
  int n = 0;
  NODE *current_node = malloc(sizeof(NODE));
  ELEMENT *current_element;

  while((c = getc(in)) != 10) {
    //printf("c is %c\n", c);
    if(c != ' ' && c != ')') {
      if(n == 0) {
        current_node->operator = c;
      } else {
        current_element = malloc(sizeof(ELEMENT));
        if(c == '(') {
          NODE *child = parse(in, current_element);
          current_element->type = 2;
          current_element->node = child;
        } else {
          //printf("number is %d\n", c - '0');
          current_element->type = 1;
          current_element->number = c - '0';

          if(n == 1) {
            current_node->element = current_element;
          }
        }

        if(previous_element != NULL) {
          previous_element->next = current_element;
        }

        previous_element = current_element;
      }

      n = n + 1;
    } else if(c == ')') {
      //current_element = NULL;
      previous_element->next = NULL;
      break;
    }
  }

  //printf("the operator of node is %c\n", current_node->operator);
  //if(current_node->element != NULL) {
  //  printf("the type of first element of node is %d\n", current_node->element->type);
  //}

  return current_node;
}

int compute(char operator, int operand1, int operand2) {
  switch(operator) {
  case '+':
    return operand1 + operand2;
  case '-':
    return operand1 - operand2;
  case '*':
    return operand1 * operand2;
  case '/':
    return operand1 / operand2;
  default:
    return 0;
  }
}

int compute_node_element(ELEMENT *element, char operator) {
  if(element->next == NULL) {
    if(element->type == 1) {
      return element->number;
    } else {
      return compute_node_element(element->node->element,
                                  element->node->operator);
    }
  } else {
    if(element->type == 1) {
      return compute(operator,
                     element->number,
                     compute_node_element(element->next,
                                          operator));
    } else {
      return compute(operator,
                     compute_node_element(element->node->element,
                                          element->node->operator),
                     compute_node_element(element->next,
                                          operator));
    }
  }
}

int main(void) {
  //char* exp;
  while(1) {
    printf("> ");

    int c = getc(stdin);
    //printf("c is %c = %d\n", c, c);
    if(c == '(') {
      NODE * root = parse(stdin, NULL);
      int result = compute_node_element(root->element, root->operator);

      c = getc(stdin);
      if(c == 10) {
        printf("%d\n", result);
      }
    }
  }
}
