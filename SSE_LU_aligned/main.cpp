#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <pmmintrin.h>
using namespace std;

long long head, tail, freq;
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
void sse_lu_aligned(int n)
{
    __m128 m1, m2, m3;
    for (int k = 0; k < n; k++)
    {
        for (int j = k + 1; j < n; j++)
            matix[k][j] = matix[k][j] / matix[k][k];
        matix[k][k] = 1.0;
        for (int i = k + 1; i < n; i++)
        {
            float temp[4] __attribute__((aligned(16))) = { matix[i][k], matix[i][k], matix[i][k], matix[i][k] };
            m1 = _mm_load_ps(temp);
            int j = k + 1;
            for (j; j % 4 != 0; j++)
                matix[i][j] = matix[i][j] - matix[i][k] * matix[k][j];
            for (j; j < n - 3; j += 4)
            {
                m2 = _mm_load_ps(matix[i] + j);
                m3 = _mm_load_ps(matix[k] + j);
                m3 = _mm_mul_ps(m1, m3);
                m2 = _mm_sub_ps(m2, m3);
                _mm_store_ps(matix[i] + j, m2);
            }
            for (j; j < n; j++)
                matix[i][j] = matix[i][j] - matix[i][k] * matix[k][j];
            matix[i][k] = 0;
        }
    }
}
void timestart()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
}
void timestop()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);	// end time
    cout << ((tail - head) * 1000.0 / freq) / 1000 << endl;


}
int main()
{
    setmatix(100);
    timestart();
    sse_lu_aligned(100);
    timestop();

    setmatix(300);
    timestart();
    sse_lu_aligned(300);
    timestop();

    setmatix(600);
    timestart();
    sse_lu_aligned(600);
    timestop();

    setmatix(1024);
    timestart();
    sse_lu_aligned(1024);
    timestop();

    setmatix(1200);
    timestart();
    sse_lu_aligned(1200);
    timestop();

    setmatix(1400);
    timestart();
    sse_lu_aligned(1400);
    timestop();

    setmatix(1600);
    timestart();
    sse_lu_aligned(1600);
    timestop();

    setmatix(1800);
    timestart();
    sse_lu_aligned(1800);
    timestop();

    setmatix(2000);
    timestart();
    sse_lu_aligned(2000);
    timestop();

    setmatix(2024);
    timestart();
    sse_lu_aligned(2024);
    timestop();
    return 0;
}
