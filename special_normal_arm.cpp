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
	unsigned int bits_array[col];//存储二进制集合中的实际内容
	int big_value = -1;//记录二进制集合中的最大数值
	//具体实现方式是将整数 num 转换为二进制后，将对应的位上的值设置为 1。
	void addNum(int num) {//用于向bitset中添加一个整数
		int n1 = num / 32, n2 = num % 32;//n1 和 n2 分别表示 num 对应的无符号整型数组 bits 中的索引和对应的二进制位数
		bits_array[n1] += (0x80000000 >> n2);//
		big_value = max(big_value, num);
		n = max(n, n1 + 1);//记录该集合中使用到的最大的整数位数
	}
};

void print(bitstorage& b, ofstream& of) {//将 bitset 中的内容输出到文件中
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

void Xor(bitstorage& b1, bitstorage& b2) {//函数用于将两个 bitset 取异或操作，实现方法是遍历每一位并进行异或运算
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
bitstorage consumption[row], eliminated[row];//R为消元子，E为被消元子
void special_normal_arm() {
	for (int i = 0; i < eliminated_row; ++i) {
		while (eliminated[i].big_value != -1) {//判断被消元子是否全0
			if (consumption[eliminated[i].big_value].big_value != -1) {//判断消元子是否存在能消元对应行的被消元行的行
				Xor(eliminated[i], consumption[eliminated[i].big_value]);//若存在两行进行异或
			}

			else {
				consumption[eliminated[i].big_value] = eliminated[i];//不存在则将这一行的被消元行存放如消元行中
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
void readData() {//函数用于读取输入数据，从文件中读取并存储到 bitset 中
	fstream fs1("E:\\并行程序\\data\\Groebner\\测试样例1 矩阵列数130，非零消元子22，被消元行8\\消元子.txt");
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
	fs1.open("E:\\并行程序\\data\\Groebner\\测试样例1 矩阵列数130，非零消元子22，被消元行8\\被消元行.txt");
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

	ofstream of("E:\\并行程序\\data\\Groebner\\测试样例1 矩阵列数130，非零消元子22，被消元行8\\output.txt");
	for (int i = 0; i < eliminated_row; ++i)
		print(eliminated[i], of);
	of.close();
}
