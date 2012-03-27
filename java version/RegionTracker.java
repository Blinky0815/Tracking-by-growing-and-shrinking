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

public class RegionTracker {

    private int dx;             //dimension of the image (in picture elements)
    private int dy;             //dimension of the image (in picture elements)
    private int nCells;         //=dx*dy
    private int[] occu;        //array of size dx*dy to mark which picture elements are occupied by a drop 
    private int gdx;            //dimension of the connectivity grid (gdx=dx+1;gdy=dy+1)
    private int gdy;            //dimension of the connectivity grid (gdx=dx+1;gdy=dy+1)
    private int nConcells;      //=gdx*gdy
    private int[] con;         //the connectivity grid: array of size gdx*gdy; it stores the boundaries of the regions
    private int trackCounter;
    private int resamplingRate; // default: 1024
    private int minContourPoints;   // default: 10
    private DropStack growList;
    private DropStack shrinkList;
    private int nSize;
    private Contour    boundaries;
    private Contour    resampledBoundaries;
    private Homogeneity homogeneity;
    //abritrarily chosen...5 is a good value
    private int distanceBetweenCountourPoints = 5;
    private int xSpacing = 1,  ySpacing = 1;

    public RegionTracker(int dx_p, int dy_p, Homogeneity homogeneity_p, int id) {
        dx = dx_p;
        dy = dy_p;
        nCells = dx * dy;
        resamplingRate  = 1024;
        boundaries  = new Contour(nCells * 2);
        resampledBoundaries  = new Contour(nCells * 3);
        minContourPoints    = 10;
        growList = new DropStack(nCells);
        shrinkList = new DropStack(nCells);
        occu = new int[nCells];
        gdx = dx + 1;
        gdy = dy + 1;
        nConcells = gdx * gdy;
        con = new int[nConcells];
        homogeneity = homogeneity_p;
        homogeneity.setRegionTracker(this);

        trackCounter = 0;

        reinit();
    }

    /**
     * The default is 5. Large values  might distort the contour
    @param args
     **/
    public void setDistanceBetweenCountourPoints(int distanceBetweenCountourPoints) {
        this.distanceBetweenCountourPoints = distanceBetweenCountourPoints;
    }

    /**
     * The default is 5. Large values  might distort the contour
    @param args
     **/
    public int getDistanceBetweenCountourPoints() {
        return distanceBetweenCountourPoints;
    }

    public int getTrackCounter() {
        return trackCounter;
    }

    public int getXSpacing() {
        return xSpacing;
    }

    public int getYSpacing() {
        return ySpacing;
    }

    public int getMinContourPoints() {
        return minContourPoints;
    }

    public void setMinContourPoints(int minContourPoints) {
        this.minContourPoints = minContourPoints;
    }

    public void setYSpacing(int ySpacing) {
        this.ySpacing = ySpacing;
    }

    public void setXSpacing(int xSpacing) {
        this.xSpacing = xSpacing;
    }

    public boolean isOccu(int x, int y) {
        int position = y * dx + x;
        return (occu[position] != 0);
    }

    public boolean isOccu(int x, int y, int id) {
        int position = y * dx + x;
        return (occu[position] == id);
    }

    public Contour getBoundaries() {
        return this.boundaries;
    }

    public void setResamplingRate(int resamplingRate) {
        this.resamplingRate = resamplingRate;
    }

    public int getResamplingRate() {
        return resamplingRate;
    }

    public Contour getResampledBoundaries() {
        return resampledBoundaries;
    }

    public int[] getOccu() {
        return occu;
    }

    public void reinit() {
        clearConnectivityGridArray(nConcells);
        clearOccupiedPixels(nCells);
        nSize = 0;
        
        growList.clear();
        shrinkList.clear();

        boundaries.reset();
    }

    public void track() {
        findSeeders();
        shrink();
        grow();
        makeBoundaries(getDistanceBetweenCountourPoints());
        trackCounter++;
    }

    private boolean findSeeders() {
        boolean foundAny = false;
        
        for (int yy = 0; yy < dy; yy += getXSpacing()) {
            int                 line    = yy * dx;
            
            for (int xx = 0; xx < dx; xx += getYSpacing()) {
                int pixelAddress = (line + xx);
                if (occu[pixelAddress] == 0) {
                    if (homogeneity.getMaskXY(xx, yy) != 0) {
                        growList.add(xx, yy);
                        occu[pixelAddress] = 1;
                        nSize++;
                        foundAny = true;
                    }
                }
            }
        }
        return foundAny;
    }

    private void grow() {
       
        int o;
        while (!growList.isEmpty()) {
            int             drx  = growList.getLastX();
            int             dry  = growList.getLastY();
            growList.remove();
            boolean generated = false;
            
            // 0
            int ax = drx;
            int ay = dry + 1;
            
            boolean edge = false;
            if(ay >= dy) {
                edge    = true;
            }
            
            if (!edge) {
                o = ay * dx + ax;

                if (occu[o] == 0) {
                    if (homogeneity.getMaskXY(ax, ay) != 0) {
                        occu[o] = 1;
                        growList.add(ax, ay);
                        nSize++;
                    } else {
                        edge = true;
                    }
                } 
            }
            
            if (edge) {
                generated = true;
                con[ay * gdx + ax] |= 1;
            }
            
            // 1
            ax++;
            ay--;
            
            edge = false;
            if(ax >= dx) {
                edge    = true;
            }

            if (!edge) {
                o = ay * dx + ax;

                if (occu[o] == 0) {
                    if (homogeneity.getMaskXY(ax, ay) != 0) {
                        occu[o] = 1;
                        growList.add(ax, ay);
                        nSize++;
                    } else {
                        edge = true;
                    }
                } 
            }


            if (edge) {
                generated = true;
                con[(ay + 1) * gdx + ax] |= 2;
            }
            
            // 2
            ax--;
            ay--;
            
            edge = false;
            if(ay < 0) {
                edge    = true;
            }

            if (!edge) {
                o = ay * dx + ax;

                if (occu[o] == 0) {
                    if (homogeneity.getMaskXY(ax, ay) != 0) {
                        occu[o] = 1;
                        growList.add(ax, ay);
                        nSize++;
                    } else {
                        edge = true;
                    }
                } 
            }
            
            if (edge) {
                generated = true;
                con[(ay + 1) * gdx + (ax + 1)] |= 4;
            }
            
            // 3
            ax--;
            ay++;
            
            edge = false;
            if(ax < 0) {
                edge    = true;
            }

            if (!edge) {
                o = ay * dx + ax;

                if (occu[o] == 0) {
                    if (homogeneity.getMaskXY(ax, ay) != 0) {
                        occu[o] = 1;
                        growList.add(ax, ay);
                        nSize++;
                    } else {
                        edge = true;
                    }
                } 
            }
            
            if (edge) {
                generated = true;
                con[ay * gdx + (ax + 1)] |= 8;
            }

            if (generated) {
                shrinkList.add(drx, dry);
            }
        }
    }

    private void shrink() {
        int o;
        for (int i = 0; i < shrinkList.size(); i++) {
            o = shrinkList.getY(i) * dx + shrinkList.getX(i);
            occu[o] = 0;
            nSize--;
        }

        while (!shrinkList.isEmpty()) {
//            int                 drx = shrinkList.remove();
//            int                 dry = shrinkList.remove();
            int                 drx = shrinkList.getLastX();
            int                 dry = shrinkList.getLastY();
            shrinkList.remove();

            if (homogeneity.getMaskXY(drx, dry) != 0) {
                growList.add(drx, dry);
            } else {
                int ax, ay;
                
                // 0
                ax = drx;
                ay = dry + 1;
                if (ay < dy) {
                    o = ay * dx + ax;
                    if(occu[o] != 0) {
                        occu[o] = 0;
                        nSize--;
                        shrinkList.add(ax, ay);
                    }
                }

                // 1
                ax++;
                ay--;
                if ((ax < dx)) {
                    o = ay * dx + ax;
                    if(occu[o] != 0) {
                        occu[o] = 0;
                        nSize--;
                        shrinkList.add(ax, ay);
                    }
                }

                // 2
                ax--;
                ay--;
                if ((ay >= 0)) {
                    o = ay * dx + ax;
                    if(occu[o] != 0) {
                        occu[o] = 0;
                        nSize--;
                        shrinkList.add(ax, ay);
                    }
                }

                // 3
                ax--;
                ay++;
                if ((ax >= 0)) { 
                    o = ay * dx + ax;
                    if(occu[o] != 0) {
                        occu[o] = 0;
                        nSize--;
                        shrinkList.add(ax, ay);
                    }
                }
            }
        }

        for (int i = 0; i < growList.size(); i++) {
            o = growList.getY(i) * dx + growList.getX(i);
            occu[o] = 1;
            nSize++;
        }
    }

    private int dropNextToFrame(int x, int y) {
        if((con[(y + 1) * gdx + x] & 1) !=  0) {
            return 0;
        }
        
        if((con[(y + 1) * gdx + x + 1] & 2) !=  0) {
            return 1;
        }

        if((con[y * gdx + x + 1] & 4) !=  0) {
            return 2;
        }

        if((con[y * gdx + x] & 8) !=  0) {
            return 3;
        }
        
        return -1;
    }

    private void interpolate(int x, int y, int x2, int y2) {
        boolean yLonger = false;
        int shortLen = y2 - y;
        int longLen = x2 - x;
        if (Math.abs(shortLen) > Math.abs(longLen)) {
            int swap = shortLen;
            shortLen = longLen;
            longLen = swap;
            yLonger = true;
        }
        int decInc;
        if (longLen == 0) {
            decInc = 0;
        } else {
            decInc = (shortLen << 16) / longLen;
        }
        if (yLonger) {
            if (longLen > 0) {
                longLen += y;
                for (int j = 0x8000 + (x << 16) + decInc; y < longLen; ++y) {
                    boundaries.add(j >> 16, y);
                    j += decInc;
                }
                return;
            }
            longLen += y;
            for (int j = 0x8000 + (x << 16) - decInc; y > longLen; --y) {
                boundaries.add(j >> 16, y);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += x;
            for (int j = 0x8000 + (y << 16) + decInc; x < longLen; ++x) {
                boundaries.add(x, j >> 16);
                j += decInc;
            }
            return;
        }
        longLen += x;
        for (int j = 0x8000 + (y << 16) - decInc; x > longLen; --x) {
            boundaries.add(x, j >> 16);
            j -= decInc;
        }
        return;
    }
    
    /**
     * The default is 5. Large values might distort the contour
    @param args
     **/
    private void makeBoundaries(int everyX) {
        boundaries.reset();
        
        int                         iOfFirstUndeleted   = 0;
        int                         currX   = 0;
        int                         currY   = 0;
        int                         dir     = -1;
        
        while(iOfFirstUndeleted < shrinkList.size() && dir == -1) {
            currX   = shrinkList.getX(iOfFirstUndeleted);
            currY   = shrinkList.getY(iOfFirstUndeleted);
            
            dir     = dropNextToFrame(currX, currY);
            
            iOfFirstUndeleted++;
        }
        
        int                         p0X     = 0;
        int                         p0Y     = 0;
        int                         p1X     = 0;
        int                         p1Y     = 0;
        
        while(dir != -1) {
            switch(dir) {
                case 0:
                    p0X     = currX;
                    p0Y     = currY + 1;
                    p1X     = currX + 1;
                    p1Y     = currY + 1;
                    break;
                case 1:
                    p0X     = currX + 1;
                    p0Y     = currY + 1;
                    p1X     = currX + 1;
                    p1Y     = currY;
                    break;
                case 2:
                    p0X     = currX + 1;
                    p0Y     = currY;
                    p1X     = currX;
                    p1Y     = currY;
                    break;
                case 3:
                    p0X     = currX;
                    p0Y     = currY;
                    p1X     = currX;
                    p1Y     = currY + 1;
                    break;
            }
            
            boolean     found;
            int         counter = 0;
            
            do {
                if((counter % everyX) == 0) {
                    if(!this.boundaries.lineEmpty()) {
                        this.interpolate(this.boundaries.getLastX(), this.boundaries.getLastY(), p0X, p0Y);
                    }
                    
                    this.boundaries.add(p0X, p0Y);
                }
                
                int     oO  = p0Y * gdx + p0X;
                found   = false;
                
                if(con[oO] != 0) {
                    switch(dir) {
                        case 0:
                            if((con[oO] & 0x8) != 0) {
                                p1X     = p0X;
                                p1Y     = p0Y + 1;
                                found   = true;
                                con[oO] ^= 0x8;
                                dir     = 3;
                            }
                            break;
                        case 1:
                            if((con[oO] & 0x1) != 0) {
                                p1X     = p0X + 1;
                                p1Y     = p0Y;
                                found   = true;
                                con[oO] ^= 0x1;
                                dir     = 0;
                            }
                            break;
                        case 2:
                            if((con[oO] & 0x2) != 0) {
                                p1X     = p0X;
                                p1Y     = p0Y - 1;
                                found   = true;
                                con[oO] ^= 0x2;
                                dir     = 1;
                            }
                            break;
                        case 3:
                            if((con[oO] & 0x4) != 0) {
                                p1X     = p0X - 1;
                                p1Y     = p0Y;
                                found   = true;
                                con[oO] ^= 0x4;
                                dir     = 2;
                            }
                            break;
                    }
                    
                    if(!found) {
                        switch(dir) {
                            case 0:
                                if((con[oO] & 0x1) != 0) {
                                    p1X     = p0X + 1;
                                    p1Y     = p0Y;
                                    found   = true;
                                    con[oO] ^= 0x1;
                                    dir     = 0;
                                }
                            break;
                            case 1:
                                if((con[oO] & 0x2) != 0) {
                                    p1X     = p0X;
                                    p1Y     = p0Y - 1;
                                    found   = true;
                                    con[oO] ^= 0x2;
                                    dir     = 1;
                                }
                                break;
                            case 2:
                                if((con[oO] & 0x4) != 0) {
                                    p1X     = p0X - 1;
                                    p1Y     = p0Y;
                                    found   = true;
                                    con[oO] ^= 0x4;
                                    dir     = 2;
                                }
                                break;
                            case 3:
                                if((con[oO] & 0x8) != 0) {
                                    p1X     = p0X;
                                    p1Y     = p0Y + 1;
                                    found   = true;
                                    con[oO] ^= 0x8;
                                    dir     = 3;
                                }
                            break;
                        }
                    }
                    
                    if(!found) {
                        switch(dir) {
                            case 0:
                                if((con[oO] & 0x2) != 0) {
                                    p1X     = p0X;
                                    p1Y     = p0Y - 1;
                                    found   = true;
                                    con[oO] ^= 0x2;
                                    dir     = 1;
                                }
                                break;
                            case 1:
                                if((con[oO] & 0x4) != 0) {
                                    p1X     = p0X - 1;
                                    p1Y     = p0Y;
                                    found   = true;
                                    con[oO] ^= 0x4;
                                    dir     = 2;
                                }
                                break;
                            case 2:
                                if((con[oO] & 0x8) != 0) {
                                    p1X     = p0X;
                                    p1Y     = p0Y + 1;
                                    found   = true;
                                    con[oO] ^= 0x8;
                                    dir     = 3;
                                }
                                break;
                            case 3:
                                if((con[oO] & 0x1) != 0) {
                                    p1X     = p0X + 1;
                                    p1Y     = p0Y;
                                    found   = true;
                                    con[oO] ^= 0x1;
                                    dir     = 0;
                                }
                                break;
                        }
                    }
                }
                
                p0X = p1X;
                p0Y = p1Y;
                counter++;
            } while(found);
            
            this.boundaries.addInci();

            dir     = -1;
        
            while(iOfFirstUndeleted < shrinkList.size() && dir == -1) {
                currX   = shrinkList.getX(iOfFirstUndeleted);
                currY   = shrinkList.getY(iOfFirstUndeleted);

                dir     = dropNextToFrame(currX, currY);
                
                iOfFirstUndeleted++;
            }
        }
        
        resampledBoundaries.reset();
        
        for(int line = 0; line < boundaries.getNumberOfLines(); line++) {
            int                 start   = boundaries.lineStart(line);
            int                 end     = boundaries.lineEnd(line);
            double              factor  = (double)(end - start) / (double)resamplingRate;

            if(end - start >= minContourPoints) {
                for(double index = start; index < end; index += factor) {
                    resampledBoundaries.add(boundaries.getX((int)index), boundaries.getY((int)index));
                }

                resampledBoundaries.addInci();
            }
        }
    }

    private void clearConnectivityGridArray(int value) {
        for (int i = 0; i < value; i++) {
            con[i] = 0;
        }
    }

    private void clearOccupiedPixels(int value) {
        for (int i = 0; i < value; i++) {
            occu[i] = 0;
        }
    }
}
