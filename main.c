/**
 * \author Recep Ömer Zorlu
 */

#define _GNU_SOURCE

#include "connmgr.h"
#include "datamgr.h"
#include "sbuffer.h"
#include "sensor_db.h"
#include "config.h"

#include <stdio.h>
#include <sys/stat.h>
#include <pthread.h>
#include <wait.h>

static FILE *log_writer = NULL;

void log_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    char *result = NULL;

    vasprintf(&result, format, ap);
    va_end(ap);

    fprintf(log_writer, "%s", result);
    fflush(log_writer);
    free(result);
}

static int usage()
{
    printf("Usage: <command> <port number> \n");
    return -1;
}

static void *datamgr_run(void *buffer)
{
    FILE *sensorMap = fopen("room_sensor.map", "r");
    if (sensorMap != NULL)
    {
        datamgr_parse_sensor_files(sensorMap);

        sbuffer_listen(buffer, datamgr_buffer_read, NULL);

        datamgr_free();

        fclose(sensorMap);
    }

    return NULL;
}

bool sensor_db_insert_data(const sensor_data_t *data, void *db)
{
    bool failed = insert_sensor(db, data->id, data->value, data->ts);
    return failed;
}

static void *sensor_db_run(void *buffer)
{
    DBCONN *db = init_connection(1);

    if (db != NULL)
    {
        sbuffer_listen(buffer, sensor_db_insert_data, db);

        disconnect(db);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return usage();
    char *strport = argv[1];
    char *error_char = NULL;
    int port_number = strtol(strport, &error_char, 10);
    if (strport[0] == '\0' || error_char[0] != '\0')
        return usage();
    remove(FIFO_NAME);

    int error = mkfifo(FIFO_NAME, S_IRUSR | S_IWUSR);
    (void)error;

    fflush(stdout);
    pid_t pID = fork();

    if (pID)
    {
        // parent process handles different program parts using threads.
        log_writer = fopen(FIFO_NAME, "w");

        sbuffer_t *buffer;
        sbuffer_init(&buffer);

        // creates datamgr_thread with buffer listener.
        pthread_t datamgr_thread;
        error = pthread_create(&datamgr_thread, NULL, datamgr_run, buffer);

        // creates storagemgr_thread with buffer listener.
        pthread_t storagemgr_thread;
        error = pthread_create(&storagemgr_thread, NULL, sensor_db_run, buffer);

        // makes sure that both reader threads are ready.
        sbuffer_check_readers(buffer, 2);

        // starts writer thread.
        // Although I don't use a separate thread for connection_mgr, it works in the main process which is only a
        // starter point for other threads. Other than that it serves as a separate thread for connection manager.
        connmgr_listen(port_number, buffer);
        connmgr_free();

        sbuffer_close(buffer);

        pthread_join(datamgr_thread, NULL);
        pthread_join(storagemgr_thread, NULL);

        sbuffer_free(&buffer);

        fclose(log_writer);
        wait(NULL);
    }
    else
    {
        // child process handles logging. Log messages written in the FIFO by readers and writer threads.
        // Child process logs these messages to gateway.log file.
        FILE *log_reader = fopen(FIFO_NAME, "r");
        FILE *gateway_log = fopen("gateway.log", "w");

        char *line = NULL;
        size_t chars = 0;
        ssize_t bytes_read = -1;
        int counter = 0;

        while ((bytes_read = getline(&line, &chars, log_reader)) != -1)
        {
            printf("%s", line);

            char *log_message;
            asprintf(&log_message, "%d %ld %s", counter, time(NULL), line);
            
            fprintf(gateway_log, "%s", log_message);
            fflush(gateway_log);
            fflush(stdout);
            counter++;
        }

        free(line);
        fclose(gateway_log);
        fclose(log_reader);
    }

    return 0;
}