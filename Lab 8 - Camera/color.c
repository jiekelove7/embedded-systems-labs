#include <stdio.h>
#include <stdlib.h>
#include "eyebot.h"

// 42 is set to red
#define INITIAL_HUE 42
// Initial position of servo
// Center it as much as possible
#define INITIAL_SERVO 128

#define TIMES_PER_SECOND 50 // For timer purposes

/**
 *  IMPORTANT!!! READ BELOW!
**/
// Hue threshold refers to how much larger / smaller the hue
// value can be for it to be recognised as the same colour

// Intensity threshold refers to the smallest intensity allowed to
// be considered.

// These values MUST BE ADJUSTED as they deviate depending on
// object, colour, LIGHTING!! and LOCATION!!
// Otherwise unintended objects will be considered
#define HUE_THRESHOLD 10
#define INTENSITY_THRESHOLD 10

int servoposition;

void servoSet() {
    SERVOSetRaw(1, servoposition);
}


int main() {
    // Initialising Camera
    CAMInit(QQVGA);
    BYTE frame[QQVGA_SIZE];
    BYTE gray[QQVGA_PIXELS];

    // Disable when attempting Q1
    int hue = INITIAL_HUE;

    // Q2
    // /*
    LCDClear();
    LCDMenu("Set HUE", "placeholder", "placeholder", "TERMINATE");
    servoposition = INITIAL_SERVO;

    // Timer for moving servo. Remove this if not doing Experiment 3
    TIMER t1 = OSAttachTimer(TIMES_PER_SECOND, &servoSet);
    
    while(1) {
        int key = KEYRead();
        if(key == KEY4) break;
        CAMGet(frame);
        
        LCDImageStart(0, 0, QQVGA_X, QQVGA_Y);
        LCDImage(frame);

        LCDImageStart(QQVGA_X + 1, 0, QQVGA_X, QQVGA_Y);
        IPCol2Gray(frame, gray);
        //LCDImageGray(gray);

        BYTE test[QQVGA_SIZE];

        BYTE H[QQVGA_PIXELS];
        BYTE S[QQVGA_PIXELS];
        BYTE I[QQVGA_PIXELS];
        IPCol2HSI(frame, H, S, I);

        // Press KEY1 for Q1
        if(key == KEY1) {
            // Adds hue value of the 9 by 9 square in the centre
            for(int i = -1 ; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    hue += (int) H[(QQVGA_Y/2 + j) * QQVGA_X + (QQVGA_X/2)];
                }
            }
            hue = hue / 9; // Average the total
            LCDSetPos(10, 0);
            LCDPrintf("Average hue = %d  ", hue);
        }
        BYTE B_image[QQVGA_PIXELS] = {0};
        // Create binary image
        
        // 
        int rowhistogram[QQVGA_Y] = {0};
        int colhistogram[QQVGA_X] = {0};
        for(int i = 0; i < QQVGA_X; i++) {
            for(int j = 0; j < QQVGA_Y; j++) {
                int pixel = j * QQVGA_X + i;
                if(H[pixel] > (hue - HUE_THRESHOLD) && H[pixel] < (hue + HUE_THRESHOLD) && I[pixel] > INTENSITY_THRESHOLD) {
                    B_image[pixel] = 1; // For binary image of object
                    rowhistogram[j]++;
                    colhistogram[i]++;
                    
                }
            }
        }
        IPOverlayGray(gray, B_image, RED, test); // Generate red highlighting
        LCDImage(test);
        // LCDImageStart(QQVGA_X + 1, 0, QQVGA_X, QQVGA_Y);
        // LCDImageBinary(B_image);

        // Erases the histograms per each iteration
        // Without this, histograms are permanently displayed on screen
        // It does this by just drawing black rectangles
        // (since the LCD background is black)
        LCDArea(320, 0, 400, 240, BLACK, 1);
        LCDArea(160 , 120, 320, 240, BLACK, 1);

        // To generate lines and histograms
        int max_x = 0;
        int pos_x = -1;
        int max_y = 0;
        int pos_y = -1;
        for(int i = 0; i < QQVGA_X; i++) {
            if(colhistogram[i] > max_x ) {
                max_x = colhistogram[i];
                pos_x = i;
            }
            LCDLine(QQVGA_X + 1 + i, 240 - (colhistogram[i]/2), QQVGA_X + 1 + i, 240, YELLOW);
        }
        for(int j = 0; j < QQVGA_Y; j++) {
            if(rowhistogram[j] > max_y ) {
                max_y = rowhistogram[j];
                pos_y = j;
            }
            LCDLine(400 - (rowhistogram[j]/2), 0 + j, 400, 0 + j, YELLOW);
        }

        LCDSetPos(11,0);
        LCDPrintf("Max h pixels: %d ", max_y);
        LCDSetPos(12,0);
        LCDPrintf("Max v pixels: %d ", max_x);
        LCDLine(pos_x ,0, pos_x, QQVGA_Y, BLACK);
        LCDLine(0, pos_y, QQVGA_X, pos_y, BLACK);

//        usleep(1000000);

        // Scuffed method of moving servo. Remove this if not doing Experiment 3
       if(pos_x < 54) {
           if(servoposition < 200) {
               servoposition++;
           }
            
       } else if(pos_x > 106) {
           //
           if(servoposition > 50) {
               servoposition--;
           }
       } else {
           //

       }

    }
    
    //*/
    OSDetachTimer(t1);

    CAMRelease();
    return 0;
}