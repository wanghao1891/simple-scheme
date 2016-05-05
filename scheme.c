/*
 * gcc scheme.c -o scheme
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct OPERAND {
  int value;
  struct OPERAND *next;
} OPERAND;

struct AST {
  char operator;
  OPERAND operand;
};

typedef struct ELEMENT {
  int type; //0, start; 1, number; 2, node
  int number;
  struct NODE *node;
  struct ELEMENT *next;
} ELEMENT;

//typedef struct NODE {
//  int type; //1, operator; 2, number; 3, node
//  char operator; //+ - * /
//  int number;
//  NODE node;
//  NODE *next;
//} NODE;

typedef struct NODE {
  char operator; //+ - * /
  ELEMENT *element;
} NODE;

int add_operand(OPERAND *current_operand);

//(+ 1 (+ 2 3))
NODE *parse(FILE *in, ELEMENT *previous_element) {
  int c;
  int n = 0;
  NODE *current_node = malloc(sizeof(NODE));
  ELEMENT *current_element;

  while((c = getc(in)) != 10) {
    printf("c is %c\n", c);
    if(c != ' ') {
      if(n == 0) {
        current_node->operator = c;
      } else {
        current_element = malloc(sizeof(ELEMENT));
        if(c == '(') {
          NODE *child = parse(in, current_element);
          current_element->type = 1;
          current_element->node = child;
        } else {
          current_element->type = 0;
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
      current_element = NULL;
      break;
    }
  }

  printf("the operator of node is %c\n", current_node->operator);
  if(current_node->element != NULL) {
    printf("the type of first element of node is %d\n", current_node->element->type);
  }

  return current_node;
}

int read(FILE *in) {
  struct AST ast;
  int c;
  char exp[100];
  int i = 0;
  int n = 0;

  OPERAND *current_operand = NULL;

  while((c = getc(in)) != 10) {
    //printf("c is %c = %d\n", c, c);

    if(c == '(') {
      //printf("e is (\n");
      n = n + 1;
    } else if(c == ')') {
      //printf("e is )\n");
    } else if(c != ' ') {
      if(n == 1) {
        ast.operator = c;
      } else {
        if(current_operand == NULL) {
          //printf("-------1\n");
          current_operand = (OPERAND *)malloc(sizeof(OPERAND));
          current_operand->value = c - '0';
          current_operand->next = NULL;
        } else {
          //printf("-------2\n");
          OPERAND *next_operand = (OPERAND *)malloc(sizeof(OPERAND));
          next_operand->value = c - '0';
          next_operand->next = current_operand;
          current_operand = next_operand;
        }
      }

      n = n + 1;
    }

    exp[i] = c;
    i = i + 1;
  }

  printf("%d\n", add_operand(current_operand));

  //printf("You entered: %s\n", exp);
  //printf("current operand is %d\n", current_operand->value);
  //printf("next operand is %d\n", current_operand->next->value);
  //printf("the result is %d\n", add_operand(current_operand));
  //printf("operator is %c, operand_1 is %d, operand_2 is %d\n", ast.operator, ast.operand_1, ast.operand_2);
  //printf("The result is %d\n", (int)ast.operand_1 + (int)ast.operand_2);  //putc(c, stdout);
  return 1;
}

int add_operand(OPERAND *current_operand) {
  //printf("\ncurrent operand is %d\n", current_operand->value);
  //printf("the next operand is %p\n", current_operand->next);
  int result = 0;
  if(current_operand->next == NULL) {
    //printf("last step\n");
    result = current_operand->value;
    //printf("the last one is %d\n", result);
    return result;
  } else {
    //printf("middle step\n");
    result = current_operand->value + add_operand(current_operand->next);
    //printf("the middle one is %d\n", result);
    return result;
  }
}

int main(void) {
  //char* exp;
  while(1) {
    printf("> ");
    //scanf("%s", exp);
    //gets(exp);
    //read(stdin);

    int c = getc(stdin);
    printf("c is %c\n", c);
    if(c == '(') {
      //ELEMENT *start = malloc(sizeof(ELEMENT));
      //start->type = 0;
      NODE * root = parse(stdin, NULL);

      printf("the operator of root node is %c\n", root->operator);
    }

    //printf("You entered: %s\n", exp);
  }
}
