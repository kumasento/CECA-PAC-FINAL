#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <assert.h>
#include <sys/mman.h>


#define SAVEFILES 0 // 1 if saving results
#define DBG 0    // Print out results
#define REAL double

#define PAD64 0

#define WIDTH 5900

#if PAD64
#define WIDTHP ((((WIDTH*sizeof(REAL))+63)/64)*(64/sizeof(REAL)))
#else
#define WIDTHP WIDTH
#endif

#define HEIGHT 10000


void initbuf(REAL *fbuf, const int width, const int height) 
{

  REAL val;
  int x,y;

  for (y=0; y < height; y++) {
    val = (y % 10) ? 0 : 1.0;
    for (x=0; x < width; x++) {
	fbuf[x+y*WIDTHP] = val;
    }
  }
  return;
}

void
stencil9pt_base(REAL *finp, REAL *foutp, 
		int width, int height,
                REAL ctr, REAL next, REAL diag,int count)
{
  REAL *fin = finp;
  REAL *fout = foutp;
  int i,x,y;

  for (i=0; i<count; i++) {
    for (y=1; y < height-1; y++) {
       // starting center pt (avoid halo)
      int c = 1 + y*WIDTHP+1;
      // offsets from center pt.
      int n = c-WIDTHP;
      int s = c+WIDTHP;
      int e = c+1;
      int w = c-1;
      int nw = n-1;
      int ne = n+1;
      int sw = s-1;
      int se = s+1;

      for (x=1; x < width-1; x++) {
        fout[c] = diag * fin[nw] +  
                  diag * fin[ne] +
                  diag * fin[sw] +
                  diag * fin[se] +
                  next * fin[w] +
                  next * fin[e] +
                  next * fin[n] + 
                  next * fin[s] +
                  ctr  * fin[c];

        // increment to next location
        c++;n++;s++;e++;w++;nw++;ne++;sw++;se++;
      }
    }
    REAL *ftmp = fin;
    fin = fout;
    fout = ftmp;
  }
  return;
}

static double dtime() {
  double tseconds = 0.0;
  struct timeval mytime;
  gettimeofday(&mytime,(struct timezone *) 0);
  tseconds = (double) (mytime.tv_sec + (double)mytime.tv_usec * 1.0e-6);
  return (tseconds) ;
}

#if DBG
dbgprint(REAL *fbuf,int width, int height)
{
  // print out a chunk for review of calc..
  //
  int x, y, offset;

  // Head left
  for (y=0; y<20; y++)
  {
	  offset = y*width;
	  for (x=0; x<10; x++)
	  {
		  printf("%10.3f", fbuf[offset+x]);
	  }
	  printf("\n");
  }
  printf("Tail Left\n");
  for (y=height-20; y<height; y++)
  {
	  offset = y*width;
	  for (x=0; x<10; x++)
	  {
		  printf("%10.3f", fbuf[offset+x]);
	  }
	  printf("\n");
  }

  printf("Tail Right\n");
  for (y=height-20; y<height; y++)
  {
	  offset = y*width;
	  for (x=width-10; x<width; x++)
	  {
		  printf("%10.3f", fbuf[offset+x]);
	  }
	  printf("\n");
  }
  

  printf("\n\n");
}
#endif

void savetofile(char *filename, REAL *fbuf, int width, int height) {
  FILE *fp = fopen(filename, "w");
  assert(fp);
  size_t numitems = width * height;
  fwrite(fbuf, sizeof(REAL), numitems, fp);

  fclose(fp);
}

int main(int argc, char *argv[]) 
{
  double t_begin, t_end;
  int    i;
  int    width    = WIDTH;
  int    height   = HEIGHT;
  REAL   *fout, *fin, *ftmp;

  REAL *fa = (REAL *)malloc(sizeof(REAL)*WIDTHP*HEIGHT);
  REAL *fb = (REAL *)malloc(sizeof(REAL)*WIDTHP*HEIGHT);

  REAL   time  = 0.0;
  int    count = 1000;  

  REAL stendiag, stennext, stenctr;
 
  fin = fa;
  fout = fb;

  printf("Initializing..%d Threads, %d x %d, PAD=%d..\n\n",
         omp_get_num_threads(),WIDTH, HEIGHT, WIDTHP);

  initbuf(fin, width, height);
  initbuf(fout, width, height);

#if DBG
  dbgprint(fin, WIDTHP, height);
#endif

#if SAVEFILES
  savetofile("stencil9pt_init.dat", fin, WIDTHP, HEIGHT);
#endif
  // stencil params: slowly blurring...
  
  stendiag = 0.00125;
  stennext = 0.00125;
  stenctr = 0.99;

  printf("Running stencil kernel %d times\n", count);

  t_begin = dtime();

  stencil9pt_base(fin, fout, width, height, 
                  stenctr, stennext, stendiag,count);

  t_end = dtime();  

#if DBG
  dbgprint(fout, WIDTHP, height);
#endif
#if SAVEFILES
  savetofile("stencil9pt_result.dat",fb, WIDTHP, HEIGHT);
#endif

  double elapsed_time = t_end - t_begin;
  REAL mflops = (width*height)*17.0*count/elapsed_time * 1.0e-06;

  printf("Elapsed time : %.3f (s)\n", elapsed_time);
  printf("FLOPS        : %.3f (MFlops)\n", mflops);
 
  free(fa);
  free(fb);

  return 0;
}
