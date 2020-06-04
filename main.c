#include <stdio.h>
#include "gps.h"

int main()
{
    GPS gps;
    char gps_data[] = "$GPGSA,A,3,01,20,19,13,,,,,,,,,40.4,24.4,32.2*0A\r\n"
                        "$GPGGA,092204.999,4250.5589,S,14718.5084,E,1,04,24.4,12.2,M,19.7,M,,0800*1F\r\n"
                        "$GPGSV,3,1,10,20,78,331,45,01,59,235,47,22,41,069,,13,32,252,45*70\r\n"
                        "$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50\r\n"
                        "$GPVTG,89.68,T,,M,0.00,N,0.0,K*5F\r\n"
                        "$GPGLL,4250.5589,S,14718.5084,E,092204.999,A*2D\r\n";
    gps = gps_data_parse(gps_data);
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

    printf("----------GLL DATA----------\n");
    printf("utc:%s\n", gps.gll_data.utc);
    printf("lat:%f\n", gps.gll_data.lat);
    printf("lat_dir:%c\n", gps.gll_data.lat_dir);
    printf("lon:%f\n", gps.gll_data.lon);
    printf("lon_dir:%c\n", gps.gll_data.lon_dir);
    printf("data_status:%c\n", gps.gll_data.data_status);


    return 0;
}
