#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define sampcnt 1048576

struct complex_t {
	float i;
	float q;
};

// compute a good approximation for cos/sin using parablic expansion.
// provides a good-enough solution fast..
struct complex_t fast_cos_sin(float rads) {
	float x = rads;
  float sin, cos;
  if (x < -3.14159265)
      x += 6.28318531;
  else
  	if (x >  3.14159265)
      x -= 6.28318531;
  //compute sine
  if (x < 0)
  {
    sin = 1.27323954 * x + .405284735 * x * x;
    if (sin < 0)
      sin = .225 * (sin *-sin - sin) + sin;
    else
      sin = .225 * (sin * sin - sin) + sin;
  }
  else
  {
    sin = 1.27323954 * x - 0.405284735 * x * x;
    if (sin < 0)
    	sin = .225 * (sin *-sin - sin) + sin;
    else
      sin = .225 * (sin * sin - sin) + sin;
  }
  //compute cosine: sin(x + PI/2) = cos(x)
  x += 1.57079632;
  if (x >  3.14159265)
    x -= 6.28318531;

  if (x < 0)
  {
    cos = 1.27323954 * x + 0.405284735 * x * x;
    if (cos < 0)
      cos = .225 * (cos *-cos - cos) + cos;
    else
      cos = .225 * (cos * cos - cos) + cos;
  }
  else
  {
    cos = 1.27323954 * x - 0.405284735 * x * x;
    if (cos < 0)
      cos = .225 * (cos *-cos - cos) + cos;
    else
      cos = .225 * (cos * cos - cos) + cos;
  }
	struct complex_t rval = { cos,sin };
  return rval;
}

int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{  
  // preserve *y
  struct timeval yy = *y;
  y = &yy;

  /* Perform the carry for the later subtraction by updating y. */  
  if (x->tv_usec < y->tv_usec) {  
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;  
    y->tv_usec -= 1000000 * nsec;  
    y->tv_sec += nsec;  
  }  
  if (x->tv_usec - y->tv_usec > 1000000) {  
    int nsec = (y->tv_usec - x->tv_usec) / 1000000;  
    y->tv_usec += 1000000 * nsec;  
    y->tv_sec -= nsec;  
  }  

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */  
  result->tv_sec = x->tv_sec - y->tv_sec;  
  result->tv_usec = x->tv_usec - y->tv_usec;  

  /* Return 1 if result is negative. */  
  return x->tv_sec < y->tv_sec;  
}  

int main(void) {
  //printf("Test to test implementions of computing vector rotations.\n");
  float rads=0;
	float rad_step=M_PI/2/128; // 128 steps between 0 and M_PI/2
  while (rads <= M_PI ) {
		struct complex_t fast;
		struct complex_t slow;
    struct complex_t error;
		fast = fast_cos_sin(rads);
	  slow.i = cos(rads);
	  slow.q = sin(rads);
		error.i = slow.i - fast.i;
		error.q = slow.q - fast.q;
    //printf("rads = %f fast: %f,%f slow: %f,%f  error: %f,%f\n", 
		//	rads, fast.i, fast.q, slow.i, slow.q, error.i, error.q );
    rads = rads + rad_step;
  }

  struct timeval start, end, diff;
  struct complex_t *data = malloc( sizeof(struct complex_t)*sampcnt );

  rad_step = 2*M_PI/sampcnt;
  rads = 0;
  gettimeofday(&start, NULL);
  for ( int idx=0; idx < sampcnt; idx++ ) {
    data[idx] = fast_cos_sin(rads);
    rads = rads + rad_step;
  }
  gettimeofday(&end, NULL);
	timeval_subtract( &diff, &end, &start);
	double interval_us = diff.tv_sec * 1e6 + diff.tv_usec;
	printf("Fast cos_sin produced %d samples in %f micro seconds.\n", sampcnt, interval_us);

  gettimeofday(&start, NULL);
  for ( int idx=0; idx < sampcnt; idx++ ) {
    data[idx].i = cos(rads);
	  data[idx].q = sin(rads);
    rads = rads + rad_step;
  }
  gettimeofday(&end, NULL);
	timeval_subtract( &diff, &end, &start);
	interval_us = diff.tv_sec * 1e6 + diff.tv_usec;
	printf("gcc cos/sin produced %d samples in %f micro seconds.\n", sampcnt, interval_us);

  return 0;
}

