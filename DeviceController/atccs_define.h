#ifndef ATCCS_DEFINE
#define ATCCS_DEFINE
#pragma once

namespace ATCCS
{
	/*---------------------------------------------------------------
	* 说明：	以下部分定义全局编译条件
	* Author:	Geniuswei
	* Date:		2017-03-30
	* IDE:		VS2010
	* OS:		windows xp sp3
	*--------------------------------------------------------------*/

#ifndef C11
#define C11 0
#endif

#ifndef C98
#define C98 1
#endif

#ifndef QT
#define QT 0
#endif

#ifndef nullptr
#define nullptr 0
#endif
#ifndef NULL
#define NULL 0
#endif

#ifndef UINT8
	typedef unsigned char UINT8;
#endif
#ifndef UINT16
	typedef unsigned short UINT16;
#endif
#ifndef UINT32
	typedef unsigned int UINT32;
#endif
#ifndef UINT64
	typedef unsigned long long UINT64;
#endif

#ifndef INT8
	typedef signed char INT8;
#endif

#ifndef INT16
	typedef signed short INT16;
#endif

#ifndef INT32
	typedef signed int INT32;
#endif

#ifndef DOUBLE64
	typedef double DOUBLE64;
#endif

#ifndef FLOAT32
	typedef float FLOAT32;
#endif

const int MAGIC = 0x1a2b3c4d;
const unsigned short VERSION = 1;
const unsigned int SIZE_ATCCSPHEADER = 28;
const unsigned int SIZE_INSTRUCTIONHEADER = 20;

	/*!
	* ----------------------------------------------------------------------------------------------------
	* 该结构体定义ATCCS协议头部结构
	* 大小：							28 B
	* 字段说明：
	* magic：						协议标识号，如果值不等于0x1a2b3c4d，非ATCCS协议数据
	* majorVersion/minorVersion:	协议版本号，便于将来扩展
	* msg：							协议数据体中的数据类型
	* length:						加上该头部后，协议数据报的总长度
	* sequence：						数据报的唯一标识号
	* tv_sec/tv_usec:				协议数据报时间戳
	* AT.at/AT.device:				数据报数据指向的望远镜和设备
	* env：							数据报数据指向的环境监测设备
	* ---------------------------------------------------------------------------------------------------
	* 使用说明: 使用该结构体解析char* 字符串, 示例如下：
	* 注意, 确保data有值, 且数据长度大于等于 sizeof(_ATCCSPHeader)
	* char *data;
	* _ATCCSPHeader *header = (_ATCCSPHeader*)data;
	* int seq = header->sequence;
	* int at = header->AT.at;
	* int device = header->device;
	*/

#pragma pack(1)
	struct _ATCCSPHeader{
		UINT32 magic;
		UINT16 version;
		UINT16 msg;
		UINT32 length;
		UINT32 sequence;
		UINT32 tv_sec;
		UINT32 tv_usec;
		union
		{
			struct
			{
				UINT16 at;
				UINT16 device;
			}AT;
			UINT32 env;
		};
	};
#pragma pack()



	/*
	* ---------------------------------------------------------------------------------------------------
	* 以下部分定义ATCCS协议的公共数据信息
	* ---------------------------------------------------------------------------------------------------
	* verion:              1.00.00
	* struct defined by:   Xinglong station of NAOC.
	* file created by:     UKC.
	* modified date:       2016-10-18.
	* CPP version:         C++11/14.
	* compiler:            gcc 4.8.5.
	* IDE:                 Qt 5.6.0.
	* debugger:			gdb 7.6.1.
	* OS:                  CentOS 7.0 64bit.
	* kernel:              3.10.0.
	* ---------------------------------------------------------------------------------------------------
	* 该文件拷贝于 (version 1.00.00)
	* ---------------------------------------------------------------------------------------------------
	* version:				1.00.1
	* modified data:		2017-04-06
	* IDE:					VS2010
	* OS:					windows xp sp3
	*
	*/
	/*!
* ----------------------------------------------------------------------------------------------------
* @brief:   ENUM_AT_MSG_TYPE enumerates ATCCS's message.
* ----------------------------------------------------------------------------------------------------
*/
enum ENUM_AT_MSG_TYPE
{
    ENVDATAREPORT = 0x01,
    ATATTRIBUTE,
    ATATTRIBUTEACK,
    ATSTATUSREPORT,
    ATSTATUSACK,
    ATINSTRUCTION,
    ATINSTRUCTIONACK,
    ATPLAN,
    ATPLANACK,
    ATHEARTBEAT,
    ATTAKEOVER,
};

/*!
* ----------------------------------------------------------------------------------------------------
* @brief:   ENUM_AT_AT_TYPE enumerates ATCCS's at.
* ----------------------------------------------------------------------------------------------------
*/
enum ENUM_AT_AT_TYPE
{
    AT216 = 0x20,
    AT126,
    AT100,
    AT85,
    AT80,
    AT60,
    AT50,
    ATAE,
    AT_AT_NUM
};

/*!
* ----------------------------------------------------------------------------------------------------
* @brief:   ENUM_AT_DEVICE_TYPE enumerates ATCCS's devices of AT.
* ----------------------------------------------------------------------------------------------------
*/
enum ENUM_AT_DEVICE_TYPE{
    GIMBAL=0x40,
    CCD,
    FILTER,
    SLAVEDOME,
    FULLOPENEDDOME,
    FOCUS,
    GUIDESCOPE,
    DPM
};

/*!
* ----------------------------------------------------------------------------------------------------
* @brief:   ENUM_AT_ENV_TYPE enumerates ATCCS's environmental devices.
* ----------------------------------------------------------------------------------------------------
*/
enum ENUM_AT_ENV_TYPE{
    ASC = 0x60,
    WS,
    CS,
    SQM,
    DIMM,
    DUST
};

/**
 * -----------------------------------------------------------------------------
 * @brief:  ENUM_AT_INSTRUCTION_RESULT enumerates ATCCS's instruction result.
 * -----------------------------------------------------------------------------
 */
enum ENUM_AT_INSTRUCTION_RESULT
    {
        RESULT_NOTEXECUTED,
        RESULT_SUCCESS,
        RESULT_PARAMOUTOFRANGE,
        RESULT_CANNTEXECUTE,    
        RESULT_SENDERROR
    };




enum ENUM_CONNECT{
    CONNECT = 1,
    DISCONNECT = 2
};

enum ENUM_ACTION
{
    OPEN = 0x01,
    CLOSE
};

/**
 *------------------------------------------------------------------------------
 * @brief:      _AT_PUBLIC_PROPERTY defines AT Device's public property.
 *------------------------------------------------------------------------------
 * @version:    2.00.00
 * @size:       60 bytes.
 * @author:     Geniuswei
 * @date:       2017-05-15
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 *              char: 1 bytes.
 *------------------------------------------------------------------------------
*/
#pragma pack(1)
struct _AT_PUBLIC_PROPERTY{
    char id[8];
    char name[48];
    unsigned short telescope;
    unsigned short type;
};
#pragma pack()

/**
 *------------------------------------------------------------------------------
 * @brief:      _AT_PUBLIC_STATUS defines AT Device's public status.
 *------------------------------------------------------------------------------
 * @version:    2.00.00
 * @size:       72 bytes.
 * @author:     Geniuswei
 * @date:       2017-05-15
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned int: 4 bytes.
 *              unsigned long long: 8 bytes.
 *              char: 1 bytes.
 *------------------------------------------------------------------------------
*/
#pragma pack(1)
struct _AT_PUBLIC_STATUS{
    unsigned int status;
    unsigned int lastStatus;
    unsigned long long warning;
    unsigned long long  error;
    char errorString[48];
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:      _AT_OCCUPATION defines AT occupation.
 * -----------------------------------------------------------------------------
 * @size:       20 bytes.
 * -----------------------------------------------------------------------------
 * @author:     Geniuswei
 * @date:       2017-05-12
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_OCCUPATION
{
    unsigned int _user;
    unsigned short _at;
    unsigned short _occupation;
    unsigned int _start;
    unsigned int _end;
    unsigned int _priority;
};
#pragma pack()


/*!
 *------------------------------------------------------------------------------
 * @brief:      _AT_INSTRUCTION_HEADER defines AT instruction header.
 *------------------------------------------------------------------------------
 * @user:       The instruction's user.
 * @plan:       The instruction's plan.
 * @at:         The AT that execute the instruction.
 * @device:     The device that execute the instruction.
 * @sequence:   The instruction's sequence.
 * @operation:  The instruction identifier, = ENUM_*_INSTRUCTION.
 * -----------------------------------------------------------------------------
 * @size:       20 bytes.
 * -----------------------------------------------------------------------------
 * @author      Geniuswei
 * @date	2016-12-28
 * @modified	2017-03-31
 *------------------------------------------------------------------------------
*/
#pragma pack(1)
struct _AT_INSTRUCTION_HEADER{
    unsigned int user;
    unsigned int plan;
    unsigned short at;
    unsigned short device;
    unsigned int sequence;
    unsigned int operation;
};
#pragma pack()


	/*!
 *------------------------------------------------------------------------------
 * @brief:       _AT_INSTRUCTION_RESULT defines the instruction's result.
 *------------------------------------------------------------------------------
 * @user:       The instruction's user.
 * @plan:       The instruction's plan.
 * @at:         The AT that execute the instruction.
 * @device:     The device that execute the instruction.
 * @sequence:   The instruction's sequence.
 * @operation:  The instruction's identifier, = ENUM_*_INSTRUCTION.
 * @result:     The instruction's result, = ENUM_AT_INSTRUCTION_RESULT.
 * -----------------------------------------------------------------------------
 * @size:       28 bytes.
 * -----------------------------------------------------------------------------
 * @author	Geniuswei
 * @date	2016-12-28
 * @modified    2017-03-31
 *------------------------------------------------------------------------------
*/
#pragma pack(1)
struct _AT_INSTRUCTION_RESULT{
    unsigned int user;
    unsigned int plan;
    unsigned short at;
    unsigned short device;
    unsigned int sequence;
    unsigned int timeout;
    unsigned int operation;
    unsigned int result;
};
#pragma pack()

#pragma pack(1)
struct _AT_PLAN{
    unsigned short at;
    char user[48];
    char project[48];
    char target[48];
    unsigned int type;
    double rightAscension;
    double declination;
    unsigned int epoch;
    double  exposureTime;
    double delayTime;
    unsigned int exposureCount;
    char filter[8];
    unsigned short gain;
    unsigned short bin;
    unsigned short readout;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_GIMBAL_INSTRUCTION enumerates the gimbal's instruction.
 * -----------------------------------------------------------------------------
 */
enum ENUM_AT_GIMBAL_INSTRUCTION
{
    _GIMBAL_INSTRUCTION_CONNECT = 1,    
    _GIMBAL_INSTRUCTION_FINDHOME,               // =2
    _GIMBAL_INSTRUCTION_TRACKSTAR,              // =3
    _GIMBAL_INSTRUCTION_SETOBJECTNAME,          // =4
    _GIMBAL_INSTRUCTION_SLEWAZEL,               // =5
    _GIMBAL_INSTRUCTION_SLEWDEROTATOR,          // =6
    _GIMBAL_INSTRUCTION_CONFIGDEROTATOR,        // =7
    _GIMBAL_INSTRUCTION_STOP,                   // =8
    _GIMBAL_INSTRUCTION_SPEEDCORRECT,           // =9
    _GIMBAL_INSTRUCTION_PARK,                   // =10
    _GIMBAL_INSTRUCTION_FIXEDMOVE,              // =11
    _GIMBAL_INSTRUCTION_POSITIONCORRECT,        // =12
    _GIMBAL_INSTRUCTION_COVERACTION,            // =13
    _GIMBAL_INSTRUCTION_FOCUSACTION,            // =14
    _GIMBAL_INSTRUCTION_EMERGENCYSTOP,          // =15
    _GIMBAL_INSTRUCTION_SAVESYNCDATA,           // =16
    _GIMBAL_INSTRUCTION_TRACKSATELLITE,         // =17
    _GIMBAL_INSTRUCTION_CONFIGPROPERTIES        // =18
};
/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_GIMBAL_STATUS enumerates the Gimbal's status.
 * ----------------------------------------------------------------------------- 
 */
enum ENUM_AT_GIMBAL_STATUS 
{
    _GIMBAL_STATUS_DISCONNECT = 1,
    _GIMBAL_STATUS_CONNECTING,                  // =2
    _GIMBAL_STATUS_DISCONNECTING,               // =3
    _GIMBAL_STATUS_NOTFINDHOME,                 // =4
    _GIMBAL_STATUS_HOMING,                      // =5
    _GIMBAL_STATUS_FREEZING,                    // =6
    _GIMBAL_STATUS_FREEZED,                     // =7
    _GIMBAL_STATUS_EMERGENCYING,                // =8
    _GIMBAL_STATUS_EMERGENCY,                   // =9
    _GIMBAL_STATUS_PARKING,                     // =10
    _GIMBAL_STATUS_PARKED,                      // =11
    _GIMBAL_STATUS_WAITINGTEACKING,             // =12    
    _GIMBAL_STATUS_TRACKING,                    // =13
    _GIMBAL_STATUS_TRACKINGBIASSPEED,           // =14
    _GIMBAL_STATUS_TRACKINGBIASPOS,             // =15
    _GIMBAL_STATUS_WAITINGMOVING,               // =16    
    _GIMBAL_STATUS_MOVING,                      // =17
    _GIMBAL_STATUS_SLEWING,                     // =18
    _GIMBAL_STATUS_SLEWED,                      // =19
    _GIMBAL_STATUS_WAITINGRUNNING,              // =20
    _GIMBAL_STATUS_RUNNING,                     // =21
    _GIMBAL_STATUS_ERROR                        // =22
};
//
/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_GIMBAL_WARNINGS enumerates the Gimbal's warnings.
 * ----------------------------------------------------------------------------- 
 */
enum ENUM_AT_GIMBAL_WARNINGS
{
    _GIMBAL_WARN_NORMAL = 0x0,
    _GIMBAL_WARN_ENCODER = 0x1,
    _GIMBAL_WARN_ENCODER_AXIS1 =0x2,
    _GIMBAL_WARN_ENCODER_AXIS2 = 0x4,
    _GIMBAL_WARN_ENCODER_AXIS3 = 0x8,
    _GIMBAL_WARN_ENCODER_AXIS4 = 0x10,
    _GIMBAL_WARN_ENCODER_AXIS5 = 0x20,
    _GIMBAL_WARN_LIMIT_AXIS1 = 0x40,
    _GIMBAL_WARN_LIMIT_AXIS2 = 0x80,
    _GIMBAL_WARN_LIMIT_AXIS3 = 0x100,
    _GIMBAL_WARN_LIMIT_AXIS4 = 0x200,
    _GIMBAL_WARN_LIMIT_AXIS5 = 0x400
};

/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_GIMBAL_ERRORS enumerates the Gimbal's errors.
 * -----------------------------------------------------------------------------
 */

    const unsigned long _GIMBAL_ERROR_NORMAL = 0x0;
    const unsigned long _GIMBAL_ERROR_BOTTOMCOM = 0x1;
    const unsigned long _GIMBAL_ERROR_TOPCOM = 0x2;
    const unsigned long _GIMBAL_ERROR_LIMIT_AXIS1 = 0x4;
    const unsigned long _GIMBAL_ERROR_LIMIT_AXIS2 = 0x8;
    const unsigned long _GIMBAL_ERROR_LIMIT_AXIS3 = 0x10;
    const unsigned long _GIMBAL_ERROR_LIMIT_AXIS4 = 0x20;
    const unsigned long _GIMBAL_ERROR_LIMIT_AXIS5 = 0x40;
    const unsigned long _GIMBAL_ERROR_VELOCITY_AXIS1 = 0x100;
    const unsigned long _GIMBAL_ERROR_VELOCITY_AXIS2 = 0x200;
    const unsigned long _GIMBAL_ERROR_VELOCITY_AXIS3 = 0x400;
    const unsigned long _GIMBAL_ERROR_VELOCITY_AXIS4 = 0x800;
    const unsigned long _GIMBAL_ERROR_VELOCITY_AXIS5 = 0x1000;
    const unsigned long _GIMBAL_ERROR_POSITION_AXIS1 = 0x2000;
    const unsigned long _GIMBAL_ERROR_POSITION_AXIS2 = 0x4000;
    const unsigned long _GIMBAL_ERROR_POSITION_AXIS3 = 0x8000;
    const unsigned long _GIMBAL_ERROR_POSITION_AXIS4 = 0x10000;
    const unsigned long _GIMBAL_ERROR_POSITION_AXIS5 = 0x20000;
    const unsigned long _GIMBAL_ERROR_STOP_AXIS1 = 0x40000;
    const unsigned long _GIMBAL_ERROR_STOP_AXIS2 = 0x80000;
    const unsigned long _GIMBAL_ERROR_STOP_AXIS3 = 0x100000;
    const unsigned long _GIMBAL_ERROR_STOP_AXIS4 = 0x200000;
    const unsigned long _GIMBAL_ERROR_STOP_AXIS5 = 0x400000;
    const unsigned long _GIMBAL_ERROR_ENCODER_AXIS1 = 0x800000;
    const unsigned long _GIMBAL_ERROR_ENCODER_AXIS2 = 0x1000000;
    const unsigned long _GIMBAL_ERROR_ENCODER_AXIS3 = 0x2000000;
    const unsigned long _GIMBAL_ERROR_ENCODER_AXIS4 = 0x4000000;
    const unsigned long _GIMBAL_ERROR_ENCODER_AXIS5 = 0x8000000;
    const unsigned long _GIMBAL_ERROR_SLIP_AXIS1 = 0x10000000;
    const unsigned long _GIMBAL_ERROR_SLIP_AXIS2 = 0x20000000;
    const unsigned long _GIMBAL_ERROR_SLIP_AXIS3 = 0x40000000;
    const unsigned long _GIMBAL_ERROR_SLIP_AXIS4 = 0x80000000;
    const unsigned long long _GIMBAL_ERROR_SLIP_AXIS5 = 0x100000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL1_AXIS1 = 0x200000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL1_AXIS2 = 0x400000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL1_AXIS3 = 0x800000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL1_AXIS4 = 0x1000000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL1_AXIS5 = 0x2000000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL2_AXIS1 = 0x4000000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL2_AXIS2 = 0x8000000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL2_AXIS3 = 0x10000000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL2_AXIS4 = 0x20000000000;
    const unsigned long long _GIMBAL_ERROR_LIMITLEVEL2_AXIS5 = 0x40000000000;
    const unsigned long long _GIMBAL_ERROR_SERVO_AXIS1 = 0x80000000000;
    const unsigned long long _GIMBAL_ERROR_SERVO_AXIS2 = 0x100000000000;
    const unsigned long long _GIMBAL_ERROR_SERVO_AXIS3 = 0x200000000000;
    const unsigned long long _GIMBAL_ERROR_SERVO_AXIS4 = 0x400000000000;
    const unsigned long long _GIMBAL_ERROR_SERVO_AXIS5 = 0x800000000000;

//
//
//
//
//
//
/*!
*-------------------------------------------------------------------------------
 * @brief:   _AT_GIMBAL_ATTRIBUTES defines Gimbal's property.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   396 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
*-------------------------------------------------------------------------------
 * @type-info:  char: 1 bytes.
 *              unsigned short: 0-65535 2 bytes.
 *              double: 8 bytes.
*-------------------------------------------------------------------------------
*/
#pragma pack(1)
struct _AT_GIMBAL_PROPERTY{
    char id[8];
    char name[48];
    unsigned short observatory;
    double longitude;
    double latitude;
    double altitude;
    double aperture;
    unsigned short type;
    unsigned short focusType[10];
    double focusRatio[10];
    double focusLength[10];
    double maxAxis1Speed;
    double maxAxis2Speed;
    double maxAxis3Speed;
    double maxAxis1Acceleration;
    double maxAxis2Acceleration;
    double maxAxis3Acceleration;
    double axis1ParkPosition;
    double axis2ParkPosition;
    double axis3ParkPosition;
    unsigned short haveAxis3;
    unsigned short haveAxis5;
    double minElevation;
    unsigned short numTemperatureSensor;
    unsigned short numHumiditySensor;
    unsigned short canConnect;
    unsigned short canFindHome;
    unsigned short canTrackStar;
    unsigned short canSetObjectName;
    unsigned short canSlewAzEl;
    unsigned short canSlewDerotator;
    unsigned short canConfigDerotator;
    unsigned short canStop;
    unsigned short canSetTrackSpeed;
    unsigned short canPark;
    unsigned short canFixedMove;
    unsigned short canPositionCorrect;
    unsigned short canCoverOperation;
    unsigned short canFocusOperation;
    unsigned short canScram;
    unsigned short canSaveSyncData;
    unsigned short canTrackSatellite;
    unsigned short canConfigProperity;
};
#pragma pack()
//
//
/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_STATUS defines Gimbal's real time status.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   614 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 *              unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_STATUS{
    _AT_PUBLIC_STATUS status;
    double siderealTime;
    double hourAngle;
    double rightAscension;
    double declination;
    double J2000RightAscension;
    double J2000Declination;
    double azmiuth;
    double elevation;
    double derotatorPositon;
    double targetRightAscension;
    double targetDeclination;
    double targetJ2000RightAscension;
    double targetJ2000Declination;
    double targetAzmiuth;
    double targetElevation;
    double targetDerotatorPosition;
    double axis1TrackError;
    double axis2TrackError;
    double axis3TrackError;
    unsigned short focusTypeIndex;
    unsigned short targetFocusTypeIndex;
    unsigned short coverIndex;
    unsigned short targetCoverIndex;
    double axis4Angle;    
    double coverPosition;    
    double targetAxis4Angle;    
    double axis1Speed;
    double axis2Speed;
    double axis3Speed;
    unsigned short trackType;
    unsigned short axis3Mode;
    char trackObjectName[48];
    double refractionCorrection;
    double axis1Encoder;
    double axis2Encoder;
    double axis3Encoder;    
    double axis1PMCorrection;
    double axis2PMCorrection;
    double axis3PMCorrection;
    double axis1ManualCorrection;
    double axis2ManualCorrection;
    double axis3ManualCorrection;
    double temperature[12];
    double humidity[12];
    unsigned short pieSide;
    unsigned short targetPieSide;
    unsigned short isAxis1FindHome;
    unsigned short isAxis2FindHome;
    unsigned short isAxis3FindHome;
};
#pragma pack()
//
/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_CONNECT defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_CONNECT] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_CONNECT{
    //1:connect, 2:disconnect
    //according to ENUM_CONNECT
    unsigned short connect;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_FINDHOME defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_FINDHOME] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_FINDHOME{
    unsigned short axis;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_TRACKSTAR defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_TRACKSTAR] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   20 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 *              unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_TRACKSTAR{
    double rightAscension;
    double declination;
    unsigned short epoch;
    unsigned short speed;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_SETOBJECTNAME defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_SETOBJECTNAME] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   50 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  char: 1 bytes.
 *              unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_SETOBJECTNAME{
    char objectName[48];
    unsigned short objectType;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_SLEWAZEL defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_SLEWAZEL] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   16 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_SLEWAZEL{
    double azimuth;
    double elevation;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_SLEWDEROTATOR defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_SLEWDEROTATOR] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   8 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_SLEWDEROTATOR{
    double position;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_CONFIGDEROTATOR defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_CONFIGDEROTATOR] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   10 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 *              unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_CONFIGDEROTATOR{
    unsigned short mode;
    double polarizingAngle; //only used in mode 2
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_SPEEDCORRECT defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_SPEEDCORRECT] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   10 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 *              unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_SPEEDCORRECT{
    unsigned short axis;
    double correction;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_FIXEDMOVE defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_FIXEDMOVE] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   10 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 *              unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_FIXEDMOVE{
    unsigned short axis;
    double speed;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_POSITIONCORRECT defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_POSITIONCORRECT] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   10 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 *              unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_POSITIONCORRECT{
    unsigned short axis;
    double correction;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_COVERACTION defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_COVERACTION] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_COVERACTION{
    unsigned short action;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_GIMBAL_PARAM_FOCUSACTION defines Gimbal's
 *          [_GIMBAL_INSTRUCTION_FOCUSACTION] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_GIMBAL_PARAM_FOCUSACTION{
    unsigned short action;
};
#pragma pack()


/*!
* ------------------------------------------------------------------------------
* @brief:   ENUM_AT_CCD_INSTRUCTION enumerates CCD's instruction.
* ------------------------------------------------------------------------------
*/
enum ENUM_AT_CCD_INSTRUCTION
{
    _CCD_INSTRUCTION_CONNECT = 0x01,
    _CCD_INSTRUCTION_SETCOOLERT,            // =2
    _CCD_INSTRUCTION_SETEXPOSURETACTIC,     // =3
    _CCD_INSTRUCTION_STARTEXPOSURE,         // =4
    _CCD_INSTRUCTION_STOPEXPOSURE,          // =5
    _CCD_INSTRUCTION_ABORTEXPOSURE,         // =6
    _CCD_INSTRUCTION_SETGAIN,               // =7
    _CCD_INSTRUCTION_SETRSMODE,             // =8
    _CCD_INSTRUCTION_SETTSMODE,             // =9
    _CCD_INSTRUCTION_SETBIN,                // =10
    _CCD_INSTRUCTION_SETROI,                // =11
    _CCD_INSTRUCTION_SETSHUTTER,            // =12
    _CCD_INSTRUCTION_SETFULLFRAME,          // =13
    _CCD_INSTRUCTION_SETEM,                 // =14
    _CCD_INSTRUCTION_SETNOISEFILTER,        // =15
    _CCD_INSTRUCTION_SETBASELINE,           // =16
    _CCD_INSTRUCTION_SETOVERSCAN            // =17
};

/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_CCD_ENUM_STATUS enumerates the CCD's status
 * -----------------------------------------------------------------------------
 */
enum ENUM__AT_CCD_STATUS
{
    _CCD_STATUS_DISCONNECT = 0x01,
    _CCD_STATUS_CONNECTING,
    _CCD_STATUS_DISCONNECTING,
    _CCD_STATUS_WAITINGEXPOSURE,
    _CCD_STATUS_EXPOSING,
    _CCD_STATUS_READING,
    _CCD_STATUS_SAVING,
    _CCD_STATUS_ABORTING,
    _CCD_STATUS_IDLE
};


/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_CCD_ENUM_WARNINGS enumerates the CCD's warnings
 * -----------------------------------------------------------------------------
 */
enum ENUM_AT_CCD_WARNINGS
{
    _CCD_WARN_NORMAL,
    _CCD_WARN_TOPCOM,
    _CCD_WARN_COOLER,
    _CCD_WARN_FAN = 0x4,
    _CCD_WARN_IMG_TIMEOUT = 0x8,
    _CCD_WARN_SPACE_LESS100 = 0x10
};

/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_CCD_ENUM_ERRORS enumerates the CCD's errors
 * -----------------------------------------------------------------------------
 */
enum ENUM_AT_CCD_ERRORS
{
    _CCD_ERROR_NORMAL,
    _CCD_ERROR_CCDCOM,
    _CCD_ERROR_EXPOSURE_START_TIMEOUT,
    _CCD_ERROR_EXPOSURE_TIMEOUT = 0x4,
    _CCD_ERROR_READ_TIMEOUT = 0x8,
    _CCD_ERROR_EXPOSURE_STOP_TIMEOUT = 0x10,
    _CCD_ERROR_EXPOSURE_ABORT_TIMEOUT = 0x20,
    _CCD_ERROR_SPACE_LESS10 = 0x40
};


/*!
*-------------------------------------------------------------------------------
* @brief:       _AT_CCD_ATTRIBUTES defines CCD's attributes.
*-------------------------------------------------------------------------------
 * @size:       208 bytes.
 * -----------------------------------------------------------------------------
* @author Geniuswei
* @date 2016-12-26 11:25
*-------------------------------------------------------------------------------
*/
#pragma pack(1)
struct _AT_CCD_PROPERTY{
    _AT_PUBLIC_PROPERTY header;
    unsigned int xPixel;
    unsigned int yPixel;
    double xPixelSize;
    double yPixelSize;
    char sensorName[20];
    unsigned short imageBits;
    unsigned short coolerMode;
    double minCoolerTemperature;
    double maxExposureTime;
    double minExposrueTime;
    unsigned int exposureTimeResolution;
    unsigned int fullWellDepth;
    unsigned short shutterType;
    unsigned short isFullFrame;
    unsigned short isEM;
    unsigned short isNoiseFilter;
    unsigned short isBaseline;
    unsigned short isOverScan;
    unsigned short isROI;
    unsigned int maxEM;
    unsigned int minEM;
    unsigned short canConnect;
    unsigned short canSetCoolerT;
    unsigned short canSetExposureParam;
    unsigned short canStartExposure;
    unsigned short canStopExposure;
    unsigned short canAbortExposure;
    unsigned short canSetGain;
    unsigned short canSetReadSpeedMode;
    unsigned short canSetTransferSpeedMode;
    unsigned short canSetBin;
    unsigned short canSetROI;
    unsigned short canSetShutter;
    unsigned short canSetFullFrame;
    unsigned short canSetEM;
    unsigned short canNoiseFilter;
    unsigned short canSetBaseline;
    unsigned short canOverScan;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_STATUS defines CCD's real time status.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   330 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  char: 1 bytes.
 *              double: 8 bytes.
 *              unsigned short: 2 bytes.
 *              unsigned int: 4 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_STATUS{
    _AT_PUBLIC_STATUS status;
    char observeName[48];
    unsigned short observeType;
    unsigned short epoch;
    double J2000RightAscension;
    double J2000Declination;    
    char band[8];
    unsigned int binX;
    unsigned int binY;
    unsigned int startX;
    unsigned int startY;
    unsigned int imageWidth;
    unsigned int imageHeight;
    double exposureTime;
    double delayTime;
    unsigned int frameSequence;
    unsigned int frameTotal;
    unsigned short indexOfRSMode;
    unsigned short indexOfTSMode;
    unsigned short indexOfGainMode;
    unsigned short indexOfGain;
    double coolerT;
    double targetCoolerT;
    char lastTargetName[48];
    unsigned int lastExposureTime;
    double lastExposureDuration;    
    double exposurePercent;
    unsigned short bitsOfPixel;    
    unsigned short readMode;
    unsigned short exposureTriggerMode;
    unsigned short fanOn;
    unsigned short shutterMode;
    unsigned short isFullFrame;
    unsigned short isEM;
    unsigned short isNoiseFilter;
    unsigned short isBaseline;
    unsigned int EM;    
    unsigned int baseline;
    double pixelScaleX;
    double pixelScaleY;	
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_CONNECT defines CCD's
 *          [_CCD_INSTRUCTION_CONNECT] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_CONNECT{
    //1:connect, 2:disconnect
    //according to ENUM_CONNECT
    unsigned short connect;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETCOOLERT defines CCD's
 *          [_CCD_INSTRUCTION_SETCOOLERT] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   8 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETCOOLERT{
    double temperature;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETEXPOSURETATIC defines CCD's
 *          [_CCD_INSTRUCTION_SETEXPOSURETATIC] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   264 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  double: 8 bytes.
 *              unsigned long long: 8 bytes.
 *              char: 1 bytes.
 *              unsigned int: 4 bytes.
 *              unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETEXPOSURETACTIC
{
    unsigned long long validFlag;
    unsigned int startTime;
    double duration;
    double delay;
    char objectName[48];
    unsigned short objectType;
    double objectRightAscension;
    double objectDeclination;
    unsigned short objectEpoch;
    char objectBand[8];
    unsigned short objectFilter;
    unsigned short isSaveImage;
    unsigned int weatherGatherTime;
    double temperature;
    double humidity;
    double windSpeed;
    double pressure;
    unsigned int skyGatherTime;
    unsigned short skyState;
    unsigned short clouds;
    unsigned int seeingGatherTime;
    double seeing;
    unsigned int dustGatherTime;
    double dust;
    double AMS;
    unsigned int extinctionGatherTime;
    double rightAscension;
    double declination;
    char band[8];
    double extinctionFactor1;
    double extinctionFactor2;
    double extinctionFactor3;
    double telescopeRightAscension;
    double telescopeDeclination;
    double focusLength;
    unsigned int frameNum;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_STARTEXPOSURE defines CCD's
 *          [_CCD_INSTRUCTION_STARTEXPOSURE] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   6 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned int: 4 bytes.
 *              unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_STARTEXPOSURE{
    unsigned short isReadFrameSeq;
    unsigned int frameSequence;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETGAIN defines CCD's
 *          [_CCD_INSTRUCTION_SETGAIN] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   4 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETGAIN{
    unsigned short mode;
    unsigned short gear;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETRSMODE defines CCD's
 *          [_CCD_INSTRUCTION_SETRSMODE] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETRSMODE{
    unsigned short mode;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETTSMODE defines CCD's
 *          [_CCD_INSTRUCTION_SETTSMODE] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETTSMODE{
    unsigned short mode;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETBIN defines CCD's
 *          [_CCD_INSTRUCTION_SETBIN] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   8 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETBIN
{
    unsigned int binX;
    unsigned int binY;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETROI defines CCD's
 *          [_CCD_INSTRUCTION_SETROI] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   16 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETROI
{
    unsigned int startX;
    unsigned int startY;
    unsigned int imageWidth;
    unsigned int imageHeight;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETSHUTTER defines CCD's
 *          [_CCD_INSTRUCTION_SETSHUTTER] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETSHUTTER
{
    unsigned short shutter;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETFULLFRAME defines CCD's
 *          [_CCD_INSTRUCTION_SETFULLFRAME] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETFULLFRAME
{
    unsigned short fullFrame;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETEM defines CCD's
 *          [_CCD_INSTRUCTION_SETEM] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   6 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETEM{
    unsigned short isEM;
    unsigned int EM;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETNOISEFILTER defines CCD's
 *          [_CCD_INSTRUCTION_SETNOISEFILTER] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETNOISEFILTER{
    unsigned short isNoiseFilter;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETBASELINE defines CCD's
 *          [_CCD_INSTRUCTION_SETBASELINE] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   6 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETBASELINE{
    unsigned short isBaseline;
    unsigned int baseline;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_CCD_PARAM_SETOVERSCAN defines CCD's
 *          [_CCD_INSTRUCTION_SETOVERSCAN] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_CCD_PARAM_SETOVERSCAN{
    unsigned short isOverScan;
};
#pragma pack()



/*!
* ----------------------------------------------------------------------------------------------------
* @brief    ENUM_AT_FILTER_INSTRUCTION enumerates Filter's instruction.
* ----------------------------------------------------------------------------------------------------
*/
enum ENUM_AT_FILTER_INSTRUCTION
{
    _FILTER_INSTRUCTION_CONNECT = 1,
    _FILTER_INSTRUCTION_SETPOSITION,
    _FILTER_INSTRUCTION_FINDHOME
};

/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_FILTER_ENUM_STATUS enumerates the Filter's status
 * -----------------------------------------------------------------------------
 */
enum ENUM__AT_FILTER_STATUS
{
    _FILTER_STATUS_DISCONNECT = 0x01,
    _FILTER_STATUS_CONNECTING,
    _FILTER_STATUS_DISCONNECTING,
    _FILTER_STATUS_FREEZING,
    _FILTER_STATUS_FREEZED,
    _FILTER_STATUS_SLEWING,
    _FILTER_STATUS_SLEWED,
    _FILTER_STATUS_NOTFINDHOME,
    _FILTER_STATUS_HOMING,    
    _FILTER_STATUS_EMERGENCYING,
    _FILTER_STATUS_EMERGENCY
};



#pragma pack(1)
struct _AT_FILTER_PROPERTY{
    _AT_PUBLIC_PROPERTY header;
    unsigned int filterSize[3];
    unsigned short numFilter;    
    unsigned short filterShape;
    unsigned short canSetFilterPosition;
    unsigned short canConnect;
    unsigned short canFindHome;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FILTER_STATUS defines Filter's real time status.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   78 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  char: 1 bytes.
 *              double: 8 bytes.
 *              unsigned short: 2 bytes.
 *              unsigned int: 4 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FILTER_STATUS{
    _AT_PUBLIC_STATUS status;
    unsigned short filterPosition;
    unsigned short targetFilterPosition;
    unsigned short isHomed;	
};
#pragma pack()



/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FILTER_PARAM_CONNECT defines Filter's
 *          [_FILTER_INSTRUCTION_CONNECT] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FILTER_PARAM_CONNECT{
    //1:connect, 2:disconnect
    //according to ENUM_CONNECT
    unsigned short connect;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FILTER_PARAM_SETPOSITION defines Filter's
 *          [_FILTER_INSTRUCTION_SETPOSITION] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FILTER_PARAM_SETPOSITION{
    unsigned short position;
};
#pragma pack()



/*!
* ----------------------------------------------------------------------------------------------------
* @brief    ENUM_AT_FOCUS_INSTRUCTION enumerates Focus's instrution.
* ----------------------------------------------------------------------------------------------------
*/
enum ENUM_AT_FOCUS_INSTRUCTION
{
    _FOCUS_INSTRUCTION_CONNECT = 0x01,          // =1
    _FOCUS_INSTRUCTION_SETPOSITION,             // =2
    _FOCUS_INSTRUCTION_SETFIXEDSPEED,           // =3
    _FOCUS_INSTRUCTION_STOP,                    // =4
    _FOCUS_INSTRUCTION_ENABLETCOMPENSATE,       // =5
    _FOCUS_INSTRUCTION_SETTCOMPENSATE,          // =6
    _FOCUS_INSTRUCTION_FINDHOME                 // =7
};

/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_FOCUS_STATUS enumerates the Filter's status
 * -----------------------------------------------------------------------------
 */
enum ENUM__AT_FOCUS_STATUS
{
    _FOCUS_STATUS_DISCONNECT = 0x01,
    _FOCUS_STATUS_CONNECTING,                   // =2
    _FOCUS_STATUS_DISCONNECTING,                // =3
    _FOCUS_STATUS_FREEZING,                     // =4
    _FOCUS_STATUS_FREEZED,                      // =5
    _FOCUS_STATUS_SLEWING,                      // =6
    _FOCUS_STATUS_SLEWED,                       // =7
    _FOCUS_STATUS_NOTFINDHOME,                  // =8
    _FOCUS_STATUS_HOMING,                       // =9
    _FOCUS_STATUS_EMERGENCYING,                 // =10
    _FOCUS_STATUS_EMERGENCY                     // =11
};



#pragma pack(1)
typedef struct FocusAttributes{
    _AT_PUBLIC_PROPERTY header;
    double maxValue;
    double minValue;
    double increment;
    unsigned short isFindHome;
    unsigned short isTCompensate;
    double maxSpeed;
    unsigned short canConnect;
    unsigned short canSetPosition;
    unsigned short canSetSpeed;
    unsigned short canStop;
    unsigned short canEnableTCompensate;
    unsigned short canFindHome;
}_AT_FOCUS_ATTRIBUTES;
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FOCUS_STATUS defines CCD's real time status.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   108 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  char: 1 bytes.
 *              double: 8 bytes.
 *              unsigned short: 2 bytes.
 *              unsigned int: 4 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FOCUS_STATUS{    
    _AT_PUBLIC_STATUS status;
    double position;
    double targetPosition;
    double temperature;    
    double TCompenensation;
    unsigned short isHomed;
    unsigned short isTCompensation;    
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FOCUS_PARAM_CONNECT defines Focus's
 *          [_FOCUS_INSTRUCTION_CONNECT] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FOCUS_PARAM_CONNECT{
    //1:connect, 2:disconnect
    //according to ENUM_CONNECT
    unsigned short connect;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FOCUS_PARAM_SETPOSITION defines Focus's
 *          [_FOCUS_INSTRUCTION_SETPOSITION] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   8 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FOCUS_PARAM_SETPOSITION{
    double position;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FOCUS_PARAM_SETFIXEDSPEED defines Focus's
 *          [_FOCUS_INSTRUCTION_SETFIXEDSPEED] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   8 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FOCUS_PARAM_SETFIXEDSPEED{
    double speed;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FOCUS_PARAM_ENABLETCOMPENSATE defines Focus's
 *          [_FOCUS_INSTRUCTION_ENABLETCOMPENSATE] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FOCUS_PARAM_ENABLETCOMPENSATE{
    unsigned short enbale;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FOCUS_PARAM_SETTCOMPENSATE defines Focus's
 *          [_FOCUS_INSTRUCTION_SETTCOMPENSATE] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   8 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FOCUS_PARAM_SETTCOMPENSATE
{
    double coefficient;
};
#pragma pack()


/*!
* ------------------------------------------------------------------------------
* @brief    ENUM_AT_SLAVEDOME_INSTRUCTION enumerates slavedome's instruction.
* ------------------------------------------------------------------------------
*/
enum ENUM_AT_SLAVEDOME_INSTRUCTION
{
    _SLAVEDOME_INSTRUCTION_CONNECT =1,
    _SLAVEDOME_INSTRUCTION_SETDOMEPOSITION,     // =2
    _SLAVEDOME_INSTRUCTION_SETSHADEPOSITION,    // =3
    _SLAVEDOME_INSTRUCTION_SETROTATESPEED,      // =4
    _SLAVEDOME_INSTRUCTION_STOP,                // =5
    _SLAVEDOME_INSTRUCTION_SCUTTLEACTION,       // =6
    _SLAVEDOME_INSTRUCTION_SHADEACTION          // =7
};

/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_SLAVEDOME_STATUS enumerates the SlaveDome's status
 * -----------------------------------------------------------------------------
 */
enum ENUM_AT_SLAVEDOME_STATUS
{
    _SLAVEDOME_STAUTS_DISCONNECT = 0x01,
    _SLAVEDOME_STAUTS_CONNECTING,
    _SLAVEDOME_STAUTS_DISCONNECTING,
    _SLAVEDOME_STAUTS_FREEZING,
    _SLAVEDOME_STAUTS_FREEZED,
    _SLAVEDOME_STAUTS_PARKING,
    _SLAVEDOME_STAUTS_PARKED,
    _SLAVEDOME_STAUTS_SLEWING,
    _SLAVEDOME_STAUTS_SLEWED,
    _SLAVEDOME_STAUTS_WAITINGFOLLOWING,
    _SLAVEDOME_STAUTS_FOLLOWING,    
    _SLAVEDOME_STAUTS_ERROR
};



/*!
* ----------------------------------------------------------------------------------------------------
* @brief    _AT_FULLOPENEDDOME_INSTRUCTION enumerates Full Opened Dome's instruction
* ----------------------------------------------------------------------------------------------------
*/
enum ENUM_AT_FULLOPENEDDOME_INSTRUCTION
{
     _FULLOPENEDDOME_INSTRUCTION_CONNECT = 0x01,
     _FULLOPENEDDOME_INSTRUCTION_OPENDOME
};

/**
 * -----------------------------------------------------------------------------
 * @brief ENUM_AT_FULLOPENEDDOME_STATUS enumerates the FullOpenedDome's status
 * -----------------------------------------------------------------------------
 */
enum ENUM_AT_FULLOPENEDDOME_STATUS
{
    _FULLOPENEDDOME_STATUS_DISCONNECT = 0x01,
    _FULLOPENEDDOME_STATUS_CONNECTING,
    _FULLOPENEDDOME_STATUS_DISCONNECTING,
    _FULLOPENEDDOME_STATUS_FREEZING,
    _FULLOPENEDDOME_STATUS_FREEZED,
    _FULLOPENEDDOME_STATUS_OPENING,
    _FULLOPENEDDOME_STATUS_CLOSING,
    _FULLOPENEDDOME_STATUS_OPENED,
    _FULLOPENEDDOME_STATUS_CLOSED
};


#pragma pack(1)
typedef struct SlaveDomeAttributes{
    _AT_PUBLIC_PROPERTY header;
    unsigned short hasShade;
    double maxSpeed;
    double diameter;
    unsigned short canSetDomePosition;
    unsigned short canSetShadePosition;
    unsigned short canSetSpeed;
    unsigned short canStop;
    unsigned short canOpenScuttle;
    unsigned short canSetShadeSpeed;
    unsigned short canConnect;
}_AT_SLAVEDOME_ATTRIBUTES;
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_SLAVEDOME_STATUS defines SlaveDome's real time status.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   124 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_SLAVEDOME_STATUS{
    _AT_PUBLIC_STATUS status;
    unsigned short scuttleStatus;
    unsigned short shadeStatus;
    double domePosition;
    double scuttlePercent;
    double shadePosition;
    double targetDomePosition;
    double targetScuttlePosition;
    double targetShadePosition;	
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_SLAVEDOME_PARAM_CONNECT defines Gimbal's
 *          [_SLAVEDOME_INSTRUCTION_CONNECT] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_SLAVEDOME_PARAM_CONNECT{
    //1:connect, 2:disconnect
    //according to ENUM_CONNECT
    unsigned short connect;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_SLAVEDOME_PARAM_SETDOMEPOSITION defines FullOpenedDome's
 *          [_SLAVEDOME_INSTRUCTION_SETDOMEPOSITION] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   8 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_SLAVEDOME_PARAM_SETDOMEPOSITION{
    double position;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_SLAVEDOME_PARAM_SETSHADEPOSITION defines FullOpenedDome's
 *          [_SLAVEDOME_INSTRUCTION_SETSHADEPOSITION] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   8 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_SLAVEDOME_PARAM_SETSHADEPOSITION{
    double position;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_SLAVEDOME_PARAM_SETROTATESPEED defines FullOpenedDome's
 *          [_SLAVEDOME_INSTRUCTION_SETROTATESPEED] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   8 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_SLAVEDOME_PARAM_SETROTATESPEED{
    double speed;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_SLAVEDOME_PARAM_SHUTTERACTION defines FullOpenedDome's
 *          [_SLAVEDOME_INSTRUCTION_SHUTTERACTION] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_SLAVEDOME_PARAM_SHUTTERACTION{
    unsigned short action;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_SLAVEDOME_PARAM_SHADEACTION defines FullOpenedDome's
 *          [_SLAVEDOME_INSTRUCTION_SHADEACTION] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_SLAVEDOME_PARAM_SHADEACTION{
    unsigned short action;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FULLOPENEDDOME_STATUS defines FullOpenedDome's property.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   72 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FULLOPENEDDOME_PROPERTY{
    _AT_PUBLIC_PROPERTY header;
    double diameter;
    unsigned short canConnect;
    unsigned short canOpenDome;
};
#pragma pack()

/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FULLOPENEDDOME_STATUS defines FullOpenedDome's real time status.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   80 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FULLOPENEDDOME_STATUS{
    _AT_PUBLIC_STATUS status;
    double domePosition;
	
};
#pragma pack()



/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FULLOPENEDDOME_PARAM_CONNECT defines FullOpenedDome's
 *          [_FULLOPENEDDOME_INSTRUCTION_CONNECT] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FULLOPENEDDOME_PARAM_CONNECT{
    //1:connect, 2:disconnect
    //according to ENUM_CONNECT
    unsigned short connect;
};
#pragma pack()


/**
 * -----------------------------------------------------------------------------
 * @brief:  _AT_FULLOPENEDOME_PARAM_OPENDOME defines FullOpenedDome's
 *          [_FULLOPENEDDOME_INSTRUCTION_OPENDOME] parameter.
 * -----------------------------------------------------------------------------
 * @version:2.00.00 
 * @size:   2 bytes.
 * @date:   2017-05-15
 * @author: Geniuswei
 * -----------------------------------------------------------------------------
 * @type-info:  unsigned short: 2 bytes.
 * -----------------------------------------------------------------------------
 */
#pragma pack(1)
struct _AT_FULLOPENEDDOME_PARAM_OPENDOME{
    unsigned short action;
};
#pragma pack()
}
#endif