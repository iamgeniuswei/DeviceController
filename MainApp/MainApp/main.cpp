#include <iostream>
using namespace std;
#include "atccs_define.h"
#include "atccs.h"
using namespace ATCCS;
int main()
{
	//��ϸ�Ķ�ʾ����ʾ���л�һ������˵����ʵ��Ӧ������Ҫ�޸ĵĲ��֡�


	//˵�����ͻ�������Ҫ�����ǣ�ģ��ASCRealtimeDataReportʵ���Լ��Ļ�����������ϱ���

	//ASCʵʱ��������ϱ�ʾ����
	//��Ҫ����ʵ��������޸�ASCRealtimeDataReport::setData()
	//��һ������������ʾ��
	ASCRealtimeDataReport report;
	//�ڶ���������Զ�̽��յ�ַ
	report.setRemoteAddress("192.168.2.115", 4747);
	for(int i=0; i<20; i++)
	{
		//�����������ͳ�ȥ
		cout << report.report() << endl;
		Sleep(2000);
	}
	return 0;
}