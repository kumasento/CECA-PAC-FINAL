#第一次讨论的补充：

首先感谢大家能加入PAC团队！这里有一些细节需要告诉大家：

1. PAC大赛相关的信息，比如硬件平台，赛制，联系方式
2. Gromacs的相关信息，主要是如何编译和运行的材料
3. 相关的阅读材料，主要是Xeon Phi的相关教程和论文，以及上次讨论时用到的MD的论文

##PAC大赛相关信息：

1. 硬件平台：
	* 目前使用与最终比赛测试的平台都是在大赛主办方提供的天河2号平台上
	* 登陆方式： 主要是通过`ssh`访问主办方提供的服务器
			
			Host name: 182.92.69.131
			User: pacuser12
	* 有关这台服务器的信息：
		1. 软件环境：
			* **编译环境**： 
				* **编译器**： 支持完整的Intel compiler的工具链，即可以把代码上传到这个服务器上进行编译
				* **CMake**： 服务器上只有2.6版的CMake，编译Gromacs的话需要安装2.8版的
			* **MPI配置**： 
				* 服务器上只有OpenMPI，未安装Intel MPI，我们最后应该要用后者；**安装Intel的软件的话，服务器上有相关的license**
				* 天河的机器上也有一套自己的MPI实现，这里我也还不太清楚最后要用哪个……
		2. 硬件环境：
			* 在一般的Xeon + Xeon Phi的环境上，从主机OS访问Xeon Phi的OS需要用ssh访问，比如
					
					ssh username@mic0
			* 但是在天河的服务器上需要首先申请**计算节点**，登陆到计算节点以后才能进行对Xeon Phi的访问以及计算
			* 有关计算节点相关的材料请参考`docs/org`下的资料
2. 赛制：
	* 这里是大赛的官方网站，里面有这个比赛的具体介绍；
	* 我们应该是11月6号到8号进行比赛
3. 大赛主办方的联系方式：
	* 邮件： `pac@paratera.com`
	* QQ: 318382031 PAC2014技术交流群（群介绍里面也有vpn的相关信息）
	* 负责天河机器的联系人为`李伯杨`，负责参赛具体事项的为`陈慧斌`
	* 相关的重要邮件我都放在`docs/org/mails`里面了
	
##Gromacs的相关信息：

1. Gromacs的官方网站： gromacs.org
2. 安装信息：
	* 选择gromacs5.0以上的版本，最终选用哪个还没确定，最好是最新版
	* 有关决赛题目的信息在`PAC决赛通知`这封邮件里面有
	* 在`docs/org`下面有一个`How to Compiler(...) Gromacs.DOCX`的文件，里面有主办方提供的安装信息
	* 在gromacs的源文件目录下也有安装指南
3. 有关gromacs的代码结构：
	* 有些发现：
		* 在`program/mdrun`里面是程序的入口
		* 其他的就没有了……
	> @Xuechao麻烦在下次组会的时候能介绍一下代码的结构以及安装信息和遇到的问题~！

##阅读材料：

1. 有关Xeon Phi：
	* 目前最权威的教材就是Intel Xeon Phi High Performance Programming这本书；如果只是简单了解一下Xeon Phi的话，可以只读前两章；
	* 这本书的前四章的代码我放在`codes/`下面了，**希望大家能把那些样例在Xeon Phi机器上编译然后运行一下！**
	* 其他相关的材料基本上都是Intel官方网站的blog
		* 有两篇不错的文档： *Optimization and Performance Tuning for Intel® Xeon Phi Coprocessors, Part 1~2*
2. 有关MD和gromacs：
	* gromacs发展历史上的几篇论文都在`docs/papers`下面
	* 上次提到的MD的算法概述是从这篇论文里面找到的`docs/papers/acc-CUDA`
	> @Xiaolong 师兄下次麻烦介绍一下有关分子动力学的并行算法的知识！
	* 上次的slides我放在`docs/slides`下面了
	
谢谢大家！
			