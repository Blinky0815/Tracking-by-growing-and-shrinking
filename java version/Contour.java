import javax.vecmath.Point2d;

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
 * Java version by Olaf Christ: Email:christ_o@gmx.de
 * 
 * @author Olaf Christ
 **/

public class Contour extends DropStack {
    private int nLines = 0;
    private int[] inci;
   
    public Contour(int size) {
        super(size);
        inci = new int[size];
    }

    public void addInci() {
        this.nLines++;
        this.inci[this.nLines]  = this.size();
    }

    public void reset() {
        this.clear();
        nLines = 0;
    }

    public int lineStart(int index) {
        return this.inci[index];
    }
    
    public int lineEnd(int index) {
        return this.inci[index + 1];
    }
    
    public boolean lineEmpty() {
        return this.size() == this.inci[this.nLines];
    }

    public int getNumberOfPoints() {
        return this.size();
    }

    public int getNumberOfLines() {
        return nLines;
    }
}
