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
#include <tracker.h>

#include <cstring>
#include <iostream>

Tracker::DropStack::DropStack(int size) : x(new int[size]), y(new int[size]), pos(0) {
}

Tracker::DropStack::~DropStack() {
    if (this->x != NULL) {
        delete[] this->x;
    }

    if (this->y != NULL) {
        delete[] this->y;
    }
}

void Tracker::DropStack::clear() {
    this->pos = 0;
}

void Tracker::DropStack::push(const int& x, const int& y) {
    this->x[this->pos] = x;
    this->y[this->pos++] = y;
}

void Tracker::DropStack::pop() {
    this->pos--;
}

const int& Tracker::DropStack::size() const {
    return this->pos;
}

const int& Tracker::DropStack::getX(const int& index) const {
    return this->x[index];
}

const int& Tracker::DropStack::getY(const int& index) const {
    return this->y[index];
}

bool Tracker::DropStack::isEmpty() const {
    return this->pos <= 0;
}

const int& Tracker::DropStack::getLastX() const {
    return this->x[this->pos - 1];
}

const int& Tracker::DropStack::getLastY() const {
    return this->y[this->pos - 1];
}

Tracker::Contour::Contour(int size) : DropStuck(size), linecount(0), lines(new int[size]) {
    this->lines[0] = 0;
}

Tracker::Contour::~Contour() {
    if (this->lines != NULL) {
        delete[] this->lines;
    }
}

void Tracker::Contour::addLine() {
    this->linecount++;
    this->lines[this->linecount] = this->size();
}

void Tracker::Contour::clear() {
    Tracker::DropStack::clear();
    this->linecount = 0;
}

const int& Tracker::Contour::lineStart(const int& index) const {
    return this->lines[index];
}

const int& Tracker::Contour::lineEnd(const int& index) const {
    return this->lines[index + 1];
}

bool Tracker::Contour::lineEmpty() const {
    return this->lines[this->linecount] == this->size();
}

const int& Tracker::Contour::getNumberOfPoints() const {
    return this->size();
}

const int& Tracker::Contour::getNumberOfLines() const {
    return this->linecount;
}

Tracker::Tracker(int dx, int dy, int seedspacingx, int seedspacingy, Homogeneity* homogeneity) : dx(dx), dy(dy), nCells(dx * dy), seedspacingx(seedspacingx), seedspacingy(seedspacingy), occu(new int[nCells]), cdx(dx + 1), cdy(dy + 1), nConCells(cdx * cdy), con(new int[nConCells]), growList(nCells), shrinkList(nCells), contour(nConCells), homogeneity(homogeneity) {
    this->reinit();
}

Tracker::~Tracker() {
    if (this->occu != NULL) {
        delete[] this->occu;
    }

    if (this->con != NULL) {
        delete[] this->con;
    }
}

void Tracker::reinit() {
    std::memset(this->occu, 0, this->nCells * sizeof (int));
    std::memset(this->con, 0, this->nConCells * sizeof (int));

    this->growList.clear();
    this->shrinkList.clear();
}

void Tracker::findSeeders() {
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

void Tracker::shrink() {
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

void Tracker::grow() {
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

int Tracker::dropNextToFrame(const int& x, const int& y) const {
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

const int* Tracker::getOccu() const {
    return occu;
}

void Tracker::makeContour() {
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

void Tracker::track() {
    this->findSeeders();
    this->shrink();
    this->grow();
    this->makeContour();
    this->homogeneity->update(this->occu);
}

Tracker::Contour& Tracker::getContour() {
    return this->contour;
}
