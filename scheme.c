/*
 * gcc scheme.c -o scheme
 */

#include <stdio.h>
#include <stdlib.h>

//#define START 0
#define IN 1 // inside a word
#define OUT 0 // outside a word
#define END 2 // end for a node

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

  printf("element is %p, number is %d, type is %d\n", element, element->number, element->type);
}

//(+ 1 (+ 2 3))
//(+ 1 (- 3 2) (* 2 3) (/ 6 2))
NODE *parse(FILE *in, ELEMENT *previous_element) {
  printf("\nloop start\n");
  int c;
  int n = 0;
  int state = OUT;
  NODE *current_node = malloc(sizeof(NODE));
  ELEMENT *current_element;

  char *operator = malloc(10);
  int i = 0;

  char *number = malloc(10);

  while((c = getc(in)) != 10) {
    printf("c is %c = %d\n", c, c);

    switch(c) {
    case ' ':
      if(state == IN) {
        if(n == 0) {
          operator[i] = '\0';
          current_node->operator = operator;
          printf("operator is %s\n", operator);
        } else {
          process_number(number, i, n, current_element, current_node);
        }

        n = n + 1;
        i = 0;
      }
      state = OUT;
      break;
    case ')':
      printf("previous element is %p, state is %d\n", previous_element, state);
      previous_element->next = NULL;
      if(state == IN) {
        process_number(number, i, n, current_element, current_node);

        n = n + 1;
        i = 0;
      }
      state = END;
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
          printf("previous element is %p, current_element is %p\n", previous_element, current_element);
        }

        if(c == '(') {
          NODE *child = parse(in, current_element);
          current_element->type = 2;
          current_element->node = child;
          printf("\nelement is %p, node is %p, type is %d\n", current_element, current_element->node, current_element->type);
        } else {
          number[i] = c;
          i = i + 1;
        }
      }

      if(c == '(') {
        state = OUT;
      } else {
        state = IN;
      }
    }

    if(state == END) {
      break;
    }
  }

  printf("loop end\n");
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
  printf("\nelement is %p, type is %d, next is %p\n", element, element->type, element->next);

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
      //printf("root node is %p\n", root);
      //printf("operator is %s\n", root->operator);
      //printf("element of root is %p\n", root->element);
      int result = compute_node_element(root->element, root->operator);

      c = getc(stdin);
      if(c == 10) {
        printf("%d\n", result);
      }
    }
  }
}
