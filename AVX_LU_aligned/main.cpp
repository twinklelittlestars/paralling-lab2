#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <immintrin.h> //AVX��AVX2��AVX-512
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
void avx_lu_aligned(int n)
{
    __m256 m1, m2, m3;
    for (int k = 0; k < n; k++)
    {
        for (int j = k + 1; j < n; j++)
            matix[k][j] = matix[k][j] / matix[k][k];
        matix[k][k] = 1.0;
        for (int i = k + 1; i < n; i++)
        {
            int j = k + 1;
            for (j; j % 8 != 0; j++)
                matix[i][j] = matix[i][j] - matix[i][k] * matix[k][j];
            float temp[8] = { matix[i][k], matix[i][k], matix[i][k], matix[i][k], matix[i][k], matix[i][k], matix[i][k], matix[i][k] };
            m1 = _mm256_load_ps(temp);
            for (j; j < n - 7; j += 8)
            {
                m2 = _mm256_load_ps(matix[i] + j);
                m3 = _mm256_load_ps(matix[k] + j);
                m3 = _mm256_mul_ps(m1, m3);
                m2 = _mm256_sub_ps(m2, m3);
                _mm256_store_ps(matix[i] + j, m2);
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
    avx_lu_aligned(100);
    timestop();

    setmatix(300);
    timestart();
    avx_lu_aligned(300);
    timestop();

    setmatix(600);
    timestart();
    avx_lu_aligned(600);
    timestop();

    setmatix(1024);
    timestart();
    avx_lu_aligned(1024);
    timestop();

    setmatix(1200);
    timestart();
    avx_lu_aligned(1200);
    timestop();

    setmatix(1400);
    timestart();
    avx_lu_aligned(1400);
    timestop();

    setmatix(1600);
    timestart();
    avx_lu_aligned(1600);
    timestop();

    setmatix(1800);
    timestart();
    avx_lu_aligned(1800);
    timestop();

    setmatix(2000);
    timestart();
    avx_lu_aligned(2000);
    timestop();

    setmatix(2024);
    timestart();
    avx_lu_aligned(2024);
    timestop();
    return 0;
}
