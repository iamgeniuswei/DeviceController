#include <iostream>
#include "atccs_define.h"
#include "atccs.h"
using namespace ATCCS;
#include <ctime>



	void controlDeviceGimbal(std::tr1::shared_ptr<ATCCSData> data)
	{
		if(data)
			std::cout << "control device\n" << std::endl;
	}


int main()
{
	std::cout << "------------------------ATCCS ָ�����ϵͳ----------------------" << std::endl;
	std::cout << "Hello World" << std::endl;


	//���ղ���ʾ������

	//ATCCSDataReceiver *reciver = new ATCCSDataReceiver;
	//reciver->setRecvAddress("192.168.0.105", 4747);
	//reciver->start();


	//ATCCSDataHandler *dataHandler = new ATCCSDataHandler();


	//ATCCSDeviceController *ccd = new ATCCSDeviceController;
	//ccd->setDeviceController(controlDeviceGimbal);
	//ccd->start();

	//dataHandler->registerDeviceController(GIMBAL, ccd);

	//dataHandler->start();

	//std::cin.get();
	

	//ATCCSDataHandler *dataHandler = new ATCCSDataHandler();


	//ATCCSDeviceController *ccd = new ATCCSDeviceController;
	//ccd->setDeviceController(controlDeviceGimbal);
	//ccd->start();

	//dataHandler->registerDeviceController(GIMBAL, ccd);

	//dataHandler->start();





	//���²��־�Ϊʾ�����룬�����Ҫ������ѡ��ע�͵��������ִ���
	
	//һ���������Ͳ���ʾ������
	//������Ϣ����ִ������һ�д��룬�����ĸ�������
	//1) ������IP
	//2��������port
	//3) ��Զ������
	//4���豸����

	//ATCCSHeartbeatReport *hbReport = new ATCCSHeartbeatReport("192.168.0.115", 4747,AT60, GIMBAL);

	//std::cin.get();

	//�����豸״̬���Ͳ���ʾ������
	//ִ���������д��룬����������������Ҫ�Լ���д����״̬���ݵ�setData������
	//1) ������IP
	//2��������port

	//GimbalStatusReport *gimbal = new GimbalStatusReport;
	//gimbal->setRemoteAddress("192.168.0.115", 4747);
	//for(int i=0; i<100; i++)
	//{
	//	gimbal->report();
	//	Sleep(1000);
	//}



	//����ָ��ִ�н�����Ͳ���ʾ������

	//ATCCSInstructionResultReport *result = new ATCCSInstructionResultReport;
	//result->setRemoteAddress("192.168.2.120", 4747);
	//result->setInstructionResult(0, AT216, CCD, 10000, _AT_GIMBAL_CONNECT, 1);
	//result->report();


	//ATCCSUdpSocket *so = new ATCCSUdpSocket;
	//so->setTargetAddress("192.168.0.115", 4747);
	//_ATCCSPHeader header;
	//memset(&header, 0, sizeof(_ATCCSPHeader));
	//header.magic = MAGIC;
	//header.version = VERSION;
	//header.sequence = 1;
	//header.length = sizeof(_ATCCSPHeader) + sizeof(_AT_PLAN);
	//header.AT.at = AT60;
	//header.AT.device = 0;
	//header.msg = ATPLAN;

	//_AT_PLAN plan;
	//memset(&plan, 0, sizeof(_AT_PLAN));
	//plan.at = AT60;
	//memcpy(&plan.user, "admin", 48);
	//memcpy(&plan.project, "project", 48);
	//memcpy(&plan.target, "genius", 48);
	//plan.type = 0;
	//plan.rightAscension = 100.111;
	//plan.declination = 200.222;
	//plan.epoch = 1;
	//plan.exposureTime = 60;
	//plan.delayTime = 10;
	//plan.exposureCount = 4;
	//memcpy(&plan.filter, "b", 8);
	//char *data = new char[header.length];
	//memcpy(data, &header, sizeof(_ATCCSPHeader));
	//memcpy(data+sizeof(_ATCCSPHeader), &plan, sizeof(_AT_PLAN));
	//std::cout << so->sendData(data, header.length) << std::endl;




	//�ġ�ָ��ʹ���


	ATCCSUdpSocket *so = new ATCCSUdpSocket;
	so->setTargetAddress("192.168.0.115", 4747);
	static int seq = 1;
	_ATCCSPHeader header;
	header.magic = MAGIC;
	header.version = VERSION;
	header.sequence = seq++;

	////ע������ʱ���޸�������������
	header.AT.at = AT60;
	header.AT.device = GIMBAL;

	header.msg = ATINSTRUCTION;

	_AT_INSTRUCTION_HEADER _inHeader;
	memset(&_inHeader, 0, sizeof(_AT_INSTRUCTION_HEADER));
	//
	////ע������ʱ���޸�������������
	_inHeader.at = AT60;
	_inHeader.device = GIMBAL;
	_inHeader.operation = _GIMBAL_INSTRUCTION_FINDHOME;

	////����ָ���ʱ����������ṹ����ݾ���ָ��������޸�
	_AT_GIMBAL_PARAM_FINDHOME fh;
	fh.axis = 100;

	////���췢������
	////����ָ�������޸ĵ������ṹ��
	unsigned int _size = sizeof(_ATCCSPHeader) + sizeof(_AT_INSTRUCTION_HEADER) + sizeof(_AT_GIMBAL_PARAM_FINDHOME);
	header.length = _size;
	char *data = new char[_size];
	memcpy(data, &header, sizeof(_ATCCSPHeader));
	memcpy(data+sizeof(_ATCCSPHeader), &_inHeader, sizeof(_AT_INSTRUCTION_HEADER));

	////����ָ�������޸���Ҫ�����Ľṹ��
	memcpy(data+sizeof(_ATCCSPHeader)+sizeof(_AT_INSTRUCTION_HEADER), &fh, sizeof(_AT_GIMBAL_PARAM_FINDHOME));
	for(int i=0; i<2000;i++)
	{
		std::cout << i << "--";
		std::cout << so->sendData(data, _size);
		std::cout << std::endl;
		//Sleep(1000);
	}
	delete [] data;






	//while(true)
	//{
	//	std::cout << so->sendData(data, sizeof(_ATCCSPHeader) + sizeof(_AT_GIMBAL_FH_PARAM)) << std::endl;
	//	Sleep(2000);
	//}
	//





	//ATCCSDataHandler *dataHandler = new ATCCSDataHandler();


	//ATCCSDeviceController *ccd = new ATCCSDeviceController;
	//ccd->setDeviceController(nullptr);
	//ccd->start();

	//dataHandler->registerDeviceController(CCD, ccd);

	//dataHandler->start();


	//ATCCSSetting *set = ATCCSSetting::instance();
	//ATCCSDataQueue *queue = ATCCSDataQueue::instance();
	//std::cout << "Test End" << std::endl;

	//std::shared_ptr<ATCCSData> data (new ATCCSData(100));
	//std::shared_ptr<ATCCSData> data1 (new ATCCSData(100));
	//std::shared_ptr<ATCCSData> data2 (new ATCCSData(100));
	//queue->push(data);
	//queue->push(data1);
	//queue->push(data2);




	////������Ϣ������ɣ�ֱ����������һ�д��뼴�ɣ�ע���м����豸���м����������������ĸ�����
	//ATCCSHeartbeatReport *hbReport = new ATCCSHeartbeatReport("192.168.2.120", 4747,AT216, GIMBAL);
	//ATCCSHeartbeatReport *hbReport1 = new ATCCSHeartbeatReport("192.168.2.120", 4747,AT216, CCD);
	
	//����״̬��Ϣʾ�����̳�ATCCSReport������ʵ��setData()��Ա����
	//��setData������������豸��״̬��Ϣ��Ȼ�����Report��������״̬���ݷ��ͳ�ȥ
	//��ת̨״̬����Ϊ����
	//�ڱ����У��Ѿ��̳�ʵ����ת̨��״̬�����ϱ��ࣺ GimbalStatusReport;����ϸ��setData�ĺ�����д��
	//���÷���������Ҫ����״̬�ĵط���
	//����һ������
//	GimbalStatusReport *gimbal = new GimbalStatusReport;
//	//�������ݽ��յ�ַ
//	gimbal->setRemoteAddress("192.168.2.115", 4747);
//	//��������
//
//		ATCCSInstructionResultReport *result = new ATCCSInstructionResultReport;
//	result->setRemoteAddress("192.168.2.115", 4747);
//	result->setInstructionResult(0, AT216, CCD, 10000, _AT_GIMBAL_CONNECT, 1);
//	int i = 0;
//	time_t now = time(NULL);
//	while(i <30000)
//	{
//		gimbal->report();
////		std::cout << result->report() << std::endl;
//		//Sleep(1);
//		i++;
//	}
//	time_t now_new = time(NULL);
//	std::cout << (now_new - now) << std::endl;



	////����ָ��ִ�н������ʾ��
	////����һ������
	//ATCCSInstructionResultReport *result = new ATCCSInstructionResultReport;
	//result->setRemoteAddress("192.168.2.115", 4747);
	//result->setInstructionResult(0, AT216, GIMBAL, 10000, _AT_GIMBAL_CONNECT, 1);
	//result->report();

	////����ָ������ʾ��
	////dataΪ���յ�������
	//char *data;
	////�Ƚ���ָ���ͷ��
	//_AT_INSTRUCTION_HEADER *header = (_AT_INSTRUCTION_HEADER*)(data);
	////����header->operation ���жϺ����������ʲôָ��Ĳ������ٶ�Ϊ_AT_GIMBAL_FINDHOME
	//_AT_GIMBAL_FINDHOME *fh = (_AT_GIMBAL_FINDHOME*)(data+sizeof(_AT_INSTRUCTION_HEADER));
	////��ʱ�����������ָ��Ĳ�����Ϣ������fh->axis���豸��ִ������ָ��





	std::cin.get();
	//hbReport->setStop(true);
	//hbReport1->setStop(true);
	//hbReport->join();
	//hbReport1->join();

	return 0;
}