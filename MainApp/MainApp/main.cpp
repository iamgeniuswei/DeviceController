#include <iostream>
using namespace std;
#include "atccs_define.h"
#include "atccs.h"
using namespace ATCCS;
int main()
{
	//仔细阅读示例，示例中会一步步的说明在实际应用中需要修改的部分。


	//说明，客户程序需要做的是，模仿ASCRealtimeDataReport实现自己的环境监测数据上报类

	//ASC实时监测数据上报示例，
	//需要根据实际情况，修改ASCRealtimeDataReport::setData()
	//第一步，声明对象示例
	ASCRealtimeDataReport report;
	//第二步，设置远程接收地址
	report.setRemoteAddress("192.168.2.115", 4747);
	for(int i=0; i<20; i++)
	{
		//第三步，发送出去
		cout << report.report() << endl;
		Sleep(2000);
	}
	return 0;
}