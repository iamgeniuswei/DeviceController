#ifndef ATCCS_H
#define ATCCS_H
#include "atccs_define.h"
#ifdef WIN32
typedef unsigned long ssize_t;
typedef int socklen_t;
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#ifdef WIN32
#include <Windows.h>
#else
typedef void* HANDLE;
#endif
#include <string>
#include <queue>
#ifdef WIN32
#include <Windows.h>
#include <process.h>
#else
typedef void* HANDLE;
#endif
#include <memory>
#if C11
#include <mutex>
#include <condition_variable>
#endif
#include <map>
#include "export_define.h"

namespace ATCCS
{
	class ATCCSData;
	class ATCCSSingleton;
	class ATCCSThread;
	class ATCCSUdpSocket;
	class ATCCSDeviceController;
	/*---------------------------------------------------------------
	* 说明：	该类封装传输数据的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	class DEVICECONTROLLER_API ATCCSData
	{
	public:
		ATCCSData(void);
		ATCCSData(unsigned int size);
		~ATCCSData(void);

		char *data();
		unsigned int size() const
		{
			return _size;
		}
		bool validate() const;

	private:
		char *_data;
		unsigned int _size;
		static unsigned int _maxSize;
	};

	/*---------------------------------------------------------------
	* 说明：	该类封装线程的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	extern unsigned int _stdcall worker(void *pHandle);
	class DEVICECONTROLLER_API ATCCSThread
	{
	public:
		ATCCSThread(void);
		virtual ~ATCCSThread(void);
		virtual void start();
		virtual void join();
		void setStop(bool stop)
		{
			EnterCriticalSection(&_cs);
			_stop = stop;
			LeaveCriticalSection(&_cs);
		}
		bool stop()
		{
			bool temp = false;
			EnterCriticalSection(&_cs);
			temp = _stop;
			LeaveCriticalSection(&_cs);
			return temp;
		}
		void setFrequence(unsigned int frequence)
		{
			EnterCriticalSection(&_cs);
			_frequence = frequence;
			LeaveCriticalSection(&_cs);
		}
		unsigned int frequence()
		{
			unsigned int temp = 1;
			EnterCriticalSection(&_cs);
			temp = _frequence;
			LeaveCriticalSection(&_cs);
			return temp;
		}


	private:
		static void cleaner(void *pHandle);
		friend unsigned int _stdcall worker(void *pHandle);

	protected:
		virtual void run() = 0;
		HANDLE _hThread;
		unsigned int _threadID;
		bool _stop;
		unsigned int _frequence;
		CRITICAL_SECTION _cs;
	};

	/*---------------------------------------------------------------
	* 说明：	该类封装UDP通信的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	class DEVICECONTROLLER_API ATCCSUdpSocket
	{
	public:
		ATCCSUdpSocket(void);
		~ATCCSUdpSocket(void);
		void setTargetAddress(const std::string &ip, unsigned short port);
		int setHostAddress(const std::string &ip, unsigned short port);
		ssize_t sendData(const char *data, ssize_t size);
		ssize_t recvData(char *data, unsigned int MAXSIZE);
	private:
		std::string _targetIP;
		unsigned short _targetPort;
		sockaddr_in _targetAddr;
		std::string _hostIP;
		unsigned short _hostPort;
		sockaddr_in _hostAddr;
#ifdef WIN32
		SOCKET   send_socket_descriptor;
		SOCKET recv_socket_descriptor;
		WSADATA ws;
#else
		int send_socket_descriptor = -1;
		int recv_socket_descriptor = -1;
#endif
	};

	/*---------------------------------------------------------------
	* 说明：	该类封装可支持多线程操作的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	template <typename T>
	class ATCCSQueue
	{
	public:
		ATCCSQueue(void)
		{
#if C11
#elif C98
			InitializeCriticalSection(&_cs);
#endif
		}	
		virtual ~ATCCSQueue(void)
		{
			EnterCriticalSection(&_cs);
			while(_dataQueue.size() > 0)
			{
				_dataQueue.pop();				
			}
			LeaveCriticalSection(&_cs);
			std::cout << "~ATCCSQueue" <<std::endl;
#if C11
#elif C98
			DeleteCriticalSection(&_cs);
#endif
		};

		void push(T new_value)
		{
#if C11
			std::lock_guard<std::mutex> lk(_queueLock);
			_dataQueue.push(new_value);
			_dataCond.notify_one();
#elif C98
			EnterCriticalSection(&_cs);
			if(new_value != nullptr)
				_dataQueue.push(new_value);
			LeaveCriticalSection(&_cs);
#endif
		}

		T wait_and_pop()
		{
#if C11
			std::unique_lock<std::mutex> lk(_queueLock);
			_dataCond.wait(lk, [this]{return !_dataQueue.empty();});
			T value = _dataQueue.front();
			_dataQueue.pop();
			return value;
#elif C98
			EnterCriticalSection(&_cs);
			T value = nullptr;
			if(!_dataQueue.empty())
			{
				value = _dataQueue.front();
				_dataQueue.pop();
			}
			LeaveCriticalSection(&_cs);
			return value;
#endif
		}

		bool empty()
		{
#if C11
			std::lock_guard<std::mutex> lk(_queueLock);
			return _dataQueue.empty();
#elif C98
			EnterCriticalSection(&_cs);
			bool isEmpty = true;
			isEmpty = _dataQueue.empty();
			LeaveCriticalSection(&_cs);
			return isEmpty;
#endif
		}


		size_t size()
		{
#if C11
			std::lock_guard<std::mutex> lk(_queueLock);
			return _dataQueue.size();
#elif C98
			EnterCriticalSection(&_cs);
			size_t size = 0;
			size = _dataQueue.size();
			LeaveCriticalSection(&_cs);
			return size;
#endif
		}


	private:
		std::queue<T> _dataQueue;	
#if C11
		mutable std::mutex _queueLock;
		std::condition_variable _dataCond;
#elif C98
		CRITICAL_SECTION _cs;
#endif
	};


	/*---------------------------------------------------------------
	* 说明：	该类封装系统数据池的数据成员与操作
	* 该类继承自ATCCSQueue，并使用std::shared_ptr<ATCCSData>实例化
	* 该类为单例类，也就是，系统中只允许有一个数据池
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	class DEVICECONTROLLER_API ATCCSDataQueue : public ATCCSQueue<std::shared_ptr<ATCCSData>>
	{
	private:
		ATCCSDataQueue(void);
		ATCCSDataQueue(const ATCCSDataQueue&);
		ATCCSDataQueue& operator=(const ATCCSDataQueue&);
		static ATCCSDataQueue *_instance;
		class ATCCSDataQueueGarbo
		{
		public:
			~ATCCSDataQueueGarbo()
			{
				if(_instance != nullptr)
				{
					delete _instance;
					_instance = nullptr;
				}
			}
		};
		static ATCCSDataQueueGarbo singleGarbo;

	public:
		~ATCCSDataQueue(void);
		static ATCCSDataQueue *instance();
	};


	/*---------------------------------------------------------------
	* 说明：	该类封装数据接收功能的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	class DEVICECONTROLLER_API ATCCSDataReceiver : public ATCCSThread
	{
	public:
		ATCCSDataReceiver(void);
		~ATCCSDataReceiver(void);
		int setRecvAddress(const std::string &ip, unsigned short port);
	protected:
		void run();
	private:
		ATCCSDataQueue *_dataQueue;
		ATCCSUdpSocket *_socket;
	};


	/*---------------------------------------------------------------
	* 说明：	该类封装数据处理功能的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	class DEVICECONTROLLER_API ATCCSDataHandler : public ATCCSThread
	{
	public:
		ATCCSDataHandler(void);
		~ATCCSDataHandler(void);
		void registerDeviceController(unsigned int id, ATCCSDeviceController *controller = nullptr);
		ATCCSDeviceController *controller(unsigned int id);
		void dispatchControlData(std::shared_ptr<ATCCSData> data);
	public:
		void run();
	private:
		ATCCSDataQueue *_dataQueue;
		std::map<unsigned int,ATCCSDeviceController*> _controllers;
	};


	/*---------------------------------------------------------------
	* 说明：	该类封装设备控制功能的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	typedef void (*controlDevice)(std::shared_ptr<ATCCSData> data);
	class DEVICECONTROLLER_API ATCCSDeviceController : public ATCCSThread
	{
	public:
		ATCCSDeviceController(void);
		~ATCCSDeviceController(void);
		void pushControlData(std::shared_ptr<ATCCSData> data)
		{
			_fifoQueue.push(data);
		}
		virtual void run();
		void setDeviceController(controlDevice controller = nullptr)
		{
			if(controller)
				_controller = controller;
		}
	protected:
		ATCCSQueue<std::shared_ptr<ATCCSData>> _fifoQueue;
		controlDevice _controller;
	};

	/*---------------------------------------------------------------
	* 说明：	该类封装上报数据功能类的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-31
	* IDE:		VS2010
	* OS:		windows xp sp3
	* Modified:	2017-09-05
	* Description:	增加了环境监测上报数据头部内容设置函数
	*--------------------------------------------------------------*/
	class ATCCSUdpSocket;
	class DEVICECONTROLLER_API ATCCSReport
	{
	public:
		ATCCSReport(void);
		virtual ~ATCCSReport(void);
		virtual unsigned int report();
		virtual void setData() = 0;
		virtual void setProtocolHeader(unsigned int msg, unsigned int length, unsigned int sequence, unsigned short at, unsigned short device);
		virtual void setProtocolHeader(unsigned int msg, unsigned int length, unsigned int sequence, unsigned int em);
		void setRemoteAddress(const std::string &ip, unsigned short port);
		void setATDevice(unsigned short at, unsigned short device)
		{
			_at = at;
			_device = device;
		}
		void setEMDevice(unsigned int em)
		{
			_em = em;
		}
	protected:
		ATCCSUdpSocket *_socket;
		char *_data;
		unsigned int _size;
		_ATCCSPHeader header;
		unsigned short _at;
		unsigned short _device;
		unsigned int _em;
	};






	/*---------------------------------------------------------------
	* 说明：	该类封装心跳数据的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-31
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	class DEVICECONTROLLER_API ATCCSHeartbeatReport : public ATCCSReport, public ATCCSThread
	{
	public:
		ATCCSHeartbeatReport(void);
		ATCCSHeartbeatReport(const std::string &ip, unsigned short port, unsigned short at, unsigned short device);
		~ATCCSHeartbeatReport(void);
		void run();
	private:
		void setData();
		static unsigned int sequence;
		
	};

	/*---------------------------------------------------------------
	* 说明：	该类封装ATCCS指令执行结果的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-31
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	class DEVICECONTROLLER_API ATCCSInstructionResultReport : public ATCCSReport
	{
	public:
		ATCCSInstructionResultReport(void);
		ATCCSInstructionResultReport(unsigned int plan, unsigned short at, unsigned short device, unsigned int sequence, unsigned int operation, unsigned int timeout, unsigned int result);
		void setInstructionResult(unsigned int plan, unsigned short at, unsigned short device, unsigned int sequence, unsigned int operation,unsigned int timeout, unsigned int result);
		void setData();
		~ATCCSInstructionResultReport(void);
	private:
		void fillStruct(unsigned int plan, unsigned short at, unsigned short device, unsigned int sequence, unsigned int operation, unsigned int timeout, unsigned int result);
	private:
		_AT_INSTRUCTION_RESULT _instruction;
	};



	/*---------------------------------------------------------------
	* 说明：	该类封装系统设置相关的数据成员与操作
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/
	class DEVICECONTROLLER_API ATCCSSetting
	{
	private:
		ATCCSSetting(void);
		ATCCSSetting(const ATCCSSetting&);
		ATCCSSetting& operator=(const ATCCSSetting&);
		static ATCCSSetting *_instance;
		class ATCCSSettingGarbo
		{
		public:
			~ATCCSSettingGarbo()
			{
				if(_instance != nullptr)
				{
					delete _instance;
					_instance = nullptr;
				}
			}
		};
		static ATCCSSettingGarbo singleGarbo;

	public:
		~ATCCSSetting(void);
		static ATCCSSetting *instance();
		void setHostAddress(const std::string &_ip, unsigned short _port);
		void setRemoteAddress(const std::string &_ip, unsigned short _port);
		std::string hostAddress() const
		{
			return _hostAddress;
		}
		unsigned short hostPort()
		{
			return _hostPort;
		}
		std::string remoteAddress() const
		{
			return _remoteAddress;
		}
		unsigned short remotePort()
		{
			return _remotePort;
		}
		unsigned short at() const
		{
			return _at;
		}
		unsigned short device() const
		{
			return _device;
		}

		void setATControllerOnline(bool online)
		{
			EnterCriticalSection(&_cs);
			_atControllerOnline = online;
			LeaveCriticalSection(&_cs);
		}

		bool atControllerOnline()
		{
			bool temp = true;
			EnterCriticalSection(&_cs);
			temp = _atControllerOnline;
			LeaveCriticalSection(&_cs);
			return temp;
		}
	private:
		std::string _hostAddress;
		unsigned short _hostPort;
		std::string _remoteAddress;
		unsigned short _remotePort;
		unsigned short _at;
		unsigned short _device;
		bool _atControllerOnline;
		unsigned int _lastOnlineTime;
		CRITICAL_SECTION _cs;
	};

	class DEVICECONTROLLER_API GimbalStatusReport : public ATCCSReport
	{
	public:
		GimbalStatusReport(void);
		~GimbalStatusReport(void);
		void setData();

	private:
		static unsigned int sequence;
	};

	class DEVICECONTROLLER_API ASCRealtimeDataReport : public ATCCSReport
	{
	public:
		ASCRealtimeDataReport(void);
		~ASCRealtimeDataReport(void);
		void setData();
	private:
		static unsigned int sequence;
	};
}
#endif