# NMEA GPS PARSE
GPS数据解析开源项目。
测试数据:
```bash
"$GNRMC,013300.00,A,2240.84105,N,11402.70763,E,0.007,,220319,,,D*69\r\n"
"$GNVTG,,T,,M,0.007,N,0.014,K,D*3A\r\n"
"$GNGGA,013300.00,2240.84105,N,11402.70763,E,2,12,0.59,70.5,M,-2.5,M,,0000*68\r\n"
"$GNGSA,A,3,10,12,14,20,25,31,32,26,29,40,41,22,1.09,0.59,0.91*1F\r\n"
"$GNGSA,A,3,74,70,73,80,69,,,,,,,,1.09,0.59,0.91*17\r\n"
"$GPGSV,4,1,16,01,00,300,,10,56,178,51,12,12,038,38,14,47,345,48*79\r\n"
"$GPGSV,4,2,16,16,00,207,,18,06,275,30,20,28,165,43,22,10,319,43*76\r\n"
"$GPGSV,4,3,16,25,46,050,47,26,29,205,44,29,13,108,45,31,50,296,52*7E\r\n"
"$GPGSV,4,4,16,32,56,010,52,40,20,257,40,41,46,237,48,42,46,123,42*77\r\n"
"$GLGSV,2,1,06,69,27,136,49,70,76,057,50,71,34,338,50,73,64,276,55*6B\r\n"
"$GLGSV,2,2,06,74,24,231,46,80,35,019,46*60\r\n"
"$GNGLL,2240.84105,N,11402.70763,E,013300.00,A,D*7C\r\n";
```
## 使用说明
主要用到`gpc.c`和`gps.h`两个文件
- gps.h: 定义了GPS的数据结构，语句ID的宏定义，ID解析开关;
- gps.c: 实现语句的解析。

在项目中您只需要调用`gps_data_parse(char* gps_src)`函数，就可以获取到解析后的数据，具体使用方法请参考`main.c`文件。

您只需要修改`gps.h`文件中的两个地方，就可以完成配置：
1.以下宏定义参数需要和您的GPS数据相对应；
```bash
// 根据实际的数据修改前缀
#define PRE_GGA     "$GNGGA"
#define PRE_GLL     "$GNGLL"
#define PRE_GSA     "$GNGSA"
#define PRE_GPGSV   "$GPGSV"
#define PRE_GNGSV   "$GNGSV"
#define PRE_GLGSV   "$GLGSV"
#define PRE_RMC     "$GNRMC"
#define PRE_VTG     "$GNVTG"
```

2.通过以下宏定义，可以设置某个语句是否需要解析，从而减小您的代码体积。
```bash
// 语句解析控制 1：使能，0：禁用
#define ENABLE_GGA  1
#define ENABLE_GLL  1
#define ENABLE_GSA  1
#define ENABLE_GSV  1
#define ENABLE_RMC  1
#define ENABLE_VTG  1
#define ENABLE_UTC  1
```
## 注意事项
1.为确保您的正常使用，`gps.h`文件中的<语句解析控制>宏定义块应至少保持一项为1。

2.若是使用UTC解析，请确保`ENABLE_RMC`和`ENABLE_UTC`宏定义都处于 1状态。因为，UTC在解析过程中，会使用到RMC语句中的utc和time两个字段。

3.若是用到GSA和GSV两个语句，在使用完成后，请调用`free(gps.gsa_data.gsa_prn)`和`free(gps.gXgsv_data.sat_info)`来释放内存。因为，这两个语句中的卫星数据个数不确定，所以需要动态分配内存。

4.为确保数据正常解析，GPS数据中的每个语句应以\r\n结束符结尾。

