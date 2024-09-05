/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include "logger.h"

#include "HALina.hpp"
#include "NXP_Kitty.hpp"
#include "clock_config.h"


#define LOG_SERVO
//#define LOG_FLR
#define LOG_MEAN
#define EN_OUR

float roundToThreeDecimalPlaces(float num) {
    // Zaokrąglenie do 3 miejsc po przecinku
    float roundedNum = (int)(num * 1000.0 + 0.5) / 1000.0;
    return roundedNum;
}

int main() {
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();
    kitty.init();

    log_alert("test");


    NXP_Camera::Type type = NXP_Camera::Type::CAMERA_1;


        log_alert("__38");
    /*int max = 10;
    for (int i = -max; i<=max; i++){
            delay_ms(10);
            float ser = (float)i/100.0;
            kitty.servo.set(ser);
            if(i==max) i = -max;
        }*/

        log_alert("__47");
    //kitty.servo.set(0.0);
    #ifdef EN_OUR

    log_alert("test1");
    while (true) {

        log_alert("test22");
        kitty.proc();

        delay_ms(20);
        uint16_t dataBuf[128] = {0};
        kitty.camera.getData(type, dataBuf);

        uint16_t filter[128] = {0};

        for(int i = 0; i<128; i++){
            if(dataBuf[i]>25000) filter[i] = 1;
        }

        /*for (int j = 0; j<128; j++) {
            delay_ms(1);
            //log_notice("%d",filter[j])
        }*/

        int first {};
        int last {};

        float mean = 0;
        for (int i = 0; i<128; i++) {
            if(filter[i] == 1) {
                first = i;
                mean +=i;
                break;
            }
        }

        for (int i = 127; i>=0; i--) {
            if(filter[i] == 1) {
                last = i;
                mean+=i;
                break;
            }
        }

        int range = last - first;

        #ifdef LOG_FLR
        log_alert("f: %d", first);
        log_alert("l: %d", last);
        log_alert("r: %d", range);
        #endif

        mean = (mean/2) - 64.0;// + 2.0;

        #ifdef LOG_MEAN
        char buffer[5]; 
        sprintf(buffer, "%f", mean);
        log_alert("mean: %f", mean);
        #endif
        


        float s1 = (mean/90.0);// - 0.15;

        s1=roundToThreeDecimalPlaces(s1);
        #ifdef LOG_SERVO
        char servobuffer[5]; 
        sprintf(servobuffer, "%f", s1);

        log_alert("servo: %f", s1);
        #endif

        kitty.servo.set(s1);

        /*delay_ms(1000);
        // log_notice("%d", dataBuf[i%128]);

        for (int j = 0; j<128; j++) {
            delay_ms(1);
            log_notice("%d",dataBuf[j]);
        }*/
    }
    #endif
    
}
