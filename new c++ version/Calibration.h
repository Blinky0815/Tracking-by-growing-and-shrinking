/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2014 Olaf Christ 
 * email: christ_o@gmx.de
 * 
 * Tracking-by-growing-and-shrinking is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tracking-by-growing-and-shrinking is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
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
