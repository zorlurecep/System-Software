#include <stdlib.h>
#include <string.h>

#include "datamgr.h"
#include "config.h"
#include "lib/dplist.h"


dplist_t *sensors = NULL;

void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data) {
    sensors = dpl_create(element_copy, element_free, element_compare);

    ERROR_HANDLER(sensors == NULL, MEMORY_ERROR);
    ERROR_HANDLER(fp_sensor_map == NULL || fp_sensor_data == NULL, FILE_READ_ERROR);

    sensor_node_t *sensorNode = NULL;
    uint16_t roomIdBuffer;
    uint16_t sensorIdBuffer;
    int sensorIndex = 0;

    while (fscanf(fp_sensor_map, "%hd %hd", &roomIdBuffer, &sensorIdBuffer) > 0) {
        sensorNode = malloc(sizeof(sensor_node_t));
        sensorNode->roomID = roomIdBuffer;
        sensorNode->sensorID = sensorIdBuffer;
        sensorNode->average = 0;

        for (int i = 0; i < RUN_AVG_LENGTH; i++) {
            sensorNode->temperatures[i] = 0;
        }

        dpl_insert_at_index(sensors, sensorNode, sensorIndex, false);
        sensorIndex++;
    }

    int nodeIndex;
    double temperatureBuffer;
    time_t timeBuffer;
    while (fread(&sensorIdBuffer, sizeof(sensorIdBuffer), 1, fp_sensor_data) > 0) {
        // Finding the sensor node for this temperature measurement
        nodeIndex = dpl_get_index_of_element(sensors, (void *) &sensorIdBuffer);
        // If nodeIndex is -1 this means that sensors list doesn't contain a sensor with given sensorId
        if (nodeIndex != -1) {
            sensorNode = (sensor_node_t *) dpl_get_element_at_index(sensors, nodeIndex);

            // Updating measure temperatures and calculating running average for this sensor node
            fread(&temperatureBuffer, sizeof(temperatureBuffer), 1, fp_sensor_data);
            double runningAverage = 0;
            for (int i = 0; i < RUN_AVG_LENGTH - 1; i++) {
                sensorNode->temperatures[i] = sensorNode->temperatures[i + 1];
                runningAverage += sensorNode->temperatures[i];
            }
            sensorNode->temperatures[RUN_AVG_LENGTH - 1] = temperatureBuffer;
            runningAverage += temperatureBuffer;
            sensorNode->average = runningAverage / RUN_AVG_LENGTH;

            // Changing the last modified time for this sensor
            fread(&timeBuffer, sizeof(timeBuffer), 1, fp_sensor_data);
            sensorNode->lastModified = timeBuffer;

            // Checking if room temperature is okay for this sensor
            //I assume temperature reading is different from zero and checking the array if all default values has changed
            if (sensorNode->temperatures[0] != 0) {
                if (sensorNode->average < SET_MIN_TEMP) {
                    fprintf(stderr,
                            "It is too cold in room %hd according to readings from sensor %hd with average temperature of %lf measured at %ld \n",
                            sensorNode->roomID, sensorNode->sensorID, sensorNode->average, sensorNode->lastModified);
                }

                if (sensorNode->average > SET_MAX_TEMP) {
                    fprintf(stderr,
                            "It is too hot in room %hd according to readings from sensor %hd with average temperature of %lf measured at %ld  \n",
                            sensorNode->roomID, sensorNode->sensorID, sensorNode->average, sensorNode->lastModified);
                }
            }
        } else {
            fread(&temperatureBuffer, sizeof(temperatureBuffer), 1, fp_sensor_data);
            fread(&timeBuffer, sizeof(timeBuffer), 1, fp_sensor_data);
        }
    }
}

void datamgr_free() {
    dpl_free(&sensors, true);
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id) {
    int sensorCount = dpl_size(sensors);
    sensor_node_t *currentSensor = NULL;
    for (int i = 0; i < sensorCount; i++) {
        currentSensor = dpl_get_element_at_index(sensors, i);

        if (currentSensor->sensorID == sensor_id) {
            return currentSensor->roomID;
        }
    }

    ERROR_HANDLER(true, INVALID_ERROR);
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id) {
    int sensorCount = dpl_size(sensors);
    sensor_node_t *currentSensor = NULL;
    for (int i = 0; i < sensorCount; i++) {
        currentSensor = dpl_get_element_at_index(sensors, i);

        if (currentSensor->sensorID == sensor_id) {
            return currentSensor->average;
        }
    }

    ERROR_HANDLER(true, INVALID_ERROR);
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id) {
    int sensorCount = dpl_size(sensors);
    sensor_node_t *currentSensor = NULL;
    for (int i = 0; i < sensorCount; i++) {
        currentSensor = dpl_get_element_at_index(sensors, i);

        if (currentSensor->sensorID == sensor_id) {
            return currentSensor->lastModified;
        }
    }

    ERROR_HANDLER(true, INVALID_ERROR);
}

int datamgr_get_total_sensors() {
    return dpl_size(sensors);
}

// DPLIST Required Callback Functions

// Be careful about this function could be wrong
void *element_copy(void *element) {
    sensor_node_t *copy = malloc(sizeof(sensor_node_t));
    memcpy(copy, element, sizeof(sensor_node_t));
    return copy;
}

void element_free(void **element) {
//    free(((sensor_node_t *) element)->temperatures);
    free(*element);
    *element = NULL;
}

int element_compare(void *x, void *y) {
    if ((sensor_id_t)(((sensor_node_t *) x)->sensorID) == *((sensor_id_t *) y)) {
        return 0;
    }
    else{
        return 1;
    }
}