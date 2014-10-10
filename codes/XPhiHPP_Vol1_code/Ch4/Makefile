CC              =       icc
CFLAGS          =      -openmp -O3 -std=c99 -vec-report=3
MICDIR          =      /home/mic
MICLIBS		=      /opt/intel/composerxe/lib/mic

base: diffusion_base.c
	$(CC) $(CFLAGS) -mmic diffusion_base.c -o diffusion_base_xphi

omp: diffusion_omp.c
	$(CC) $(CFLAGS) -mmic diffusion_omp.c -o diffusion_omp_xphi

ompvect: diffusion_ompvect.c
	$(CC) $(CFLAGS) -mmic diffusion_ompvect.c -o diffusion_ompvect_xphi

peel: diffusion_peel.c
	$(CC) $(CFLAGS) -mmic diffusion_peel.c -o diffusion_peel_xphi

tiled: diffusion_tiled.c
	$(CC) $(CFLAGS) -mmic diffusion_tiled.c -o diffusion_tiled_xphi

all: base omp ompvect peel tiled

miccopy: 
	sudo scp *_xphi mic0:$(MICDIR)

miclibcopy:
	sudo scp $(MICLIBS)/libiomp5.so mic0:$(MICDIR)
	@echo ""
	@echo "REMEMBER TO export LD_LIBRARY_PATH=$(MICDIR) ON THE COPROCESSOR (if needed)"
	@echo ""


clean: 
	rm -f ./*_xphi



