/* code from George Marsaglia on http://www.cse.yorku.ca/~oz/marsaglia-rng.html */
#include "random.h"
#define znew (z=36969*(z&65535)+(z>>16))
#define wnew (w=18000*(w&65535)+(w>>16))
#define MWC ((znew<<16)+wnew )
#define SHR3 (jsr^=(jsr<<17), jsr^=(jsr>>13), jsr^=(jsr<<5))
#define CONG (jcong=69069*jcong+1234567)
#define FIB ((b=a+b),(a=b-a))
#define KISS ((MWC^CONG)+SHR3)
#define LFIB4 (c++,t[c]=t[c]+t[UC(c+58)]+t[UC(c+119)]+t[UC(c+178)])
#define SWB (c++,bro=(x<y),t[c]=(x=t[UC(c+34)])-(y=t[UC(c+19)]+bro))
#define UNI (KISS*2.328306e-10)
#define VNI ((long) KISS)*4.656613e-10
#define UC (unsigned char) /*a cast operation*/
typedef unsigned long UL;
/* Global static variables: */
static UL z=362436069, w=521288629, jsr=123456789, jcong=380116160;
static UL a=224466889, b=7584631, t[256];

void srand(UL seed) { //utilise SHR3 pour générer les seed de MWC, puis MWC pour générer les seed de KISS, puis KISS pour le reste
  jsr = seed;
  z = SHR3;
  w = SHR3;
  jcong = MWC;
  a = MWC;
  b = MWC;
  for(int i=0;i<256;i=i+1) t[i]=KISS;
}

int randint(int i1, int i2) {
  return i1 + KISS%(i2-i1);
}
