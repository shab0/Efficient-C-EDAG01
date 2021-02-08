# Lab 6
---

### Point 1-4

- Compile with optimization 0-5
- `size --common *.o`
	- "Text" is the size of instructions and constants
	- "BSS" is global variables which should be initialized to zero
- `nm 0.o`
	- T: Text - code/instructions
	- U: Undefined
	- C: Common - uninitialized data
	- G: Initialized data section for small objects
- `diff 3.o 4.o` `diff 4.o 5.o`
	- No difference between files

#### Size of the files
```
text	   data bss	    dec	     hex	filename
19044	    272	5093552	5112868	 4e0424	0.o
11695	    320	5093552	5105567	 4de79f	1.o
12939	    320	5093552	5106811	 4dec7b	2.o
35371	    296	5093552	5129219	 4e4403	3.o
35371	    296	5093552	5129219	 4e4403	4.o
35371	    296	5093552	5129219	 4e4403	5.o
```

### Point 5

```for x in s 0 1 2 3
do
gcc -O$x -o $x mipsx.c tbr.s timebase.c
./$x
done
```

Output not really clear, not sure how to interpret

### Point 6-7

From gcc docs:
	- "Vectorization is enabled by the flag -ftree-vectorize and by default at -O3. To allow vectorization on powerpc* platforms also use -maltivec. On i?86 and x86_64 platforms use -msse/-msse2. To enable vectorization of floating point reductions use -ffast-math or -fassociative-math."

- `gcc -O3 -ftree-vectorize -maltivec -o vect.o mipsx.c timebase.c tbr.s`
- `objdump -d vect.o > x`

Found stvx and multiple lvx but no vmaddfp, vperm

### Point 8

- `-fprofile-generate`: Additional code for profiling gcov
- `-fprofile-use`: If a profile file is present, the optimization will be based on it

We generate a profile by compiling `gcc -O3 -fprofile-generate mipsx.c tbr.s timebase.c`, then run the program and recompile with `gcc -O3 -fprofile-use mipsx.c tbr.s timebase.c -mcpu=power8` to use the profile and adapt to the power CPU
The programm runs faster, is better optimizied

### Point 10
`-flto` (`gcc -O3 -flto a.c b.c` + `objdump -d a.out > x`) gives us a shorter program (less instructions)

### Point 11
As per the explanation in the book, the loop can be optimized to better use spatial locality by switching the loops

When changed, the compiler generates vector innstructions, so the vectorization is working

### Point 12
- clang: `clang -O3 matmul.c tbr.s timebase.c -mcpu=power8 -o matmul.clang`
- gcc: `gcc -O3 matmul.c tbr.s timebase.c -mcpu=power8 -o matmul.gcc`
- IBM xlc: `xlc -O3 matmul.c tbr.s timebase.c -mcpu=power8 -o matmul.ibm`
- Nvidia: `/opt/nvidia/hpc_sdk/Linux_ppc64le/20.9/compilers/bin/pgcc -O3 matmul.c tbr.s timebase.c -tp=pwr8 -o matmul.nvidia`

```
echo -n "clang: "; ./matmul.clang

echo -n "gcc: "; ./matmul.gcc

echo -n "ibm: "; ./matmul.ibm

echo -n "nvidia: "; ./matmul.nvidia
```

1. IBM xlc: 0.138s
2. Nvidia: 0.288s
3. GCC: 0.790s
4. Clang: 1.849s