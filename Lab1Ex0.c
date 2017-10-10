#include "xc_config_settings.h"
#include "adc.h"
#include "delays.h"
#include "timers.h"

void main(void) {
    char result=0;
    
    TRISA = 0b11101111;
    LATA = 0b11111111;
    TRISF = 0b00000000;
    
    while(1)
    {
        result++;
        
        LATF = result;
        
        Delay10KTCYx(50);
        
        if (result == 256)
        {
            result = 0;
        }
    }
}