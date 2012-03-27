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
#include <homogeneity.h>
#include <iostream>

Homogeneity::Homogeneity(uint16_t* model, int width, int height) : bgmodel(model, width, height), current(model), width(width), height(height) {
}

void Homogeneity::update(uint16_t* model) {
    this->current = model;
}

void Homogeneity::update(int* occu) {
    this->bgmodel.update(this->current);
    this->bgmodel.update(occu);
}


bool Homogeneity::getCriteria(int x, int y) {
	
    int sum = 0;
    double currentPixelLuminance = 0;

    int width = this->width;
    int height = this->height;

    int up = (y > 0 ? y - 1 : y) * width;
    int there = y * width;
    int down = (y + 1 < height ? y + 1 : y) * width;
    int left = (x > 0 ? x - 1 : x);
    int right = (x + 1 < width ? x + 1 : x);
    int here = x;

/*
Put your own criteria here
*/
    /*    
        sum =
                this->current[up + left]
                + this->current[up + here]
                + this->current[up + right]
                + this->current[there + left]
                + this->current[there + right]
                + this->current[down + left]
                + this->current[down + here]
                + this->current[down + right];
     */
    currentPixelLuminance = (this->current[here + there]);

    if (currentPixelLuminance < this->bgmodel.getThreshold(x, y) {
        return true;
    }

    return false;
}

uint16_t* Homogeneity::getThresholds() {
    bgmodel.getThresholds();
}
