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

import java.awt.Dimension;

public class BackgroundModel{
    private Dimension imageResolution;
    private Dimension tileDimension;
    private int[] occu;
    private byte[] pixelArray;

    public BackgroundModel(int id, int[] soccu) {
        this.occu   = soccu;
    }

    public byte[] getPixelArray() {
        return pixelArray;
    }

    public void setPixelArray(byte[] pixelArray) {
        this.pixelArray = pixelArray;
    }
    

    public void recomputeStatistics() {
	/*call methods to compute statistics*/
    }

    private boolean isOccu(int x, int y) {
        int position = y * (int)getImageResolution().getWidth() + x;
        return (occu[position] != 0);
    }

    void setImageResolution(Dimension imageResolution) {
        this.imageResolution = imageResolution;
    }

    public Dimension getImageResolution() {
        return imageResolution;
    }

    public void initialize() {
	//put initialiation code here	
    }
}
