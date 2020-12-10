// NOTE: This code is incomplete and does not
// function as intended.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "eyebot.h"

// NOTE: Timer frequency is 1000Hz
#define TICKS_PER_SECOND 50
#define TIMES_PER_SECOND 20 

// Destination to create outfile
#define OUT_FILE "/home/pi/usr/outputp.csv"

int v_des;

int Ki;
int Kd;
FILE *fp; // File pointer
int count;
int v_act;

void bb_controller() {
    int enc_new;
    // int v_act;
    int r_mot;
    static int enc_old;
    
    enc_new = ENCODERRead(1); 
    v_act = (enc_new - enc_old) * TICKS_PER_SECOND;
    if(v_act < v_des) {
        r_mot = 100;
    } else {
        r_mot = 0;
    }
    MOTORDrive(1, r_mot);
    enc_old = enc_new;
    count++;
    // fprintf(fp, "%d,", v_act);
}



void p_controller() {
    
    static int enc_old;
    int enc_new;
    int e_func;
    int r_mot;
    double Kp = 0.007;
    
    enc_new = ENCODERRead(1);
    v_act = (enc_new - enc_old) * TICKS_PER_SECOND;
    e_func = v_des - v_act;
    r_mot = Kp * e_func;
    if(r_mot > 100) {
        r_mot = 100;
    } else if(r_mot < -100) {
        r_mot = -100;
    }
    MOTORDrive(1, r_mot);
    enc_old = enc_new;
    
    count++;
    fprintf(fp, "%d,", v_act);
}


void pi_controller() {
    static int enc_old;
    static int r_old = 0;
    static int e_old = 0;
    double Kp = 0.007;
    double Ki = 0.001;
    int enc_new = ENCODERRead(1);
    int v_act = (enc_new - enc_old) * TICKS_PER_SECOND;
    int e_func = v_des - v_act;
    int r_mot = r_old + Kp * (e_func - e_old) + Ki * (e_func + e_old) / 2;
    if(r_mot > 100) {
        r_mot = 100;
    } else if(r_mot < -100) {
        r_mot = -100;
    }

    MOTORDrive(1, r_mot);
    r_old = r_mot;
    e_old = e_func;

    count++;
    fprintf(fp, "%d,", v_act);
}

void initFile() {
    fp = fopen(OUT_FILE, "w");
    fprintf(fp, "START\n");
}

int main(void) {
    //LCDInit();
    //KEYInit();
    //LCDClear();
    //LCDMenu("0","1","2","EXIT");
    MOTORInit();
    //LCDSetPos(10,10);
    //LCDPrintf("...");
    

    //int key = KEYRead();
    //while(key != KEY4) {
    //    key = KEYRead();
    //}
    // FILE *fp;
    count = 0;

    initFile()

    v_des = 10000;

    TIMER t1;
    // Change to whatever function currently in test
    t1 = OSAttachTimer(TIMES_PER_SECOND, &pi_controller);
    
    
    while(1) {
        if(count > 1000) {
            fclose(fp);
            break;
        }
    }
    OSDetachTimer(t1);

    
    return 0;
}

