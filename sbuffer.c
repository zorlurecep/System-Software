/**
 * \author Recep Ömer Zorlu
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "sbuffer.h"

/**
 * basic node for the buffer, these nodes are linked together to create the buffer
 */
typedef struct sbuffer_node {
    struct sbuffer_node *next;  /**< a pointer to the next node*/
    sensor_data_t data;         /**< a structure containing the data */
    bool isRead[2];             // This variable will keep track if this node has been read by both of the readers
} sbuffer_node_t;

/**
 * a structure to keep track of the buffer
 */
struct sbuffer {
    sbuffer_node_t *head;       /**< a pointer to the first node in the buffer */
    sbuffer_node_t *tail;       /**< a pointer to the last node in the buffer */
    int numberReaders;          // For my implementation there can be at most 2 readers. This will be used to check
    // if each reader had read the sensor node data.
    pthread_mutex_t mutex;      // Mutex for synchronisation between 2 reader threads.
    pthread_cond_t condData;    // To notify threads that a new data available.
    pthread_cond_t condReader;  // To notify threads that a new readers is available.
    bool isClosed;                // Status of the buffer. If reader is available value is false else it is true
};

static sbuffer_node_t *createNode(sensor_data_t *sensorData) {
    sbuffer_node_t *newNode = malloc(sizeof(sbuffer_node_t));
    newNode->data = *sensorData;
    newNode->next = NULL;
    newNode->isRead[0] = false;
    newNode->isRead[1] = false;

    return newNode;
}

int sbuffer_init(sbuffer_t **buffer) {
    *buffer = malloc(sizeof(sbuffer_t));
    if (*buffer == NULL) return SBUFFER_FAILURE;

    (*buffer)->numberReaders = 0;
    (*buffer)->head = NULL;
    (*buffer)->tail = NULL;

    pthread_mutex_init(&(*buffer)->mutex, NULL);
    pthread_cond_init(&(*buffer)->condData, NULL);
    pthread_cond_init(&(*buffer)->condReader, NULL);

    (*buffer)->isClosed = false;
    return SBUFFER_SUCCESS;
}

// No changes required
int sbuffer_free(sbuffer_t **buffer) {
    sbuffer_node_t *dummy;
    if ((buffer == NULL) || (*buffer == NULL)) {
        return SBUFFER_FAILURE;
    }
    while ((*buffer)->head) {
        dummy = (*buffer)->head;
        (*buffer)->head = (*buffer)->head->next;
        free(dummy);
    }
    free(*buffer);
    *buffer = NULL;
    return SBUFFER_SUCCESS;
}

// No changes required
int sbuffer_remove(sbuffer_t *buffer) {
    if (buffer == NULL) return SBUFFER_FAILURE;
    if (buffer->head == NULL) return SBUFFER_NO_DATA;

    sbuffer_node_t *dummy = buffer->head;
    if (buffer->head == buffer->tail) // buffer has only one node
    {
        buffer->head = buffer->tail = NULL;
    } else  // buffer has many nodes empty
    {
        buffer->head = buffer->head->next;
    }
    free(dummy);
    return SBUFFER_SUCCESS;
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data) {
    if (buffer == NULL) return SBUFFER_FAILURE;

    pthread_mutex_lock(&buffer->mutex);
    if (buffer->isClosed) {
        pthread_mutex_unlock(&buffer->mutex);
        return SBUFFER_FAILURE;
    }

    sbuffer_node_t *node = createNode(data);

    // First node is added
    if (buffer->head == NULL) {
        buffer->head = node;
        buffer->tail = node;
    }
        // Rest of the nodes added
    else {
        buffer->tail->next = node;
        buffer->tail = node;
    }

    pthread_cond_broadcast(&buffer->condData);
    pthread_mutex_unlock(&buffer->mutex);

    return SBUFFER_SUCCESS;
}

// Maximum allowed reader amount for this project is 2. If we exceed this amount reader is not registered.
// If reader is not added returning -1
static int sbuffer_add_reader(sbuffer_t *buffer) {
    if (buffer->numberReaders++ < 2) {
        int readerId = buffer->numberReaders++;
        pthread_cond_broadcast(&buffer->condReader);
        return readerId;
    } else {
        return -1;
    }
}


void sbuffer_listen(sbuffer_t *buffer, bool (*reader_process)(sensor_data_t const *, void *), void *arg) {
    pthread_mutex_lock(&buffer->mutex);

    int readerId = sbuffer_add_reader(buffer);
    sbuffer_node_t *currentRead = buffer->head;

    while (true) {
        if (currentRead != NULL) {
            while (currentRead->next == NULL) {
                if (buffer->isClosed) {
                    pthread_mutex_unlock(&buffer->mutex);
                    return;
                }

                pthread_cond_wait(&buffer->condData, &buffer->mutex);
            }

            sbuffer_node_t *nextRead = currentRead->next;
            if (currentRead->isRead[0] == true && currentRead->isRead[1]) {
                sbuffer_remove(buffer);
            } else {
                currentRead->isRead[readerId] = true;
            }

            currentRead = nextRead;

            pthread_mutex_unlock(&buffer->mutex);
            if (reader_process(&currentRead->data, arg) == true) {
                buffer->isClosed = true;
            }

            pthread_mutex_lock(&buffer->mutex);
        }
    }
    pthread_mutex_unlock(&buffer->mutex);
}

void sbuffer_wait_readers(sbuffer_t *buffer, int numberOfReaders) {
    pthread_mutex_unlock(&buffer->mutex);
    while (buffer->numberReaders != numberOfReaders) {
        pthread_cond_wait(&buffer->condReader, &buffer->mutex);
    }
    pthread_mutex_unlock(&buffer->mutex);
}

int sbuffer_close(sbuffer_t *buffer) {
    if (buffer == NULL) {
        return SBUFFER_FAILURE;
    }

    pthread_mutex_lock(&buffer->mutex);
    buffer->isClosed = true;
    pthread_cond_broadcast(&buffer->condData);
    pthread_mutex_unlock(&buffer->mutex);

    return SBUFFER_SUCCESS;
}