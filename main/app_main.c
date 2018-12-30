
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "trackpad.h"

void loopTask(void *pvParameters)
{
	trackpad_init();
	packet_t * packet;
	while(1) {
		packet = getNewPacket();
		printf("%u, %u, %u, %u, %d, %d\n", packet->x, packet->y, packet->z, packet->w, packet->leftButtonDown, packet->rightButtonDown);
	}
}


void app_main(void)
{
	xTaskCreatePinnedToCore(&loopTask, "loopTask", 32000, NULL, 5, NULL, 0/*tskNO_AFFINITY*/);
}
