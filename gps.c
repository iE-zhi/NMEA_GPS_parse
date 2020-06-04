//
// Created by ihz on 2020/6/4.
//
#include "gps.h"

// 数据分割，可以分割两个连续的分隔符
static char* strsep(char** stringp, const char* delim)
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

// GGA数据解析
static GGA gga_data_parse(char *gga_data)
{
    GGA gga;
    unsigned char times = 0;
    char *p;
    char *end;
    char *s = strdup(gga_data);

    printf("gga:%s\n", gga_data);

    p = strsep(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   //  UTC
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
        p = strsep(&s, ",");
        times++;
    }
    free(s);
    return gga;
}

// GGA数据解析
static GLL gll_data_parse(char *gll_data)
{
    GLL gll;
    unsigned char times = 0;
    char *p;
    char *s = strdup(gll_data);

    printf("gll:%s\n", gll_data);

    p = strsep(&s, ",");
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
        p = strsep(&s, ",");
        times++;
    }
    free(s);
    return gll;
}

// 解析全部的GPS数据
GPS gps_data_parse(char* gps_src)
{
    GPS gps_all;
    GGA default_gga_data  = {"\0",0.0,'N',0.0,'S',0,0,0,0,0,0,0};
    GLL default_gll_data = {0.0,'\0',0.0,'\0',"\0",'\0'};
    char *str_buffer = strdup(gps_src);

    gps_all.gga_data = strstr(gps_src, PRE_GGA) ? gga_data_parse(strtok(strstr(gps_src, PRE_GGA), "\r\n")) : default_gga_data;
    gps_src = strdup(str_buffer);
    gps_all.gll_data = strstr(gps_src, PRE_GLL) ? gll_data_parse(strtok(strstr(gps_src, PRE_GLL), "\r\n")) : default_gll_data;

//    ps = strtok(gps_src, "\r\n");
//    while (ps)
//    {
//        printf("--%s\n", ps);
//        gps_all.gga_data = strstr(ps, PRE_GGA) ? gga_data_parse(ps) : default_gga_data;
//        ps = strtok(NULL, "\r\n");
//    }
    free(str_buffer);
    free(gps_src);
    return gps_all;
}
