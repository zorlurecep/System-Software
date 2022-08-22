/**
 * \author Recep Ömer Zorlu
 */
#include "lib/dplist.h"
#include "lib/tcpsock.h"
#include "config.h"

#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <fcntl.h>
#include "unistd.h"
#include "sbuffer.h"

/**
 * @brief Listens for new TCP connection. If a connection is established receives data
 * and writes it to the shared buffer for other threads to read it.
 * 
 * @param port_number 
 * @param buffer 
 */
void connmgr_listen(int port_number, sbuffer_t* buffer);

void connmgr_free();
