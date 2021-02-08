#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"

#define ispace(c) (c == ' ')

typedef struct poly_t poly_t;

struct poly_t {
  int coe;
  int exp;
  poly_t *next;
};

poly_t *new_term(int c, int e) {
  poly_t *term = malloc(sizeof(poly_t));
  term->coe = c;
  term->exp = e;
  term->next = NULL;

  return term;
}

void free_poly(poly_t *p) {
  poly_t *q = p;

  while (p) {
    q = p->next;
    free(p);
    p = q;
  }
}

int get_lexp(poly_t *p) {
  int l = 0;

  while (p != NULL) {
    if (p->exp > l) {
      l = p->exp;
    }

    p = p->next;
  }

  return l;
}

poly_t *clean(poly_t *p) {
  poly_t *pr = NULL;
  poly_t *cu = p;
  poly_t *su = NULL;

  // Find largest exponent l in p
  int l = get_lexp(p);
  // create term t with exp l
  poly_t *t = new_term(0, l);

  while (cu != NULL) {
    su = cu->next;

    // add coe to t, remove term from p
    if (cu->exp == l) {
      t->coe += cu->coe;

      if (pr != NULL) {
        pr->next = su;
      } else {
        p = su;
      }

      free(cu);
    } else {
      pr = cu;
    }

    cu = su;
  }

  // Repeat if p not empty
  if (p != NULL) {
    t->next = clean(p);
  }

  return t;
}

void space_jump(const char **str) {
  while (ispace(**str)) (*str)++;
}

int parse_sign(const char **str) {
  if (**str == '-') {
    (*str)++;
    return -1;
  } else if (**str == '+') {
    (*str)++;
  }

  return 1;
}

int parse_number(const char **str) {
  int num = 0;

  while (isdigit(**str)) {
    num = num * 10 + (**str - '0');
    (*str)++;
  }

  return num ? num : 1;
}

poly_t *parse_term(const char **str) {
  space_jump(str);
  int sig = parse_sign(str);
  space_jump(str);
  int coe = parse_number(str);

  int exp = 0;
  if (**str == 'x') {
    // If next char is == ^ and char after exists
    if (*(++(*str)) == '^' && *(++(*str))) {
      exp = parse_number(str);
    } else {
      exp = 1;
    }
  }

  poly_t *term = new_term(coe * sig, exp);
  return term;
}

poly_t *new_poly_from_string(const char *str) {
  poly_t *poly = NULL;
  poly_t *term = NULL;

  while (*str) {
    if (term == NULL) {
      term = parse_term(&str);
      poly = term;
    } else {
      term->next = parse_term(&str);
      term = term->next;
    }
  }

  return poly;
}

poly_t *mul_terms(poly_t *a, poly_t *b) {
  return new_term(a->coe * b->coe, a->exp + b->exp);
}

poly_t *mul(poly_t *a, poly_t *b) {
  poly_t *r = mul_terms(a, b);
  poly_t *p = r;
  poly_t *q = b;

  while (q->next) {
    p->next = mul_terms(a, q->next);
    p = p->next;
    q = q->next;
  }

  if (a->next) {
    p->next = mul(a->next, b);
  }

  return clean(r);
}

void print_poly(poly_t *p) {
  int first = 1;

  while (p) {
    if (!first) {
      printf(p->coe < 0 ? " - " : " + ");
    } else if (p->coe < 0) {
      printf("- ");
    }

    if (p->exp == 0) {
      printf("%d", abs(p->coe));
    } else {
      if (p->coe != 1) printf("%d", abs(p->coe));
      printf("x");
      if (p->exp != 1) printf("^%d", p->exp);
    }

    if (first) first = 0;

    p = p->next;
  }

  printf("\n");
}

/*int main() {
  poly_t *a = new_poly_from_string("x^2 + 1 - 7x");
  print_poly(a);
  poly_t *b = new_poly_from_string("3x + 2");
  print_poly(b);
  poly_t *c = mul(a, b);  // 3x^3 - 19x^2 - 11x + 2
  print_poly(c);

  free_poly(a);
  free_poly(b);
  free_poly(c);

  return 0;
}
*/