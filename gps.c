//
// Created by ihz on 2020/6/4.
//
#include "gps.h"

// 数据分割，可以分割两个连续的分隔符
static char* strsplit(char** stringp, const char* delim)
{
    char* start = *stringp;
    char* p;

    p = (start != NULL) ? strpbrk(start, delim) : NULL;

    if (p == NULL)
    {
        *stringp = NULL;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}

// 统计字符串在另一个字符串中出现的次数
static int strstr_cnt(char *str, char *substr)
{
    char *srcStr = str;
    int count = 0;

    do
    {
        srcStr = strstr(srcStr, substr);
        if(srcStr != NULL)
        {
            count++;
            srcStr = srcStr + strlen(substr);
        }
        else
        {
            break;
        }
    }while (*srcStr != '\0');

    return count;
}

#if ENABLE_GGA
// GGA数据解析
static GGA gga_data_parse(char *gga_data)
{
    GGA gga;
    unsigned char times = 0;
    char *p;
    char *end;
    char *s = strdup(gga_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // UTC
                strcpy(gga.utc, p);
                break;
            case 2:   // lat
                gga.lat = strtod(p, NULL);
                break;
            case 3:   // lat dir
                gga.lat_dir = p[0];
                break;
            case 4:   // lon
                gga.lon = strtod(p, NULL);
                break;
            case 5:   // lon dir
                gga.lon_dir = p[0];
                break;
            case 6:   // quality
                gga.quality = (unsigned char)strtol(p, NULL, 10);
                break;
            case 7:   // sats
                gga.sats = (unsigned char)strtol(p, NULL, 10);
                break;
            case 8:   // hdop
                gga.hdop = (unsigned char)strtol(p, NULL, 10);
                break;
            case 9:   // alt
                gga.alt = strtof(p, NULL);
                break;
            case 11:  // undulation
                gga.undulation = strtof(p, NULL);
                break;
            case 13:  // age
                gga.age = (unsigned char)strtol(p, NULL, 10);
                break;
            case 14:  // stn_ID
                end = (char *)malloc(sizeof(p));
                strncpy(end, p, strlen(p)-3);
                end[strlen(p)-3] = '\0';
                gga.stn_ID = (unsigned short )strtol(end, NULL, 10);
                free(end);
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return gga;
}
#endif

#if ENABLE_GLL
// GLL数据解析
static GLL gll_data_parse(char *gll_data)
{
    GLL gll;
    unsigned char times = 0;
    char *p;
    char *s = strdup(gll_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // lat
                gll.lat = strtod(p, NULL);
                break;
            case 2:   // lat dir
                gll.lat_dir = p[0];
                break;
            case 3:   // lon
                gll.lon = strtod(p, NULL);
                break;
            case 4:   // lon dir
                gll.lon_dir = p[0];
                break;
            case 5:   // lon dir
                strcpy(gll.utc, p);
                break;
            case 6:  // data status
                gll.data_status = p[0];
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return gll;
}
#endif

#if ENABLE_GSA
// 得到GSA数据中的信道信息
static GSA_PRN *get_prn_data(char *gps_data)
{
    GSA_PRN *gsa_prn;
    unsigned char times = 0;
    unsigned char i;
    unsigned char sentens_index = 0;  // 累计找到gsa字段的个数
    char *p;
    char *s;
    char *sentens;
    int gsa_count;

    // 统计GSA字段的个数
    gsa_count = strstr_cnt(gps_data, PRE_GSA);

    gsa_prn = (GSA_PRN *)malloc(sizeof(GSA_PRN) * (gsa_count * 12 + 1));
    memset(gsa_prn, 0, sizeof(GSA_PRN) * (gsa_count * 12 + 1));
    sentens = strtok(gps_data, "\r\n");
    while (sentens)
    {
        if (strstr(sentens, PRE_GSA))
        {
            sentens_index++;
            s = strdup(sentens);
            p = strsplit(&s, ",");
            while (p)
            {
                if (times == 2)
                {
                    for (i=0; i<12; i++)
                    {
                        p = strsplit(&s, ",");
                        (gsa_prn+i+(sentens_index-1)*12)->total = (unsigned char)gsa_count * 12;
                        (gsa_prn+i+(sentens_index-1)*12)->prn_ID = i + (sentens_index - 1) * 12;
                        (gsa_prn+i+(sentens_index-1)*12)->prn = (unsigned char)strtol(p, NULL, 10);
                    }
                }
                p = strsplit(&s, ",");
                times++;
            }
            times = 0;
        }
        sentens = strtok(NULL, "\r\n");
    }
    free(s);
    return gsa_prn;
}

// GSA数据解析
static GSA gsa_data_parse(char *gsa_data, char *gpsdata)
{
    GSA gsa;
    unsigned char times = 0;
    char *p;
    char *end;
    char *s = strdup(gsa_data);
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
                gsa.gsa_prn = get_prn_data(alldata);
            case 15:  // pdop
                gsa.pdop = strtod(p, NULL);
                break;
            case 16:  // hdop
                gsa.hdop = strtod(p, NULL);
                break;
            case 17:  // vdop
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
#endif

#if ENABLE_RMC
// RMC数据解析
static RMC rmc_data_parse(char *rmc_data)
{
    RMC rmc;
    unsigned char times = 0;
    char *p;
    char *s = strdup(rmc_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
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
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return rmc;
}
#endif

#if ENABLE_VTG
// VTG数据解析
static VTG vtg_data_parse(char *vtg_data)
{
    VTG vtg;
    unsigned char times = 0;
    char *p;
    char *s = strdup(vtg_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // track true
                vtg.track_true = strtod(p, NULL);
                break;
            case 3:   // track mag
                vtg.track_mag = strtod(p, NULL);
                break;
            case 5:   // speed Kn
                vtg.speed_Kn = strtod(p, NULL);
                break;
            case 7:   // speed Km
                vtg.speed_Km = strtod(p, NULL);
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return vtg;
}
#endif

#if ENABLE_GSV
/*
 * function:  获取GSV字段中的GPS信息
 * gps_data:  最原始的GPS字符串
 * stas:      卫星数量
 * prefix:    GSV信息字段前缀
*/
static SAT_INFO *get_sats_info(char *gps_data, unsigned char sats, char *prefix)
{
    SAT_INFO *sats_info;
    unsigned char times = 0;
    unsigned char msgs = 0;
    unsigned char msg = 0;
    unsigned char for_times;
    unsigned char i;
    char *p;
    char *s;
    char *sentens;

    sats_info = (SAT_INFO *)malloc(sizeof(SAT_INFO) * (sats+1));
    memset(sats_info, 0, sizeof(SAT_INFO) * (sats+1));
    sentens = strtok(gps_data, "\r\n");
    while (sentens)
    {
        if (strstr(sentens, prefix))
        {
            s = strdup(sentens);
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
                        for_times = (msgs == msg) ? ((sats % 4) ? sats % 4 : 4) : 4;
                        for (i = 0; i < for_times; i++)
                        {
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
        sentens = strtok(NULL, "\r\n");
    }
    free(s);
    return sats_info;
}

// GSV数据解析
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
#endif

#if ENABLE_UTC
// UTC数据解析
static UTC utc_parse(char *date, char *time)
{
    UTC utc_data;
    unsigned int date_int;
    double time_float;

    date_int = (unsigned int)strtol(date, NULL, 10);
    utc_data.DD = date_int / 10000;
    utc_data.MM = date_int % 10000 / 100;
    utc_data.YY = date_int % 100;
    time_float = strtod(time, NULL);
    utc_data.hh = (unsigned int)time_float / 10000;
    utc_data.mm = (unsigned int)time_float % 10000 / 100;
    utc_data.ss = (unsigned int)time_float % 100;
    utc_data.ds = (unsigned short)(time_float - (unsigned int)time_float);

    return utc_data;
}
#endif

// 解析全部的GPS数据
GPS gps_data_parse(char* gps_src)
{
    GPS gps_all;
    char *str_buffer = strdup(gps_src);

    // GGA数据解析
#if ENABLE_GGA
    GGA default_gga_data = {"\0",0.0,'N',0.0,'S',0,0,0,0,0,0,0};
    gps_src = strdup(str_buffer);
    gps_all.gga_data = strstr(gps_src, PRE_GGA) ? gga_data_parse(strtok(strstr(gps_src, PRE_GGA), "\r\n")) : default_gga_data;
#endif

    // GLL数据解析
#if ENABLE_GLL
    GLL default_gll_data = {0.0,'\0',0.0,'\0',"\0",'\0'};
    gps_src = strdup(str_buffer);
    gps_all.gll_data = strstr(gps_src, PRE_GLL) ? gll_data_parse(strtok(strstr(gps_src, PRE_GLL), "\r\n")) : default_gll_data;
#endif

    // GSA数据解析
#if ENABLE_GSA
    GSA_PRN default_gsa_prn_data = {0,0,0};
    GSA default_gsa_data = {'\0','\0',0.0,0.0,0.0,&default_gsa_prn_data};
    gps_src = strdup(str_buffer);
    gps_all.gsa_data = strstr(gps_src, PRE_GSA) ? gsa_data_parse(strtok(strstr(gps_src, PRE_GSA), "\r\n"), str_buffer) : default_gsa_data;
#endif

    // RMC数据解析
#if ENABLE_RMC
    RMC default_rmc_data = {"\0",'\0',0.0,'\0',0.0,'\0',0.0,0.0,"\0",0.0,'\0','\0'};
    gps_src = strdup(str_buffer);
    gps_all.rmc_data = strstr(gps_src, PRE_RMC) ? rmc_data_parse(strtok(strstr(gps_src, PRE_RMC), "\r\n")) : default_rmc_data;
#endif

    // VTG数据解析
#if ENABLE_VTG
    VTG default_vtg_data = {0.0,0.0,0.0,0.0};
    gps_src = strdup(str_buffer);
    gps_all.vtg_data = strstr(gps_src, PRE_VTG) ? vtg_data_parse(strtok(strstr(gps_src, PRE_VTG), "\r\n")) : default_vtg_data;
#endif

    // GSV数据解析
#if ENABLE_GSV
    SAT_INFO default_sat_info_data = {0,0,0,0};
    GSV default_gsv_data = {0,0,0,&default_sat_info_data};
    // GPGSV数据段解析
    gps_src = strdup(str_buffer);
    gps_all.gpgsv_data = strstr(gps_src, PRE_GPGSV) ? gsv_data_parse(strtok(strstr(gps_src, PRE_GPGSV), "\r\n"), str_buffer, PRE_GPGSV) : default_gsv_data;
    // GNGSV数据段解析
    gps_src = strdup(str_buffer);
    gps_all.gngsv_data = strstr(gps_src, PRE_GNGSV) ? gsv_data_parse(strtok(strstr(gps_src, PRE_GNGSV), "\r\n"), str_buffer, PRE_GNGSV) : default_gsv_data;
    // GLGSV数据段解析
    gps_src = strdup(str_buffer);
    gps_all.glgsv_data = strstr(gps_src, PRE_GLGSV) ? gsv_data_parse(strtok(strstr(gps_src, PRE_GLGSV), "\r\n"), str_buffer, PRE_GLGSV) : default_gsv_data;
#endif

    // UTC数据解析，UTC数据取自RMC段数据
#if ENABLE_UTC && ENABLE_RMC
    gps_all.utc = utc_parse(gps_all.rmc_data.date, gps_all.rmc_data.utc);
#endif

    free(str_buffer);
    free(gps_src);
    return gps_all;
}
