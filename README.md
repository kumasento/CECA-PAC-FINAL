
# Table of contents

Quick Start

|-tianhe
  |-coding
  |-gromacs

|-cecag1
  |-coding
  |-gromacs

Dirty Usage

|-tianhe
  |-coding
  |-gromacs

|-cecag1
  |-coding
  |-gromacs

# Quick Start

## tianhe

## cecag1

### coding

### gromacs

登录公共帐号

	$ ssh pacuser12@222.29.98.5

设置编译环境变量

	$ . ./toolchain.sh

编译

	$ cd $HOME/gromacs-5.0.2
	$ mkdir build
	$ cd build
	$ ln -s $HOME/build-gromacs.sh build-gromacs.sh
	$ bash build-gromacs.sh
	$ make -j 4

运行

	$ cd bin
	$ ./gmx_mpi mdrun -v -deffnm $HOME/benchmark/test-gmx5.tpr

使用mpi运行

	$ mpirun -np 1 -ppn 2 gmx_mpi mdrun -v -deffnm $HOME/benchmark/test-gmx5.tpr

其中<code>-np</code>指定结点数，<code>-ppn</code>指定每个结点上的进程数。

调试

	$ gdb -q ./gmx_mpi
	(gdb) start mdrun -v -deffnm $HOME/benchmark/test-gmx5.tpr

注意调试时给gdb的程序不能是mpirun，因为我们调试的是gmx_mpi。目前好像除了观察输出没有办法调试mpi程序。

# Dirty Usage

## tianhe

### Coding

在本地（这里及以后提到的“本地”指的是天河或cecag1的host）和cn结点上运行不带“-mmic”参数的程序可以成功，运行命令：

	$ ./sten2d9pt_base_xeon
	$ yhrun -w cn[2] -p PAC sten2d9pt_base_xeon

但是如果运行带“-mmic”参数的程序，分别会报不同的错误：

	$ ./sten2d9pt_base_xphi
	Not an executable
	$ yhrun -w cn[2] -p PAC sten2d9pt_base_xphi
	ld error: cannot find some shared libraries

第一个错误可以想见，可能用“-mmic”参数编译后的程序只能在MIC上运行；对于第二个错误，我觉得可能是程序需要的某些共享库没有在MIC上，于是我尝试将MIC上所缺失的库拷贝到MIC上：

	$ scp libxx.so cn2

却得到了如下失败信息：

	Access denied. User pacuser12 has no active job now on cn2.

可能可以使用的解决方法（因为tianhe直到现在都不能访问，所以还未尝试）：

1. 先安排一个较长时间运行的任务到结点上，然后将文件拷贝到结点上，最后运行真正需要执行的程序；

2. 应该可以有预先分配计算资源和时间片的命令，可能是yhalloc。

### GROMACS

#### Prerequisites

1. icc

tianhe上已经装好了。我们的cecag1上也有。

2. Intel MPI Library

Contains <code>mpiicc</code> and <code>mpiicpc</code>, etc.

目前我不知道怎样setup license，不过在tianhe的/opt/intel/licenses下发现了一些license文件，不知道用哪一个。。。目前先用evaluation版本，时间一个月，应该够了。等确定了用哪一个license文件后，再设置不迟。

3. CMake (>=2.8.8)

tianhe上需要自己装。

4. Zlib

如果zlib要连接的gromacs要到MIC上运行，编译时要同样要加<code>-mmic</code>参数。但是按照pac给出的文档设置的cmake参数，目前连接到gromacs的libz.so并不是我们自己编译得到的版本，而是系统中在/usr/lib64/下的版本。不知道这对最后的在MIC上的执行会不会有影响。

#### Build

1. 设置环境变量和cmake参数

2. 编译时遇到的问题及解决方案

error: "rsqrft" is undefined.

PAC给出的解决方法是每次cmake以后修改buid/src/config.h文件。其实我们可以修改源代码中的src/config.h.cmakein文件，这样只需修改一次即可。

error: undefined reference to "_aligned_free"

同样的方法。

error: undefined reference to "_finite"

修改源代码中src/gromacs/math/utilities.c文件。

warning: Clock skew detected. Your build may be incomplete.

[http://smilejay.com/2012/08/warning-clock-skew/](http://smilejay.com/2012/08/warning-clock-skew/)

[http://www.letuknowit.com/topics/20120131/linux-gcc-clock-skew-detected.html/](http://www.letuknowit.com/topics/20120131/linux-gcc-clock-skew-detected.html/)

[https://github.com/linux-sunxi/linux-sunxi/issues/62](https://github.com/linux-sunxi/linux-sunxi/issues/62)

原因是当前系统时间比编译目录中的文件（包括源程序文件和编译后生成的.o目标文件）修改时间早。可以用<code>date</code>命令查看当前系统时间。

可以用touch方法：

	find . -type f | xargs -n 5 touch
	make clean
	make

首先用“make clean”将之前编译的“.o”目标文件清除掉，然后再运行“make”进行编译，即使再有这样的warning也没关系，因为所有的目标都会被重新编译。但是要彻底去掉这个warning，需要用“date -s”命令将当前时间修改正确即可。

目前无法解决这个warning，因为执行date命令需要root权限。

#### Usage

	$ yhbatch -N 1 -p PAC -w cn2 ./batch.sh

batch.sh中的内容是：

	mpirun -hostfile hostlist -np 8 ~/Public/gromacs/bin/mdrun_mpi -v -deffnm ~/benchmark/test-gmx5.tpr

没有运行成功，yhbatch给出的信息是：

	mpirun: line 124: whoami: command not found
	mpirun: line 273: /tmp/slurm_.10390: Permission denied
	...

都是权限问题。应该是intel mpi library的问题。如果将mpirun换成mpich的版本，就只会报出缺少共享库的错误，所以只要想办法把共享库拷贝到MIC上就可以了。

## cecag1

### Coding

MIC的密码是什么？

	ssh mic0

提示密码，但是使用普通用户和root密码都不行。

### GROMACS

#### Prerequisites

1. mpich

基于intel编译器，configure时要注意使用的编译器为icc和icpc：

	./configure --prefix=$MPICHHOME CC=icc CXX=icpc

2. mpich2

基于gcc，应该不会用它。

3. Intel MPI Library

g1上的OS（Open SUSE）不支持，tianhe上OS为RedHat，因此目前暂时用mpich代替。

#### Build

由于compiler和机器发生了变化，编译时的参数要重新设置。

首先要把编译器mpiicc和mpiicpc分别换成mpicc和mpicxx。

然后由于g1不支持一些指令，如avx，所以要把cmake编译选项中关于特殊指令的选项关闭，如

	-DGMX_SIMD=AVX2_256

改为

	_DGMX_SIMD=OFF

再如

	-DCMAKE_C_FLAGS="... -xCORE-AVX2 -fimf-domain-exclusion=15..."

删掉，等等。

但是在tianhe上编译时，这些选项都要恢复。

#### Run

在本地上运行示例命令：

	$ mpirun -np 1 -ppn 2 gmx_mpi mdrun -v -deffnm test-gmx5.tpr

在MIC上运行示例命令：


