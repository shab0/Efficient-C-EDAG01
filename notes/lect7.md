# Lecture 7

*Edit*: A variable may only be modified once between two sequence points. Additionally, a variable may not be read after a modification before the next sequence point.

Declaration:
	- Associates an identifier with:
		- variable
		- function
		- type

When looking at symbol table:
	- Letters tell us where the variable resides in memory
		- D: Data
		- B: BSS
		- C: Common (global variables that have not yet been initialized)

Defining global variables:
	- Best practice is to use extern in a separate header file

When using VLA:s:
	- Size should always be checked, so that stack doesn't collide with heap

### Presedence and associativity
- Binary operators are left-associative
	- a - b - c = (a - b) - c
- The unary, assignment operators and the condotional expression are right associative
	- a = b = c = a = (b = c)
	1 << 2 + 3 = 1 << (2 + 3) = 1 << 5
- Floating point instructions do not contain the constants, these are treated as variables
- Biggest type in an operation has presedence (that will be the type of the result)

### Compound literals
- You can pass a struct point_t to a function as f((point_t) {val, val})
	- Treated as anonymous variable
You can use this notation to create `int *p = (int[]) { 1, 2 }`

### Relational and equality expressions
- If we have two pointers pointing to elememnts of the same array
	- We can use all relational expressions < <= > >=
	- Not the best practice
- If we have two pointers to different "nodes" allocated at different times
	- We can use equality expressions == !=
	- Relational expressions are undefined behaviour

### Alignment
- Every type has an alignment requirement. This means that the value of a variable of a certain type may only be stored at an address which is a multiple of the size of the type
- We can use `alignof` to find out 

## Extra stuff
- The formula for a % b = a - (a/b) * b
- Doing arithmetic on equivalent types which differ in signed/unsigned
	- The signed gets converted to a unsigned
	- This can result in funny behaviour
- Otherwise, the type with highest rank wins
- If the higher ranking type cannot take the value of the signed type
	- For example if both types are 32 bits, one is signed with higher rank and one unsigned
	- Then the signed type gets converted to its equivalent unsigned type
In short: avoid comparing signed and unsigned types


