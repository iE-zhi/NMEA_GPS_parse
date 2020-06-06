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

## 科普
### GPS语句含义
- GPGSV：可见卫星信息
- GPGLL：地理定位信息
- GPRMC：推荐最小定位信息
- GPVTG：地面速度信息
- GPGGA：GPS定位信息
- GPGSA：当前卫星信息

### GPRMC 最小定位信息：
例：$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A\*50  

字段0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息  
字段1：UTC时间，hhmmss.sss格式  
字段2：状态，A=定位，V=未定位  
字段3：纬度ddmm.mmmm，度分格式（前导位数不足则补0）  
字段4：纬度N（北纬）或S（南纬）  
字段5：经度dddmm.mmmm，度分格式（前导位数不足则补0）  
字段6：经度E（东经）或W（西经）  
字段7：速度，节，Knots  
字段8：方位角，度  
字段9：UTC日期，DDMMYY格式  
字段10：磁偏角，（000 - 180）度（前导位数不足则补0）  
字段11：磁偏角方向，E=东W=西  
字段12：模式，A=自动，D=差分，E=估测，N=数据无效（3.0协议内容）  
字段13：校验值（$与\*之间的数异或后的值）  
### GPGGA GPS定位数据
例：$GPGGA,092204.999,4250.5589,S,14718.5084,E,1,04,24.4,12.2,M,19.7,M,,0000\*1F  
  
字段0：$GPGGA，语句ID，表明该语句为Global Positioning System Fix Data（GGA）GPS定位信息  
字段1：UTC 时间，hhmmss.sss，时分秒格式  
字段2：纬度ddmm.mmmm，度分格式（前导位数不足则补0）  
字段3：纬度N（北纬）或S（南纬）  
字段4：经度dddmm.mmmm，度分格式（前导位数不足则补0）  
字段5：经度E（东经）或W（西经）  
字段6：GPS状态，0=不可用(FIX NOT valid)，1=单点定位(GPS FIX)，2=差分定位(DGPS)，3=无效PPS，4=实时差分定位（RTK FIX），5=RTK FLOAT，6=正在估算  
字段7：正在使用的卫星数量（00 - 12）（前导位数不足则补0）  
字段8：HDOP水平精度因子（0.5 - 99.9）  
字段9：海拔高度（-9999.9 - 99999.9）  
字段10：单位：M（米）  
字段11：地球椭球面相对大地水准面的高度 WGS84水准面划分  
字段12：WGS84水准面划分单位：M（米）  
字段13：差分时间（从接收到差分信号开始的秒数，如果不是差分定位将为空）  
字段14：差分站ID号0000 - 1023（前导位数不足则补0，如果不是差分定位将为空）  
字段15：校验值（$与\*之间的数异或后的值）  
  
### GPVTG 地面速度信息
例：$GPVTG,89.68,T,,M,0.00,N,0.0,K*5F  

字段0：$GPVTG，语句ID，表明该语句为Track Made Good and Ground Speed（VTG）地面速度信息   
字段1：运动角度，000 - 359，（前导位数不足则补0）  
字段2：T=真北参照系  
字段3：运动角度，000 - 359，（前导位数不足则补0）  
字段4：M=磁北参照系  
字段5：水平运动速度（0.00）（前导位数不足则补0）  
字段6：N=节，Knots  
字段7：水平运动速度（0.00）（前导位数不足则补0）  
字段8：K=公里/时，km/h  
字段9：校验值（$与\*之间的数异或后的值）  

### GPGSV 可视卫星状态
例：$GPGSV,3,1,10,20,78,331,45,01,59,235,47,22,41,069,,13,32,252,45\*70  

字段0：$GPGSV，语句ID，表明该语句为GPS Satellites in View（GSV）可见卫星信息  
字段1：本次GSV语句的总数目（1 - 3）  
字段2：本条GSV语句是本次GSV语句的第几条（1 - 3）  
字段3：当前可见卫星总数（00 - 12）（前导位数不足则补0）  
字段4：PRN 码（伪随机噪声码）（01 - 32）（前导位数不足则补0）  
字段5：卫星仰角（00 - 90）度（前导位数不足则补0）  
字段6：卫星方位角（00 - 359）度（前导位数不足则补0）  
字段7：信噪比（00－99）dbHz  
字段8：PRN 码（伪随机噪声码）（01 - 32）（前导位数不足则补0）  
字段9：卫星仰角（00 - 90）度（前导位数不足则补0）  
字段10：卫星方位角（00 - 359）度（前导位数不足则补0）  
字段11：信噪比（00－99）dbHz  
字段12：PRN 码（伪随机噪声码）（01 - 32）（前导位数不足则补0）  
字段13：卫星仰角（00 - 90）度（前导位数不足则补0）  
字段14：卫星方位角（00 - 359）度（前导位数不足则补0）  
字段15：信噪比（00－99）dbHz  
字段16：校验值（$与\*之间的数异或后的值）  

### GPGSA 当前卫星信息
例：$GPGSA,A,3,01,20,19,13,,,,,,,,,40.4,24.4,32.2\*0A  

字段0：$GPGSA，语句ID，表明该语句为GPS DOP and Active Satellites（GSA）当前卫星信息  
字段1：定位模式(选择2D/3D)，A=自动选择，M=手动选择  
字段2：定位类型，1=未定位，2=2D定位，3=3D定位  
字段3：PRN码（伪随机噪声码），第1信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段4：PRN码（伪随机噪声码），第2信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段5：PRN码（伪随机噪声码），第3信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段6：PRN码（伪随机噪声码），第4信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段7：PRN码（伪随机噪声码），第5信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段8：PRN码（伪随机噪声码），第6信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段9：PRN码（伪随机噪声码），第7信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段10：PRN码（伪随机噪声码），第8信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段11：PRN码（伪随机噪声码），第9信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段12：PRN码（伪随机噪声码），第10信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段13：PRN码（伪随机噪声码），第11信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段14：PRN码（伪随机噪声码），第12信道正在使用的卫星PRN码编号（00）（前导位数不足则补0）  
字段15：PDOP综合位置精度因子（0.5 - 99.9）  
字段16：HDOP水平精度因子（0.5 - 99.9）  
字段17：VDOP垂直精度因子（0.5 - 99.9）  
字段18：校验值（$与\*之间的数异或后的值）  


## 代码分析
代码较多，这里只分析RMC、GSA和GSV三个语句的。  

### RMC解析
RMC结构体  
```bash  
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
```

RMC解析函数  
```bash
// RMC数据解析
static RMC rmc_data_parse(char *rmc_data)             // 定义RMC结构体函数
{
    RMC rmc;
    unsigned char times = 0;
    char *p;
    char *s = strdup(rmc_data);                       // 将传入的数据拷贝一份，否则分割函数会影响原始数据

    // strtok遇到两个连续的分割符(,)时，无法正常切割，所以自己写了个函数，在源文件中
    p = strsplit(&s, ",");                            // 开始分割，按','切分，p为切割出来的字段
    while (p)
    {
        switch (times)                                // times记录当前切割的位置
        {
            case 1:   // UTC
                strcpy(rmc.utc, p);
                break;
            case 2:   // pos status
                rmc.pos_status = p[0];
                break;
            case 3:   // lat
                rmc.lat = strtod(p, NULL);
                break;
            case 4:   // lat dir
                rmc.lat_dir = p[0];
                break;
            case 5:   // lon
                rmc.lon = strtod(p, NULL);
                break;
            case 6:   // lon dir
                rmc.lon_dir = p[0];
                break;
            case 7:   // speen Kn
                rmc.speed_Kn = strtod(p, NULL);
                break;
            case 8:   // track true
                rmc.track_true = strtod(p, NULL);
                break;
            case 9:   // date
                strcpy(rmc.date, p);
                break;
            case 10:  // mag var
                rmc.mag_var = strtod(p, NULL);
                break;
            case 11:  // var dir
                rmc.var_dir = p[0];
                break;
            case 14:  // mode ind
                rmc.mode_ind = p[0];
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");                          // 继续切割
        times++;
    }
    free(s);
    return rmc;
}
```

### GSA解析
GSA结构体  
```bash
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
```

GSA解析函数  
```bash
// 得到GSA数据中的信道信息,由于一帧GPS数据中，GSA语句的个数不一致，所以需要传入原始的GPS数据，找出所有的GSA字段
static GSA_PRN *get_prn_data(char *gps_data)
{
    GSA_PRN *gsa_prn;
    unsigned char times = 0;
    unsigned char i;
    unsigned char sentences_index = 0;  // 累计找到gsa字段的个数
    // 从语句中切割的字段
    char *p;
    // 存放拷贝的语句
    char *s;
    // 从原始数据中切割出来的字段
    char *sentences;
    // gsa语句的个数 
    int gsa_count;

    // 统计GSA字段的个数
    gsa_count = strstr_cnt(gps_data, PRE_GSA);
    
    // 根据GSA语句的个数，动态分配内存
    gsa_prn = (GSA_PRN *)malloc(sizeof(GSA_PRN) * (gsa_count * 12 + 1));
    memset(gsa_prn, 0, sizeof(GSA_PRN) * (gsa_count * 12 + 1));
    // 切割原始数据
    sentences = strtok(gps_data, "\r\n");
    while (sentences)
    {
        // 判断切割出来的语句是否是GSA语句
        if (strstr(sentences, PRE_GSA))
        {
            // 每找到一条GSA语句，就加1
            sentences_index++;
            s = strdup(sentences);
            // 开始切割语句
            p = strsplit(&s, ",");
            while (p)
            {
                if (times == 2)
                {
                    // 每条GSA语句包含12个卫星信息
                    for (i=0; i<12; i++)
                    {
                        p = strsplit(&s, ",");
                        (gsa_prn+i+(sentences_index-1)*12)->total = (unsigned char)gsa_count * 12;
                        (gsa_prn+i+(sentences_index-1)*12)->prn_ID = i + (sentences_index - 1) * 12;
                        (gsa_prn+i+(sentences_index-1)*12)->prn = (unsigned char)strtol(p, NULL, 10);
                    }
                }
                // 继续下一次语句切割
                p = strsplit(&s, ",");
                times++;
            }
            times = 0;
        }
        // 继续下一次原始数据切割
        sentences = strtok(NULL, "\r\n");
    }
    free(s);
    return gsa_prn;
}

// GSA数据解析
//gsa_data: 传入的GSA语句
//gpsdata: 传入的原始GPS数据
static GSA gsa_data_parse(char *gsa_data, char *gpsdata)
{
    GSA gsa;
    unsigned char times = 0;
    char *p;
    char *end;
    // GSA语句拷贝到s中，方便切割
    char *s = strdup(gsa_data);
    // 将原始数据拷贝一份
    char *alldata = strdup(gpsdata);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // mode_MA
                gsa.mode_MA = p[0];
                break;
            case 2:   // mode_123
                gsa.mode_123 = p[0];
                break;
            case 3:   // prn
                // 获得所有GSA语句中的PRN信息，传入原始的GPS数据
                gsa.gsa_prn = get_prn_data(alldata);
                break;
            case 15:  // pdop
                gsa.pdop = strtod(p, NULL);
                break;
            case 16:  // hdop
                gsa.hdop = strtod(p, NULL);
                break;
            case 17:  // vdop
                // 提取最后一个数据
                end = (char *)malloc(sizeof(p));
                strncpy(end, p, strlen(p)-3);
                end[strlen(p)-3] = '\0';
                gsa.vdop = strtod(end, NULL);
                free(end);
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return gsa;
}
```
### GSV解析
GSV结构体
```bash
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
```

GSV解析函数  
```bash
/*
 * function:  获取GSV字段中的GPS信息
 * gps_data:  最原始的GPS字符串，用于找到所有的GSV语句
 * stas:      卫星数量
 * prefix:    GSV信息字段前缀
*/
static SAT_INFO *get_sats_info(char *gps_data, unsigned char sats, char *prefix)
{
    SAT_INFO *sats_info;
    unsigned char times = 0;
    // 保存GSV语句总数
    unsigned char msgs = 0;
    // 记录当前时第几条GSV语句
    unsigned char msg = 0;
    // 存放计算完的for循环次数
    unsigned char for_times;
    unsigned char i;
    // 语句切割出来的字段
    char *p;
    // 拷贝一份语句便于切割
    char *s;
    // 从原始数据中切割出来的语句
    char *sentences;

    sats_info = (SAT_INFO *)malloc(sizeof(SAT_INFO) * (sats+1));
    memset(sats_info, 0, sizeof(SAT_INFO) * (sats+1));
    sentences = strtok(gps_data, "\r\n");
    while (sentences)
    {
        if (strstr(sentences, prefix))
        {
            s = strdup(sentences);
            p = strsplit(&s, ",");
            while (p)
            {
                switch (times)
                {
                    case 1:   // msgs
                        msgs = (unsigned char) strtol(p, NULL, 10);
                        break;
                    case 2:   // msg
                        msg = (unsigned char) strtol(p, NULL, 10);
                        break;
                    case 3:   // sat info
                        // 计算当前GSV语句卫星信息的个数，也就是for循环的次数
                        for_times = (msgs == msg) ? ((sats % 4) ? sats % 4 : 4) : 4;
                        for (i = 0; i < for_times; i++)
                        {
                            // 从第4个字段开始，每4段代表一个卫星的信息
                            p = strsplit(&s, ",");
                            (sats_info+(msg-1)*4+i)->prn = (unsigned char) strtol(p, NULL, 10);
                            p = strsplit(&s, ",");
                            (sats_info+(msg-1)*4+i)->elev = (unsigned char) strtol(p, NULL, 10);
                            p = strsplit(&s, ",");
                            (sats_info+(msg-1)*4+i)->azimuth = (unsigned short) strtol(p, NULL, 10);
                            p = strsplit(&s, ",");
                            (sats_info+(msg-1)*4+i)->SNR = (unsigned char) strtol(p, NULL, 10);
                        }
                        break;
                    default:
                        break;
                }
                p = strsplit(&s, ",");
                times++;
            }
            times = 0;
        }
        // 切割出下一个语句
        sentences = strtok(NULL, "\r\n");
    }
    free(s);
    return sats_info;
}

// GSV数据解析
// gsv_data: 传入的GSV语句，用于提取GSV语句总数和卫星总数
// gps_data: 原始的GPS数据，用于在函数get_sats_info中找到所有的GSV语句
// prefix：  GSV语句的前缀，根据不同的定位组合方式，在一组GPS数据中可能包含GPGSV、GLGSV和GNGSV，根据需要传入
static GSV gsv_data_parse(char *gsv_data, char *gps_data, char *prefix)
{
    GSV gsv;
    unsigned char times = 0;
    char *p;
    char *s = strdup(gsv_data);
    char *src_data = strdup(gps_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // msgs
                gsv.msgs = (unsigned char)strtol(p, NULL, 10);
                break;
            case 2:   // msg
                gsv.msg = (unsigned char)strtol(p, NULL, 10);
                break;
            case 3:   // sats
                gsv.sats = (unsigned char)strtol(p, NULL, 10);
                // 获得所有GSV语句中的卫星信息。传入原始的GPS数据，卫星总数和GSV语句的ID
                gsv.sat_info = get_sats_info(src_data, gsv.sats, prefix);
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return gsv;
}
```
