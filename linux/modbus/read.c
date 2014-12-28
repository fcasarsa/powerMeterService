#include <stdlib.h>
#include <stdio.h>
#include <modbus.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


int main () {

    modbus_t *mb;
    uint16_t tab_reg[100];
    int i=0;
    int rc;

//     mb = modbus_new_tcp("127.0.0.1", 1502);
    mb = modbus_new_rtu("/dev/ttyUSB0", 9600, 'E', 8, 1);
if (mb == NULL) {
    fprintf(stderr, "Unable to create the libmodbus context\n");
    return -1;
}


    rc = modbus_set_slave(mb, 1);


if (modbus_connect(mb) == -1) {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    return -1;
}


if (rc == -1) {
    fprintf(stderr, "Set slave: %s\n", modbus_strerror(errno));
    return -1;
}


while (1) {

    rc=  modbus_read_input_registers(mb, 0, 0x50, tab_reg);

if (rc == -1) {
    fprintf(stderr, "Read: %s\n", modbus_strerror(errno));
    continue;
}

    for (i=0; i < rc; i+=2) {
	uint16_t  tmp1 = tab_reg[i];
        uint16_t  tmp2 = tab_reg[i+1];
        tab_reg[i] = tmp2;
        tab_reg[i+1] = tmp1;
    }
/*
    printf("V:%f\n",modbus_get_float(&tab_reg[0]));
    printf("A:%f\n",modbus_get_float(&tab_reg[0x6]));
    printf("kW:%f\n",modbus_get_float(&tab_reg[0xC]));
    printf("kWA:%f\n",modbus_get_float(&tab_reg[0x12]));
    printf("cos:%f\n",modbus_get_float(&tab_reg[0x24]));
    printf("FP:%f\n",modbus_get_float(&tab_reg[0x1E]));
*/

    char buffer[1000];
    int cp = 0;
    cp += sprintf(&buffer[cp],"{\"V\":%f,\n",modbus_get_float(&tab_reg[0]));
    cp += sprintf(&buffer[cp],"\"A\":%f,\n",modbus_get_float(&tab_reg[0x6]));
    cp += sprintf(&buffer[cp],"\"W\":%f,\n",modbus_get_float(&tab_reg[0xC]));
    cp += sprintf(&buffer[cp],"\"VA\":%f,\n",modbus_get_float(&tab_reg[0x12]));
    cp += sprintf(&buffer[cp],"\"cos\":%f,\n",modbus_get_float(&tab_reg[0x24]));
    cp += sprintf(&buffer[cp],"\"fp\":%f}\n",modbus_get_float(&tab_reg[0x1E]));

    fprintf (stdout,"%s",buffer);

    fflush(stdout);

    usleep (3000L*1000L);
}


    
    modbus_close(mb);
    modbus_free(mb);
    return 0;
}
