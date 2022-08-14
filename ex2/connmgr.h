#include "lib/dplist.h"
#include "lib/tcpsock.h"
#include "config.h"

#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <fcntl.h>
#include "unistd.h"

void connmgr_listen(int port_number);

void connmgr_free();
