# Report : Analyse the performance of the intopt.c program

## Function calls, time of execution and pipeline

First off, using `operf`, we saw which functions take the most amount of time when running the solver with a relatively large input (n = 15).

    samples  %        symbol name
    407578   92.3706  pivot
    21665     4.9100  xsimplex
    3515      0.7966  initial

It turns out that the pivot operation, and its corresponding `pivot` function were responsible for around 90 percent of the execution time.

### Analyse function calls with `gprof`

     %   cumulative   self              self     total           
    time   seconds   seconds    calls  ms/call  ms/call  name    
    92.9      11.73    11.73   981541     0.01     0.01  pivot [7]
     5.1      12.38     0.65    33047     0.02     0.37  xsimplex <cycle 1> [6]
    ...

This output shows us that the `xsimplex` function takes the most time by call. Moreover, with `gprof` we can examine the callers and callees for each function. Thus, we can see that `pivot` is called mostly by `xsimplex` and conclude that this is the reason for `xsimplex` taking most time per call.

### Analyse line calls with `gcov`

         36255724:  167:  for (i = 0; i < m; i++){
         35274183:  168:    if (i != row){
        742832617:  169:      for (int j = 0; j < n; j++){
        708539975:  170:        if (j != col){
        674247333:  171:          a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
        ...
    
An analysis of the line calls gives us an idea as to which specific lines of code are most frequently executed. As can be seen above, the nested for loops inside `pivot` are called a substantial amount of times. This, of course, shines some light as to why the function takes so much execution time.

### Pipeline stalling, localisation and explanation

When analysing our program using the pipeline simulator, we found that the instruction `fsub f0, f12, f0 (pivot 10001424)` always stalls for many clock cycles. Upon further investigation, we found that this happens because the previous instruction needs to finish. That instuction, `fdiv f0, f0, f11 (pivot 10001420)`, executes for a pretty long time and also stalls for some clock cycles. It stalls because of three different reasons:

1. cannot issue unit not free
2. cannot issue not my unified issue queue quads cycle
3. cannot issue sources not ready

While the first and third reasons are quite self-explanatory, neither we nor our lab examinator could quite understand the second one.

**It's worth mentioning that the scrollpv program was slightly buggy on MacOs and vertically cut of the description text. Thus, the second stall reason might have differed from what we've written above.**

## Cache behaviour analysis with Cachegrind

    ==3423== I   refs:      3,907,443
    ==3423== I1  misses:        4,026
    ==3423== LLi misses:          884
    ==3423== I1  miss rate:      0.10% 
    ==3423== LLi miss rate:      0.02%
    ==3423== 
    ==3423== D   refs:      1,735,094  (1,583,348 rd   + 151,746 wr)
    ==3423== D1  misses:        2,587  (    2,035 rd   +     552 wr)
    ==3423== LLd misses:        1,786  (    1,351 rd   +     435 wr)
    ==3423== D1  miss rate:       0.1% (      0.1%     +     0.4%  )
    ==3423== LLd miss rate:       0.1% (      0.1%     +     0.3%  )
    ==3423== 
    ==3423== LL refs:           6,613  (    6,061 rd   +     552 wr)
    ==3423== LL misses:         2,670  (    2,235 rd   +     435 wr)
    ==3423== LL miss rate:        0.0% (      0.0%     +     0.3%  )

By using Cachegrind, we found that the miss rates are pretty acceptable. Most notably, we have a miss rate of 0.1% for the I1 cache and 0.1% for the D1 cache which leaves some room for improvements. We could for example analyse how our code relates to spatial locality in the cache and explore which code lines that are not read using `gcov`.

## Compilation modes

As we saw in the last lab, different optimization modes can give us different benefits for different algorithms. This was only tested on example programs, but leaves some room for experimentation when it comes to our `intopt` program.  

### Optimze the speed or size

### Vectorize and profiling 

Another important lesson from the last lab was how to make the compiler vectorize our programs and the benefits of this. Until this point, we've only compiled `intopt` with no optimization and no flags but we believe that enabling vectorization and making  sure it works correctly would greatly improve the performance of our pogram.

### Different compilers

Interestingly, we found that different compilers had vastly differing performances on the example programs. When running the matrix multiplication program we got the following results:

1. IBM xlc: 0.138s
2. Nvidia: 0.288s
3. GCC: 0.790s
4. Clang: 1.849s

While the difference in execution time might be surprising, it is no surprise that the IBM compiler outperforms all the others. After all, we're running the program on a POWER processor. Using the IBM compiler with `intopt` on the power machine would therefore most certainly improve our execution times.

## Conclusion : how improve the performance of our intopt.c program

As has been stated, execution time of our `intopt` program mostly consists of the `pivot` function. Since the function has quite many calculations and some nested loops, we would probably start by examining the code in detail and investigate how its structure impacts the cache misses we saw through `cachegrind`. Furthermore, we could examine the pipeline stalls in more detail and try to get to the bottom of why some instructions stall for so many clock cycles. Last but not least, we would experiment with optimization modes, vectorization and different compilers to maximize the performance.







