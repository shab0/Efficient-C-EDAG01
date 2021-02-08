# Lecture 1

C is one of three most commonly used fast languages alongside FORTRAN and C++.

### The C Preprocessor (cpp)
- Handles includes
	- Basically pastes the whole content
- Deals with macros (e.g INT_MAX)
- Knows knothing about C syntax

### Compiling a C Program (gcc)
- To compile and make an executable: gcc hello.c
- The command will first run cpp, then the C compiler, then assembler and a link-editor
- The default executable will be called a.out
- To execute it: ./a.out
- To specifically name output file: gcc hello.c -o hello

### The C main method
- Takes two parameters: int argc, char** argv
- First parameter specifies length of second
- Convention is to have the last pointer in array to be nullpointer

### IO arrays and memory allocation
- Some <stdio> functionality:
	- printf
	- scanf
	- fopen: open file
	- fprintf: print to file
	- fclose: close file
	- fscanf: read data from file
- Format-specifiers
	- %d: integer (base 10)
	- %o: base 8
	- %x base 16
	- %f: float
	- %lf: double
	- %s: string
	- %c: char
	- %zu: sizeof
- Given a variable a we get its adress with: &a
- We can get the size (in bytes) of types with: sizeof(int)
- We can allocate memory with calloc and malloc:
	- int* a = calloc(numOfElements, sizeOfElement)
- We can free memory with free(pointer)
- static keyword before variable declaration limits it to the current scope (not visible from other files)

### Structs
- Like objects in java, but everything is public and there are no methods
- We declare structs with a "type", which lives in a different namespace than variables and functions
	- struct s /* <- type */ { ... } s /* <- variable name */;
- Curiosa in Java: if the type is a class name, the variable is a pointer (object reference)
- If one of the struct parameters is a reference to the struct we're currently defining, the reference must be a pointer

 
### More about pointers
- Pointers are just adress values
- Pointers of pointers can be passed to functions by adding another *
- "The goal of C programmers is the opposite of restaurant owners - as few stars as possible"
- Pointers can make a program faster but reduce readability to both programmers and compilers
- If we make a local struct in a function and return it from the function, the struct is created outside and the pointer to it is passed by the compiler (usually)
- Another way of returning multiple values from function is passing pointers and setting their values
- Address + integer gets converted to address + integer * size

### Arrays vs Pointers
- Passing array parameters is eqwual to passing pointers, as the compiler automatically switches this
	- First [] always gets changed to *

### Memory allocation
- malloc/calloc/realloc take memory from heap
	- all return void pointer
	- realloc(void* p, size_t s) tries to extend the memory of pointer p
	- here, free(void* p) can be used
		- void* (void pointer) can be used like Javas object - for any data
	- malloc/free take time, somethimes a free-list is useful
		- instead of calling free, we put aside allocated memory for future use
	
- alloca takes memory from stack
	- this memory cannot be freed
	- this is faster than malloc/alloc/realloc
	- not standard but available on Linux & MacOS
- All above reutn NULL if no memory is available
- USE alloca WITH CAUTION

