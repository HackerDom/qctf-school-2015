#ifndef _RAND_H_
#define _RAND_H_
#define YCIKPHJBWX_N 624
#define YCIKPHJBWX_M 397
#define YCIKPHJBWX_MATRIX_A 0x9908b0dfUL   
#define UPPER_YCIKPHJBWX_MASK 0x80000000UL
#define LOWER_YCIKPHJBWX_MASK 0x7fffffffUL

static unsigned long ycikphjbwx_mt[YCIKPHJBWX_N];
static int ycikphjbwx_mti=YCIKPHJBWX_N+1;

void z_srand(unsigned long s)
{
    ycikphjbwx_mt[0]= s & 0xffffffffUL;
    for (ycikphjbwx_mti=1; ycikphjbwx_mti<YCIKPHJBWX_N; ycikphjbwx_mti++) {
        ycikphjbwx_mt[ycikphjbwx_mti] = 
	    (1812433253UL * (ycikphjbwx_mt[ycikphjbwx_mti-1] ^ (ycikphjbwx_mt[ycikphjbwx_mti-1] >> 30)) + ycikphjbwx_mti); 
        ycikphjbwx_mt[ycikphjbwx_mti] &= 0xffffffffUL;
    }
}

unsigned long rand32()
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, YCIKPHJBWX_MATRIX_A};

    if (ycikphjbwx_mti >= YCIKPHJBWX_N) { 
        int kk;

        if (ycikphjbwx_mti == YCIKPHJBWX_N+1) 
            z_srand(5489UL);

        for (kk=0;kk<YCIKPHJBWX_N-YCIKPHJBWX_M;kk++) {
            y = (ycikphjbwx_mt[kk]&UPPER_YCIKPHJBWX_MASK)|(ycikphjbwx_mt[kk+1]&LOWER_YCIKPHJBWX_MASK);
            ycikphjbwx_mt[kk] = ycikphjbwx_mt[kk+YCIKPHJBWX_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<YCIKPHJBWX_N-1;kk++) {
            y = (ycikphjbwx_mt[kk]&UPPER_YCIKPHJBWX_MASK)|(ycikphjbwx_mt[kk+1]&LOWER_YCIKPHJBWX_MASK);
            ycikphjbwx_mt[kk] = ycikphjbwx_mt[kk+(YCIKPHJBWX_M-YCIKPHJBWX_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (ycikphjbwx_mt[YCIKPHJBWX_N-1]&UPPER_YCIKPHJBWX_MASK)|(ycikphjbwx_mt[0]&LOWER_YCIKPHJBWX_MASK);
        ycikphjbwx_mt[YCIKPHJBWX_N-1] = ycikphjbwx_mt[YCIKPHJBWX_M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        ycikphjbwx_mti = 0;
    }
  
    y = ycikphjbwx_mt[ycikphjbwx_mti++];

    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

long rand31(void)
{
    return (long)(rand32()>>1);
}

unsigned long long rand64()
{
	long long a = rand32();
	long long b = rand32();
	return (a << 32) | b;
}

long long rand63()
{
	long long a = rand31();
	long long b = rand32();
	return (a << 32) | b;
}

/* generates a random number in [l,r]-integer-interval */
int rand(int l, int r)
{
	return l + (int) (rand64() % (unsigned) (r - l + 1));
}

/* generates a random number on [0,1]-real-interval */
double randreal1(void)
{
    return (double)rand32()*(1.0/4294967295.0); 
}

/* generates a random number on [0,1)-real-interval */
double randreal2(void)
{
    return (double)rand32()*(1.0/4294967296.0); 
}

/* generates a random number on (0,1)-real-interval */
double randreal3(void)
{
    return (((double)rand32()) + 0.5)*(1.0/4294967296.0); 
}

/* generates a random number on [0,1) with 53-bit resolution*/
double randreal53(void) 
{ 
    unsigned long a=rand32()>>5, b=rand32()>>6; 
    return((double)a*67108864.0+(double)b)*(1.0/9007199254740992.0); 
} 

template <class T> void z_swap(T * el1, T * el2)
{
	T val = *el1;
	*el1 = *el2;
	*el2 = val;
}

template <class T> void z_random_shuffle(T * first, T * last)
{
	int n = (int) (last - first);
	for (int i = 0; i < n - 1; i++){
		int pos = rand(i, n - 1);
		z_swap(first + i, first + pos);
	}
}

template <class T> void z_swap(T & el1, T & el2)
{
	T val = el1;
	el1 = el2;
	el2 = val;
}

template <class iterator> void z_random_shuffle(iterator first, iterator last)
{
	int n = (int)(last - first);
	for (int i = 0; i < n - 1; i++){
		int pos = rand(i, n - 1);
		z_swap(*(first + i), *(first + pos));
	}
}

#endif
