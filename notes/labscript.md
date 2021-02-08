# Lab 4

`gcc -g branch.c -lm`

## Point 5 (`operf`)`
- `operf -e CYCLES:100000:0:0:1 ./a.out < i` // pc will be saved every 100,000 clock cycles
- `opreport -t 0.7 -l` // prints all functions in which at least 0.7% of the cycles were sampled
- `opannotate -s` // needs -g when compiling


### "Figure out which functions take most of the time in your program"
~90% of the samples are from `pivot`
~6% of the samples are from `xsimplex`

## Point 6 (`gprof`)
- `gcc -pg -g branch.c -lm`
- `gprof -T a.out`

### "What does the output mean and how can you use it to better understand your program?"
- Which functions take the most time
- Exact calls to functions
- For each functions f, which other function called f (callers of f, above)
- Which function f calls (callees of f, below)

## Point 7 (`gcov`)
- `gcc -fprofile-arcs -ftest-coverage -g branch.c -lm`
- `./a.out < i`
- `gcov branch.c` // creates branch.c.gcov, -b option gives extra output

### "What does `branch.c.gcov` contain how and can you use this information?"
- How many times we've called each line
- Used for understanding the behaviour of our program, for example to make sure that test cases execute each line at least once
- the -b option gives us branch frequencies

## Point 8 (`valgrind`)
- `gcc -g -O3 branch.c -lm`
- `valgrind --tool=cachegrind --I1=65536,1,128 --D1=32768,2,128 --LL=1048576,8,128 ./a.out < i`

### "How can you see how many instructions in total, load instructions, and store instructions are executed?"
- Ir: L1 I-cache reads; executed instructions (1,198,949,184)

### "What are the cache miss rates and are the cache likely to be a performance problem for this input?"
- Miss rates:
  - L1 I: 0.01% read misses
  - L1 D: 0.0% read misses
  - LL:		0.0% misses

Not likely to be a problem

---

# Lab 5

### Point 3
Guess was: about 5 cycles

### Point 9
The first load lfdx left the reorder buffer of the pipeline, i.e. completed: cycle 49 (starts execution at 24 so takes 25 cycles)

First lfdx load has Iop 13, and the other ldfx is always one the operation after that. One loop is 7 operations and the we loop 100 times. Last lfdx should have Iop 13 (start) + 7 * 99 (iteration loops) = 706

blr is Iop 714 - makes sense

last of these loads is at cycle 341 which means that we have (341 - 49) / 100 ≈ 3 cycles/iteration

### Point 10
First ldfx: Iop 17, completed cycle 51

Last ldfx: Iop should be at 17 + 7 (opi) * 98 (i starts at 1) = 703. Completes at cycle 653


Gives us: (653 - 51) / 99 ≈ 6 cycles/iteration

#### Why so long iterations?
A lot of s:es, meaning "cannot issue, sources not ready" when doing fmadd

fmadd [f1, f2, f3, f4]: f1 <- f2 * f3 + f4

We are waiting for stfdx (writing registers to memory), which increases the clock cycles. This is done here because we use a value from the previous iteration, so it must complete. In a, we don't need to wait for the previous iteration to complete.

## Point 12

fsub f0, f12, f0 (pivot 10001424) always stalls for many clock cycles.

this happens because the instruction before needs to finish. that instuction is:

fdiv f0, f0, f11 (pivot 10001420) which executes for a pretty long time and also stalls for some clock cycles

it stalls because either:
1. cannot issue unit not free
2. cannot issue not my unified issue queue quads cycle (???)
3. cannot issue sources not ready

## Point 13

### Data dependencies:
- True dependency: an instruction produces a value which another instruction will consume
- Anti dependency: an instruction will read the value that a later instruction will overwrite. the later instruction needs to wait until it is safe to overwrite
- Output dependency: two instructions will overwrite a location and their writes must happen in that order
- Input dependency: two instructions will read the same location, but this type of dependence does not constrain execution

### Rename registers:
These are registers which can be reserved and used by instructions. A subsequent instruction that wants to read the register value sees in a table that the value should be taken from the rename register instead.

This eliminates anti and output dependencies