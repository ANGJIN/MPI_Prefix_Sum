***
# Prefix sum program using MPI
***
Please change 'hosts' file before run
## HOW TO USE
1. type "make"
2. check following 7 executables are made
    * 2_a_block
    * 2_a_nonblock
    * 2_b_block
    * 2_b_nonblock
    * 2_scan_block
    * 2_scan_nonblock
    * 2_serial
3. program need 1 argument of n ( how many random number to generate, and make prefix sum)
4. 2_serial can execute directly, all others can execute with mpiexec command  
    example :  
        ./2_serial 100  
        mpiexec -np 9 -mca btl ^openib -hostfile hosts ./2_a_block 100  

        
