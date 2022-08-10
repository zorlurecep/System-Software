#define _GNU_SOURCE

#include <stdio.h>
#include <sqlite3.h>

#include "sensor_db.h"

callback_t print_data_callback(void *NotUsed, int argc, char **argv,
                    char **azColName) {

    NotUsed = 0;

    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");

    return 0;
}

int main() {
    char clear_flag = 0;
    DBCONN *db = init_connection(clear_flag);
    printf("Connection established\n");


    int isInserted = insert_sensor(db, 17, 15, 1658588652);
    printf("Data is inserted?: %d\n", isInserted);
    find_sensor_all(db, (callback_t) &print_data_callback);
    disconnect(db);
    return 0;
}
