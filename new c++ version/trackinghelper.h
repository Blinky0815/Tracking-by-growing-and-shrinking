/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2012 Olaf Christ 
 * email: christ_o@gmx.de
 * 
 * TrackingHelper is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * TrackingHelper is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRACKINGHELPER_H
#define TRACKINGHELPER_H


#include<SDL2Image.h>
#include <tracker.h>
#include <homogeneity.h>
#include <config.h>
#include "Point.h"
#include "RGBA.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <iterator>     // std::back_inserter
#include <algorithm>
#include <cstdio>
#include <climits>
#include <rotatingcaliper.h>
#include <ConvexHull.h>
#include <GrahamScanConvexHull.h>

using namespace std;

template <typename HOMOGENEITY> class TrackingHelper {
private:

    Configuration configuration;
    bool configured;
    HOMOGENEITY* homogeneity;
    Tracker<HOMOGENEITY>* tracker;

    std::vector<std::pair<int, int> > contour_list;
    std::vector<std::pair<int, int> > contour_points;
    float scalingX;
    float scalingY;


public:

    /*
    TrackingHelper(Configuration config) : configuration(config), configured(false), scalingX(1.0), scalingY(1.0) {
        homogeneity = new HOMOGENEITY(new uint16_t[640, 480], 640, 480);
    }
     */
    TrackingHelper(Configuration config, int width, int height) : configuration(config), configured(false), scalingX(1.0), scalingY(1.0) {
        homogeneity = new HOMOGENEITY(new uint16_t[width, height], width, height);
    }

    ~TrackingHelper() {
    }

    const int* getOccu() {
        return tracker->getOccu();
    }

    Configuration& getConfiguration() {
        return configuration;
    }

    void setScalingX(float value) {
        this->scalingX = value;
    }

    void setScalingY(float value) {
        this->scalingY = value;
    }

    float getScalingX() {
        return this->scalingX;
    }

    float getScalingY() {
        return this->scalingY;
    }

    void draw_contours(Contour& contour, SDL2Image* surface, RGBA& color) {
        for (int i = 0; i < contour.size(); i++) {
            surface->_pixelRGBA(contour.getX(i) * getScalingX(), contour.getY(i) * getScalingY(), color.getR(), color.getG(), color.getB(), color.getA());
        }
    }

    void draw_single_contour(Contour& contour, SDL2Image* surface, const int& start, const int& end, RGBA& color) {
        for (int i = start; i < end; i++) {
            surface->_pixelRGBA(contour.getX(i) * getScalingX(), contour.getY(i) * getScalingY(), color.getR(), color.getG(), color.getB(), color.getA());
        }
    }

    void getCenterOfMass(Contour& contour, const int& start, const int& end, int*x, int*y) {
        int x_ = 0;
        int y_ = 0;
        for (int i = start; i < end; i++) {
            x_ += contour.getX(i);
            y_ += contour.getY(i);
        }
        x_ /= (end - start);
        y_ /= (end - start);

        *x = x_;
        *y = y_;
    }

    void getMedianCenterOfMass(Contour& contour, const int& start, const int& end, int*x, int*y) {
        vector<int> xValues;
        vector<int> yValues;

        for (int i = start; i < end; i++) {
            xValues.push_back(contour.getX(i));
            yValues.push_back(contour.getY(i));
        }

        std::nth_element(xValues.begin(), xValues.begin() + xValues.size() / 2, xValues.end());
        int medianX = xValues[xValues.size() / 2];
        std::nth_element(yValues.begin(), yValues.begin() + yValues.size() / 2, yValues.end());
        int medianY = yValues[yValues.size() / 2];
        *x = medianX;
        *y = medianY;
    }

    void getAxisAlignedBoundingBox(Contour& contour, const int& start, const int& end, int*x0, int*y0, int*x1, int*y1) {
        int xmin = INT32_MAX;
        int ymin = INT32_MAX;
        int xmax = 0;
        int ymax = 0;

        for (int i = start; i < end; i++) {
            if (contour.getX(i) < xmin) {
                xmin = contour.getX(i);
            }
            if (contour.getY(i) < ymin) {
                ymin = contour.getY(i);
            }

            if (contour.getX(i) > xmax) {
                xmax = contour.getX(i);
            }
            if (contour.getY(i) > ymax) {
                ymax = contour.getY(i);
            }
        }

        *x0 = xmin;
        *y0 = ymin;
        *x1 = xmax;
        *y1 = ymax;
    }

    void applyMask(const uint16_t* mask, uint16_t* map, const int size) {
        for (int i = 0; i < size; i++) {
            map[i] *= mask[i];
        }
    }

    void convertTo16Bit(const unsigned char* mapImage, uint16_t* mask, const int size, const int numChannels) {
        for (int i = 0; i < size; i++) {
            mask[i] = mapImage[numChannels * i] == 255 ? 1 : 0;
        }
        /*
        int k = 0;
        for (int y = 0; y < 480; y++) {
            for (int x = 0; x < 640; x++) {
                cout << (int) mask[k++];
            }
            cout << endl;
        }
         */
    }

    void convertFloatToUint16(const vector<float_t>& normalizedDepthMap, vector<uint16_t>& integerDepthMap, float_t maxDepthValue = 10000.0f) {
        std::transform(normalizedDepthMap.begin(), normalizedDepthMap.end(), integerDepthMap.begin(), std::bind1st(std::multiplies<float_t>(), maxDepthValue));
    }

    void getConvexCombination(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int*x_ab, int*y_ab, int*x_bc, int*y_bc, int*x_cd, int*y_cd, int*x_da, int*y_da) {
        //http://en.wikipedia.org/wiki/Convex_combination
        *x_ab = 0.5 * x0 + x1 * 0.5;
        *y_ab = 0.5 * y0 + y1 * 0.5;

        *x_bc = 0.5 * x1 + x2 * 0.5;
        *y_bc = 0.5 * y1 + y2 * 0.5;

        *x_cd = 0.5 * x2 + x3 * 0.5;
        *y_cd = 0.5 * y2 + y3 * 0.5;

        *x_da = 0.5 * x3 + x0 * 0.5;
        *y_da = 0.5 * y3 + y0 * 0.5;
    }

    void getOBB(Contour& contour, const int& start, const int& end, int*x0, int*y0, int*x1, int*y1, int*x2, int*y2, int*x3, int*y3) {

        std::vector<point2d> point;
        for (int i = start; i < end; i++) {
            point2d p2d;
            p2d.x = contour.getX(i);
            p2d.y = contour.getY(i);
            point.push_back(p2d);
        }

        std::vector<point2d> convex_hull;
        GrahamScanConvexHull()(point, convex_hull);

        std::vector<std::pair<int, int> > convexhull;
        for (int i = convex_hull.size() - 1; i >= 0; i--) {
            std::pair<int, int> pair;
            pair.first = convex_hull.at(i).x;
            pair.second = convex_hull.at(i).y;
            convexhull.push_back(pair);
        }

        RotatingCaliper rcal;
        BoundingBox bbox = rcal.getMinimumBoundingBox(convexhull);
        *x0 = bbox.a.first;
        *y0 = bbox.a.second;
        *x1 = bbox.b.first;
        *y1 = bbox.b.second;
        *x2 = bbox.c.first;
        *y2 = bbox.c.second;
        *x3 = bbox.d.first;
        *y3 = bbox.d.second;
    }

    void getLargestContour(Contour& contours, int* start, int* end) {
        int maxSize = 0;
        int start_result = 0;
        int end_result = 0;

        for (int i = 0; i < contours.getNumberOfLines(); i++) {
            int start = contours.lineStart(i);
            int end = contours.lineEnd(i);
            if ((end - start) > maxSize) {
                maxSize = (end - start);
                start_result = start;
                end_result = end;
            }
        }
        *start = start_result;
        *end = end_result;
    }

    const int* getOccu() const {
        return tracker->getOccu();
    }

    HOMOGENEITY * getHomogeneity() {
        return homogeneity;
    }

    Contour & process(uint16_t* depthMap, int width, int height) {
        if (!configured) {
            tracker = new Tracker <HOMOGENEITY>(width, height, this->configuration.tracker_seed_spacing_x, this->configuration.tracker_seed_spacing_y, homogeneity);
            configured = true;
        }

        if (homogeneity != NULL) {
            homogeneity->update(depthMap);
        }

        tracker->track();
        return tracker->getContour();
    }

    void resampleAndConvertSingleContour(Contour& in, std::vector<std::pair<int, int> >& out, const double& sampleRate, const int& start, const int& end) {
        double step = (end - start) / sampleRate;
        for (double i = start; i < end; i += step) {
            int index = (int) i;
            std::pair<int, int> point;
            point.first = in.getX(index);
            point.second = in.getY(index);
            out.push_back(point);
        }
    }

    void interpolate(std::vector<std::pair<int, int> >& out, int x, int y, int x2, int y2) {
        bool yLonger = false;
        int shortLen = y2 - y;
        int longLen = x2 - x;
        if (abs(shortLen) > abs(longLen)) {
            int swap = shortLen;
            shortLen = longLen;
            longLen = swap;
            yLonger = true;
        }
        int decInc;
        if (longLen == 0) {
            decInc = 0;
        } else {
            decInc = (shortLen << 16) / longLen;
        }

        if (yLonger) {
            if (longLen > 0) {
                longLen += y;
                for (int j = 0x8000 + (x << 16) + decInc; y < longLen; ++y) {
                    std::pair<int, int> point;
                    point.first = j >> 16;
                    point.second = y;
                    out.push_back(point);
                    j += decInc;
                }
                return;
            }
            longLen += y;
            for (int j = 0x8000 + (x << 16) - decInc; y > longLen; --y) {
                std::pair<int, int> point;
                point.first = j >> 16;
                point.second = y;
                out.push_back(point);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += x;
            for (int j = 0x8000 + (y << 16) + decInc; x < longLen; ++x) {
                std::pair<int, int> point;
                point.first = x;
                point.second = j >> 16;
                out.push_back(point);
                j += decInc;
            }
            return;
        }
        longLen += x;
        for (int j = 0x8000 + (y << 16) - decInc; x > longLen; --x) {
            std::pair<int, int> point;
            point.first = x;
            point.second = j >> 16;
            out.push_back(point);
            j -= decInc;
        }
        return;
    }
};

#endif // TRACKINGHELPER_H
