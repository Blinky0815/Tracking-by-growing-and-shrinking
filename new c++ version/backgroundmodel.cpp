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

#include <backgroundmodel.h>
#include <iostream>
#include <cstring>

BackgroundModel::BackgroundModel(uint16_t* model, int width, int height) : width(width), height(height), thresholds(new uint16_t[width*height]), size(width*height) {
//initialize thresholds.
//change the initial value according to your needs
    std::memset(this->thresholds, 0, width * height * sizeof (uint16_t));
}

BackgroundModel::~BackgroundModel() {
    if (this->thresholds != NULL) {
        delete[] this->thresholds;
    }
}

void BackgroundModel::update(uint16_t* model) {
    this->model = model;
}

void BackgroundModel::update(int* occu) {

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

		/*
		put your model update code here
		*/
            }
        } 
    }
}

int BackgroundModel::getThreshold(int x, int y) {
    return this->thresholds[y * width + x];
}

uint16_t* BackgroundModel::getThresholds() {
    return this->thresholds;
}
