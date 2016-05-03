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

int add_operand(OPERAND *current_operand);

char *read(FILE *in) {
  struct AST ast;
  int c;
  char exp[100];
  int i = 0;
  int n = 0;

  OPERAND *current_operand = NULL;

  while((c = getc(in)) != 10) {
    printf("c is %c = %d\n", c, c);

    if(c == '(') {
      printf("e is (\n");
      n = n + 1;
    } else if(c == ')') {
      printf("e is )\n");
    } else if(c != ' ') {
      if(n == 1) {
        ast.operator = c;
      } else {
        if(current_operand == NULL) {
          printf("-------1\n");
          current_operand = (OPERAND *)malloc(sizeof(OPERAND));
          current_operand->value = c - '0';
          current_operand->next = NULL;
        } else {
          printf("-------2\n");
          OPERAND *next_operand = (OPERAND *)malloc(sizeof(OPERAND));
          next_operand->value = c - '0';
          next_operand->next = current_operand;
          current_operand = next_operand;
        }
      }

      /*if(n == 1) {
        ast.operator = c;
      } else if(n == 2) {
        ast.operand_1 = c - '0';
      } else if(n == 3) {
        ast.operand_2 = c - '0';
        }*/

      n = n + 1;
    }

    exp[i] = c;
    i = i + 1;
  }
  printf("You entered: %s\n", exp);
  //printf("current operand is %d\n", current_operand->value);
  //printf("next operand is %d\n", current_operand->next->value);
  printf("the result is %d\n", add_operand(current_operand));
  //printf("operator is %c, operand_1 is %d, operand_2 is %d\n", ast.operator, ast.operand_1, ast.operand_2);
  //printf("The result is %d\n", (int)ast.operand_1 + (int)ast.operand_2);
  //putc(c, stdout);
  return 1;
}

int add_operand(OPERAND *current_operand) {
  printf("\ncurrent operand is %d\n", current_operand->value);
  printf("the next operand is %p\n", current_operand->next);
  int result = 0;
  if(current_operand->next == NULL) {
    printf("last step\n");
    result = current_operand->value;
    printf("the last one is %d\n", result);
    return result;
  } else {
    printf("middle step\n");
    //printf("the pointer of next operand is %p\n", current_operand->next);
    result = current_operand->value + add_operand(current_operand->next);
    printf("the middle one is %d\n", result);
    return result;
  }
}

int main(void) {
  char* exp;
  while(1) {
    printf("> ");
    //scanf("%s", exp);
    //gets(exp);
    read(stdin);
    //printf("You entered: %s\n", exp);
  }
}
