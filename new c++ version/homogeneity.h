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
#ifndef HOMOGENEITY_H
#define HOMOGENEITY_H

#include <backgroundmodel.h>

class Homogeneity {
private:
    BackgroundModel bgmodel;

    uint16_t* current;
    int width;
    int height;

public:
    Homogeneity(uint16_t* model, int width, int height);
    uint16_t* getThresholds();
    void update(uint16_t* model);
    void update(int* occu);
    bool getCriteria(int x, int y);
};

#endif // HOMOGENITY_H
