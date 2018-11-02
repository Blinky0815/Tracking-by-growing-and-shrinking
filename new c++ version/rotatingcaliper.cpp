#include <rotatingcaliper.h>
#include <iostream>
#include <cmath>

void RotatingCaliper::getNextIndex(const std::vector<std::pair<int, int> >& convexhull, int clIndexA, int clIndexB, int& ctIndex, int& coIndex, int& cbIndex, float& fdist) {
    std::pair<int, int> clVectA(convexhull[clIndexA]);
    std::pair<float, float> lVect(convexhull[clIndexB].first - clVectA.first, convexhull[clIndexB].second - clVectA.second);

    float lVectLen = std::sqrt(lVect.first * lVect.first + lVect.second * lVect.second);

    if (lVectLen == 0) {
        lVectLen = 1;
    }

    std::pair<float, float> uVect(lVect.first / lVectLen, lVect.second / lVectLen);
    std::pair<float, float> nVect(-uVect.second, uVect.first);

    // ----------------- top ---------------
    std::pair<float, float> tVect(convexhull[ctIndex].first - clVectA.first, convexhull[ctIndex].second - clVectA.second);

    float tdist = tVect.first * uVect.first + tVect.second * uVect.second;

    ctIndex++;

    if ((unsigned) ctIndex >= convexhull.size()) {
        ctIndex = 0;
    }

    while (ctIndex != clIndexA) {
        //tVect = {convexhull[ctIndex].first - clVectA.first, convexhull[ctIndex].second - clVectA.second};
        tVect.first = (convexhull[ctIndex].first - clVectA.first);
        tVect.second = (convexhull[ctIndex].second - clVectA.second);

        float dist = tVect.first * uVect.first + tVect.second * uVect.second;

        if (dist < tdist) {
            break;
        }

        tdist = dist;
        ctIndex++;

        if ((unsigned) ctIndex >= convexhull.size()) {
            ctIndex = 0;
        }
    }

    ctIndex--;

    if (ctIndex < 0) {
        ctIndex = convexhull.size() - 1;
    }

    // ----------------- transition --------

    if (ctIndex > coIndex || (ctIndex < coIndex && ctIndex < clIndexA)) {
        coIndex = ctIndex;
    }

    // ----------------- opposite ----------
    std::pair<float, float> oVect(convexhull[coIndex].first - clVectA.first, convexhull[coIndex].second - clVectA.second);

    fdist = std::abs(oVect.first * nVect.first + oVect.second * nVect.second);

    coIndex++;

    if ((unsigned) coIndex >= convexhull.size()) {
        coIndex = 0;
    }

    while (coIndex != clIndexA) {
        //oVect = {convexhull[coIndex].first - clVectA.first, convexhull[coIndex].second - clVectA.second};
        oVect.first = (convexhull[coIndex].first - clVectA.first);
        oVect.second = (convexhull[coIndex].second - clVectA.second);

        float dist = std::abs(oVect.first * nVect.first + oVect.second * nVect.second);

        if (dist < fdist) {
            break;
        }

        fdist = dist;
        coIndex++;

        if ((unsigned) coIndex >= convexhull.size()) {
            coIndex = 0;
        }
    }

    coIndex--;

    if (coIndex < 0) {
        coIndex = convexhull.size() - 1;
    }

    // ----------------- transition --------

    if (coIndex > cbIndex || (coIndex < cbIndex && cbIndex < clIndexA)) {
        cbIndex = coIndex;
    }

    // ----------------- bottom ------------

    std::pair<float, float> bVect(convexhull[cbIndex].first - clVectA.first, convexhull[cbIndex].second - clVectA.second);

    float bdist = bVect.first * uVect.first + bVect.second * uVect.second;

    cbIndex++;

    if ((unsigned) cbIndex >= convexhull.size()) {
        cbIndex = 0;
    }

    while (cbIndex != clIndexA) {
        //tVect = {convexhull[cbIndex].first - clVectA.first, convexhull[cbIndex].second - clVectA.second};
        bVect.first = (convexhull[cbIndex].first - clVectA.first);
        bVect.second = (convexhull[cbIndex].second - clVectA.second);

        float dist = bVect.first * uVect.first + bVect.second * uVect.second;

        if (dist > bdist) {
            break;
        }

        bdist = dist;
        cbIndex++;

        if ((unsigned) cbIndex >= convexhull.size()) {
            cbIndex = 0;
        }
    }

    cbIndex--;

    if (cbIndex < 0) {
        cbIndex = convexhull.size() - 1;
    }
}

BoundingBox RotatingCaliper::getBoundingBox(const std::vector<std::pair<int, int> >& convexhull, int sclIndexA, int sclIndexB, int sctIndex, int scoIndex, int scbIndex) {
    std::pair<int, int> vecA(convexhull[sclIndexA]);
    std::pair<int, int> vecB(convexhull[sclIndexB]);
    std::pair<int, int> vecCt(convexhull[sctIndex]);
    std::pair<int, int> vecCo(convexhull[scoIndex]);
    std::pair<int, int> vecCb(convexhull[scbIndex]);

    std::pair<float, float> dir(vecB.first - vecA.first, vecB.second - vecA.second);

    float dirLen = sqrt(dir.first * dir.first + dir.second * dir.second);

    if (dirLen == 0) {
        dirLen = 1;
    }

    dir.first = dir.first / dirLen;
    dir.second = dir.second / dirLen;

    std::pair<float, float> ltVecA(vecCt.first - vecA.first, vecCt.second - vecA.second);
    std::pair<float, float> ltVecB(vecCt.first - vecCo.first, vecCt.second - vecCo.second);
    std::pair<float, float> lbVecA(vecCb.first - vecA.first, vecCb.second - vecA.second);
    std::pair<float, float> lbVecB(vecCb.first - vecCo.first, vecCb.second - vecCo.second);

    float ltLenA = dir.first * ltVecA.first + dir.second * ltVecA.second;
    float ltLenB = dir.first * ltVecB.first + dir.second * ltVecB.second;
    float lbLenA = dir.first * lbVecA.first + dir.second * lbVecA.second;
    float lbLenB = dir.first * lbVecB.first + dir.second * lbVecB.second;

    return BoundingBox(std::pair<float, float>(vecA.first + dir.first * ltLenA, vecA.second + dir.second * ltLenA),
            std::pair<float, float>(vecCo.first + dir.first * ltLenB, vecCo.second + dir.second * ltLenB),
            std::pair<float, float>(vecCo.first + dir.first * lbLenB, vecCo.second + dir.second * lbLenB),
            std::pair<float, float>(vecA.first + dir.first * lbLenA, vecA.second + dir.second * lbLenA));
}

BoundingBox RotatingCaliper::getMinimumBoundingBox(const std::vector<std::pair<int, int> >& convexhull) {
    if (convexhull.size() < 3) {
        std::cerr << "Invalid Convex Hull." << std::endl;
        return BoundingBox();
    }

    int clIndexA = 0;
    int clIndexB = 1;
    int ctIndex = 1;
    int coIndex = 1;
    int cbIndex = 1;
    float fdist;

    // Find first opposite index
    this->getNextIndex(convexhull, clIndexA, clIndexB, ctIndex, coIndex, cbIndex, fdist);

    int sclIndexA = clIndexA;
    int sclIndexB = clIndexB;
    int sctIndex = ctIndex;
    int scoIndex = coIndex;
    int scbIndex = cbIndex;
    float sclDist = fdist;

    clIndexA++;
    clIndexB++;

    if ((unsigned) clIndexB >= convexhull.size()) {
        clIndexB = 0;
    }

    while ((unsigned) clIndexA < convexhull.size()) {
        this->getNextIndex(convexhull, clIndexA, clIndexB, ctIndex, coIndex, cbIndex, fdist);

        if (fdist < sclDist) {
            sclIndexA = clIndexA;
            sclIndexB = clIndexB;
            sctIndex = ctIndex;
            scoIndex = coIndex;
            scbIndex = cbIndex;
            sclDist = fdist;
        }

        clIndexA++;
        clIndexB++;

        if ((unsigned) clIndexB >= convexhull.size()) {
            clIndexB = 0;
        }
    }

    return this->getBoundingBox(convexhull, sclIndexA, sclIndexB, sctIndex, scoIndex, scbIndex);
}