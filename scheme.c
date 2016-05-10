/*
 * gcc scheme.c -o scheme
 */

#include <stdio.h>
#include <stdlib.h>

//#define START 0
#define IN 1 // inside a word
#define OUT 0 // outside a word

typedef struct ELEMENT {
  int type; //1, number; 2, node
  int number;
  struct NODE *node;
  struct ELEMENT *next;
} ELEMENT;

typedef struct NODE {
  char *operator; //+ - * / and others
  ELEMENT *element;
} NODE;

void process_number(char *number, int i, int n, ELEMENT *element, NODE *node) {
  number[i] = '\0';
  element->type = 1;
  element->number = atoi(number);

  if(n == 1) {
    node->element = element;
  }

  printf("number is %d\n", element->number);
  printf("type is %d\n", element->type);
}

//(+ 1 (+ 2 3))
//(+ 1 (- 3 2) (* 2 3) (/ 6 2))
NODE *parse(FILE *in, ELEMENT *previous_element) {
  int c;
  int n = 0;
  int state = OUT;
  NODE *current_node = malloc(sizeof(NODE));
  ELEMENT *current_element;

  char *operator = malloc(10);
  int i = 0;

  char *number = malloc(10);

  while((c = getc(in)) != 10) {
    printf("c is %c\n", c);

    switch(c) {
    case ' ':
      if(state == IN) {
        if(n == 0) {
          operator[i] = '\0';
          current_node->operator = operator;

        } else {
//          number[i] = '\0';
//          current_element->type = 1;
//          current_element->number = atoi(number);
//
//          if(n == 1) {
//            current_node->element = current_element;
//          }
//
//          printf("number is %d\n", current_element->number);

          process_number(number, i, n, current_element, current_node);
        }

        n = n + 1;
        i = 0;
      }
      state = OUT;
      break;
    case ')':
      previous_element->next = NULL;
      if(state == IN) {
//        number[i] = '\0';
//        current_element->type = 1;
//        current_element->number = atoi(number);
//
//        printf("number is %d\n", current_element->number);
//
//        if(n == 1) {
//          current_node->element = current_element;
//        }

        process_number(number, i, n, current_element, current_node);

        n = n + 1;
        i = 0;
      }
      break;
    default:
      if(n == 0) {// operator
        operator[i] = c;

        i = i + 1;
      } else {// operand
        if(state == OUT) {
          current_element = malloc(sizeof(ELEMENT));

          if(previous_element != NULL) {
            previous_element->next = current_element;
          }

          previous_element = current_element;
        }

        if(c == '(') {
          NODE *child = parse(in, current_element);
          current_element->type = 2;
          current_element->node = child;
        } else {
          //printf("number is %d\n", c - '0');
          //current_element->type = 1;
          //current_element->number = c - '0';

          number[i] = c;
          i = i + 1;

          //if(n == 1) {
          //  current_node->element = current_element;
          //}
        }
      }

      state = IN;
      //n = n + 1;
    }

    //printf("operator is %s\n", operator);

//    if(c != ' ' && c != ')') {
//      if(n == 0) {
//        current_node->operator = c;
//      } else {
//        current_element = malloc(sizeof(ELEMENT));
//        if(c == '(') {
//          NODE *child = parse(in, current_element);
//          current_element->type = 2;
//          current_element->node = child;
//        } else {
//          //printf("number is %d\n", c - '0');
//          current_element->type = 1;
//          current_element->number = c - '0';
//
//          if(n == 1) {
//            current_node->element = current_element;
//          }
//        }
//
//        if(previous_element != NULL) {
//          previous_element->next = current_element;
//        }
//
//        previous_element = current_element;
//      }
//
//      n = n + 1;
//    } else if(c == ')') {
//      //current_element = NULL;
//      previous_element->next = NULL;
//      break;
//    }
  }

  //printf("the operator of node is %c\n", current_node->operator);
  //if(current_node->element != NULL) {
  //  printf("the type of first element of node is %d\n", current_node->element->type);
  //}

  //printf("current node is %p\n", current_node);
  return current_node;
}

int compute(char *operator, int operand1, int operand2) {
  switch(operator[0]) {
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

int compute_node_element(ELEMENT *element, char *operator) {
  printf("element is %p\n", element);
  printf("type is %d\n", element->type);
  printf("next is %p\n", element->next);
  if(element->next == NULL) {
    if(element->type == 1) {
      printf("number -- 1 -- %d\n", element->number);
      return element->number;
    } else {
      return compute_node_element(element->node->element,
                                  element->node->operator);
    }
  } else {
    if(element->type == 1) {
      printf("number -- 2 -- %d\n", element->number);
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
      printf("root node is %p\n", root);
      printf("operator is %s\n", root->operator);
      printf("element of root is %p\n", root->element);
      int result = compute_node_element(root->element, root->operator);

      c = getc(stdin);
      if(c == 10) {
        printf("%d\n", result);
      }
    }
  }
}
