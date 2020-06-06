//
// Created by Administrator on 2020/6/4.
//

#ifndef __GPS_H__
#define __GPS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRE_GGA     "$GNGGA"
#define PRE_GLL     "$GNGLL"
#define PRE_GSA     "$GNGSA"
#define PRE_GPGSV   "$GPGSV"
#define PRE_GNGSV   "$GNGSV"
#define PRE_GLGSV   "$GLGSV"
#define PRE_RMC     "$GNRMC"
#define PRE_VTG     "$GNVTG"

#define ENABLE_GGA  1
#define ENABLE_GLL  1
#define ENABLE_GSA  1
#define ENABLE_GSV  1
#define ENABLE_RMC  1
#define ENABLE_VTG  1
#define ENABLE_UTC  1

#if ENABLE_GGA
// GGA字段结构体（GPS定位数据）
typedef struct
{
    char utc[11];                     // UTC时间，格式为hhmmss.sss
    double lat;                       // 纬度，格式为ddmm.mmmm
    char lat_dir;                     // 纬度半球，N或S
    double lon;                       // 经度，格式为dddmm.mmmm
    char lon_dir;                     // 经度半球，E或W
    unsigned char quality;            // 0=定位无效，1=定位有效
    unsigned char sats;               // 使用卫星数量，从00到12
    double hdop;                      // 水平精确度，0.5到99.9，单位m
    double alt;                       // 海平面的高度，-9999.9到9999.9米
    double undulation;                // 大地水准面高度，-9999.9到9999.9米
    unsigned char age;                // 差分时间
    unsigned short stn_ID;            // 差分站ID号0000 - 1023
} GGA;
#endif

#if ENABLE_GLL
// GPGLL数据结构体（地理定位信息）
typedef struct
{
    double lat;                       // 纬度，格式为ddmm.mmmm
    char lat_dir;                     // 纬度半球，N或S
    double lon;                       // 经度，格式为dddmm.mmmm
    char lon_dir;                     // 经度半球，E或W
    char utc[11];                     // UTC时间，格式为hhmmss.sss
    char data_status;                 // 状态标志位，A：有效，V无效
}GLL;
#endif

#if ENABLE_GSA
#pragma pack(1)                       // 便于指针偏移取值
// 信道信息结构体
typedef struct
{
    unsigned char total;              // 总信道个数
    unsigned char prn_ID;             // prn信道
    unsigned char prn;                // PRN 码（伪随机噪声码）
}GSA_PRN;
#pragma pack()

//GPGSA数据结构体（当前卫星信息）
typedef struct
{
    unsigned char mode_MA;            // 定位模式(选择2D/3D)，A=自动选择，M=手动选择
    unsigned char mode_123;           // 定位类型，1=未定位，2=2D定位，3=3D定位
    double pdop;                      // PDOP综合位置精度因子（0.5 - 99.9）
    double hdop;                      // HDOP水平精度因子（0.5 - 99.9）
    double vdop;                      // VDOP垂直精度因子（0.5 - 99.9）
    GSA_PRN *gsa_prn;                 // 存放信道信息
}GSA;
#endif

#if ENABLE_GSV
#pragma pack(1)                       // 便于指针偏移取值
// 可见卫星信息结构体
typedef struct
{
    unsigned char prn;                // PRN 码（伪随机噪声码）
    unsigned char elev;               // 卫星仰角（00 - 90）度
    unsigned short azimuth;           // 卫星方位角（00 - 359）度
    unsigned char SNR;                // 信噪比
}SAT_INFO;
#pragma pack()

// GPGSV数据结构体(可见卫星信息)
typedef struct
{
    unsigned char msgs;               // 本次GSV语句的总数目（1 - 3）
    unsigned char msg;                // 本条GSV语句是本次GSV语句的第几条（1 - 3）
    unsigned char sats;               // 当前可见卫星总数（00 - 12）
    SAT_INFO *sat_info;               // 卫星信息
}GSV;
#endif

#if ENABLE_RMC
//RMC数据结构体（推荐定位信息数据格式）
typedef struct
{
    char utc[11];                     // UTC时间，hhmmss.sss格式
    unsigned char pos_status;         // 状态，A=定位，V=未定位
    double lat;                       // 纬度ddmm.mmmm，度分格式
    char lat_dir;                     // 纬度N（北纬）或S（南纬）
    double lon;                       // 经度dddmm.mmmm，度分格式
    char lon_dir;                     // 经度E（东经）或W（西经）
    double speed_Kn;                  // 速度
    double track_true;                // 方位角，度
    char date[7];                     // UTC日期，DDMMYY格式
    double mag_var;                   // 磁偏角，（000 - 180）度
    char var_dir;                     // 磁偏角方向，E=东W=西
    char mode_ind;                    // 模式，A=自动，D=差分，E=估测，N=数据无效（3.0协议内容）
}RMC;
#endif

#if ENABLE_VTG
//VTG数据结构体（地面速度信息）
typedef struct
{
    double track_true;                // 运动角度，000 - 359，真北参照系
    double track_mag;                 // 运动角度，000 - 359，磁北参照系
    double speed_Kn;                  // 水平运动速度（0.00），节，Knots
    double speed_Km;                  // 水平运动速度（0.00）, 公里/时，km/h
}VTG;
#endif

#if ENABLE_UTC
//UTC时间结构体
typedef struct
{
    unsigned char YY;                 // 年
    unsigned char DD;                 // 日
    unsigned char MM;                 // 月
    unsigned char hh;                 // 时
    unsigned char mm;                 // 分
    unsigned char ss;                 // 秒
    unsigned short ds;                // 毫秒
}UTC;
#endif

//定义GPS结构体
typedef struct
{
#if ENABLE_GGA
    GGA gga_data;
#endif
#if ENABLE_GLL
    GLL gll_data;
#endif
#if ENABLE_GSA
    GSA gsa_data;
#endif
#if ENABLE_GSV
    GSV gpgsv_data;
    GSV gngsv_data;
    GSV glgsv_data;
#endif
#if ENABLE_RMC
    RMC rmc_data;
#endif
#if ENABLE_VTG
    VTG vtg_data;
#endif
#if ENABLE_UTC
    UTC utc;
#endif
}GPS;

GPS gps_data_parse(char* gps_src);

#endif //__GPS_H__
