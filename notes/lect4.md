# Lecture 4

## Processor
- Arithmetic and logic unit (ALU)
	- Can e.g add two numbers and jump to a function
- Three sets of registers
	- Integer registers r0...r31: For integers and addresses
	- Floating point registers f0...f31 for real numbers
	- SIMD vector registers v0...v31 which can contain e.g four array elements at a time - either floats or integers
. Program counter (special register)
	- Holds the address of the memory location which contains the next instruction to fetch and execute

- Processors can do the following things:
	- Transfer data from a memory cell A to a register R (read)
	- Transfer data from a register R to a memory cell A (write)
	- ALU operation son the contents of registers and store result in another register
	- Branch - TBA
