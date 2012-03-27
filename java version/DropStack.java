/*
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
 * Java version by Olaf Christ and Matthias - Kodoschaf - Klostermann: Email:christ_o@gmx.de
 * @author Olaf Christ & Matthias - Kodoschaf - Klostermann
 **/

public class DropStack{
    private final int[] x;
    private final int[] y;
    private int pos;

    public DropStack(int size) {
        this.x = new int[size];
        this.y = new int[size];
        this.pos = 0;
    }

    public void clear() {
        this.pos = 0;
    }

    public void add(int x, int y) {
        this.x[this.pos]      = x;
        this.y[this.pos++]    = y;
    }

    public int getX(int index) {
        return this.x[index];
    }

    public int getY(int index) {
        return this.y[index];
    }

    public int getLastX() {
        return this.x[this.pos - 1];
    }

    public int getLastY() {
        return this.y[this.pos - 1];
    }
    
    public void remove() {
        this.pos--;
    }

    public boolean isEmpty() {
        return this.pos == 0;
    }

    public int size() {
        return this.pos;
    }
}
