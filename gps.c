//
// Created by ihz on 2020/6/4.
//
#include "gps.h"

// GGA数据解析
static GGA gga_data_parse(char *gga_data)
{
    GGA gga;
    unsigned char times = 0;
    char *p;
    char *end;
    char *s = strdup(gga_data);

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


// 解析全部的GPS数据
GPS gps_data_parse(char* gps_src)
{
    GPS gps_all;
    GGA default_data  = {"\0",0.0,'N',0.0,'S',0,0,0,0,0,0,0};
    char *ps;

    gps_all.gga_data.age = 8;
    ps = strtok(gps_src, "\r\n");
    while (ps)
    {
        printf("--%s\n", ps);
        gps_all.gga_data = strstr(ps, PRE_GGA) ? gga_data_parse(ps) : default_data;
        ps = strtok(NULL, "\r\n");
    }
    return gps_all;
}
