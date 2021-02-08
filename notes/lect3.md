# Lecture 3

## Arrays and Matrices (03/1)
- First problem: Allocate m x n matrix with calloc without knowing m and n
	- Array is fixed allocated memory
	- For a simple array, we can say: double* a = calloc(n, sizeof(double));
		- No difference in usage compared to: double a[2] = ...

## malloc/calloc/realloc/free
- Malloc: unitialized allocation (typically just garbage)
	- OS clears memory given to new programs, so initially malloc might contain zeros
	- This is done as a security measure, but might make us miss some bugs

- Use valgrind (with -g when compiling) to check for uninitialized data
- When using realloc:
	- Can be used to extend an array for example
	- If there is no memory left, realloc will return null
		- Best to save return value of realloc in new var, as reference to old might be lost
	- May move the data to another location
- Never mention a freed variable/pointer
- About word assignment:
	- Allocate 10 bytes, if insufficient, double it with realloc

## Lists in C
- Given a pointer li to a struct called list_t:
	- li->someattr = ... is the same as (*li).someattr = ...
	- Jonas recommends second notation, as it's more self-explanatory

## Strings
- Creating a pointer initialized as a string literal, its read only
- All "string" contain a terminating nullbyte
- We do not need to specify size if its initialized right away

## Behaviours
- Implementation-defined behaviour (implementation = compiler + standard lib)
	- Must be documented
- Unspecified behaviour
	- What the compiler does, is not documented
	- Example: order of evaluation of +
- Undefined behaviour
	- Ugly form of bug
	- The implementation is permitted to do anything including
		- Terminating
		- Continuing without knowing what happened
		- Continuing with warning message
	- Example: a stack variable is used before its given a value
	- Example: array index out of range


## Keywords to know:
- auto
- break
- case
- char
- const
- continue
- default
- do
- double
- else
- enum
- extern
- float
- for
- goto
- if
- inline
- int
- long
- register
- restrict
- return
- short
- signed
- sizeof
- static
- struct
- switch
- typedef
- union
- unsigned
- void
- volatile
- while

## Constants
watch again





















 
