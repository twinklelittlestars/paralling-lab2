#include<iostream>
#include<fstream>
#include<sstream>
#include<stdio.h>
#include<time.h>
#include<stdio.h>
#include<time.h>
#include<arm_neon.h>
struct timespec sts, ets;
const int col = 4000, row = 60000;
using namespace std;
int n, eliminated_row;
struct bitstorage {
	unsigned int bits_array[col];//�洢�����Ƽ����е�ʵ������
	int big_value = -1;//��¼�����Ƽ����е������ֵ
	//����ʵ�ַ�ʽ�ǽ����� num ת��Ϊ�����ƺ󣬽���Ӧ��λ�ϵ�ֵ����Ϊ 1��
	void addNum(int num) {//������bitset�����һ������
		int n1 = num / 32, n2 = num % 32;//n1 �� n2 �ֱ��ʾ num ��Ӧ���޷����������� bits �е������Ͷ�Ӧ�Ķ�����λ��
		bits_array[n1] += (0x80000000 >> n2);//
		big_value = max(big_value, num);
		n = max(n, n1 + 1);//��¼�ü�����ʹ�õ�����������λ��
	}
};

void print(bitstorage& b, ofstream& of) {//�� bitset �е�����������ļ���
	if (b.big_value == -1) {
		of << endl;
		return;
	}
	for (int i = n; i >= 0; --i) {
		for (unsigned int temp = 1, j = 31; temp != 0; temp <<= 1, --j)
			if (temp & b.bits_array[i])
				of << i * 32 + j << " ";
	}
	of << endl;
}

void Xor(bitstorage& b1, bitstorage& b2) {//�������ڽ����� bitset ȡ��������ʵ�ַ����Ǳ���ÿһλ�������������
	for (int i = n; i >= 0; --i) {
		b1.bits_array[i] ^= b2.bits_array[i];
	}
	int i = n, j = 31;
	while (b1.bits_array[i] == 0 && i >= 0)--i;
	if (i < 0) {
		b1.big_value = -1;
		return;
	}
	unsigned temp = 1;
	while ((b1.bits_array[i] & temp) == 0)temp <<= 1, --j;
	b1.big_value = 32 * i + j;
}
bitstorage consumption[row], eliminated[row];//RΪ��Ԫ�ӣ�EΪ����Ԫ��
void special_normal_arm() {
	for (int i = 0; i < eliminated_row; ++i) {
		while (eliminated[i].big_value != -1) {//�жϱ���Ԫ���Ƿ�ȫ0
			if (consumption[eliminated[i].big_value].big_value != -1) {//�ж���Ԫ���Ƿ��������Ԫ��Ӧ�еı���Ԫ�е���
				Xor(eliminated[i], consumption[eliminated[i].big_value]);//���������н������
			}

			else {
				consumption[eliminated[i].big_value] = eliminated[i];//����������һ�еı���Ԫ�д������Ԫ����
				break;
			}
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
void readData() {//�������ڶ�ȡ�������ݣ����ļ��ж�ȡ���洢�� bitset ��
	fstream fs1("E:\\���г���\\data\\Groebner\\��������1 ��������130��������Ԫ��22������Ԫ��8\\��Ԫ��.txt");
	string line;
	while (getline(fs1, line)) {
		stringstream ss(line);
		int index, num;
		ss >> index;
		consumption[index].addNum(index);
		while (ss >> num)
			consumption[index].addNum(num);
	}
	fs1.close();
	fs1.open("E:\\���г���\\data\\Groebner\\��������1 ��������130��������Ԫ��22������Ԫ��8\\����Ԫ��.txt");
	while (getline(fs1, line)) {
		stringstream ss(line);
		int num;
		while (ss >> num)
			eliminated[eliminated_row].addNum(num);
		++eliminated_row;
	}
	fs1.close();
}
int main() {
	readData();
	timestart();
	special_normal_arm();
	timestop();

	ofstream of("E:\\���г���\\data\\Groebner\\��������1 ��������130��������Ԫ��22������Ԫ��8\\output.txt");
	for (int i = 0; i < eliminated_row; ++i)
		print(eliminated[i], of);
	of.close();
}
