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

#ifndef HOMOGENEITY_H
#define HOMOGENEITY_H

#include <backgroundmodel.h>
#include <iostream>
#include <stdlib.h>
#include <complex>
#include <stdbool.h>

using namespace std;

template <typename BACKGROUNDMODEL> class Homogeneity {
private:
    BACKGROUNDMODEL* bgmodel;

    uint16_t* current;
    int width;
    int height;
    uint16_t maxDistance;
    uint16_t minDistance;
    uint16_t thresholdOffset;

public:

    Homogeneity(uint16_t* depthMap, int width, int height) : bgmodel(new BACKGROUNDMODEL(depthMap, width, height)), current(depthMap), width(width), height(height), maxDistance(2100), minDistance(800), thresholdOffset(15) {
    }

    virtual ~Homogeneity() {
        delete bgmodel;
    }

    uint16_t* getThresholds() {
        return bgmodel->getThresholds();
    }

    BACKGROUNDMODEL* getBackgroundModel() {
        return this->bgmodel;
    }

    virtual void update(uint16_t* depthMap) {
        this->current = depthMap;
    }

    virtual void update(int* occu) {
        getBackgroundModel()->update(this->current);
        getBackgroundModel()->update(occu);
    }

    inline uint16_t getValue(const uint16_t* map, int wrap, int x, int y, int w, int h) {
        if (x < 0) x = wrap ? (x + w) : 0;
        if (y < 0) y = wrap ? (y + h) : 0;
        if (x >= w) x = wrap ? (x - w) : (w - 1);
        if (y >= h) y = wrap ? (y - h) : (h - 1);
        int idx = x + y * w;
        return map[idx];
    }

    virtual bool getCriteria(int x, int y) {
        /*
        int sum = 0;
        double currentDistance = 0;

        int width = this->width;
        int height = this->height;

        int up = (y > 0 ? y - 1 : y) * width;
        int there = y * width;
        int down = (y + 1 < height ? y + 1 : y) * width;
        int left = (x > 0 ? x - 1 : x);
        int right = (x + 1 < width ? x + 1 : x);
        int here = x;
        currentDistance = (this->current[here + there]);

        if (currentDistance == FREENECT_DEPTH_MM_NO_VALUE || currentDistance > getMaxDistance() || currentDistance < getMinDistance()) {
            return false;
        }

        if (currentDistance < getBackgroundModel()->getThreshold(x, y) && (fabs(getBackgroundModel()->getThreshold(x, y) - currentDistance) > getThresholdOffset())) {
            return true;
        }
        return false;
         */
    }

    uint16_t* getCurrent() {
        return this->current;
    }

    void setCurrent(uint16_t* value) {
        this->current = value;
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

    void setMaxDistance(uint16_t value) {
        this->maxDistance = value;
    }

    uint16_t getMaxDistance() {
        return this->maxDistance;
    }

    void setMinDistance(uint16_t value) {
        this->minDistance = value;
    }

    uint16_t getMinDistance() {
        return this->minDistance;
    }

    void setThresholdOffset(uint16_t value) {
        this->thresholdOffset = value;
    }

    uint16_t getThresholdOffset() {
        return this->thresholdOffset;
    }
};

#endif // HOMOGENITY_H
