#include "fasttrig.h"

// compute a good approximation for cos/sin using parablic expansion.
// provides a good-enough solution fast..
struct fc32_t fast_cos_sin(float rads) {
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
	struct fc32_t rval = { cos,sin };
  return rval;
}


