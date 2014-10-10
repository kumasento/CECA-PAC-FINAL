Intel(r) Xeon Phi(tm) Coprocessor High Performance Programming

Chapter 2 Code Samples

The files in this directory and all subdirectories are licensed to you according to LICENSE.txt, which is one level up from this directory. 

Building and Running

You may build and run the examples as directed in the book or we have provided a simple Makefile to make it a bit easier.

Before compiling and running we assume:

1) You have installed a version Intel(r) [C++] Composer XE 2013, Intel(r) Parallel Studio XE 2013, or Intel(r) Cluster Studio XE 2013 version 117 or higher.  The makefile assumes the Intel development tools environment, if you are using another supported compiler you may need to adapt the Makefile and follow the compiler provider's instructions.

2) Ensure your environment is setup for compiling. If you are using a supercomputing cluster such as TACC's Stampede, please read the information on compiling for Intel(r) Xeon Phi(tm) from the provider.  On a standalone workstation or single node with the compiler installed with defaults you should ensure you 'source' the compiler with a command similar to below:

>source /opt/intel/composer_xe_2013/bin/compilervars.sh intel64

3) You have at least one Intel(r) Xeon Phi(tm) coprocessor in the host platform your are running.

4) A version of the Intel(r) Manycore Software Stack (Intel(r) MPSS) that 
is compatible with the Intel Composer XE 2013 version described in 1 is installed. The coprocessor is booted and running and you have your account settings to allow SSH access to the coprocessor.  On a standalone workstation or a cluster node that is unmanaged the following command should start the coprocessor:

>sudo service mpss start
  

Here are the make options provided in the Makefile:

make all - will build all the code for the coprocessor and processor

make hflops1 - will build the first code sample with no threading

make hflops2 - will build the second code sample with 2 OpenMP threads

make hflops3 - will build the third code sample with general OpenMP threads control using "export OMP_NUM_THREADS=<thread count>" and "export KMP_AFFINITY=<affinity setting>"  at the command prompt.

make hflops3f - will build a Fortran version of the helloflops3

make hflops3o - will build an Offload version run from the processor command prompt

make hmem - will build the memory bandwidth code sample.
 

make clean - will remove all execution binaries files generated with make

make miccopy - will copy all coprocessor execution binaries [*_xpi] to the first mic card [mic0].  NOTE: not necessary if you home directory is NFS mounted by the coprocessor (e.g. TACC Stampede).

make miclibcopy - will copy the required library files to the coprocessor (OpenMP only currently). NOTE: not necessary if you environment has been preconfigured by the system admin or cluster provider. You may need to set the library path if not preconfigured in your environment such as:
[mic@mic0 mic] % export LD_LIBRARY_PATH=/home/mic


