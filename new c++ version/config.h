/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2012 Olaf Christ 
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

#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>

#define SEED_SPACING_X   20
#define SEED_SPACING_Y   20
#define NUM_TILES_X   20
#define NUM_TILES_Y   20
#define MIN_CONTOUR_LENGTH  150
#define TRIM_CONTOUR_LENGTH  4096
#define N_LONGEST_CONTOURS  0
#define SAMPLE_RATE                     1024

class Configuration {
private:
public:
    bool verbose;
    std::string timelog_filename;
    int timelog_compression_level;
    std::string contourlog_filename;
    int contourlog_compression_level;
    std::string convexhulllog_filename;
    int convexhulllog_compression_level;
    std::string boundingboxlog_filename;
    int boundingboxlog_compression_level;
    int tracker_seed_spacing_x;
    int tracker_seed_spacing_y;
    int tracker_num_tiles_x;
    int tracker_num_tiles_y;
    int tracker_min_contour_length;
    int tracker_trim_contour_length;
    int tracker_n_longest_contours;
    double samplerate;
    int minDistance = 300;
    int maxDistance = 1300;
    int thresholdOffset = 40;

    Configuration() :
    verbose(false),
    timelog_compression_level(9),
    contourlog_compression_level(9),
    convexhulllog_compression_level(9),
    boundingboxlog_compression_level(9),
    tracker_seed_spacing_x(SEED_SPACING_X),
    tracker_seed_spacing_y(SEED_SPACING_Y),
    tracker_num_tiles_x(NUM_TILES_X),
    tracker_num_tiles_y(NUM_TILES_Y),
    tracker_min_contour_length(MIN_CONTOUR_LENGTH),
    tracker_trim_contour_length(TRIM_CONTOUR_LENGTH),
    tracker_n_longest_contours(N_LONGEST_CONTOURS),
    samplerate(SAMPLE_RATE) {
    }

    ~Configuration() {
    }
};
#endif // CONFIG_H
