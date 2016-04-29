/*
 * gcc scheme.c -o scheme
 */

#include <stdio.h>

int main(void) {
  char exp[100];
  while(1) {
    printf("> ");
    //scanf("%s", exp);
    gets(exp);
    printf("You entered: %s\n", exp);
  }
}
