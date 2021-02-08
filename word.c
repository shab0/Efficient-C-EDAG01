#include <stdio.h> // getchar
#include <ctype.h> // isalpha
#define isend(c) (c == 32 || c == 10)

int main() {
  int c, len = 0, offset = 0;
  int maxlen = 0, maxidx = 0;
  char* input = NULL;
  
  while((c = getchar()) != EOF) {
    if (isend(c)) {
      if (len - offset > maxlen) {
        maxidx = offset;
        maxlen = len - offset;
      }
      offset = len;
    } else if (isalpha(c)) {
      input = realloc(input, ++len);
      input[len-1] = c;
    }
  }
  
  input[maxidx + maxlen] = 0;
  printf("%d characters in longest word: %s\n", maxlen, input + maxidx);
  free(input);
	return 0;
}
