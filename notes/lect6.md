# Lecture 6

- Pipelined execution: Instructions executed simultaniously

- Instruction execution:
	1. Fetch instruction
	2. Decode instruction, fetch operand
	3. Execute
	4. Memory access
	5. Write back


- Less instructions = more code. If we have cmany complex instructions, code will be smaller


branch instructions (4 bits):
	- positive
	- negative
	- equal
	- overflow

pipeline stalls:
	- Some instructions need to be stalled
	- This is controlled by which stops execution
	- It happens when we for example use the ALU to calculate the new address of the PC (branch)
	- Modern processors guess when we are going to branch to do this more efficiently
	- How to avoid stalls:
		- Avoid using global variables (stall happens when loading the var from memory)
		- Avoid using function calls through pointers (hard for compiler and hardware to predict branch but be wise)
		- Avoid using many branches in inner loops
		- Compilers can try to rearrange instructions to avoid stalls

Superscalar processors:
	- Latency of instruction is number of clock cycles needed (not impacted by pipelining)
	- Throughput is the number of instructions (of a certain kind) the processor can complete per cycle (pipelinening improves this)
	- Speculative execution:
		- Instructions can be executed before we know they should be, as long as they dont permanently modify or destoy data
	- Three essential features of superscalar processors:
		- Branch prediction
			- Done by having tables of previous outcomes
		- Reorder buffer
			- FIFO queue
			- Controls that all instructions finish in the program order
			- "... put the instructions back in the original program order after the instructions have finished execution possibly out of order"
			- The oldest instructions is the only one allowed to modify real registers (not rename registers)
		- Register renaming
			- Registers have another field which points to the rename register which has the most recent value for this register
			- Removes anti and output dependencies

