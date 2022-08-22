/**
 * \author Recep Ömer Zorlu
 */
#define _GNU_SOURCE

#include "sensor_db.h"
#include "config.h"

#include <sqlite3.h>
#include <stdio.h>
#include <stdbool.h>

#define EXEC_QUERY(db, callback, query_failed, format...)       \
    char *query = NULL;                                         \
    asprintf(&query, format);                                   \
                                                                \
    char *err_msg = NULL;                                       \
    int rc = !SQLITE_OK;                                        \
    int counter = 0;                                            \
                                                                \
    query_failed = false;                                       \
    do                                                          \
    {                                                           \
        rc = sqlite3_exec(db, query, callback, NULL, &err_msg); \
        counter++;                                              \
    } while (rc != SQLITE_OK && counter < 3);                   \
                                                                \
    free(query);                                                \
    if (rc != SQLITE_OK)                                        \
    {                                                           \
        log_printf("Unable to connect to SQL server.\n");       \
        sqlite3_free(err_msg);                                  \
        sqlite3_close(db);                                      \
        query_failed = true;                                    \
    }

DBCONN *init_connection(char clear_up_flag)
{
    DBCONN *db;
    char *error_msg = 0;

    int rc = !SQLITE_OK;
    int counter = 0;

    do
    {
        rc = sqlite3_open(TO_STRING(DB_NAME), &db);
        counter++;
    } while (rc != SQLITE_OK && counter < 3);
    if (rc != SQLITE_OK)
    {
        log_printf("Unable to connect to SQL server.\n");
        sqlite3_close(db);

        return NULL;
    }
    log_printf("Connection to SQL server established\n");

    // If clear_up_flag set dropping the existing table to clear out its content
    // and recreating the table with desired properties
    // If clear_up_flag is not set I just check if table exist or not. If it is not existing then creating the table
    // with desired columns.
    char *query = clear_up_flag == 1 ? "DROP TABLE IF EXISTS SensorData; "
                                       "CREATE TABLE " TO_STRING(TABLE_NAME) "("
                                                                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                                             "sensor_id INT, "
                                                                             "sensor_value DECIMAL(4,2), "
                                                                             "timestamp TIMESTAMP "
                                                                             ");"
                                     : "CREATE TABLE IF NOT EXISTS " TO_STRING(TABLE_NAME) "( "
                                                                                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                                                           "sensor_id INT, "
                                                                                           "sensor_value DECIMAL(4,2), "
                                                                                           "timestamp TIMESTAMP "
                                                                                           ");";

    rc = sqlite3_exec(db, query, NULL, NULL, &error_msg);

    if (rc != SQLITE_OK)
    {
        log_printf("Query failed to execute.\n");

        sqlite3_free(error_msg);
        sqlite3_close(db);

        return NULL;
    }

    log_printf("New table %s created.\n", TO_STRING(TABLE_NAME));
    return db;
}

void disconnect(DBCONN *conn)
{
    sqlite3_close(conn);
}

int insert_sensor(DBCONN *conn, sensor_id_t id, sensor_value_t value, sensor_ts_t ts)
{
    bool query_failed;
    EXEC_QUERY(conn, NULL, query_failed, "INSERT INTO %s (sensor_id, sensor_value, timestamp) VALUES(%d, %f, %ld);",
               TO_STRING(TABLE_NAME), id, value, ts);

    return query_failed;
}

int insert_sensor_from_file(DBCONN *conn, FILE *sensor_data)
{
    sensor_data_t data;
    while (!feof(sensor_data))
    {
        fread(&(data.id), sizeof(data.id), 1, sensor_data);
        fread(&(data.value), sizeof(data.value), 1, sensor_data);
        fread(&(data.ts), sizeof(data.ts), 1, sensor_data);

        bool query_failed;
        EXEC_QUERY(conn, NULL, query_failed, "INSERT INTO %s (sensor_id,sensor_value,timestamp) VALUES (%d,%f,%ld);",
                   TO_STRING(TABLE_NAME), data.id, data.value, data.ts);
        if (query_failed)
        {
            return query_failed;
        }
    }

    return 0;
}

int find_sensor_all(DBCONN *conn, callback_t f)
{
    bool query_failed;
    EXEC_QUERY(conn, f, query_failed, "SELECT * FROM %s;", TO_STRING(TABLE_NAME));
    return query_failed;
}

int find_sensor_by_value(DBCONN *conn, sensor_value_t value, callback_t f)
{
    bool query_failed;
    EXEC_QUERY(conn, f, query_failed, "SELECT * FROM %s WHERE sensor_value == %f", TO_STRING(TABLE_NAME), value);
    return query_failed;
}

int find_sensor_exceed_value(DBCONN *conn, sensor_value_t value, callback_t f)
{
    bool query_failed;
    EXEC_QUERY(conn, f, query_failed, "SELECT * FROM %s WHERE sensor_value > %f;", TO_STRING(TABLE_NAME), value)
    return query_failed;
}

int find_sensor_by_timestamp(DBCONN *conn, sensor_ts_t ts, callback_t f)
{
    bool query_failed;
    EXEC_QUERY(conn, f, query_failed, "SELECT * FROM %s WHERE timestamp == %ld;", TO_STRING(TABLE_NAME), ts);
    return query_failed;
}

int find_sensor_after_timestamp(DBCONN *conn, sensor_ts_t ts, callback_t f)
{
    bool query_failed;
    EXEC_QUERY(conn, f, query_failed, "SELECT * FROM %s WHERE timestamp > %ld;", TO_STRING(TABLE_NAME), ts);
    return query_failed;
}