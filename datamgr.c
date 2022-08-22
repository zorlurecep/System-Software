/**
 * \author Recep Ömer Zorlu
 */
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "datamgr.h"
#include "config.h"
#include "lib/dplist.h"

dplist_t *sensors = NULL;

void datamgr_parse_sensor_files(FILE *fp_sensor_map)
{
    sensors = dpl_create(element_copy, element_free, element_compare);

    ERROR_HANDLER(sensors == NULL, MEMORY_ERROR);
    ERROR_HANDLER(fp_sensor_map == NULL, FILE_READ_ERROR);

    sensor_node_t *sensorNode = NULL;
    uint16_t roomIdBuffer;
    uint16_t sensorIdBuffer;
    int sensorIndex = 0;

    while (fscanf(fp_sensor_map, "%hu %hu", &roomIdBuffer, &sensorIdBuffer) > 0)
    {
        sensorNode = malloc(sizeof(sensor_node_t));
        sensorNode->roomID = roomIdBuffer;
        sensorNode->sensorID = sensorIdBuffer;
        sensorNode->average = 0;

        for (int i = 0; i < RUN_AVG_LENGTH; i++)
        {
            sensorNode->temperatures[i] = 0;
        }

        dpl_insert_at_index(sensors, sensorNode, sensorIndex, false);
        sensorIndex++;
    }
}

void datamgr_free()
{
    dpl_free(&sensors, true);
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id)
{
    int sensorCount = dpl_size(sensors);
    sensor_node_t *currentSensor = NULL;
    for (int i = 0; i < sensorCount; i++)
    {
        currentSensor = dpl_get_element_at_index(sensors, i);

        if (currentSensor->sensorID == sensor_id)
        {
            return currentSensor->roomID;
        }
    }

    ERROR_HANDLER(true, INVALID_ERROR);
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id)
{
    int sensorCount = dpl_size(sensors);
    sensor_node_t *currentSensor = NULL;
    for (int i = 0; i < sensorCount; i++)
    {
        currentSensor = dpl_get_element_at_index(sensors, i);

        if (currentSensor->sensorID == sensor_id)
        {
            return currentSensor->average;
        }
    }

    ERROR_HANDLER(true, INVALID_ERROR);
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id)
{
    int sensorCount = dpl_size(sensors);
    sensor_node_t *currentSensor = NULL;
    for (int i = 0; i < sensorCount; i++)
    {
        currentSensor = dpl_get_element_at_index(sensors, i);

        if (currentSensor->sensorID == sensor_id)
        {
            return currentSensor->lastModified;
        }
    }

    ERROR_HANDLER(true, INVALID_ERROR);
}

bool datamgr_buffer_read(const sensor_data_t *data, void *arg)
{
    sensor_node_t *currentSensor = get_sensor_with_id(data->id);

    if (currentSensor == NULL)
    {
        log_printf("Received sensor data with invalid sensor node ID %d.\n", data->id);
        return false;
    }

    currentSensor->lastModified = data->ts;
    currentSensor->temperatures[currentSensor->count % RUN_AVG_LENGTH] = data->value;
    currentSensor->average = calculate_running_average(currentSensor);
    currentSensor->count++;

    if (currentSensor->count >= RUN_AVG_LENGTH)
    {
        if (currentSensor->average < SET_MIN_TEMP)
        {
            log_printf("The sensor node with %d reports it's too cold (running avg temperature = %f) \n", currentSensor->sensorID, currentSensor->average);
        }

        if (currentSensor->average > SET_MAX_TEMP)
        {
            log_printf("The sensor node with %d reports it's too hot (running avg temperature = %f) \n", currentSensor->sensorID, currentSensor->average);
        }
    }

    return false;
}

int datamgr_get_total_sensors()
{
    return dpl_size(sensors);
}

sensor_node_t *get_sensor_with_id(sensor_id_t sensorId)
{
    int sensorCount = dpl_size(sensors);
    sensor_node_t *currentSensor = NULL;
    for (int i = 0; i < sensorCount; i++)
    {
        currentSensor = dpl_get_element_at_index(sensors, i);
        if (currentSensor->sensorID == sensorId)
        {
            return currentSensor;
        }
    }

    return NULL;
}

sensor_value_t calculate_running_average(sensor_node_t *sensor)
{
    sensor_value_t total = 0;
    for (int i = 0; i < RUN_AVG_LENGTH; i++)
    {
        total += sensor->temperatures[i];
    }

    return total / RUN_AVG_LENGTH;
}

// DPLIST Required Callback Functions

// Be careful about this function could be wrong
void *element_copy(void *element)
{
    sensor_node_t *copy = malloc(sizeof(sensor_node_t));
    memcpy(copy, element, sizeof(sensor_node_t));
    return copy;
}

void element_free(void **element)
{
    free(*element);
    *element = NULL;
}

int element_compare(void *x, void *y)
{
    if ((sensor_id_t)(((sensor_node_t *)x)->sensorID) == *((sensor_id_t *)y))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}