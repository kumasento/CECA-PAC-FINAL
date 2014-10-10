CC              =       icc
CFLAGS          =      -openmp -O3 -vec-report=3
FC		=	ifort
FFLAGS		=      -align array64byte -openmp -O3 -vec-report=3
MICDIR          =      /home/mic
MICLIBS		=      /opt/intel/composerxe/lib/mic

helloflops1: helloflops1.c
	$(CC) $(CFLAGS) -mmic helloflops1.c -o helloflops1.o

hflops2: helloflops2.c
	$(CC) $(CFLAGS) -mmic helloflops2.c -o helloflops2_xphi

hflops3: helloflops3.c
	$(CC) $(CFLAGS) -mmic helloflops3.c -o helloflops3_xphi

hflops3f: helloflops3.f90
	$(FC) $(FFLAGS) -mmic helloflops3.f90 -o helloflops3f_xphi

hflops3o: helloflops3offload.c
	$(CC) $(CFLAGS) helloflops3offload.c -o helloflops3o_xeon

hmem: hellomem.c
	$(CC) $(CFLAGS) -mmic hellomem.c -o hellomem_xphi

all: hflops1 hflops2 hflops3 hflops3f hflops3o hmem

miccopy: 
	sudo scp *_xphi mic0:$(MICDIR)

miclibcopy:
	sudo scp $(MICLIBS)/libiomp5.so mic0:$(MICDIR)
	@echo ""
	@echo "REMEMBER TO export LD_LIBRARY_PATH=$(MICDIR) ON THE COPROCESSOR (if needed)"
	@echo ""


clean: 
	rm -f ./*_xphi
	rm -f ./*_xeon



