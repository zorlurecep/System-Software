/**
 * \author Recep Ömer Zorlu
 */

#ifndef _SBUFFER_H_
#define _SBUFFER_H_

#include "config.h"

#define SBUFFER_FAILURE -1
#define SBUFFER_SUCCESS 0
#define SBUFFER_NO_DATA 1

#include <stdbool.h>
#include <sys/types.h>

typedef struct sbuffer sbuffer_t;

/**
 * Allocates and initializes a new shared buffer
 * \param buffer a double pointer to the buffer that needs to be initialized
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_init(sbuffer_t **buffer);

/**
 * All allocated resources are freed and cleaned up
 * \param buffer a double pointer to the buffer that needs to be freed
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_free(sbuffer_t **buffer);

/**
 * Removes the first sensor data in 'buffer' (at the 'head') and returns this sensor data as '*data'
 * If 'buffer' is empty, the function doesn't block until new sensor data becomes available but returns SBUFFER_NO_DATA
 * \param buffer a pointer to the buffer that is used
 * \param data a pointer to pre-allocated sensor_data_t space, the data will be copied into this structure. No new memory is allocated for 'data' in this function.
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_remove(sbuffer_t *buffer);

/**
 * Inserts the sensor data in 'data' at the end of 'buffer' (at the 'tail')
 * \param buffer a pointer to the buffer that is used
 * \param data a pointer to sensor_data_t data, that will be copied into the buffer
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occured
*/
int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data);

/**
 * @brief This process checks buffer for new data. If that is the case readers are notified and data processing starts.
 * 
 * @param buffer 
 * @param reader_process 
 * @param arg 
 */
void sbuffer_listen(sbuffer_t* buffer, bool (*reader_process)(sensor_data_t const*, void*), void* arg);

/**
 * @brief Forces sbuffer to stop processing data. Because after this function called this means connection manager is closed.
 * 
 * @param buffer 
 * @return int 
 */
int sbuffer_close(sbuffer_t *buffer);

/**
 * @brief Before starting conn manager, this method makes sure that readers of the other two process is ready.
 * 
 * @param buffer 
 * @param nr_readers 
 */
void sbuffer_check_readers(sbuffer_t* buffer, int nr_readers);

#endif  //_SBUFFER_H_
