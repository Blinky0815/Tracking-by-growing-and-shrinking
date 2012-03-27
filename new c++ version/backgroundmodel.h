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
 * c++ version by Olaf Christ: Email:christ_o@gmx.de
 * 
 * @author Olaf Christ
 **/


#ifndef BACKGROUNDMODEL_H
#define BACKGROUNDMODEL_H
#include <stdint.h>

class BackgroundModel {
private:
    int width, height;
    uint16_t* thresholds;
    uint16_t* model;
    int size;

public:
    BackgroundModel(uint16_t* model, int width, int height);
    ~BackgroundModel();
    void update(uint16_t* model);
    void update(int* occu);
    int getThreshold(int x, int y);
    uint16_t* getThresholds();
};

#endif // BACKGROUNDMODEL_H
