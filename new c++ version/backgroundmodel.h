/**
 *This is one of the files implementing the Region tracking algorithm 
 *described in:
 *
 *1. Felix v. Hundelshasuen, Raul Rojas, Tracking Regions and Edges by Shrinking and Growing.
 *   In Ondrej Drbohlav, editor, Proceedings of the Computer Vision Winter Workshop, CVWW03,  pages 33-38, February 2003.
 *2. Felix v. Hundelshausen, Raul Rojas, Tracking Regions
 *   Proceedings of the RoboCup 2003 International Symposium, Padova, Italy July 10-11, 2003
 *
 *For documentation see: http://www.inf.fu-berlin.de/~hundelsh/research/RegionTracking/
 *
 * c++ version by Olaf Christ: Email:christ_o@gmx.de and and Matthias Klostermann
 * 
 * @author Olaf Christ
 **/

/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2012 Olaf Christ 
 * email: christ_o@gmx.de
 * 
 * KinectPeopleTracker is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * KinectPeopleTracker is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BACKGROUNDMODEL_H
#define BACKGROUNDMODEL_H
#include <stdint.h>
#include <iostream>
#include <cstring>

using namespace std;

class BackgroundModel {
private:
    int width, height;
    uint16_t* thresholds;
    uint16_t* depthMap;
    int size;

public:

    BackgroundModel(uint16_t* depthMap, int width, int height) : width(width), height(height), thresholds(new uint16_t[width*height]), size(width*height) {
        std::memset(this->thresholds, 0, width * height * sizeof (uint16_t));
    }

    virtual ~BackgroundModel() {
        if (this->thresholds != NULL) {
            delete[] this->thresholds;
        }
    }
    
    uint16_t* getDepthmap() {
        return this->depthMap;
    }

    void setDepthmap(uint16_t* value) {
        this->depthMap = value;
    }

    int getWidth() {
        return this->width;
    }

    int getHeight() {
        return this->height;
    }

    void setWidth(int value) {
        this->width = value;
    }

    void setHeight(int value) {
        this->height = value;
    }

    virtual void update(uint16_t* depthMap) {
        setDepthmap(depthMap);
    }

    virtual void update(int* occu) {
        /*
                for (int x = 1; x < width - 1; x++) {
                    for (int y = 1; y < height - 1; y++) {

                        int center = (y) * width + (x);
                        int topLeft = (y - 1) * width + (x - 1);
                        int topLeft_2 = (y - 2) * width + (x - 2);
                        int top = (y - 1) * width + (x);
                        int top_2 = (y - 2) * width + (x);
                        int topRight = (y - 1) * width + (x + 1);
                        int topRight_2 = (y - 2) * width + (x + 2);
                        int left = (y) * width + (x - 1);
                        int left_2 = (y) * width + (x - 2);
                        int right = (y) * width + (x + 1);
                        int right_2 = (y) * width + (x + 2);
                        int bottomleft = (y + 1) * width + (x - 1);
                        int bottomleft_2 = (y + 2) * width + (x - 2);
                        int bottom = (y - 1) * width + (x);
                        int bottom_2 = (y - 2) * width + (x);
                        int bottomRight = (y - 1) * width + (x + 1);
                        int bottomRight_2 = (y - 2) * width + (x + 2);

                        if (occu[center] == 0 &&
                                occu[topLeft] == 0 &&
                                occu[topLeft_2] == 0 &&
                                occu[top] == 0 &&
                                occu[top_2] == 0 &&
                                occu[topRight] == 0 &&
                                occu[topRight_2] == 0 &&
                                occu[left] == 0 &&
                                occu[left_2] == 0 &&
                                occu[right] == 0 &&
                                occu[right_2] == 0 &&
                                occu[bottomleft] == 0 &&
                                occu[bottomleft_2] == 0 &&
                                occu[bottom] == 0 &&
                                occu[bottom_2] == 0 &&
                                occu[bottomRight] == 0 &&
                                occu[bottomRight_2] == 0 &&
                                this->depthMap[center] != FREENECT_DEPTH_MM_NO_VALUE) {
                            thresholds[center] = (this->depthMap[center] + thresholds[center]) / 2;
                        }
                    }
                }
         */
    }

    int getThreshold(int x, int y) {
        return this->thresholds[y * width + x];
    }

    uint16_t* getThresholds() {
        return this->thresholds;
    }
};

#endif // BACKGROUNDMODEL_H
