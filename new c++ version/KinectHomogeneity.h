/* 
 * File:   KinectHomogeneity.h
 * Author: blinky0815
 *
 * Created on August 6, 2014, 6:18 PM
 */

#ifndef KINECTHOMOGENEITY_H
#define KINECTHOMOGENEITY_H

#include <backgroundmodel.h>
#include <homogeneity.h>
#include <iostream>
#include <stdlib.h>
#include <complex>
#include <stdbool.h>
using namespace std;

template <typename BACKGROUNDMODEL> class KinectHomogeneity : public Homogeneity<BACKGROUNDMODEL> {
public:

    KinectHomogeneity(uint16_t* depthMap, int width, int height) : Homogeneity<BACKGROUNDMODEL>(depthMap, width, height) {
    }

    virtual void update(uint16_t* depthMap) {
        Homogeneity<BACKGROUNDMODEL>::setCurrent(depthMap);
    }

    virtual void update(int* occu) {

        Homogeneity<BACKGROUNDMODEL>::getBackgroundModel()->update(Homogeneity<BACKGROUNDMODEL>::getCurrent());
        Homogeneity<BACKGROUNDMODEL>::getBackgroundModel()->update(occu);
    }

    virtual bool getCriteria(int x, int y) {
        int sum = 0;
        double currentDistance = 0;

        int width = Homogeneity<BACKGROUNDMODEL>::getWidth();
        int height = Homogeneity<BACKGROUNDMODEL>::getHeight();

        uint16_t* currentPtr = Homogeneity<BACKGROUNDMODEL>::getCurrent();

        currentDistance = this->getValue(currentPtr, 1, x, y, width, height);

        if (currentDistance == 0 || currentDistance > Homogeneity<BACKGROUNDMODEL>::getMaxDistance() || currentDistance < Homogeneity<BACKGROUNDMODEL>::getMinDistance()) {
            return false;
        }

        if (currentDistance < Homogeneity<BACKGROUNDMODEL>::getBackgroundModel()->getThreshold(x, y) && (fabs(Homogeneity<BACKGROUNDMODEL>::getBackgroundModel()->getThreshold(x, y) - currentDistance) > Homogeneity<BACKGROUNDMODEL>::getThresholdOffset())) {
            return true;
        }
        return false;
    }
private:
};

#endif /* KINECTHOMOGENEITY_H */

