#include <stdio.h>
#include <ctype.h> // isdigit

#define isplus(c) (c == 43)
#define ismins(c) (c == 45)
#define isdivi(c) (c == 47)
#define ismult(c) (c == 42)
#define isoper(c) (isplus(c) || ismins(c) || isdivi(c) || ismult(c))

int main() {
  
  int input;
  int stack[10];
  int index = 0;
  int number = 0;
  int line = 1;
  char err = 0;
  
  while ((input = getchar()) != EOF) {
    
    if (number && !isdigit(input)) {
      stack[index] = number;
      number = 0;
      index++;
    }
    
    // New line
    if (input == '\n') {
      printf("line %d: ", line);
      
      if (err) {
        printf("error at %c", err);
        err = 0;
      } else if (index != 1) {
        printf("error at \\n");
      } else {
        printf("%d", stack[0]);
      }
      
      printf("\n");
      index = 0;
      line++;
    
    // Digit
    } else if (isdigit(input) && index < 10) {
      number = number
        ? number * 10 + (input - '0')
        : input - '0';
    
    // Operator
    } else if (isoper(input) && index >= 2 && !(isdivi(input) && !stack[index-1])) {
      if (isplus(input)) {
        stack[index-2] += stack[index-1];
      } else if (ismins(input)) {
        stack[index-2] -= stack[index-1];
      } else if (ismult(input)) {
        stack[index-2] *= stack[index-1];
      } else if (isdivi(input)) {
        stack[index-2] /= stack[index-1];
      }
      index -= 1;
    
    } else if (input != ' ' && err == 0) {
      err = input;
    }
  }
  
  return(0);
}
