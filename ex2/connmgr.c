#define _GNU_SOURCE

#include <errno.h>
#include "connmgr.h"

dplist_t *sockets = NULL;

void free_tcp_socket(void **socket) {
    tcp_close((tcpsock_t **) socket);
}

void connmgr_listen(int port_number) {
    // Creates/opens the file to write sensor data.
    int fd = open("sensor_data_recv", O_CREAT | O_WRONLY | O_TRUNC);

    if (fd == -1) {
        // print which type of error have in a code
        printf("Error Number % d\n", errno);
    }
    // Initialize the list of sockets. This list will hold all of the sockets.
    sockets = dpl_create(NULL, free_tcp_socket, NULL);

    // Create the main socket. This socket will listen for new socket connection requests.
    tcpsock_t *listeningSocket = NULL;
    if (tcp_passive_open(&listeningSocket, port_number) != TCP_NO_ERROR) { exit(EXIT_FAILURE); }
    dpl_insert_at_index(sockets, listeningSocket, 0, false);


    // This part is for capturing new socket requests and registering new sensor data.
    bool active = true;
    struct pollfd *fileDescriptor = NULL;
    while (active) {
        // Updates the file descriptor if a new socket is added or a socket is removed from sockets list.
        fileDescriptor = realloc(fileDescriptor, dpl_size(sockets) * sizeof(*fileDescriptor));

        // Prepares sockets for accepting incoming data
        for (int i = 0; i < dpl_size(sockets); i++) {
            tcpsock_t *currentSocket = dpl_get_element_at_index(sockets, i);
            fileDescriptor[i] = (struct pollfd)
                    {
                            .fd = currentSocket->sd,
                            .events = POLLIN
                    };
        }

        int pollStatus = poll(fileDescriptor, dpl_size(sockets), TIMEOUT * 1000);
        printf("Poll status = %d and sockets size = %d and TIMEOUT is = %d\n", pollStatus, dpl_size(sockets),
               TIMEOUT * 1000);
        if (pollStatus == 0) {
            printf("No sensor data received. Quitting...\n");
            active = false;
        } else {
            for (int i = 0; i < dpl_size(sockets); ++i) {
                tcpsock_t *currentSocket = dpl_get_element_at_index(sockets, i);

                if (i != 0 && time(NULL) > (currentSocket->last_update + TIMEOUT * 1000)) {
                    printf("Current time: %ld and last update time: %ld\n", time(NULL),
                           (currentSocket->last_update + TIMEOUT * 1000));
                    printf("Sensor with ID %d has timed out.\n", currentSocket->sensor_id);
                    dpl_remove_at_index(sockets, i, true);
                } else if ((fileDescriptor[i].revents & POLLIN) != 0) {
                    currentSocket->last_update = time(NULL);

                    if (i == 0) {
                        tcpsock_t *new_socket = NULL;
                        tcp_wait_for_connection(currentSocket, &new_socket);
                        new_socket->last_update = time(NULL);
                        dpl_insert_at_index(sockets, new_socket, dpl_size(sockets), false);
//                        break;
                    } else {
                        sensor_data_t incomingData;
                        int size = sizeof(incomingData.id);
                        tcp_receive(currentSocket, &incomingData.id, &size);

                        size = sizeof(incomingData.value);
                        tcp_receive(currentSocket, &incomingData.value, &size);

                        size = sizeof(incomingData.ts);
                        int result = tcp_receive(currentSocket, &incomingData.ts, &size);

                        if (result == TCP_NO_ERROR) {
                            currentSocket->last_update = time(NULL);
                            write(fd, &incomingData.id, sizeof(incomingData.id));
                            write(fd, &incomingData.value, sizeof(incomingData.value));
                            write(fd, &incomingData.ts, sizeof(incomingData.ts));
                        } else if (result == TCP_CONNECTION_CLOSED) {
                            dpl_remove_at_index(sockets, i, true);
                        }
                    }
                }
            }
        }
    }
    free(fileDescriptor);
    close(fd);
}

void connmgr_free() {
    dpl_free(&sockets, true);
}
