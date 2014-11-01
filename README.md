
# Table of Contents

<a href="#qstart">Quick Start</a><br>
&nbsp;&nbsp;<a href="#sample1.1">Sample</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;<a href="#cecag11.1.1">cecag1</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;<a href="#tianhe1.1.2">tianhe</a><br>
&nbsp;&nbsp;<a href="#gromacs1.2">Gromacs</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;<a href="#cecag11.2.1">cecag1</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;<a href="#tianhe1.2.2">tianhe</a><br>
<a href="#dusage">Gromacs Dirty Usage</a><br>
&nbsp;&nbsp;<a href="#universe2.1">Universe</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;<a href="#pre2.1.1">Prerequisite</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;<a href="#build2.1.1">Build</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;<a href="#run2.1.2">Run</a><br>
&nbsp;&nbsp;<a href="#tianhe2.2">Tianhe</a><br>


<a name="qstart"><h1>Quick Start</h1></a>

<a name="sample1.1"><h2>Sample</h2></a>

<a name="cecag11.1.1"><h3>cecag1</h3><a>

**Offload**

登录后运行Xeon参考书上的例子：

	$ cd _CECA_PACFINAL/codes/XPhiHPP_Vol1_code/Ch2
	$ icc -openmp -vec-report=3 -O3 helloflops3offload.c -o helloflops3offload
	$ ./helloflops3offload

**Coprocessor Hosted**

编译时要加<code>-mmic</code>参数

	$ icc -mmic -vec-report=3 -O3 helloflops1.c -o helloflops1

将程序拷贝到mic卡上

	$ scp helloflops1 pacuser12@mic0:/home/pacuser12

然后登录到mic上

	$ ssh pacuser12@mic0
	% cd /home/pacuser12
	% ./helloflops1

如果在mic0上没有账户，则可以先切换到root权限

	$ su
	$ scp helloflops1 mic0:/tmp
	$ ssh mic0
	% cd /tmp
	% ./helloflops1

<a name="tianhe1.1.2"><h3>tianhe</h3></a>

**Offload**

登录后所在结点是ln0结点，上面也有MIC卡，因为在ln0上也能运行<code>helloflops3offload</code>，编译方法和<a href="#cecag11.1.1">cecag1</a>相同。

	$ yhrun -w cn0 -p PAC ./helloflops3offload

**Coprocessor Hosted**

如果运行带“-mmic”参数的程序，

	$ ./sten2d9pt_base_xphi
	Not an executable
	$ yhrun -w cn[2] -p PAC sten2d9pt_base_xphi
	ld error: cannot find some shared libraries

第一个错误可以想见，用“-mmic”参数编译后的程序只能在MIC上运行；对于第二个错误，我觉得可能是程序需要的某些共享库没有在MIC上，于是我尝试将MIC上所缺失的库拷贝到MIC上：

	$ scp libxx.so cn2

却得到了如下失败信息：

	Access denied. User pacuser12 has no active job now on cn2.

这时应该先用<code>yhalloc</code>申请计算结点：

	$ yhalloc -N 8 -n 32

其中<code>N</code>表示申请的结点数目，<code>n</code>表示总mpi进程数目，注意不是每个结点上的进程数目。

然后用<code>yhq</code>查看jobID和结点号。

这样就可以在登录计算结点后将程序拷贝到计算结点的MIC卡上进行计算。

<a name="gromacs1.2"><h2>gromacs</h2><a>

<a name="cecag11.1.1"><h3>cecag1</h3><a>

登录公共帐号

	$ ssh pacuser12@222.29.98.5

设置编译环境变量

	$ . ./toolchain.sh

编译

	$ cd $HOME/gromacs-5.0.2
	$ mkdir build
	$ cd build
	$ ln -s $HOME/build-gromacs.sh build.sh
	$ . ./build.sh
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

注意调试时给gdb的程序不能是mpirun，因为我们调试的是gmx&#95;mpi。如果调试想要调试mpi程序，需要另外加参数，而且参数与底层编译器有关，如使用的mpi编译器为基于gcc的mpich2，编译参数要加-g；如果为icc，需要加intel编译器相关的调试参数。

<a name="tianhe1.1.2"><h3>tianhe</h3></a>

编译方法和<a href="#cecag11.2.1">cecag1</a>一样。运行的话可以使用<code>mpirun</code>和tianhe上的任务提交方法<code>yhxxx</code>运行。

如果使用yh方法，也可以有多种方法。

方法一：直接提交

使用<code>yhrun</code>直接提交任务到计算结点上运行：

	$ yhrun -w cn[0-1] -n 4 -N 4 -p PAC gmx_mpi mdrun -v -deffnm ~/benchmark/test-gmx5.tpr

其中<code>-w</code>参数指定至少要包含的计算结点；<code>-n</code>和<code></code>的意义如<a href="#tianhe1.1.2">tianhe</a>中所述。不过使用<code>-w</code>参数之前最好用<code>yhi</code>看一些指定的这些结点是否空闲，以减少不必要的等待。

如果使用<code>mpirun</code>，利用多个计算结点时需要撰写<code>hostlist</code>文件。

<a name="dusage"><h1>Gromacs Dirty Usage</h1></a>

<a name="universe2.1"><h2>Universe</h2></a>

<a name="pre2.1.1"><h3>Prerequiste</h3></a>

1. icc

tianhe上已经装好了。我们的cecag1上也有。

2. Intel MPI Library

Contains <code>mpiicc</code> and <code>mpiicpc</code>, etc.

目前我不知道怎样setup license，不过在tianhe的/opt/intel/licenses下发现了一些license文件，不知道用哪一个。。。目前先用evaluation版本，时间一个月，应该够了。等确定了用哪一个license文件后，再设置不迟。而且目前Intel MPI Library只能装在Redhat和SUSE上，不支持其它发行版。

另外，作为替代，可以装基于icc的mpich和基于gcc的mpich2。

例如，mpich基于intel编译器，configure时可以指定编译它的编译器为icc和icpc：

	./configure --prefix=$MPICHHOME CC=icc CXX=icpc

但不知道使用默认的编译器，如果是gcc，会不会有问题。


3. CMake (>=2.8.8)

tianhe上需要自己装。

4. Zlib

编译可以指定编译器，如用icc

	$ export CC="icc"
	$ export CFLAGS="-O3"

如果用gromacs在mic上运行使用symmetric模式，可以加<code>-mmic</code>参数。目前由于我们使用offload模式，所以暂时不需要“-mmic”。

但是按照pac给出的文档设置的cmake参数，目前连接到gromacs的libz.so并不是我们自己编译得到的版本，而是系统中在/usr/lib64/下的版本。不知道这对最后的在MIC上的执行会不会有影响。

<a name="build2.1.2"><h3>Build</h3></a>

1. 设置环境变量和cmake参数

**intel编译运行时环境**

	$ source /opt/intel/composerxe/bin/compilervars.sh intel64

**mkl库**

	$ source /opt/intel/mkl/bin/mklvars.sh intel64

**mpi编译器**

在cmake编译参数中指定：

	-DCMAKE_C_COMPILER=$MPICCDIR/mpicc
	-DCMAKE_CXX_COMPILER=$MPICCDIR/mpicxx

**zlib**

为<code>-DCMAKE_EXE_LILNKER_FLAGS</code>添加<code>-L$ZLIB_DIR/lib</code>选项。

2. 编译时遇到的问题及解决方案

	error: "rsqrft" is undefined.

PAC给出的解决方法是每次cmake以后修改buid/src/config.h文件。其实我们可以修改源代码中的src/config.h.cmakein文件，这样只需修改一次即可。

修改位置如下：

	line 241: #cmakedefine HAVE__ALIGNED_MALLOC -> /*#cmakedefine HAVE__ALIGNED_MALLOC*/

	line 277: #cmakedefine HAVE_RSQRT -> /*#cmakedefine HAVE_RSQRT*/

	line 280: #cmakedefine HAVE_RSQRTF -> /*#cmakedefine HAVE_RSQRTF*/

	error: undefined reference to "_aligned_free"

同样的方法。

	error: undefined reference to "_finite"

修改源代码中src/gromacs/math/utilities.c文件。

加了MIC offload语法以后link时出现了的问题：

	undefined reference to '__offload_target_acquire' and '__offload_offload'

原因是没有连上包含这些符号的动态库，所以解决方法是在cmake编译选项中打开编译动态连接库的选项：

	-DBUILD_SHARED_LIBS=ON

<a name="tianhe2.1"><h2>Tianhe</h2></a>

**编译环境的选择**

intel编译与运行时环境：

	$ source /vol-th/opt/intel/composer_xe_2013.0.079/bin/compilervars.sh intel64
	$ source /vol-th/opt/intel/composer_xe_2013.0.079/mkl/bin/mklrvars.sh intel64

在tianhe上有多套intel的编译环境（在/vol-th/opt和/opt目录下），如果是在ln0结点上编译，在cn结点上运行，那么在ln0上一定要选择与cn结点上匹配的环境，否则会在cn结点上运行时出现找不到库的问题。比如，如果使用ln0上在路径<code>/opt/intel/composerxe</code>下的编译与运行时环境，程序在cn计算结点上运行时会出现找不到mkl库的问题：

	cannot find libmkl_intel_lp64.so

因为在cn计算结点上根本就没有<code>/opt/intel/mkl/</code>这个目录。

除此之外，如果程序中要进行offload操作，如果使用其它的编译环境，如sp1.2.144和sp1.3.174等等，还会出现

	cannot offload to MIC - Device not available

这也是因为ln0结点和cn结点的编译与运行时版本不兼容造成，cn结点上的版本较低。目前经测试，只有079版本不会出现这个错误。

如果要使用Intel MPI Library，也需要设置相关环境变量：

	$ source /opt/intel/impi/5.0.0.035/bin64/mpivars.sh

但是现在基本决定在tianhe上放弃使用Intel MPI Library，因为在offload时还是会出现问题：

	offload error: cannot get device 1 handle (error code 2)

因此现在mpi的编译环境只能使用mpich。使用自己安装的mpich-3.1.3没有问题，但根据组委会的要求，应该使用指定的<code>/vol-th/opt/mpich3</code>，说是只有这个版本才能使用tianhe的高速互连网络。

**编译过程中的问题**

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

不过现在好像没有这个问题了，可能管理员调整了时间。



**运行过程中的问题**

	$ yhbatch -N 1 -p PAC -w cn2 ./batch.sh

batch.sh中的内容是：

	mpirun -hostfile hostlist -np 8 ~/Public/gromacs/bin/mdrun_mpi -v -deffnm ~/benchmark/test-gmx5.tpr

没有运行成功，yhbatch给出的信息是：

	mpirun: line 124: whoami: command not found
	mpirun: line 273: /tmp/slurm_.10390: Permission denied
	...

都是权限问题。应该是intel mpi library的问题。如果将mpirun换成mpich的版本，就只会报出缺少共享库的错误，所以只要想办法把共享库拷贝到MIC上就可以了。

	./gmx_mpi mdrun ...
	/usr/lib/libz.so.1: no version information available

这是因为连进来的系统中的zlib太旧了，可以自己安装并连接新版本的zlib。

还有一个gromacs自己的线程配置问题，在运行test5.tpr时openmp线程数目不能超过32，而如果不指定的话对所有benchmark都是64，这个数字可以用-ntomp来指定。但是用yhrun没有这个问题，可能是yhrun会修改mdrun的线程与进程配置，这个还需要进一步确认。


开始使用组委会指定的mpich3时，运行时会出现MPI相关的memory不够的问题，后来经管理员放宽了内存使用限制后这个问题就没有了。
