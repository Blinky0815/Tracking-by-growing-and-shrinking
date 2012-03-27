/**
/*
 *
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
 * Java version by Olaf Christ and Matthias Klostermann: Email:christ_o@gmx.de
 * 
 * @author Olaf Christ & Matthias Klostermann
 **/

public abstract class Homogeneity {
    private RegionTracker regionTracker;
    private byte[] pixelArray;

    public Homogeneity() {
    }

    public void setRegionTracker(RegionTracker regionTracker) {
        this.regionTracker = regionTracker;
    }

    public void setPixelArray(byte[] pixelArray) {
        this.pixelArray = pixelArray;
    }

    public byte[] getPixelArray() {
        return pixelArray;
    }
    
    public abstract int getMaskXY(int x, int y);

    public boolean isOccupied(int x, int y) {
        return regionTracker.isOccu(x, y);
    }

    public boolean isOccupied(int x, int y, int id) {
        return regionTracker.isOccu(x, y, id);
    }
}
