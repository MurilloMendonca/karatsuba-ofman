#include <iostream>
#include<cmath>
#include<cstdlib>
#include <time.h>
#include<ctime>
#include <queue>
#include <fstream>
#include <windows.h>
#include <inttypes.h>
#include <stdio.h>
#include "bigInt.h"
#include <string>
using namespace std;


#define GINT BigInt::Rossi




double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

GINT BigPow(int a, int b){
    BigInt::Rossi c ("1", BigInt::DEC_DIGIT);

    for(int i =0;i<b; i++)
    {
        c=c*a;
    }
    return c;
}
GINT karatsuba(GINT u, GINT v, int n)
{
    if(n<=3)
        return u*v;
    int m = n/2;
    GINT bp = BigPow(10,m);
    GINT p = u/(bp);
    GINT q = u%(bp);
    GINT r = v/(bp);
    GINT s = v%(bp);
    GINT pr = karatsuba(p,r,m);
    GINT qs = karatsuba(q,s,m);
    GINT y= karatsuba(p+q,r+s,m+1);
    return pr* (BigPow(10,2*m)) + (y - pr -qs)*(bp) + qs;
}
int main()
{
    BigInt::Rossi a ("284724812304981334590324", BigInt::DEC_DIGIT);
    BigInt::Rossi b ("984357298374619238698347", BigInt::DEC_DIGIT);


    StartCounter();
    BigInt::Rossi c = a*b;
    cout<<"\nnormal:  "<<a.toStrDec()<<" * "<<b.toStrDec()<<" = "<<c.toStrDec();
    cout<<"\ncom tempo: "<<GetCounter()<<"ms";

    StartCounter();
    c = karatsuba(a,b,24);
    cout<<"\nkaratsuba:  "<<a.toStrDec()<<" * "<<b.toStrDec()<<" = "<<c.toStrDec();
    cout<<"\ncom tempo: "<<GetCounter()<<"ms";

/*
    StartCounter();
    GINT kara = karatsuba(a,b,24);
    cout<<"\nnormal:  ";
    print_uint128(a);
    cout<<" x ";
    print_uint128(b);
    cout<<" = ";
    print_uint128(kara);
    cout<<"  com tempo: "<<GetCounter()<<"ms";*/
    return 0;
}
