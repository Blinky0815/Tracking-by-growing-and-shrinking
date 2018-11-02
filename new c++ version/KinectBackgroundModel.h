/* 
 * File:   KinectBackgroundModel.h
 * Author: blinky0815
 *
 * Created on August 6, 2014, 6:19 PM
 */

#ifndef KINECTBACKGROUNDMODEL_H
#define KINECTBACKGROUNDMODEL_H
#include <backgroundmodel.h>
#include <stdint.h>
#include <iostream>
#include <cstring>

using namespace std;

class KinectBackgroundModel : public BackgroundModel {
public:

    KinectBackgroundModel(uint16_t* depthMap, int width, int height) : BackgroundModel(depthMap, width, height) {
    }

    virtual void update(uint16_t* depthMap) {
        setDepthmap(depthMap);
    }

    virtual void update(int* occu) {

        for (int y = 1; y < getHeight() - 1; y++) {
            for (int x = 1; x < getWidth() - 1; x++) {
                int center = (y) * getWidth() + (x);
                int topLeft = (y - 1) * getWidth() + (x - 1);
                int topLeft_2 = (y - 2) * getWidth() + (x - 2);
                int top = (y - 1) * getWidth() + (x);
                int top_2 = (y - 2) * getWidth() + (x);
                int topRight = (y - 1) * getWidth() + (x + 1);
                int topRight_2 = (y - 2) * getWidth() + (x + 2);
                int left = (y) * getWidth() + (x - 1);
                int left_2 = (y) * getWidth() + (x - 2);
                int right = (y) * getWidth() + (x + 1);
                int right_2 = (y) * getWidth() + (x + 2);
                int bottomleft = (y + 1) * getWidth() + (x - 1);
                int bottomleft_2 = (y + 2) * getWidth() + (x - 2);
                int bottom = (y - 1) * getWidth() + (x);
                int bottom_2 = (y - 2) * getWidth() + (x);
                int bottomRight = (y - 1) * getWidth() + (x + 1);
                int bottomRight_2 = (y - 2) * getWidth() + (x + 2);

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
                        getDepthmap()[center] != 0) { // 0 == NO_VALUE
                    getThresholds()[center] = (getDepthmap()[center] + getThresholds()[center]) / 2;
                }
            }
        }
    }

private:

};

#endif /* KINECTBACKGROUNDMODEL_H */

