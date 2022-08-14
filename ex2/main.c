#include "connmgr.h"

int main(){
    printf("Starting \n");
    connmgr_listen(1234);
    connmgr_free();
    return 0;
}
