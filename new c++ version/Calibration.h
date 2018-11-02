/* 
 * File:   Calibration.h
 * Author: blinky
 *
 * Created on May 11, 2014, 2:45 PM
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <iostream>

using namespace std;

class Calibration {
public:

    Calibration() {
    }

    Calibration(int depthMapSize, int numFrames) : depthMapSize(depthMapSize), numFrames(numFrames), cnt(0) {
        calibrationSurface = new float[depthMapSize];
        uint16_t_calibrationSurface = new uint16_t[depthMapSize];
        reset();
    }

    void reset() {
        this->cnt = 0;
        memset(calibrationSurface, 0, depthMapSize * sizeof (float));
        memset(uint16_t_calibrationSurface, 0, depthMapSize * sizeof (uint16_t));
    }

    bool calibrate(float* depthMap) {
        if (this->cnt < numFrames) {
            for (int i = 0; i < this->depthMapSize; i++) {
                calibrationSurface[i] = (calibrationSurface[i] + depthMap[i]) / 2.0;
            }
            this->cnt++;
        } else {
            return true;
        }
        return false;
    }

    bool calibrate(uint16_t* depthMap) {
        if (this->cnt < numFrames) {
            for (int i = 0; i < this->depthMapSize; i++) {
                uint16_t_calibrationSurface[i] = (uint16_t_calibrationSurface[i] + depthMap[i]) / 2.0;
            }
            this->cnt++;
        } else {
            return true;
        }
        return false;
    }

    float* getAverageFLOATSurface() {
        return this->calibrationSurface;
    }

    uint16_t* getAverageINTSurface() {
        return this->uint16_t_calibrationSurface;
    }

    void setNumFrames(int value) {
        this->numFrames = value;
    }

    int getNumFrames() {
        return this->numFrames;
    }

    int getProgress() {
        return this->cnt;
    }

    virtual ~Calibration() {
        delete calibrationSurface;
        delete uint16_t_calibrationSurface;
    }

private:
    float* calibrationSurface;
    uint16_t* uint16_t_calibrationSurface;
    int depthMapSize;
    int numFrames;
    int cnt;
};

#endif /* CALIBRATION_H */


/*
0 dear Ophelia!
I am ill at these numbers.
I have not art to reckon my groans.
-Hamlet (Act II, SC. 2, Line 120)
 */
