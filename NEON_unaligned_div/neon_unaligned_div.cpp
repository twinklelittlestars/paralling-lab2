#include <stdio.h>
#include <time.h>
#include <iostream>
#include<ctime>
#include<arm_neon.h>
using namespace std;
float32x4_t par1;
struct timespec sts,ets;
float matix[2048][2048];
void setmatix(int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matix[i][j] = 1.00 / (i + j + 1.00);
        }
    }
}
void timestart()
{
    timespec_get(&sts, TIME_UTC);
}
void timestop()
{
    timespec_get(&ets, TIME_UTC);
    time_t dsec=ets.tv_sec-sts.tv_sec;
    long dnsec=ets.tv_nsec-sts.tv_nsec;
    if(dnsec<0)
    {
        dsec--;
        dnsec+=1000000000ll;
    }
    printf ("%llu.%09llu \n",dsec,dnsec);

}
void neon_unaligned_div(int n)
{
    int k;
    for ( k = 0; k < n; k++)
    {
        int j;
        for ( j = k + 1; j <= n-4; j+=4)
        {
            par1=vld1q_f32(&(matix[k][j]));
            float32x4_t temp=vmovq_n_f32(matix[k][k]);
            par1=vdivq_f32(par1,temp);
            vst1q_f32(&(matix[k][j]),par1);
        }
        for(;j<n;j++)
        {
            matix[k][j]=matix[k][j]/matix[k][k];//bu zu 4 de shi hou
        }
        matix[k][k] = 0;
        for (int i = k + 1; i < n; i++)
        {
            for (int j = k + 1; j < n; j++)
            {
                matix[i][j] -= matix[i][k] * matix[k][j];
            }
            matix[i][k] = 0;
        }

    }
}
int main()
{
    setmatix(100);
    timestart();
    neon_unaligned_div(100);
    timestop();

    setmatix(300);
    timestart();
    neon_unaligned_div(300);
    timestop();

    setmatix(600);
    timestart();
    neon_unaligned_div(600);
    timestop();

    setmatix(1024);
    timestart();
    neon_unaligned_div(1024);
    timestop();

    setmatix(1200);
    timestart();
    neon_unaligned_div(1200);
    timestop();

    setmatix(1400);
    timestart();
    neon_unaligned_div(1400);
    timestop();

    setmatix(1600);
    timestart();
    neon_unaligned_div(1600);
    timestop();

    setmatix(1800);
    timestart();
    neon_unaligned_div(1800);
    timestop();

    setmatix(2000);
    timestart();
    neon_unaligned_div(2000);
    timestop();

    setmatix(2024);
    timestart();
    neon_unaligned_div(2024);
    timestop();
    return 0;
}
