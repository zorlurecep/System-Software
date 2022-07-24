#include <stdio.h>

#include "datamgr.h"

int main(int argc, char *argv[]) {
    FILE *fp_text = fopen("room_sensor.map", "r");
    FILE *fp_bin = fopen("sensor_data", "rb");

    datamgr_parse_sensor_files(fp_text, fp_bin);

    fclose(fp_text);
    fclose(fp_bin);

    printf("average temperature: %lf \n", datamgr_get_avg(15));
    printf("last modified: %ld \n", datamgr_get_last_modified(15));
    printf("roomId: %hd \n", datamgr_get_room_id(15));

    datamgr_free();

    return 0;
}