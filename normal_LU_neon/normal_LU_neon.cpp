#include<stdio.h>
#include<time.h>
#include<iostream>
#include<ctime>
using namespace std;
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
    time_t dsec = ets.tv_sec - sts.tv_sec;
    long dnsec = ets.tv_nsec - sts.tv_nsec;
    if (dnsec < 0)
    {
        dsec--;
        dnsec += 1000000000ll;
    }
    printf("%llu.%09llu\n", dsec, dnsec);
}
void neon(int n) {

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++) {

            matix[i][j] /= matix[i][i];
        }
        matix[i][i] = 1.00;
        for (int k = i + 1; k < n; k++) {

            for (int j = i + 1; j < n; j++)
            {
                matix[k][j] -= matix[k][i] * matix[i][j];
            }
            matix[k][i] = 0.00;
        }
    }
}
int main()
{
    setmatix(100);
    timestart();
    neon(100);
    timestop();

    setmatix(300);
    timestart();
    neon_unaligned(300);
    timestop();

    setmatix(600);
    timestart();
    neon_unaligned(600);
    timestop();

    setmatix(1024);
    timestart();
    neon_unaligned(1024);
    timestop();

    setmatix(1200);
    timestart();
    neon_unaligned(1200);
    timestop();

    setmatix(1400);
    timestart();
    neon_unaligned(1400);
    timestop();

    setmatix(1600);
    timestart();
    neon_unaligned(1600);
    timestop();

    setmatix(1800);
    timestart();
    neon_unaligned(1800);
    timestop();

    setmatix(2000);
    timestart();
    neon_unaligned(2000);
    timestop();

    setmatix(2024);
    timestart();
    neon_unaligned(2024);
    timestop();
    return 0;

}