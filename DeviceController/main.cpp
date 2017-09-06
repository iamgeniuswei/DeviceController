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
	std::cout << "------------------------ATCCS 指令测试系统----------------------" << std::endl;
	std::cout << "Hello World" << std::endl;


	//接收部分示例代码

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





	//以下部分均为示例代码，如果需要，可以选择注释掉其他部分代码
	
	//一、心跳发送部分示例代码
	//心跳信息仅需执行以下一行代码，设置四个参数：
	//1) 服务器IP
	//2）服务器port
	//3) 望远镜类型
	//4）设备类型

	//ATCCSHeartbeatReport *hbReport = new ATCCSHeartbeatReport("192.168.0.115", 4747,AT60, GIMBAL);

	//std::cin.get();

	//二、设备状态发送部分示例代码
	//执行以下三行代码，设置两个参数：（要自己编写构造状态数据的setData函数）
	//1) 服务器IP
	//2）服务器port

	//GimbalStatusReport *gimbal = new GimbalStatusReport;
	//gimbal->setRemoteAddress("192.168.0.115", 4747);
	//for(int i=0; i<100; i++)
	//{
	//	gimbal->report();
	//	Sleep(1000);
	//}



	//三、指令执行结果发送部分示例代码

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




	//四、指令发送代码


	ATCCSUdpSocket *so = new ATCCSUdpSocket;
	so->setTargetAddress("192.168.0.115", 4747);
	static int seq = 1;
	_ATCCSPHeader header;
	header.magic = MAGIC;
	header.version = VERSION;
	header.sequence = seq++;

	////注，测试时，修改下面两个参数
	header.AT.at = AT60;
	header.AT.device = GIMBAL;

	header.msg = ATINSTRUCTION;

	_AT_INSTRUCTION_HEADER _inHeader;
	memset(&_inHeader, 0, sizeof(_AT_INSTRUCTION_HEADER));
	//
	////注，测试时，修改下面三个参数
	_inHeader.at = AT60;
	_inHeader.device = GIMBAL;
	_inHeader.operation = _GIMBAL_INSTRUCTION_FINDHOME;

	////测试指令发送时，下面这个结构体根据具体指令的类型修改
	_AT_GIMBAL_PARAM_FINDHOME fh;
	fh.axis = 100;

	////构造发送数据
	////根据指令类型修改第三个结构体
	unsigned int _size = sizeof(_ATCCSPHeader) + sizeof(_AT_INSTRUCTION_HEADER) + sizeof(_AT_GIMBAL_PARAM_FINDHOME);
	header.length = _size;
	char *data = new char[_size];
	memcpy(data, &header, sizeof(_ATCCSPHeader));
	memcpy(data+sizeof(_ATCCSPHeader), &_inHeader, sizeof(_AT_INSTRUCTION_HEADER));

	////根据指令类型修改需要拷贝的结构体
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




	////心跳信息调试完成，直接运行以下一行代码即可，注，有几个设备运行几个！！！，设置四个参数
	//ATCCSHeartbeatReport *hbReport = new ATCCSHeartbeatReport("192.168.2.120", 4747,AT216, GIMBAL);
	//ATCCSHeartbeatReport *hbReport1 = new ATCCSHeartbeatReport("192.168.2.120", 4747,AT216, CCD);
	
	//发送状态信息示例，继承ATCCSReport，重新实现setData()成员函数
	//在setData（）函数填充设备的状态信息，然后调用Report方法，将状态数据发送出去
	//以转台状态数据为例：
	//在本例中，已经继承实现了转台的状态数据上报类： GimbalStatusReport;（详细看setData的函数编写）
	//调用方法，在需要发送状态的地方：
	//创建一个对象
//	GimbalStatusReport *gimbal = new GimbalStatusReport;
//	//设置数据接收地址
//	gimbal->setRemoteAddress("192.168.2.115", 4747);
//	//发送数据
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



	////发送指令执行结果数据示例
	////创建一个对象
	//ATCCSInstructionResultReport *result = new ATCCSInstructionResultReport;
	//result->setRemoteAddress("192.168.2.115", 4747);
	//result->setInstructionResult(0, AT216, GIMBAL, 10000, _AT_GIMBAL_CONNECT, 1);
	//result->report();

	////解析指令数据示例
	////data为接收到的数据
	//char *data;
	////先解析指令公共头部
	//_AT_INSTRUCTION_HEADER *header = (_AT_INSTRUCTION_HEADER*)(data);
	////根据header->operation 来判断后面的数据是什么指令的参数，假定为_AT_GIMBAL_FINDHOME
	//_AT_GIMBAL_FINDHOME *fh = (_AT_GIMBAL_FINDHOME*)(data+sizeof(_AT_INSTRUCTION_HEADER));
	////此时，获得了找零指令的参数信息，根据fh->axis在设备上执行找零指令





	std::cin.get();
	//hbReport->setStop(true);
	//hbReport1->setStop(true);
	//hbReport->join();
	//hbReport1->join();

	return 0;
}