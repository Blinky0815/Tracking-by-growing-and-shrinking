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

#ifndef TRACKER_H
#define TRACKER_H

#include <homogeneity.h>
#include <cstring>
#include <iostream>

class DropStack {
private:
    int* x;
    int* y;

    int pos;

public:

    DropStack(int size) : x(new int[size]), y(new int[size]), pos(0) {
    }

    ~DropStack() {
        if (this->x != NULL) {
            delete[] this->x;
        }

        if (this->y != NULL) {
            delete[] this->y;
        }
    }

    void clear() {
        this->pos = 0;
    }

    void push(const int& x, const int& y) {
        this->x[this->pos] = x;
        this->y[this->pos++] = y;
    }

    void pop() {
        this->pos--;
    }

    const int& size() const {
        return this->pos;
    }

    const int& getX(const int& index) const {
        return this->x[index];
    }

    const int& getY(const int& index) const {
        return this->y[index];
    }

    bool isEmpty() const {
        return this->pos <= 0;
    }

    const int& getLastX() const {
        return this->x[this->pos - 1];
    }

    const int& getLastY() const {
        return this->y[this->pos - 1];
    }
};

class Contour : public DropStack {
private:
    int linecount;
    int* lines;

public:

    Contour(int size) : DropStack(size), linecount(0), lines(new int[size]) {
        this->lines[0] = 0;
    }

    ~Contour() {
        if (this->lines != NULL) {
            delete[] this->lines;
        }
    }

    void addLine() {
        this->linecount++;
        this->lines[this->linecount] = this->size();
    }

    void clear() {
        DropStack::clear();
        this->linecount = 0;
    }

    const int& lineStart(const int& index) const {
        return this->lines[index];
    }

    const int& lineEnd(const int& index) const {
        return this->lines[index + 1];
    }

    bool lineEmpty() const {
        return this->lines[this->linecount] == this->size();
    }

    const int& getNumberOfPoints() const {
        return this->size();
    }

    const int& getNumberOfLines() const {
        return this->linecount;
    }
};

template <typename HOMOGENEITY> class Tracker {
private:
    int dx, dy, nCells, seedspacingx, seedspacingy;
    int* occu;
    int cdx, cdy, nConCells;
    int* con;
    DropStack growList;
    DropStack shrinkList;
    Contour contour;
    HOMOGENEITY* homogeneity;

    void reinit() {
        std::memset(this->occu, 0, this->nCells * sizeof (int));
        std::memset(this->con, 0, this->nConCells * sizeof (int));

        this->growList.clear();
        this->shrinkList.clear();
    }

    void findSeeders() {
        for (int yy = 0; yy < this->dy; yy += this->seedspacingy) {
            int line = yy * this->dx;

            for (int xx = 0; xx < this->dx; xx += this->seedspacingx) {
                int index = line + xx;

                if (this->occu[index] == 0) {
                    if (this->homogeneity->getCriteria(xx, yy)) {
                        this->growList.push(xx, yy);
                        this->occu[index] = 1;
                    }
                }
            }
        }
    }

    void shrink() {
        int o;

        for (int i = 0; i < this->shrinkList.size(); i++) {
            this->occu[this->shrinkList.getY(i) * this->dx + this->shrinkList.getX(i)] = 0;
        }

        while (!this->shrinkList.isEmpty()) {
            int drx = shrinkList.getLastX();
            int dry = shrinkList.getLastY();

            this->shrinkList.pop();

            if (this->homogeneity->getCriteria(drx, dry)) {
                this->growList.push(drx, dry);
            } else {
                int ax, ay;

                ax = drx;
                ay = dry + 1;

                o = ay * this->dx + ax;

                if (ay < this->dy && this->occu[o] != 0) {
                    this->occu[o] = 0;
                    this->shrinkList.push(ax, ay);
                }

                ax++;
                ay--;

                o += 1 - this->dx;

                if (ax < this->dx && this->occu[o] != 0) {
                    this->occu[o] = 0;
                    this->shrinkList.push(ax, ay);
                }

                ax--;
                ay--;

                o -= this->dx + 1;

                if (ay >= 0 && this->occu[o] != 0) {
                    this->occu[o] = 0;
                    this->shrinkList.push(ax, ay);
                }

                ax--;
                ay++;

                o += this->dx - 1;

                if (ax >= 0 && this->occu[o] != 0) {
                    this->occu[o] = 0;
                    this->shrinkList.push(ax, ay);
                }
            }
        }

        for (int i = 0; i < this->growList.size(); i++) {
            this->occu[this->growList.getY(i) * this->dx + this->growList.getX(i)] = 1;
        }
    }

    void grow() {
        int o;

        while (!this->growList.isEmpty()) {
            int drx = this->growList.getLastX();
            int dry = this->growList.getLastY();

            this->growList.pop();

            bool generated = false;

            int ax = drx;
            int ay = dry + 1;

            o = ay * this->dx + ax;

            if (ay >= this->dy) {
                generated = true;

                this->con[ay * this->cdx + ax] |= 1;
            } else {
                if (this->occu[o] == 0) {
                    if (this->homogeneity->getCriteria(ax, ay)) {
                        this->occu[o] = 1;
                        this->growList.push(ax, ay);
                    } else {
                        generated = true;

                        this->con[ay * this->cdx + ax] |= 1;
                    }
                }
            }

            ax++;
            ay--;

            o += 1 - this->dx;

            if (ax >= this->dx) {
                generated = true;

                this->con[(ay + 1) * this->cdx + ax] |= 2;
            } else {
                if (this->occu[o] == 0) {
                    if (this->homogeneity->getCriteria(ax, ay)) {
                        this->occu[o] = 1;
                        this->growList.push(ax, ay);
                    } else {
                        generated = true;

                        this->con[(ay + 1) * this->cdx + ax] |= 2;
                    }
                }
            }

            ax--;
            ay--;

            o -= this->dx + 1;

            if (ay < 0) {
                generated = true;

                this->con[(ay + 1) * this->cdx + (ax + 1)] |= 4;
            } else {
                if (this->occu[o] == 0) {
                    if (this->homogeneity->getCriteria(ax, ay)) {
                        this->occu[o] = 1;
                        this->growList.push(ax, ay);
                    } else {
                        generated = true;

                        this->con[(ay + 1) * this->cdx + (ax + 1)] |= 4;
                    }
                }
            }

            ax--;
            ay++;

            o += this->dx - 1;

            if (ax < 0) {
                generated = true;

                this->con[ay * this->cdx + (ax + 1)] |= 8;
            } else {
                if (this->occu[o] == 0) {
                    if (this->homogeneity->getCriteria(ax, ay)) {
                        this->occu[o] = 1;
                        this->growList.push(ax, ay);
                    } else {
                        generated = true;

                        this->con[ay * this->cdx + (ax + 1)] |= 8;
                    }
                }
            }

            if (generated) {
                this->shrinkList.push(drx, dry);
            }
        }
    }

    int dropNextToFrame(const int& x, const int& y) const {
        if ((this->con[(y + 1) * this->cdx + x] & 1) != 0) {
            return 0;
        }

        if ((this->con[(y + 1) * this->cdx + x + 1] & 2) != 0) {
            return 1;
        }

        if ((this->con[y * this->cdx + x] & 4) != 0) {
            return 2;
        }

        if ((this->con[y * this->cdx + x] & 8) != 0) {
            return 3;
        }

        return -1;
    }

    void makeContour() {
        this->contour.clear();

        int iOfFirstUndeleted = 0;

        int currX = 0;
        int currY = 0;
        int dir = -1;

        while (iOfFirstUndeleted < this->shrinkList.size() && dir == -1) {
            currX = this->shrinkList.getX(iOfFirstUndeleted);
            currY = this->shrinkList.getY(iOfFirstUndeleted);

            dir = this->dropNextToFrame(currX, currY);

            iOfFirstUndeleted++;
        }

        int p0X = 0;
        int p0Y = 0;
        int p1X = 0;
        int p1Y = 0;

        while (dir != -1) {
            switch (dir) {
                case 0:
                    p0X = currX;
                    p0Y = currY + 1;
                    p1X = currX + 1;
                    p1Y = currY + 1;
                    break;
                case 1:
                    p0X = currX + 1;
                    p0Y = currY + 1;
                    p1X = currX + 1;
                    p1Y = currY;
                    break;
                case 2:
                    p0X = currX + 1;
                    p0Y = currY;
                    p1X = currX;
                    p1Y = currY;
                    break;
                case 3:
                    p0X = currX;
                    p0Y = currY;
                    p1X = currX;
                    p1Y = currY + 1;
                    break;
                default:
                    break;
            }

            bool found;

            do {
                this->contour.push(p0X, p0Y);

                int oO = p0Y * this->cdx + p0X;
                found = false;

                if (this->con[oO] != 0) {
                    switch (dir) {
                        case 0:
                            if ((this->con[oO] & 0x8) != 0) {
                                p1X = p0X;
                                p1Y = p0Y + 1;
                                found = true;
                                this->con[oO] ^= 0x8;
                                dir = 3;
                            }
                            break;
                        case 1:
                            if ((this->con[oO] & 0x1) != 0) {
                                p1X = p0X + 1;
                                p1Y = p0Y;
                                found = true;
                                this->con[oO] ^= 0x1;
                                dir = 0;
                            }
                            break;
                        case 2:
                            if ((this->con[oO] & 0x2) != 0) {
                                p1X = p0X;
                                p1Y = p0Y - 1;
                                found = true;
                                this->con[oO] ^= 0x2;
                                dir = 1;
                            }
                            break;
                        case 3:
                            if ((this->con[oO] & 0x4) != 0) {
                                p1X = p0X - 1;
                                p1Y = p0Y;
                                found = true;
                                this->con[oO] ^= 0x4;
                                dir = 2;
                            }
                            break;
                        default:
                            break;
                    }
                }

                if (!found) {
                    if (this->con[oO] != 0) {
                        switch (dir) {
                            case 0:
                                if ((this->con[oO] & 0x1) != 0) {
                                    p1X = p0X + 1;
                                    p1Y = p0Y;
                                    found = true;
                                    this->con[oO] ^= 0x1;
                                    dir = 0;
                                }
                                break;
                            case 1:
                                if ((this->con[oO] & 0x2) != 0) {
                                    p1X = p0X;
                                    p1Y = p0Y - 1;
                                    found = true;
                                    this->con[oO] ^= 0x2;
                                    dir = 1;
                                }
                                break;
                            case 2:
                                if ((this->con[oO] & 0x4) != 0) {
                                    p1X = p0X - 1;
                                    p1Y = p0Y;
                                    found = true;
                                    this->con[oO] ^= 0x4;
                                    dir = 2;
                                }
                                break;
                            case 3:
                                if ((this->con[oO] & 0x8) != 0) {
                                    p1X = p0X;
                                    p1Y = p0Y + 1;
                                    found = true;
                                    this->con[oO] ^= 0x8;
                                    dir = 3;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }

                if (!found) {
                    if (this->con[oO] != 0) {
                        switch (dir) {
                            case 0:
                                if ((this->con[oO] & 0x2) != 0) {
                                    p1X = p0X;
                                    p1Y = p0Y - 1;
                                    found = true;
                                    this->con[oO] ^= 0x2;
                                    dir = 1;
                                }
                                break;
                            case 1:
                                if ((this->con[oO] & 0x4) != 0) {
                                    p1X = p0X - 1;
                                    p1Y = p0Y;
                                    found = true;
                                    this->con[oO] ^= 0x4;
                                    dir = 2;
                                }
                                break;
                            case 2:
                                if ((this->con[oO] & 0x8) != 0) {
                                    p1X = p0X;
                                    p1Y = p0Y + 1;
                                    found = true;
                                    this->con[oO] ^= 0x8;
                                    dir = 3;
                                }
                                break;
                            case 3:
                                if ((this->con[oO] & 0x1) != 0) {
                                    p1X = p0X + 1;
                                    p1Y = p0Y;
                                    found = true;
                                    this->con[oO] ^= 0x1;
                                    dir = 0;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }

                p0X = p1X;
                p0Y = p1Y;
            } while (found);

            this->contour.addLine();

            dir = -1;

            while (iOfFirstUndeleted < this->shrinkList.size() && dir == -1) {
                currX = this->shrinkList.getX(iOfFirstUndeleted);
                currY = this->shrinkList.getY(iOfFirstUndeleted);

                dir = this->dropNextToFrame(currX, currY);

                iOfFirstUndeleted++;
            }
        }
    }

public:

    Tracker(int dx, int dy, int seedspacingx, int seedspacingy, HOMOGENEITY* homogeneity) : dx(dx), dy(dy), nCells(dx * dy), seedspacingx(seedspacingx), seedspacingy(seedspacingy), occu(new int[nCells]), cdx(dx + 1), cdy(dy + 1), nConCells(cdx * cdy), con(new int[nConCells]), growList(nCells), shrinkList(nCells), contour(nConCells), homogeneity(homogeneity) {
        this->reinit();
    }

    ~Tracker() {
        if (this->occu != NULL) {
            delete[] this->occu;
        }

        if (this->con != NULL) {
            delete[] this->con;
        }
    }

    void track(bool updateBackgroundModel = false) {
        this->findSeeders();
        this->shrink();
        this->grow();
        this->makeContour();
        if (updateBackgroundModel) {
            this->homogeneity->update(this->occu);
        }
    }

    Contour& getContour() {
        return this->contour;
    }

    const int* getOccu() const {
        return occu;
    }
};

#endif // TRACKER_H
