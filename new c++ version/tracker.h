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
#ifndef TRACKER_H
#define TRACKER_H

#include <homogeneity.h>

class Tracker {
private:

    class DropStack {
    private:
        int* x;
        int* y;

        int pos;

    public:
        DropStack(int size);
        ~DropStack();

        void clear();
        void push(const int& x, const int& y);
        void pop();
        const int& size() const;
        const int& getX(const int& index) const;
        const int& getY(const int& index) const;
        bool isEmpty() const;
        const int& getLastX() const;
        const int& getLastY() const;
    };

public:

    class Contour : public DropStack {
    private:
        int linecount;
        int* lines;

    public:
        Contour(int size);
        ~Contour();
        
        void addLine();
        void clear();
        const int& lineStart(const int& index) const;
        const int& lineEnd(const int& index) const;
        bool lineEmpty() const;
        const int& getNumberOfPoints() const;
        const int& getNumberOfLines() const;
        int* getLines();
    };

private:
    int dx, dy, nCells, seedspacingx, seedspacingy;
    int* occu;
    int cdx, cdy, nConCells;
    int* con;
    DropStack growList;
    DropStack shrinkList;
    Contour contour;
    Homogeneity* homogeneity;

    void reinit();
    void findSeeders();
    void shrink();
    void grow();
    int dropNextToFrame(const int& x, const int& y) const;
    void makeContour();

public:
    Tracker(int dx, int dy, int seedspacingx, int seedspacingy, Homogeneity* homogeneity);
    ~Tracker();
    void track();
    Tracker::Contour& getContour();
    const int* getOccu() const;
};

#endif // TRACKER_H
