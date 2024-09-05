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
// #define LOG_FILTER
#define LOG_MEAN
// #define LOG_HELP
// #define EN_V1
// #define EN_V2
// #define LOG_RAW

float roundToThreeDecimalPlaces(float num) {
    // Zaokrąglenie do 3 miejsc po przecinku
    float roundedNum = (int)(num * 1000.0 + 0.5) / 1000.0;
    return roundedNum;
}

int cut = 20;

int main() {
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();
    kitty.init();

    NXP_Camera::Type type = NXP_Camera::Type::CAMERA_1;

    kitty.servo.set(0.0);

    #ifdef EN_V2
    int8_t pos_x[64] = {0};

    int8_t pos_y[64] = {0};

    size_t idx, idy {};
    #endif

while(true) {
        // delay_ms(100);

        uint16_t dataBuf[128] = {0};
        kitty.camera.getData(type, dataBuf);

        // dane analogowe:
        // wysoko (~5V) => białe
        // nisko  (~0V) => czarne
        // nie na odwrót!!

        // filtr:
        // 1 => czarne
        // 0 => białe
        int filter[128] = {0};

        int brightness {0};

        int mean {0};
        for (int i =10; i<110; i+=10) {
            mean += dataBuf[i];
        }
        brightness = mean/10;
        
        // log_alert("max: %d", dataBuf[64]);
        // log_alert("bri: %d", brightness);

        for(int i = cut; i<128-cut; i++){
            if(dataBuf[i]<brightness) filter[i] = 1;
        }

        filter[127] = 1;

        #ifdef LOG_RAW
        for(int i = 0; i<128; i+=8) {
            log_alert("%d %d %d %d %d %d %d %d", 
            dataBuf[i], dataBuf[i+1], dataBuf[i+2], dataBuf[i+3], dataBuf[i+4], dataBuf[i+5], dataBuf[i+6], dataBuf[i+7]);
            delay_ms(5);
        }
        #endif

        #ifdef LOG_FILTER
        for(int i = 0; i<128; i+=8) {
            log_alert("%d %d %d %d %d %d %d %d", 
            filter[i], filter[i+1], filter[i+2], filter[i+3], filter[i+4], filter[i+5], filter[i+6], filter[i+7]);
            delay_ms(5);
        }
        #endif

        int s = 64;
        int j = 0;

        int srv = 0;

        for(int i = 0; i<128; i++) {
            s=s+j;
            if(filter[s]) {
                srv=i-63;
                break;
                }
            j++;

            s=s-j;
            if(filter[s]) {
                srv=63-i;
                break;
                }
            j++;
        }


        float help = 0;

        if(srv<-20) help = ( (float)srv+20.0f ) / 90.0f;
        else if(srv>20) help = ( (float)srv-20.0f ) / 90.0f;

        kitty.servo.set(help);

        #ifdef LOG_HELP
        log_alert("srv: %d", srv);
        log_alert("help: %f", help);
        #endif




}
    #ifdef EN_V2
    while(true) {
        delay_ms(100);

        uint16_t dataBuf[128] = {0};
        kitty.camera.getData(type, dataBuf);

        // dane analogowe:
        // wysoko (~5V) => białe
        // nisko  (~0V) => czarne
        // nie na odwrót!!

        // filtr:
        // 1 => czarne
        // 0 => białe
        int filter[128] = {0};

        int brightness {0};

        int mean {0};
        for (int i =10; i<110; i+=10) {
            mean += dataBuf[i];
        }
        brightness = mean/10;

        for(int i = cut; i<128-cut; i++){
                if(dataBuf[i]<brightness) filter[i-cut] = 1;
        }


        #ifdef LOG_FILTER
        for(int i = 0; i<118; i+=10){
            log_alert("%d %d %d %d %d %d %d %d %d %d", 
            filter[i], filter[i+1], filter[i+2], filter[i+3], filter[i+4], filter[i+5], filter[i+6], filter[i+7], filter[i+8], filter[i+9]);
            delay_ms(5);
        }

        log_alert("----------------------------");
        #endif

        #ifdef LOG_RAW
        for(int i = 0; i<118; i+=10){
            log_alert("%d %d %d %d %d %d %d %d %d %d", 
            dataBuf[i], dataBuf[i+1], dataBuf[i+2], dataBuf[i+3], dataBuf[i+4], dataBuf[i+5], dataBuf[i+6], dataBuf[i+7], dataBuf[i+8], dataBuf[i+9]);
            delay_ms(5);
        }

        log_alert("----------------------------");
        #endif

        memset(pos_x,0,64);
        idx = 0;
        idy = 0;

        for (int i = 63; i>=0; i--) {
            if(filter[i] == 0) {
                delay_ms(5);
                log_alert("biale poz. %d", i);
                pos_x[idx] = i;
                idx++;

                if(filter[i-1] == 1) {
                    log_alert("czarne poz. %d", i-1);
                    break;
                }
            }
        }

        // x liczone od srodka (63) do poczatku (0)
        log_alert("=> szerokosc trasy (px) x od czarnej linii: %d", idx)
        log_alert("linia na pozycji (px) x: %d", pos_x[idx-1]);

        for (int i = 64; i<128; i++) {
            if(filter[i] == 0) {
                delay_ms(5);
                log_alert("biale poz. %d", i);
                pos_y[idy] = i;
                idy++;

                if(filter[i+1] == 1) {
                    log_alert("czarne poz. %d", i+1);
                    break;
                }
            }
        }

         // y liczone od srodka (64) do konca (128)
        log_alert("=> szerokosc trasy (px) x od czarnej linii: %d", idy)
        log_alert("linia na pozycji (px) x: %d", pos_y[idy-1]);

        log_alert("----------------------------");
        log_alert("finalna szerokosc toru: %d", idx+idy);
        // lewa patrzac z perspektywy kierowcy w aucie jadacym do przodu
        log_alert("linie toru na pozycjach: lewa: x=%d, prawa: y=%d", pos_x[idx-1], pos_y[idy-1]);
        log_alert("----------------------------");

        log_alert("roznica idx-idy: %d", idx-idy);

        delay_ms(1000);

    }
        

    #endif

    #ifdef EN_V1

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
