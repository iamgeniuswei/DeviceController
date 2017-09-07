#include "atccs.h"
#include <new>
#include <iostream>
#include <ctime>
#include "em_define.h"
namespace ATCCS
{
	//class: ATCCSData
	unsigned int ATCCSData::_maxSize = 4097;
	ATCCSData::ATCCSData(void):_data(nullptr), _size(0)
	{
	}

	ATCCSData::ATCCSData(unsigned int size) : _data(nullptr), _size(size)
	{
		if(size < _maxSize)
		{
			_data = new (std::nothrow) char [size];
			memset(_data, 0, size);
		}
		else
			_size = 0;
	}


	ATCCSData::~ATCCSData(void)
	{
		if(_data != nullptr && _size > 0)
		{
			std::cout << "~ATCCSData" << std::endl;
			delete [] _data;
			_data = nullptr;
			_size = 0;
		}
	}

	char* ATCCSData::data()
	{
		return _data;
	}

	bool ATCCSData::validate() const
	{
		if(_data == nullptr)
		{
			std::cerr << "ATCCSData: _data is nullptr\n";
			return false;
		}

		if(_size < sizeof(_ATCCSPHeader))
		{
			std::cerr << "ATCCSData: data size error #1\n";
			return false;
		}
		_ATCCSPHeader *header = (_ATCCSPHeader*)(_data);
		if(header->length != _size)
		{
			std::cerr << "ATCCSData: data size erro #2\n";
			return false;
		}
		return true;
	}


	/*ATCCSSingleton *ATCCSSingleton::_instance = nullptr;
	ATCCSSingleton::ATCCSSingletonGarbo ATCCSSingleton::singleGarbo;
	ATCCSSingleton::ATCCSSingleton(void)
	{
	}


	ATCCSSingleton::~ATCCSSingleton(void)
	{
		std::cout << "singleton" << std::endl;
	}*/

	ATCCSThread::ATCCSThread(void):_stop(false),_hThread(nullptr),_threadID(0)
	{
		InitializeCriticalSection(&_cs);
	}


	ATCCSThread::~ATCCSThread(void)
	{
		DeleteCriticalSection(&_cs);
	}

	void ATCCSThread::cleaner(void *pHandle)
	{
		if(pHandle != nullptr)
		{
			ATCCSThread *p = (ATCCSThread*)pHandle;
			delete p;
			p = nullptr;
		}
	}

	void ATCCSThread::start()
	{
#ifdef WIN32
		_hThread = (HANDLE)_beginthreadex(NULL, 0, worker, this, 0, &_threadID);
#endif
	}

	void ATCCSThread::join()
	{
#ifdef WIN32
		if(_hThread != nullptr)
			WaitForSingleObject(_hThread, INFINITE);
#endif
	}

	unsigned int _stdcall worker(void *pHandle)
	{
		if(pHandle == nullptr)
			return -1;
		ATCCSThread *pThread = static_cast<ATCCSThread*>(pHandle);
		if(pThread == nullptr)
			return -1;
		pThread->run();
		return 0;
	}

	ATCCSDataHandler::ATCCSDataHandler(void)
	{
		_dataQueue = ATCCSDataQueue::instance();
	}


	ATCCSDataHandler::~ATCCSDataHandler(void)
	{
	}

	void ATCCSDataHandler::run()
	{
		while(!stop())
		{		
			std::shared_ptr<ATCCSData> data = _dataQueue->wait_and_pop();
			dispatchControlData(data);
			Sleep(1000);
		}
	}

	void ATCCSDataHandler::registerDeviceController(unsigned int id, ATCCSDeviceController *controller)
	{
		if(controller == nullptr)
		{
			std::cout << "DeviceController is nullptr" << std::endl;
			return;
		}
		auto search = _controllers.find(id);
		if(search != _controllers.end())
		{
			std::cout << "DeviceController is already registered" << std::endl;
			return;
		}
		_controllers.insert(std::pair<unsigned int, ATCCSDeviceController*>(id, controller));
	}

	ATCCSDeviceController *ATCCSDataHandler::controller(unsigned int id)
	{
		auto search = _controllers.find(id);
		if(search == _controllers.end())
		{
			std::cout << "no such DeviceController" << std::endl;
			return nullptr;
		}

		ATCCSDeviceController *dController = search->second;
		return dController;
	}



	void ATCCSDataHandler::dispatchControlData(std::shared_ptr<ATCCSData> data)
	{
		if(data == nullptr || !(data->validate()))
			return;
		_ATCCSPHeader *header = (_ATCCSPHeader*)(data->data());
		if(header)
		{
			ATCCSDeviceController *temp = controller(header->AT.device);
			if(temp)
			{
				temp->pushControlData(data);
			}
		}
	}




	ATCCSDataQueue *ATCCSDataQueue::_instance = nullptr;
	ATCCSDataQueue::ATCCSDataQueueGarbo ATCCSDataQueue::singleGarbo;
	ATCCSDataQueue::ATCCSDataQueue(void)
	{
		//#ifdef WIN32
		//	InitializeCriticalSection(&_cs);
		//#endif
	}


	ATCCSDataQueue::~ATCCSDataQueue(void)
	{

		std::cout << "~ATCCSDataQueue\n";
		//#ifdef WIN32
		//	DeleteCriticalSection(&_cs);
		//#endif
	}

	ATCCSDataQueue* ATCCSDataQueue::instance()
	{
		if(_instance == nullptr)
		{
			if(_instance == nullptr)
			{
				_instance = new (std::nothrow) ATCCSDataQueue;
			}
		}
		return _instance;
	}

	ATCCSDataReceiver::ATCCSDataReceiver(void):ATCCSThread(), _socket(nullptr), _dataQueue(nullptr)
	{
		_socket = new (std::nothrow) ATCCSUdpSocket;
		_dataQueue = ATCCSDataQueue::instance();
	}


	ATCCSDataReceiver::~ATCCSDataReceiver(void)
	{
		if(_socket != nullptr)
		{
			delete _socket;
			_socket = nullptr;
		}
	}

	int ATCCSDataReceiver::setRecvAddress(const std::string &ip, unsigned short port)
	{
		int ret = -1;
		if(_socket != nullptr)
		{
			ret = _socket->setHostAddress(ip, port);
		}
		return ret;
	}

	void ATCCSDataReceiver::run()
	{
		char *data = new char[4096];
		while (!_stop && _socket) 
		{
			ssize_t ret = _socket->recvData(data, 4096);
			if(ret > 0)
			{
				std::shared_ptr<ATCCSData> newdata(new (std::nothrow) ATCCSData(ret));
				if(nullptr != newdata && nullptr != newdata->data())
				{
					memcpy(newdata->data(), data, ret);
					_dataQueue->push(newdata);
					std::cout << _dataQueue->size()<< std::endl;
				}
			}
		}
		delete [] data;
		std::cout << "thread is over" << std::endl;
	}

	ATCCSDeviceController::ATCCSDeviceController(void):_controller(nullptr)
	{
	}


	ATCCSDeviceController::~ATCCSDeviceController(void)
	{
	}

	void ATCCSDeviceController::run()
	{
		while(!stop())
		{
			std::shared_ptr<ATCCSData> data = _fifoQueue.wait_and_pop();
			if(data)
				_controller(data);
			Sleep(2000);
		}
	}





	unsigned int ATCCSHeartbeatReport::sequence = 0;
	ATCCSHeartbeatReport::ATCCSHeartbeatReport(void)
	{
		_size = sizeof(_ATCCSPHeader);
		try
		{
			_data = new char[_size];
		}
		catch(std::bad_alloc &e)
		{
			_size = 0;
			_data = nullptr;
		}
	}
	ATCCSHeartbeatReport::ATCCSHeartbeatReport(const std::string &ip, unsigned short port, unsigned short at, unsigned short device)
	{
		_size = sizeof(_ATCCSPHeader);
		try
		{
			_data = new char[_size];
		}
		catch(std::bad_alloc &e)
		{
			_size = 0;
			_data = nullptr;
		}
		setRemoteAddress(ip, port);
		setATDevice(at, device);
		start();
	}


	ATCCSHeartbeatReport::~ATCCSHeartbeatReport(void)
	{
	}

	void ATCCSHeartbeatReport::setData()
	{
		if(_data && _size > 0)
		{
			memset(_data, 0 ,_size);
			setProtocolHeader(ATHEARTBEAT, _size, sequence++, _at, _device);
			memcpy(_data, &header, _size);
		}
	}

	void ATCCSHeartbeatReport::run()
	{
		while(!stop())
		{
			std::cout << report() << std::endl;
			Sleep(1000);
		}
	}







	ATCCSInstructionResultReport::ATCCSInstructionResultReport(void) : ATCCSReport()
	{	
		memset(&_instruction, 0, sizeof(_AT_INSTRUCTION_RESULT));
		try
		{
			_size = sizeof(_ATCCSPHeader) + sizeof(_AT_INSTRUCTION_RESULT);
			_data = new char [_size];
		}
		catch(std::bad_alloc)
		{
			_size = 0;
			_data = nullptr;
		}
	}

	ATCCSInstructionResultReport::ATCCSInstructionResultReport(unsigned int plan, unsigned short at, unsigned short device, unsigned int sequence, unsigned int operation,unsigned int timeout, unsigned int result)
		:ATCCSReport()
	{
		memset(&_instruction, 0, sizeof(_AT_INSTRUCTION_RESULT));
		fillStruct(plan, at, device, sequence, operation, timeout, result);
		try
		{
			_size = sizeof(_ATCCSPHeader) + sizeof(_AT_INSTRUCTION_RESULT);
			_data = new char [_size];
		}
		catch(std::bad_alloc)
		{
			_size = 0;
			_data = nullptr;
		}
		
	}

	void ATCCSInstructionResultReport::setInstructionResult(unsigned int plan, unsigned short at, unsigned short device, unsigned int sequence, unsigned int operation,unsigned int timeout, unsigned int result)
	{
		fillStruct(plan, at, device, sequence, operation,timeout, result);
	}

	void ATCCSInstructionResultReport::setData()
	{
		setProtocolHeader(ATINSTRUCTIONACK, _size, _instruction.sequence, _instruction.at, _instruction.device);
		memcpy(_data, &header, sizeof(_ATCCSPHeader));
		memcpy(_data+sizeof(_ATCCSPHeader), &_instruction, sizeof(_AT_INSTRUCTION_RESULT));
	}

	void ATCCSInstructionResultReport::fillStruct(unsigned int plan, unsigned short at, unsigned short device, unsigned int sequence, unsigned int operation,unsigned int timeout, unsigned int result)
	{
		_instruction.plan = plan;
		_instruction.at = at;
		_instruction.device = device;
		_instruction.sequence = sequence;
		_instruction.operation = operation;
		_instruction.timeout = timeout;
		_instruction.result = result;
	}


	ATCCSInstructionResultReport::~ATCCSInstructionResultReport(void)
	{
	}

	ATCCSReport::ATCCSReport(void):_socket(nullptr),_data(nullptr),_size(0)
	{
		_at = 0;
		_device = 0;
		_socket = new (std::nothrow) ATCCSUdpSocket;
	}


	ATCCSReport::~ATCCSReport(void)
	{
		if(_socket != nullptr)
		{
			delete _socket;
			_socket = nullptr;
		}
		if(_data != nullptr)
		{
			delete [] _data;
			_data = nullptr;
		}
	}

	void ATCCSReport::setRemoteAddress(const std::string &ip, unsigned short port)
	{
		if(_socket != nullptr)
		{
			_socket->setTargetAddress(ip, port);
		}
	}

	unsigned int ATCCSReport::report()
	{
		setData();
		if(_socket != nullptr && _data != nullptr && _size > 0)
		{
			return _socket->sendData(_data, _size);
		}
		return 0;
	}

	void ATCCSReport::setProtocolHeader(unsigned int msg, unsigned int length, unsigned int sequence, unsigned short at, unsigned short device)
	{	
		memset(&header, 0, sizeof(_ATCCSPHeader));
		header.magic = MAGIC;
		header.version = VERSION;
		header.msg = msg;
		header.length = length;
		header.sequence = sequence;
		header.AT.at = at;
		header.AT.device = device;
		time_t now_time;  
		now_time = time(NULL);
		header.tv_sec = now_time;
	}

	void ATCCSReport::setProtocolHeader(unsigned int msg, unsigned int length, unsigned int sequence, unsigned int em)
	{
		memset(&header, 0, sizeof(_ATCCSPHeader));
		header.magic = MAGIC;
		header.version = VERSION;
		header.msg = msg;
		header.length = length;
		header.sequence = sequence;
		header.env = em;
		time_t now_time;  
		now_time = time(NULL);
		header.tv_sec = now_time;
	}



	ATCCSSetting *ATCCSSetting::_instance = nullptr;
	ATCCSSetting::ATCCSSettingGarbo ATCCSSetting::singleGarbo;
	ATCCSSetting::ATCCSSetting(void)
	{
		_at = AT216;
		_device = GIMBAL;
		InitializeCriticalSection(&_cs);
	}


	ATCCSSetting::~ATCCSSetting(void)
	{
		DeleteCriticalSection(&_cs);
		std::cout << "set" << std::endl;
	}

	ATCCSSetting* ATCCSSetting::instance()
	{
		if(_instance == nullptr)
		{
			if(_instance == nullptr)
			{
				_instance = new (std::nothrow) ATCCSSetting;
			}
		}
		return _instance;
	}


	void ATCCSSetting::setHostAddress(const std::string &_ip, unsigned short _port)
	{
		_hostAddress = _ip;
		_hostPort = _port;
	}

	void ATCCSSetting::setRemoteAddress(const std::string &_ip, unsigned short _port)
	{
		_remoteAddress = _ip;
		_remotePort = _port;
	}



	ATCCSUdpSocket::ATCCSUdpSocket(void) :
	send_socket_descriptor(-1),recv_socket_descriptor(-1)
	{
#ifdef WIN32
		if(WSAStartup(MAKEWORD(2,2), &ws) != 0)
			printf("%s\n","init WSA failed");
		send_socket_descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(send_socket_descriptor == INVALID_SOCKET)
			printf("%s\n","udp send socket created failed!");
		recv_socket_descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(recv_socket_descriptor == INVALID_SOCKET)
			printf("%s\n","udp recv socket created failed!");
#else
		send_socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
		if(send_socket_descriptor < 0)
			cerr<< "udp send socket created failed!"<<endl;
		recv_socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
		if(recv_socket_descriptor < 0)
			cerr<<"udp recv socket created failed!"<<endl;
#endif
	}


	ATCCSUdpSocket::~ATCCSUdpSocket(void)
	{
#ifdef WIN32
		if(send_socket_descriptor != INVALID_SOCKET)
			closesocket(send_socket_descriptor);
		if(recv_socket_descriptor != INVALID_SOCKET)
			closesocket(recv_socket_descriptor);;
#else
		if(send_socket_descriptor != -1)
			close(send_socket_descriptor);
		if(recv_socket_descriptor != -1)
			close(recv_socket_descriptor);
#endif
	}

	void ATCCSUdpSocket::setTargetAddress(const std::string &ip, unsigned short port)
	{
		_targetIP = ip;
		_targetPort = port;
		memset(&_targetAddr, 0, sizeof(sockaddr_in));
		_targetAddr.sin_family = AF_INET;
		_targetAddr.sin_addr.s_addr = inet_addr(_targetIP.data());
		_targetAddr.sin_port = htons(_targetPort);
	}

	int ATCCSUdpSocket::setHostAddress(const std::string &ip, unsigned short port)
	{
		int ret = -1;
		_hostIP = ip;
		_hostPort = port;
		memset(&_hostAddr, 0, sizeof(sockaddr_in));
		_hostAddr.sin_family = AF_INET;
		if(ip.empty())
			_hostAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		else
		{
			_hostAddr.sin_addr.s_addr = inet_addr(_hostIP.data());
		}
		_hostAddr.sin_port = htons(_hostPort);
		if(recv_socket_descriptor != -1)
		{
			ret = bind(recv_socket_descriptor, (sockaddr*)&_hostAddr, sizeof(sockaddr));
			if(-1 == ret)
			{
				printf("%s\n","bind udp recv ip and port failed!");
			}			
		}
		return ret;
	}

	ssize_t ATCCSUdpSocket::sendData(const char *data, ssize_t size)
	{
		if(send_socket_descriptor < 0)
			return -1;
		ssize_t ret = sendto(send_socket_descriptor,\
			data, \
			size,\
			0,\
			(struct sockaddr*)&_targetAddr,\
			sizeof(sockaddr));
		return ret;
	}

	ssize_t ATCCSUdpSocket::recvData(char *data, unsigned int MAXSIZE)
	{
		ssize_t ret = -1;
		sockaddr_in client_addr;
		socklen_t client_addr_length = sizeof(sockaddr_in);
		memset(&client_addr, 0, sizeof(sockaddr_in));
		if(data == NULL)
			return ret;

		ret = recvfrom(recv_socket_descriptor,\
			data,\
			MAXSIZE,\
			0, \
			(sockaddr*)&client_addr,\
			&client_addr_length);
		return ret;
	}


	unsigned int GimbalStatusReport::sequence = 0;
	GimbalStatusReport::GimbalStatusReport(void) : ATCCSReport()
	{
		ATCCSSetting *set = ATCCSSetting::instance();
		if(set)
		{
			_at = set->at();
			_device = set->device();
		}
		else
		{
			_at = 0;
			_device = 0;
		}

		//注意，为了提高程序运行效率，可以在类创建的时候，分配待发送数据空间，根据实际情况修改_AT_GIMBAL_STATUS
		try
		{
			_size = sizeof(_ATCCSPHeader) + sizeof(_AT_GIMBAL_STATUS);
			_data = new char[_size];
		}
		catch(std::bad_alloc)
		{
			_size = 0;
			_data = nullptr;
		}
	}


	GimbalStatusReport::~GimbalStatusReport(void)
	{
	}

	void GimbalStatusReport::setData()
	{
		//注意，为了避免有值污染，先清空待发送缓冲区,不用修改
		memset(_data, 0, _size);

		//注意，在这里，填充你的状态数据结构体
		_AT_GIMBAL_STATUS status;
		status.siderealTime = 100.101;
		status.hourAngle = 100.102;
		status.rightAscension = 100.103;
		status.declination = 100.104;
		status.J2000RightAscension = 100.105;
		status.J2000Declination = 100.106;
		status.azmiuth = 100.107;
		status.elevation = 100.108;
		status.derotatorPositon = 100.109;
		status.targetRightAscension = 100.111;
		status.targetJ2000Declination = 100.112;
		status.targetAzmiuth = 100.113;
		status.targetElevation = 100.114;
		status.targetDerotatorPosition = 100.115;
		status.axis1TrackError = 100.116;
		status.axis2TrackError = 100.117;
		status.axis3TrackError = 100.118;
		status.focusTypeIndex = 2;
		status.axis4Angle = 100.119;
		status.coverIndex = 3;
		status.coverPosition = 100.121;
		status.targetFocusTypeIndex = 4;
		status.targetAxis4Angle = 100.122;
		status.targetCoverIndex = 5;
		status.axis1Speed = 100.122;
		status.axis2Speed = 100.123;
		status.axis3Speed = 100.124;
		status.trackType = 6;
		status.refractionCorrection = 100.125;
		status.axis1Encoder = 100.126;
		status.axis2Encoder = 100.127;
		status.axis3Encoder = 100.128;
		status.axis3Mode = 7;
		status.axis1PMCorrection = 100.1131;
		status.axis2PMCorrection = 100.132;
		status.axis3PMCorrection = 100.133;
		status.axis1ManualCorrection = 100.134;
		status.axis2ManualCorrection = 100.135;
		status.axis3ManualCorrection = 100.136;
		status.pieSide = 8;
		status.targetPieSide = 9;
		status.isAxis1FindHome = 1;
		status.isAxis2FindHome = 1;
		status.isAxis3FindHome = 1;
		status.status.status = 10;
		status.status.lastStatus = 12;
		status.status.warning = 13;
		status.status.error = 14;
		memcpy(&(status.status.errorString), "test", 48);
		memcpy(&(status.trackObjectName), "test", 48);

		//注意，在这里将数据拷贝到待发送缓冲区
		//注意，第一行，为填充消息头部信息，根据实际情况修改
		//第二行，不用动
		//第三行，根据实际的结构体类型，修改_AT_GIMBAL_STATUS
		setProtocolHeader(ATSTATUSREPORT, _size, sequence++, _at, _device);
		memcpy(_data, &header, sizeof(_ATCCSPHeader));
		memcpy(_data+sizeof(_ATCCSPHeader), &status, sizeof(_AT_GIMBAL_STATUS));
	}

	unsigned int ASCRealtimeDataReport::sequence = 1;
	ASCRealtimeDataReport::ASCRealtimeDataReport()
	{
		//注意，根据环境监测信息的类型，修改此处值
		_em = ASC;
		//注意，为了提高程序运行效率，可以在类创建的时候，分配待发送数据空间，根据实际情况修改环境监测数据结构体类型
		try
		{
			_size = sizeof(_ATCCSPHeader) + sizeof(_EME_ASC_REALTIME_DATA);
			_data = new char[_size];
		}
		catch(std::bad_alloc)
		{
			_size = 0;
			_data = nullptr;
		}
	}

	ASCRealtimeDataReport::~ASCRealtimeDataReport()
	{
	}


	void ASCRealtimeDataReport::setData()
	{
		//注意，为了避免有值污染，先清空待发送缓冲区,不用修改
		memset(_data, 0, _size);

		//注意，在这里，填充你的状态数据结构体
		_EME_ASC_REALTIME_DATA realtimeData;
		realtimeData.aperture = 100.111;
		realtimeData.fits = 1;
		realtimeData.iso = 0;
		realtimeData.jpeg = 0;
		realtimeData.raw = 0;
		realtimeData.shutterspeed = 200.222;

		//注意，在这里将数据拷贝到待发送缓冲区,不用动
		setProtocolHeader(EMEREALDATA, _size, sequence++, _em);
		memcpy(_data, &header, sizeof(_ATCCSPHeader));
		memcpy(_data+sizeof(_ATCCSPHeader), &realtimeData, sizeof(_EME_ASC_REALTIME_DATA));
	}
	





}