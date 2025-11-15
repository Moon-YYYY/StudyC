#include<iostream>
using namespace std;
//ÄÚ´æ²¼¾Ö
union dataU
{
	int i;
	//double d;
	char s[7];
};
int main()
{
	cout << sizeof(dataU) << endl;
	dataU dp;
	dp.s[0] = 255;
	dp.s[1] = 1;
	dp.s[2] = 0;
	dp.s[3] = 0;
	cout << dp.i << endl;
	dp.i = 256;//00000000 00000000 00000001 11111111
	////////////////s[3]    s[2]     s[1]      s[0]
		//cout << dp.d << endl;
		cout << (int)dp.s[0] << ' ' << (int)dp.s[1] << ' ' << (int)dp.s[2] << ' ' << (int)dp.s[3] << endl;//0100
	return 0;
}
