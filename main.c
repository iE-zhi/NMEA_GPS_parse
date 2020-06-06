#include <stdio.h>
#include "gps.h"

int main()
{
    GPS gps;
    unsigned char i;
    char gps_data[] = "$GNRMC,013300.00,A,2240.84105,N,11402.70763,E,0.007,,220319,,,D*69\r\n"
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
    gps = gps_data_parse(gps_data);

#if ENABLE_GGA
    printf("----------GGA DATA----------\n");
    printf("utc:%s\n", gps.gga_data.utc);
    printf("lat:%f\n", gps.gga_data.lat);
    printf("lat_dir:%c\n", gps.gga_data.lat_dir);
    printf("lon:%f\n", gps.gga_data.lon);
    printf("lon_dir:%c\n", gps.gga_data.lon_dir);
    printf("quality:%d\n", gps.gga_data.quality);
    printf("sats:%d\n", gps.gga_data.sats);
    printf("hdop:%f\n", gps.gga_data.hdop);
    printf("alt:%f\n", gps.gga_data.alt);
    printf("undulation:%f\n", gps.gga_data.undulation);
    printf("age:%d\n", gps.gga_data.age);
    printf("stn_ID:%d\n", gps.gga_data.stn_ID);
#endif
#if ENABLE_GLL
    printf("----------GLL DATA----------\n");
    printf("utc:%s\n", gps.gll_data.utc);
    printf("lat:%f\n", gps.gll_data.lat);
    printf("lat_dir:%c\n", gps.gll_data.lat_dir);
    printf("lon:%f\n", gps.gll_data.lon);
    printf("lon_dir:%c\n", gps.gll_data.lon_dir);
    printf("data_status:%c\n", gps.gll_data.data_status);
#endif
#if ENABLE_GSA
    printf("----------GSA DATA----------\n");
    printf("mode_MA:%c\n", gps.gsa_data.mode_MA);
    printf("mode_123:%c\n", gps.gsa_data.mode_123);
    printf("total:%d\n", gps.gsa_data.gsa_prn[0].total);
    for (i=0; i<gps.gsa_data.gsa_prn[0].total; i++)
    {
        printf("prn%d:%d\n", (i+1), gps.gsa_data.gsa_prn[i].prn);
    }
    printf("pdop:%f\n", gps.gsa_data.pdop);
    printf("hdop:%f\n", gps.gsa_data.hdop);
    printf("vdop:%f\n", gps.gsa_data.vdop);
    // gps.gsa_data.gsa_prn是动态分配的内存，用完记得释放,否则会造成内存泄漏
    free(gps.gsa_data.gsa_prn);
#endif
#if ENABLE_RMC
    printf("----------RMC DATA----------\n");
    printf("utc:%s\n", gps.rmc_data.utc);
    printf("lat:%f\n", gps.rmc_data.lat);
    printf("lat_dir:%c\n", gps.rmc_data.lat_dir);
    printf("lon:%f\n", gps.rmc_data.lon);
    printf("lon_dir:%c\n", gps.rmc_data.lon_dir);
    printf("speed_Kn:%f\n", gps.rmc_data.speed_Kn);
    printf("track_true:%f\n", gps.rmc_data.track_true);
    printf("date:%s\n", gps.rmc_data.date);
    printf("mag_var:%f\n", gps.rmc_data.mag_var);
    printf("var_dir:%c\n", gps.rmc_data.var_dir);
    printf("mode_ind:%c\n", gps.rmc_data.mode_ind);
#endif
#if ENABLE_VTG
    printf("----------VTG DATA----------\n");
    printf("track_true:%f\n", gps.vtg_data.track_true);
    printf("track_mag:%f\n", gps.vtg_data.track_mag);
    printf("speen_Kn:%f\n", gps.vtg_data.speed_Kn);
    printf("speed_Km:%f\n", gps.vtg_data.speed_Km);
#endif
#if ENABLE_GSV
    printf("----------GPGSV DATA----------\n");
    printf("msgs:%d\n", gps.gpgsv_data.msgs);
    printf("msg:%d\n", gps.gpgsv_data.msg);
    printf("sats:%d\n", gps.gpgsv_data.sats);
    for (i=0;i<gps.gpgsv_data.sats; i++)
    {
        printf("prn%d:%d\n", i+1, gps.gpgsv_data.sat_info[i].prn);
        printf("evel%d:%d\n", i+1, gps.gpgsv_data.sat_info[i].elev);
        printf("azimuth%d:%d\n", i+1, gps.gpgsv_data.sat_info[i].azimuth);
        printf("SNR%d:%d\n", i+1, gps.gpgsv_data.sat_info[i].SNR);
    }
    // 用完释放gps.gpgsv_data.sat_info内存
    if (gps.gpgsv_data.sats) free(gps.gpgsv_data.sat_info);

    printf("----------GNGSV DATA----------\n");
    printf("msgs:%d\n", gps.gngsv_data.msgs);
    printf("msg:%d\n", gps.gngsv_data.msg);
    printf("sats:%d\n", gps.gngsv_data.sats);
    for (i=0; i<gps.gngsv_data.sats; i++)
    {
        printf("prn%d:%d\n", i+1, gps.gngsv_data.sat_info[i].prn);
        printf("evel%d:%d\n", i+1, gps.gngsv_data.sat_info[i].elev);
        printf("azimuth%d:%d\n", i+1, gps.gngsv_data.sat_info[i].azimuth);
        printf("SNR%d:%d\n", i+1, gps.gngsv_data.sat_info[i].SNR);
    }
    if (gps.gngsv_data.sats) free(gps.gngsv_data.sat_info);

    printf("----------GLGSV DATA----------\n");
    printf("msgs:%d\n", gps.glgsv_data.msgs);
    printf("msg:%d\n", gps.glgsv_data.msg);
    printf("sats:%d\n", gps.glgsv_data.sats);
    for (i=0;i<gps.glgsv_data.sats; i++)
    {
        printf("prn%d:%d\n", i+1, gps.glgsv_data.sat_info[i].prn);
        printf("evel%d:%d\n", i+1, gps.glgsv_data.sat_info[i].elev);
        printf("azimuth%d:%d\n", i+1, gps.glgsv_data.sat_info[i].azimuth);
        printf("SNR%d:%d\n", i+1, gps.glgsv_data.sat_info[i].SNR);
    }
    if (gps.glgsv_data.sats) free(gps.glgsv_data.sat_info);
#endif
#if ENABLE_UTC && ENABLE_RMC
    printf("----------UTC DATA----------\n");
    printf("year:20%02d\n", gps.utc.YY);
    printf("month:%02d\n", gps.utc.MM);
    printf("date:%02d\n", gps.utc.DD);
    printf("hour:%02d\n", gps.utc.hh);
    printf("minutes:%02d\n", gps.utc.mm);
    printf("second:%02d\n", gps.utc.ss);
    printf("ds:%02d\n", gps.utc.ds);
#endif
    return 0;
}
