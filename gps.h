//
// Created by Administrator on 2020/6/4.
//

#ifndef __GPS_H__
#define __GPS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define float double
#define PRE_GGA     "GPGGA"
#define PRE_GLL     "GPGGA"
#define PRE_GSA     "GPGSA"
#define PRE_GSV     "GPGSV"
#define PRE_RMC     "GPRMC"
#define PRE_VTG     "GPVTG"

// GGA字段结构体（GPS定位数据）
typedef struct
{
    char utc[11];                     // UTC时间，格式为hhmmss.sss
    float lat;                        // 纬度，格式为ddmm.mmmm
    char lat_dir;                     // 纬度半球，N或S
    float lon;                        // 经度，格式为dddmm.mmmm
    char lon_dir;                     // 经度半球，E或W
    unsigned char quality;            // 0=定位无效，1=定位有效
    unsigned char sats;               // 使用卫星数量，从00到12
    float hdop;                       // 水平精确度，0.5到99.9，单位m
    float alt;                        // 海平面的高度，-9999.9到9999.9米
    float undulation;                 // 大地水准面高度，-9999.9到9999.9米
    unsigned char age;                // 差分时间
    unsigned short stn_ID;            // 差分站ID号0000 - 1023
} GGA;

// GPGLL数据结构体（地理定位信息）
typedef struct
{
    float lat;               // 纬度，格式为ddmm.mmmm
    char lat_dir;            // 纬度半球，N或S
    float lon;               // 经度，格式为dddmm.mmmm
    char lon_dir;            // 经度半球，E或W
    char utc[10];            // UTC时间，格式为hhmmss.ss
    char data_status;        // 状态标志位，A：有效，V无效
}GLL;

//GPGSA数据结构体（当前卫星信息）
typedef struct
{
    unsigned char mode_MA;            // 定位模式(选择2D/3D)，A=自动选择，M=手动选择
    unsigned char mode_123;           // 定位类型，1=未定位，2=2D定位，3=3D定位
    unsigned char prn1;               // 第1信道正在使用的卫星PRN码编号
    unsigned char prn2;
    unsigned char prn3;
    unsigned char prn4;
    unsigned char prn5;
    unsigned char prn6;
    unsigned char prn7;
    unsigned char prn8;
    unsigned char prn9;
    unsigned char prn10;
    unsigned char prn11;
    unsigned char prn12;
    float pdop;                      // PDOP综合位置精度因子（0.5 - 99.9）
    float hdop;                      // HDOP水平精度因子（0.5 - 99.9）
    float vhop;                      // VDOP垂直精度因子（0.5 - 99.9）
}GSA;

// GPGSV数据结构体(可见卫星信息)
typedef struct
{
    unsigned char msgs;               // 本次GSV语句的总数目（1 - 3）
    unsigned char msg;                // 本条GSV语句是本次GSV语句的第几条（1 - 3）
    unsigned char sats;               // 当前可见卫星总数（00 - 12）
    unsigned char prn1;               // PRN 码（伪随机噪声码）（01 - 32）
    unsigned char elev1;              // 卫星仰角（00 - 90）度
    unsigned short azimuth1;          // 卫星方位角（00 - 359）度
    unsigned char SNR1;               // 信噪比（00－99）dbHz
    unsigned char prn2;
    unsigned char elev2;
    unsigned short azimuth2;
    unsigned char SNR2;
    unsigned char prn3;
    unsigned char elev3;
    unsigned short azimuth3;
    unsigned char SNR3;
}GSV;

//RMC数据结构体（推荐定位信息数据格式）
typedef struct
{
    char utc[11];                     // UTC时间，hhmmss.sss格式
    unsigned char pos_status;         // 状态，A=定位，V=未定位
    float lat;                        // 纬度ddmm.mmmm，度分格式
    char lat_dir;                     // 纬度N（北纬）或S（南纬）
    float lon;                        // 经度dddmm.mmmm，度分格式
    char lon_dir;                     // 经度E（东经）或W（西经）
    float speed_Kn;                   // 速度
    float track_true;                 // 方位角，度
    char date[7];                     // UTC日期，DDMMYY格式
    float mag_var;                    // 磁偏角，（000 - 180）度
    char var_dir;                     // 磁偏角方向，E=东W=西
    char mode_ind;                    // 模式，A=自动，D=差分，E=估测，N=数据无效（3.0协议内容）
}RMC;

//VTG数据结构体（地面速度信息）
typedef struct
{
    float track_true;                 // 运动角度，000 - 359，真北参照系
    float track_mag;                  // 运动角度，000 - 359，磁北参照系
    float speed_Kn;                   // 水平运动速度（0.00），节，Knots
    float speed_Km;                   // 水平运动速度（0.00）, 公里/时，km/h
}VTG;

//UTC时间结构体
typedef struct
{
    unsigned char YY;                 // 年
    unsigned char DD;                 // 日
    unsigned char MM;                 // 月
    unsigned char hh;                 // 时
    unsigned char mm;                 // 分
    unsigned char ss;                 // 秒
    unsigned char ds;                 // 毫秒
}UTC;

//定义GPS结构体
typedef struct
{
    GGA gga_data;
    GLL gll_data;
    GSA gsa_data;
    GSV gsv_data;
    RMC rmc_data;
    VTG vtg_data;
}GPS;

GPS gps_data_parse(char* gps_src);


#endif //__GPS_H__
