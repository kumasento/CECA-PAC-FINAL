CC              =       icc
CFLAGS          =      -openmp -O3 -vec-report=3
MICDIR          =      /home/mic
MICLIBS		=      /opt/intel/composerxe/lib/mic

base: sten2d9pt_base.c
	$(CC) $(CFLAGS) -mmic sten2d9pt_base.c -o sten2d9pt_base_xphi
	$(CC) $(CFLAGS) sten2d9pt_base.c -o sten2d9pt_base_xeon

vect: sten2d9pt_vect.c
	$(CC) $(CFLAGS) -mmic sten2d9pt_vect.c -o sten2d9pt_vect_xphi
	$(CC) $(CFLAGS)  sten2d9pt_vect.c -o sten2d9pt_vect_xeon

omp: sten2d9pt_omp.c
	$(CC) $(CFLAGS) -mmic sten2d9pt_omp.c -o sten2d9pt_omp_xphi
	$(CC) $(CFLAGS) sten2d9pt_omp.c -o sten2d9pt_omp_xeon

pad: sten2d9pt_pad.c
	$(CC) $(CFLAGS) -mmic  sten2d9pt_pad.c -o sten2d9pt_pad_xphi

sstores: sten2d9pt_sstores.c
	$(CC) $(CFLAGS) -mmic  sten2d9pt_sstores.c -o sten2d9pt_sstores_xphi

huge: sten2d9pt_huge.c
	$(CC) $(CFLAGS) -mmic sten2d9pt_huge.c -o sten2d9pt_huge_xphi

all: base vect omp pad sstores huge

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

