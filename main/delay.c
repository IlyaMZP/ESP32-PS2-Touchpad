#include "delay.h"

#define NOP() asm volatile ("nop")

uint32_t getApbFrequency(){
    rtc_cpu_freq_config_t conf;
    rtc_clk_cpu_freq_get_config(&conf);
    //printf("%u, %u\n", conf.source_freq_mhz, conf.div);
    if(conf.freq_mhz >= 80){
        return 80000000;
    }
    return (conf.source_freq_mhz * 1000000) / conf.div;
}

unsigned long IRAM_ATTR micros()
{
    static uint32_t _sys_time_multiplier = 1;
    //_sys_time_multiplier = 80 / getApbFrequency();
    //printf("%u\n", getApbFrequency());
    return (unsigned long) (esp_timer_get_time()) * _sys_time_multiplier;
}

void IRAM_ATTR delayMicroseconds(uint32_t us)
{
    uint32_t m = micros();
    if(us){
        uint32_t e = (m + us);
        if(m > e){ //overflow
            while(micros() > e){
                NOP();
            }
        }
	//printf("%lu , %u\n", micros(), e);
        while(micros() < e){
            NOP();
        }
    }
}