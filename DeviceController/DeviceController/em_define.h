#ifndef EM_DEFINE_H
#define EM_DEFINE_H

/**
该头文件定义环境监测设备上传数据结构体
**/

namespace ATCCS
{
/**
 * ASC realtime data structure.
 * The programmer have to guarantee that:
 * sizeof(double) == 8
 * sizeof(unsigned short) == 2 
 * sizeof(_EME_ASC_REALTIME_DATA) == 24
 */
#pragma pack(1)
struct _EME_ASC_REALTIME_DATA
{
    double shutterspeed;
    double aperture;
    unsigned short iso;
    unsigned short jpeg;
    unsigned short raw;
    unsigned short fits;
};
#pragma pack()

/**
 * WS realtime data structure.
 * The programmer have to guarantee that:
 * sizeof(double) == 8
 * sizeof(unsigned int) == 4 
 * sizeof(_EME_WS_REALTIME_DATA) == 84
 */
#pragma pack(1)
struct _EME_WS_REALTIME_DATA
{
    double temperature;
    double humidity;
    double dewpoint;
    double pressure;
    double windspeed;
    double windspeed2;
    double windspeed10;
    unsigned int windderection;
    double rainfall;
    double raidation;
    double illumination;
};
#pragma pack()

/**
 * WS realtime data structure.
 * The programmer have to guarantee that:
 * sizeof(double) == 8
 * sizeof(unsigned short) == 2 
 * sizeof(_EME_WS_REALTIME_DATA) == 62
 */
#pragma pack(1)
struct _EME_CS_REALTIME_DATA
{
    double RelSkyT;
    double AmbientT;
    double SensorT;
    unsigned short RainF;
    unsigned short RainThreshV;
    double wind;
    unsigned short WindUnits;
    double HumidityPercent;
    double DewPointT;
    double DayLightV;
};
#pragma pack()


/**
 * WS realtime data structure.
 * The programmer have to guarantee that:
 * sizeof(double) == 8
 * sizeof(_EME_SQM_REALTIME_DATA) == 8
 */
#pragma pack(1)
struct _EME_SQM_REALTIME_DATA
{
    double sqmdata;
};
#pragma pack()


/**
 * WS realtime data structure.
 * The programmer have to guarantee that:
 * sizeof(double) == 8
 * sizeof(_EME_DUST_REALTIME_DATA) == 8
 */
#pragma pack(1)
struct _EME_DUST_REALTIME_DATA
{
    double dust_data;
};
#pragma pack()

/**
 * WS realtime data structure.
 * The programmer have to guarantee that:
 * sizeof(double) == 8
 * sizeof(_EME_DIMM_REALTIME_DATA) == 8
 */
#pragma pack(1)
struct _EME_DIMM_REALTIME_DATA
{
    double dimm_data;
};
#pragma pack()

}


#endif