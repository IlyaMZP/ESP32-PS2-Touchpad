
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Trackpad.h"
//#include "delay.h"
Trackpad t(32,33);

void loopTask(void *pvParameters)
{
	printf("Hello\n");
	//delayMicroseconds(5000000);
	//printf("Hello\n");
	packet_t * packet;
	while(1) {
		packet = t.getNewPacket();
		printf("%u\n", packet->x);
	}
}


extern "C" void app_main(void)
{
	xTaskCreatePinnedToCore(&loopTask, "loopTask", 32000, NULL, 5, NULL, 0/*tskNO_AFFINITY*/);
}
