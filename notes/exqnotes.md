# Notes from solving example questions

1. Bitfields
	- Restrict amount of bits we use for a stuct member
	- If we add more memebers, and restrict them all to one bit, we can add up to 31 (to form 4 bytes which = int) before another int "block is" needed to store the struct
2. Stack Pointer
	- Indicates the last location which was put on to the stack
3. Easonable cache block size
	- POWER8:
		- L1 I: 128 bytes
		- L1 D: 128 bytes
		- L2: 128
		- L3: 128
4. Cache associativity
	- Related to where in the cache the data at a certain address can be stored
	- Direct mapped: A cache block can only go in one spot in the cache. It makes a cache block very easy to find, but it‛s not very flexible about where to put the blocks
	- 2-Way Set associative: This cache is made up of sets that can fit two blocks each. The index is now used to find the set, and the tag helps find the block within the set.
	- Fully associative: No index is needed, since a cache block can go anywhere in the cache. Every tag must be compared when finding a block in the cache, but block placement is very flexible
5. Keywords
	- #undef: undefine an identifier
	- #error: causes preprocessing to stop at the location where the directive is encountered
	- #ifndef: if not defined
	- defined: used in ‘#if’ and ‘#elif’ expressions to test whether a certain name is defined as a macro
	- ##: token pasting or token concatenation. "string" refers to a preprocessor token, or a "string of source code", and not a C-string
	- volatile: a volatile variable could change in mysterious ways, so it should always be read from memory and not registers
	- restrict: restict access of some data to one specific pointer // promise that data is not overlapping - enables some optimization
	- continue: skips rest of loop, differing a bit wether 
6. Designated initializer
	- Curly brackets to initialize stuct or array
7. Sequence point
	- Semicolon, point where previous sequence is done
8. Static
	- Initialized to 0 as globals
	- Typically resides in the data segment of the program
	- Retains value between function calls
9. Integer promotion
	- If we do aritmetic on a type smaller than int, it is converted to a signed or unsigned int
	- If an int can represent all values of the original type, the value is converted to an int . Otherwise, it is converted to an unsigned int
10. Linkage
	- Internal linkage: symbol (func or global var) is only accessible in one translation unit
	- External linkage: symbol (func or global var) is accessible throughout the program
11. Binary orerators
	- `~`: Bitwise not. Reverses the ones and zeros in a number
	- `&`: Bitwise and. Keeps bits present in both a and b (a & b)
	- `|`: Bitwise or. Keeps bits present in a or b (a|b)
	- `^`: Bitwise xor. Bitwise or but bits present in both a and b are 0
	- `>>/<<`: Bit shift. Shift the pattern in a with b steps left/right (a >>/<< b)
12. setjmp longjmp
	- setjmp: nonlocal goto
	- longjmp: go back to where setjmp was called
13. exit()
	- always called with return value when returning from main
14. Copying structs a = b is ok, copying arrays 1 = b is not permitted
15. SIMD
	- Single input multiple data
	- SIMD vectorization is when the compiler (or the programmer) replaces a single element construct with vector instructions that operate on multiple elements at a time
	- To help the compiler: for example access arrays sequentially
	
