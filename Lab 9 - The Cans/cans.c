// The cans experiment.
// The bot should start from one area, scan the area to look for red cans.
// Upon spotting a red can, it should drive to it, drive around it,
// push the can into its starting position. 
// When finished, it should repeat this.
// If no red cans are seen, it should keep scanning
// It should also move away from obstacles in it's left / right

// IMPLEMENTED:
// Scanning for cans.
// Location a can.
// Moving to can.
// Stopping before can.
// Turning around and moving back to starting position.

// NOT IMPLEMENTED:
// Driving around can and moving it back.
// Moving away from obstacles in its left/right.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eyebot.h"

// Subjected to change
// Centre
#define PSD1_THRESHOLD 100
// Left
#define PSD2_THRESHOLD 3920
// Right
 #define PSD3_THRESHOLD 100

#define TURN_SPEED 25

// int servoposition;
int pos_x;
int max_pixels;

int distance;

enum state {
    idle,
    foundCan,
    reverse,
    turnback,
    driveback
};

int final_x;
int final_y;
int final_phi;
int stopped;
enum state current;

void motorSet() {
    
    switch (current) {
        case foundCan:
            if(pos_x < 54) {
                // Right turn
                VWSetSpeed(0, -1 * TURN_SPEED);
            } else if(pos_x > 106) {
                // Left Turn
                VWSetSpeed(0, TURN_SPEED);
            } else {
                // Drive Forward
                VWSetSpeed(70,0);
            }
            break;
        case reverse:
            VWSetSpeed(0, 0);
            break;
        case turnback:
            break;
        case driveback:
            break;
        default:
            VWSetSpeed(0, TURN_SPEED);




    }
    // SERVOSetRaw(1, servoposition);
    /*
    if(current == idle) {
        VWSetSpeed(0, TURN_SPEED);
    } else if (current == foundCan){
        if(pos_x < 54) {
            // Right turn
            VWSetSpeed(0, TURN_SPEED);
        } else if(pos_x > 106) {
            // Left Turn
            VWSetSpeed(0, -1 * TURN_SPEED);
        } else {
            // Drive Forward
            VWSetSpeed(25,0);
        }
    } else {
        // Return to starting position
        VWSetSpeed(0, 0);
    }
    */
}

int main() {
    // Initialising Camera
    CAMInit(QQVGA);
    BYTE frame[QQVGA_SIZE];
    BYTE gray[QQVGA_PIXELS];
    int hue = 43;
    pos_x = -1;
    VWSetPosition(0,0,0);
    SERVOSetRaw(1, 80);
    current = idle;

    // Q2
    // /*
    LCDClear();
    LCDMenu("Set Idle", "placeholder", "placeholder", "TERMINATE");
    stopped = 0; 
    TIMER t1 = OSAttachTimer(50, &motorSet);
    while(1) {
        int key = KEYRead();
        if(key == KEY4) break;
        if(key == KEY1) {
            current = idle;
            max_pixels = 0;
            VWSetPosition(0,0,0);
            distance = 0;

        }
        CAMGet(frame);
        
        LCDImageStart(0, 0, QQVGA_X, QQVGA_Y);
        // LCDImage(frame);

        // LCDImageStart(QQVGA_X + 1, 0, QQVGA_X, QQVGA_Y);
        IPCol2Gray(frame, gray);
        //LCDImageGray(gray);

        BYTE Final[QQVGA_SIZE];
        BYTE H[QQVGA_PIXELS];
        BYTE S[QQVGA_PIXELS];
        BYTE I[QQVGA_PIXELS];
        IPCol2HSI(frame, H, S, I);
        BYTE B_image[QQVGA_PIXELS] = {0};
        // Create binary image
        // Using 10 as range, 
        int rowhistogram[QQVGA_Y] = {0};
        int colhistogram[QQVGA_X] = {0};
        for(int i = 0; i < QQVGA_X; i++) {
            for(int j = 0; j < QQVGA_Y; j++) {
                int pixel = j * QQVGA_X + i;
                if(H[pixel] > (hue - 5) && H[pixel] < (hue + 5) && I[pixel] > 40) {
                    B_image[pixel] = 1;
                    rowhistogram[j]++;
                    colhistogram[i]++;
                    //LCDPixel(QQVGA_X + 1 + i, j, BLUE);
                    
                }
            }
        }
        IPOverlayGray(gray, B_image, RED, Final);
        LCDImage(Final);

        int max_x = 0;
        for(int i = 0; i < QQVGA_X; i++) {
            if(colhistogram[i] > max_x ) {
                max_x = colhistogram[i];
                pos_x = i;
            }
            // DRAW HISTOGRAM
            // LCDLine(QQVGA_X + 1 + i, 240 - (colhistogram[i]/2), QQVGA_X + 1 + i, 240, YELLOW);
        }
        max_pixels = max_x;


        LCDSetPos(11,0);
        LCDPrintf("Position of x: %d: %d   ", pos_x, max_x);
        // Crosshair
        LCDLine(pos_x ,0, pos_x, QQVGA_Y, BLACK);

        // Set state from idle to foundCan
        if(max_pixels <= 3 && current == foundCan) {
            current = idle;
        }
        if(current == idle && max_pixels > 3) {
            current = foundCan;
        }
        /*
        if(current != reverse) {
            if(max_pixels > 3) {
                current = foundCan;
            } else {
                current = idle;
            }
        } */
        
        int angle;
        // Set state from foundCan to reverse
        // LCDSetPos(14,0);
        // LCDPrintf("PSD: %d", PSDGet(1));
        
        if(current == foundCan && max_pixels > 100) {
            current = reverse;
            
            VWGetPosition(&final_x, &final_y, &final_phi);
            distance = (int) sqrt(pow(final_x, 2) + pow(final_y, 2));
            current = turnback;
            if(final_x > 0 && final_y > 0) {
                angle = 270 - (57 * atan(abs(final_x / final_y))) - final_phi - 12;
            } else if(final_x > 0 && final_y <= 0) {
                angle = 270 - (57 * atan(abs(final_x / final_y))) + final_phi - 12;
            } else if(final_x < 0 && final_y > 0) {
                angle = 90 - (57 * atan(abs(final_x / final_y))) + final_phi - 12;
            } else {
                angle = 90 - (57 * atan(abs(final_x / final_y))) - final_phi - 12;
            }
            
            
            VWTurn(angle , 2 * TURN_SPEED);
            VWWait();
            VWStraight(distance, 70);
            
        }
        
        
        
        LCDSetPos(12,0);
        LCDPrintf("Enum: %d ", current);
        LCDSetPos(13,0);
        LCDPrintf("final pos: %d, %d, %d", final_x, final_y, final_phi);
        LCDSetPos(14,0);
        LCDPrintf("PSD%d", PSDGet(PSD_FRONT));
        // LCDPrintf("Euclid distance: %d, angle: %d", distance, angle);
        
        // In reverse state
        /*
        if(current == reverse && !stopped) {
            stopped = 1;
            current = turnback;
            VWTurn(-1 * final_phi, TURN_SPEED);
        }
        */
        

    }
    
    //*/
    OSDetachTimer(t1);
    VWSetSpeed(0 , 0);

    CAMRelease();
    return 0;
}