/*
 * gcc scheme.c -o scheme
 */

#include <stdio.h>

struct AST {
  char operator;
  int operand_1;
  int operand_2;
};

char* read(FILE *in) {
  struct AST ast;
  int c;
  char exp[100];
  int i = 0;
  int n = 0;
  while((c = getc(in)) != 10) {
    if(c == '(') {
      printf("e is (");
      n = n + 1;
    } else if(c != ' ') {
      if(n == 1) {
        ast.operator = c;
      } else if(n == 2) {
        ast.operand_1 = c - '0';
      } else if(n == 3) {
        ast.operand_2 = c - '0';
      }

      n = n + 1;
    } else if(c == ')') {
      printf("e is )");
    }
    printf("c is %d\n", c);
    exp[i] = c;
    i = i + 1;
  }
  printf("You entered: %s\n", exp);
  printf("operator is %c, operand_1 is %d, operand_2 is %d\n", ast.operator, ast.operand_1, ast.operand_2);
  printf("The result is %d\n", (int)ast.operand_1 + (int)ast.operand_2);
  //putc(c, stdout);
  return 1;
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
