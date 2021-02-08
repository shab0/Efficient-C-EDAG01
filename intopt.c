#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// ------------------------------------- Intopt IO -----------------------------------------

// Allocate a m row n col matrix
double** make_matrix(int m, int n) {
  double**  a;
  int       i;
  
  a = calloc(m, sizeof(double*));
  for (i = 0; i < m; i++)
    a[i] = calloc(n, sizeof(double));
  return a;
}

void free_matrix(double** ptr, int m) {
  for (int i = 0; i < m; i++) {
    free(ptr[i]);
  }
  
  free(ptr);
}

// Complete a vector with the entered elements
void complete_vector(int n, double* vector) {
  for (int i = 0; i < n; i++){
    scanf("%lf", &vector[i]);
  }
}

// Complete a matrix with the entered elements
void complete_matrix(int m, int n, double** matrix) {
  for (int i = 0; i < m; i++) {
    complete_vector(n, matrix[i]);
  }
}

void print_linear_program(int n,int m, double** A, double* b, double* c) {
  
  printf("-----------------------------\n");
  printf("max z =");
  
  // c array
  for (int i = 0; i < n; i++){
    printf(" %1.1lfx%d", c[i], i);
    printf(i == n-1 ? "\n" : " +");
  }
  
  // A matrix & b array
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%1.1lfx%d", A[i][j], j);
      printf(j == n-1 ? "\t" : " + ");
    }
    
    printf("\u2264 %1.1lf\n", b[i]);
  }
  
  // x:es >= 0
  for (int i = 0; i < n; i++){
    printf("x%d", i);
    printf(i == n-1 ? "\t\t\u2265 0\n" : ", ");
  }
  printf("-----------------------------\n");
}

// Print a vector
void print_vector(int m, double* vector) {
  for( int i = 0; i < m; i ++){
    printf("%6.1lf", vector[i]);
  }
  printf("\n");
}

// Print a matrix
void print_matrix(int m, int n, double** matrix) {
  for (int i = 0; i < m; i ++){
    print_vector(n, matrix[i]);
  }
}

// ------------------------------------- Simplex algo -----------------------------------------

typedef struct simplex_t {
  int      n;   // Number of decision variables
  int      m;   // Number of constraints
  double** a;   // Constraint coefficient matrix (n*m)
  double*  b;   // Constraint right-hand values (<= than, m long)
  double*  c;   // Obj func coefficients (n long)
  double*  x;   // X values (n long)
  double   y;
  int*     var;
} simplex_t;

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h);
double simplex(int m, int n, double** a, double* b, double* c, double* x, double y);
int initial(simplex_t* s, int m, int n,double** a, double* b, double* c, double* x, double y, int* var);
int init(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var);
int select_nonbasic(simplex_t* s);
void pivot(simplex_t* s, int row, int col);
void prepare(simplex_t* s, int k);

int init(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var) {
  
  int i, k;
  
  // assign each attribute
  *s = (simplex_t) {n,m,a,b,c,x,y,var};
  
  // If var is empty, we allocate memory and initialize it to [0, 1, 2, ... m+n]
  if (s->var == NULL){
    s->var = calloc(n+m+1,  sizeof(int));
    for (i = 0; i < m+n; i++){
      s->var[i] = i;
    }
  }
  
  // find index of the smallest value in b array -> store in k
  for (k = 0, i = 1; i < m; i++){
    if (b[i] < b[k]){
      k=i;
    }
  }
  
  return k;
}

int select_nonbasic(simplex_t* s){
  for (int i = 0; i < s->n; i++){
    if (s->c[i] > pow(10, -10)){
      return i;
    }
  }
  
  return -1;
}

void pivot(simplex_t* s, int row, int col) {
  double** a = s->a;
  double*  b = s->b;
  double*  c = s->c;
  int      m = s->m;
  int      n = s->n;
  int      i,j,t;
  
  t = s->var[col];
  s->var[col] = s->var[n+row];
  s->var[n+row] = t;
  s->y = s->y + c[col] * b[row] / a[row][col];
  
  for (i = 0; i < n; i++){
    if (i != col){
      c[i] = c[i] - c[col] * a[row][i] / a[row][col];
    }
  }
  
  c[col] = -c[col] / a[row][col];
  
  for (i = 0; i < m; i++){
    if (i != row){
      b[i] = b[i] - a[i][col] * b[row] / a[row][col];
    }
  }
  
  for (i = 0; i < m; i++){
    if (i != row){
      for (int j = 0; j < n; j++){
        if (j != col){
          a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
        }
      }
    }
  }
  
  for (i = 0; i < m; i++){
    if (i != row){
      a[i][col] = -a[i][col] / a[row][col];
    }
  }
  
  for (i = 0; i < n; i++){
    if (i != col){
      a[row][i] = a[row][i] / a[row][col];
    }
  }
  
  b[row] = b[row] / a[row][col];
  a[row][col] = 1 / a[row][col];
}

void prepare(simplex_t* s, int k) {
  int m = s->m;
  int n = s->n;
  
  // make room for xm+n at s.var[n] by moving s.var[n..n+m-1] one step to the right
  for (int i = m+n; i > n; i--){
    s->var[i] = s->var[i - 1];
  }
  
  s->var[n]= m+n;
  
  // add xm+n to each constraint
  // This causes segmentation fault. The allocated memory should be
  // n+1 as this is what we pass to make_matrix??
  n = n+1;
  for (int i = 0; i < m; i++){
    s->a[i][n - 1] = -1;
  }
  
  s->x = calloc(m+n,sizeof(double));
  s->c = calloc(n,sizeof(double));
  s->c[n-1] = -1;
  s->n = n;
  pivot(s, k, n-1);
}

int initial(simplex_t* s, int m, int n,double** a, double* b, double* c, double* x, double y, int* var) {
  
  int    i,j,k;
  double w;
  
  // k = index of smallest value of b array
  k = init(s, m, n, a, b, c, x, y, var);
  
  // if no negative b value -> feasible
  if (b[k] >= 0){
    return 1;
  }
  
  prepare(s,k);
  n = s->n;
  s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1);
  
  for (i = 0; i < m+n; i++){
    if (s->var[i] == m+n-1){
      if (fabs(s->x[i]) > pow(10,-10)){
        free(s->x);
        free(s->c);
        return 0; // infeasible
      }
      else{
        break;
      }
    }
  }
  
  if (i >= n){
    // xn+m is basic. ﬁnd good nonbasic
    for (j = k = 0; k < n; k++){
      if (fabs(s->a[i-n][k]) > fabs(s->a[i-n][j])) {
        j = k;
      }
    }
    
    pivot(s, i-n, j);
    i = j;
  }
  
  if (i < n-1){
    // xn+m is nonbasic and not last. swap columns i and n-1
    k = s->var[i]; s->var[i] = s->var[n-1]; s->var[n-1] = k;
    for (k = 0; k < m; k++){
      w = s->a[k][n-1]; s->a[k][n-1] = s->a[k][i]; s->a[k][i] = w;
    }
  } else{
    // xn+m is nonbasic and last. forget it
  }
  
  free(s->c);
  s->c = c;
  s->y = y;
  
  for (k = n-1; k < n+m-1; k = k + 1){
    s->var[k] = s->var[k+1];
  }
  
  n = s->n = s->n - 1;
  
  double* t = calloc(n, sizeof(double));
  
  for (k = 0; k < n; k++) {
    for (j = 0; j < n; j = j + 1){
      if (k == s->var[j]){
        // xk is nonbasic add ck
        t[j] = t[j] + s->c[k];
        goto next_k;
      }
    }
    // xk is basic
    for (j = 0; j < m; j++){
      if (s->var[n+j] == k){
        // xk is at row j
        break;
      }
    }
    
    s->y = s->y + s->c[k] * s->b[j];
    
    for (i = 0; i < n; i++){
      t[i] = t[i] - s->c[k] * s->a[j][i];
    }
    
  next_k: ;
  }
  
  for (i = 0; i < n; i++){
    s->c[i] = t[i];
  }
  
  free(t);
  free(s->x);
  return 1;
}

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h) {
  simplex_t s;
  
  int col, row, i;
  
  if (!initial(&s,m,n,a,b,c,x,y,var)) {
    free(s.var);
    return NAN;
  }
  
  while((col = select_nonbasic(&s)) >= 0) {
    row = -1;
    
    for (i = 0; i < m; i++) {
      if (a[i][col] > pow(10, -10) && (row < 0 || b[i] / a[i][col] < b[row] / a[row][col])) {
        row = i;
      }
    }
    
    if (row < 0) {
      free(s.var);
      return INFINITY;
    }
    
    pivot(&s, row, col);
  }
  
  if (h == 0) {
    for (i = 0; i < n; i++) {
      if (s.var[i] < n) {
        x[s.var[i]] = 0;
      }
    }
    for (i = 0; i < m; i++) {
      if (s.var[n+i] < n) {
        x[s.var[n+i]] = s.b[i];
      }
    }
    
    free(s.var);
  } else {
    for (i = 0; i < n; i++) {
      x[i] = 0;
    }
    for (i = n; i < m+n; i++) {
      x[i] = s.b[i-n];
    }
  }
  
  return s.y;
}

double simplex(int m, int n, double** a, double* b, double* c, double* x, double y) {
  return xsimplex(m,n,a,b,c,x,y,NULL,0);
}

// ------------------------------------- Node implementation -----------------------------------------

typedef struct node_t node_t;

struct node_t {
  int      n;   // Number of decision variables
  int      m;   // Number of constraints
	int      k;		// Parent branches on x[k]
	int			 h;		// Branch on x[h]
	double	 xh;	// Value of x[h]
	double	 ak; 	// Parent a[k] value
	double	 bk;	// Parent b[k] value
	double*	 min;	// Lower bounds
	double*	 max;	// Upper bounds
  double** a;   // Constraint coefficient matrix (n*m)
  double*  b;   // Constraint right-hand values (<= than, m long)
  double*  c;   // Obj func coefficients (n long)
  double*  x;   // X values (n long)
	double   z;
	node_t*  next;
};

void free_node(node_t *p) {
	if (p->a != NULL) {
		free_matrix(p->a, p->m+1);
		free(p->b);
		free(p->c);
		free(p->x);
	}
	
	free(p->min);
	free(p->max);
	free(p);
}

// ------------------------------------- List implementation -----------------------------------------

typedef struct list_t list_t;

struct list_t {
	node_t* first;
};

list_t* new_list(void) {
	list_t* head;
	head = malloc(sizeof(list_t));
	head->first = NULL;
	return head;
}

// Insert value first in list
void insert(list_t* head, node_t* node) {
	if (head->first != NULL) {
		node->next = head->first;
	}

	head->first = node;
}

// Called inside !empty condition (intopt)
node_t* give(list_t* head) {
	node_t* ret = head->first;
	head->first = ret->next;
	return ret;
}

// Check if list is empty
int empty(list_t* head) {
	if (head->first == NULL) {
		return 1;
	}

	return 0;
}

void remove_below(list_t* head, double pz) {
	node_t** pp = &head->first;
	node_t* p;

	while (*pp != NULL) {
		if ((*pp)->z < pz) {
			p = *pp;
			*pp = (*pp)->next;
			free_node(p);
		} else {
			pp = &(*pp)->next;
		}
	}
}

// -------------------------------------- Branch and bound ------------------------------------------

node_t* initial_node(int m, int n, double** a, double* b, double* c) {

	int i, j;

	node_t *p = malloc(sizeof(node_t));
	p->a = make_matrix(m+1, n+1);
	p->b = calloc(m+1, sizeof(double));
	p->c = calloc(n+1, sizeof(double));
	p->x = calloc(n+1, sizeof(double));
	p->min = calloc(n, sizeof(double));
	p->max = calloc(n, sizeof(double));
	p->next = NULL;

	p->m = m;
	p->n = n;

	// Set min, max and copy c to p
	for (i = 0; i < n; i++) {
		p->min[i] = -INFINITY;
		p->max[i] = INFINITY;
		p->c[i] = c[i];
	}

	// Copy a and b to p
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			p->a[i][j] = a[i][j];
		}
		p->b[i] = b[i];
	}

	return p;
}

node_t* extend(node_t* p, int m, int n, double** a, double* b, double* c, int k, double ak, double bk) {

	int i, j;

	node_t *q = malloc(sizeof(node_t));
	q->k = k;
	q->ak = ak;
	q->bk = bk;
	q->next = NULL;

	if (ak > 0 && p->max[k] < 1) {
		q->m = p->m;
	} else if (ak < 0 && p->min[k] > 0) {
		q->m = p->m;
	} else {
		q->m = p->m + 1;
	}		

	q->n = p->n;
	q->h = -1;
	q->a = make_matrix(q->m+1, q->n+1);
	q->b = calloc(q->m+1, sizeof(double));
	q->c = calloc(q->n+1, sizeof(double));
	q->x = calloc(q->n+1, sizeof(double));
	q->min = calloc(n, sizeof(double));
	q->max = calloc(n, sizeof(double));

	// copy p.min and p.max to q
	for (i = 0; i < p->n; i++) {
		q->min[i] = p->min[i];
		q->max[i] = p->max[i];
	}

	// copy m first rows of parameter a to q.a
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			q->a[i][j] = a[i][j];
		}
	}

	// copy m first elements of parameter b to q.b
	for (i = 0; i < m; i++) {
		q->b[i] = b[i];
	}

	// copy parameter c to q.c 
	for (i = 0; i < n; i++) {
		q->c[i] = c[i];
	}

	if (ak > 0) {
		if (q->max[k] == INFINITY || bk < q->max[k]) {
			q->max[k] = bk;
		}
	} else if (q->min[k] == -INFINITY || -bk > q->min[k]) {
		q->min[k] = -bk;
	}

	for (i = m, j = 0; j < n; j++) {
		if (q->min[j] > -INFINITY) {
			q->a[i][j] = -1;
			q->b[i] = -q->min[j];
			i++;
		}
		if (q->max[j] < INFINITY) {
			q->a[i][j] = 1;
			q->b[i] = q->max[j];
			i++;
		}
	}

	return q;
}

int is_integer(double* xp) {
	double x = *xp;
	double r = round(x);

	if (fabs(r - x) < pow(10,-10)) {
		*xp = r;
		return 1;
	}

	return 0;
}

int integer(node_t* p) {
	for (int i = 0; i < p->n; i++) {
		if (!is_integer(&p->x[i])) {
			return 0;
		}
	}

	return 1;
}

void bound(node_t* p, list_t* h, double* zp, double* x) {
	if (p->z > *zp) {
		*zp = p->z;

		// copy each element of p.x to x
		for (int i = 0; i < p->n; i++) {
			x[i] = p->x[i];
		}

		// remove and delete all nodes q in h with q.z < p.z
		remove_below(h, p->z);
	}
}

int branch(node_t* q, double z) {
	
	int h, min, max;
	
	if (q->z < z) {
		return 0;
	}

	for (h = 0; h < q->n; h++) {
		if (!is_integer(&q->x[h])) {
			if (q->min[h] == -INFINITY) {
				min = 0;
			} else {
				min = q->min[h];
			}
			max = q->max[h];
			if (floor(q->x[h]) < min || ceil(q->x[h]) > max) {
				continue;
			}
			q->h = h;
			q->xh = q->x[h];
			// Free q's -> a, b, c, x
			free_matrix(q->a, q->m+1);
			free(q->b);
			free(q->c);
			free(q->x);
			q->a = NULL;
			return 1;
		}
	}

	return 0;
}

void succ(node_t* p, list_t* h, int m, int n, double** a, double* b, double* c, int k, double ak, double bk, double* zp, double* x) {
	node_t* q = extend(p,m,n,a,b,c,k,ak,bk);

	if (q == NULL) {
		return;
	}

	q->z = simplex(q->m, q->n, q->a, q->b, q->c, q->x, 0);

	if (isfinite(q->z)) {
		if (integer(q)) {
			bound(q, h, zp, x);
		} else if (branch(q, *zp)) {
			insert(h, q);
			return;
		}
	}

	free_node(q);
}

double intopt(int m, int n, double** a, double* b, double* c, double* x) {
	int i;
	node_t *p = initial_node(m, n, a, b, c);

	double z = -INFINITY;
	p->z = simplex(p->m, p->n, p->a, p->b, p->c, p->x, 0);

	// Simplex found integer solution or p->z is INFINITY/NAN
	if (integer(p) || !isfinite(p->z)) {
		z = p->z;

		if (integer(p)) {
			for (i = 0; i < n; i++) {
				x[i] = p->x[i];
			}
		}

		free_node(p);
		return z;
	}

	branch(p, z);

	// set h = {p} (moved from top)
	list_t *h = new_list();
	insert(h, p);
	
	while(!empty(h)) {
		p = give(h);
		succ(p,h,m,n,a,b,c,p->h,1,floor(p->xh),&z,x);
		succ(p,h,m,n,a,b,c,p->h,-1,-ceil(p->xh),&z,x);
		free_node(p);
	}

	free(h);
	return z == -INFINITY ? NAN : z;
}