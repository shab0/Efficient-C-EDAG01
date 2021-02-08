# Lecture 5

- static:
	- same address throughout the program.
	- not deallocated when function ends
	- not allocated on stack but with global variables
	- due to one-pass compilers, static has to be used first

- extern:
	- "There exists, externally, some variable <type> <name>"
	- does not allocate memory for variable

- auto:
	- completely useless, but still in the standard to not break existing code

- register:
	- indicates to the compiler that a variable should be put in a register (used a lot)
	- pretty useless, since compliers do a better job at structuring machine instructions than the programmer
	- permitted optimization for safety-critical systems (non-optimized)

- bool:
	- any none-zero value gives the bool the value 1

- enum:
	- declares named int constants
	- better than #define because debuggers understand them

- struct:
	- bitfield types: unsigned int a:1; in struct gives one unsigned bit
	- avoid using plain int as bitfield type. Specify if it is signed or not (o.w implementation dependent)
	- functions cannot be held as struct members, but pointers to functions can
		- if we have stru->f defined as int (*f)(int, int)
		- we can use stru->f = func; (or &func)
		- we can use it with stru->f(1,2) (or (*stru->f(1,2))

- union:
	- as struct, but only one member can be used
	- look more at this, fuzzy video

- struct flexible array memebers:
	- we can define (only) the last memeber (if memebers > 1) as int a[]
	- this gives us a flexible array as memeber
	- we can then allocate memory:
		- `size_t size = sizeof(struct s) + n * sizeof(int)`
		- `struct s* s = malloc(size)`
		- `s->a[n-1] = 119 // OK`

- type qualifiers:
	- const: the variable is read only
	- volatile: var can change in mysterious ways, so dont put it in register
	- restrict (lvl 5): promise that data is not overlapping - enables some optimization	

- side note: to catch a signal from kernel use `signal(SIGINT, function_to_exec)`

-type constructors:
	- Array, Function, Pointer
	- evaluation of definition: find variable name, look to right then look to left (a bit more complex)
