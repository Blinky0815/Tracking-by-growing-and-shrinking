#ifndef ROTATINGCALIPER_H
#define ROTATINGCALIPER_H

#include <vector>

class BoundingBox {
public:
	std::pair<float, float>		a;
	std::pair<float, float>		b;
	std::pair<float, float>		c;
	std::pair<float, float>		d;

	BoundingBox(): a(std::pair<float, float>(0, 0)), b(std::pair<float, float>(0, 0)), c(std::pair<float, float>(0, 0)), d(std::pair<float, float>(0, 0)) {}
	BoundingBox(std::pair<float, float> a, std::pair<float, float> b, std::pair<float, float> c, std::pair<float, float> d): a(a), b(b), c(c), d(d) {}
};

class RotatingCaliper {
private:
	void getNextIndex(const std::vector<std::pair<int, int> >& convexhull, int clIndexA, int clIndexB, int& ctIndex, int& coIndex, int& cbIndex, float& fdist);
	BoundingBox getBoundingBox(const std::vector<std::pair<int, int> >& convexhull, int sclIndexA, int sclIndexB, int sctIndex, int scoIndex, int scbIndex);

public:
	BoundingBox getMinimumBoundingBox(const std::vector<std::pair<int, int> >& convexhull);
};

#endif // ROTATINGCALIPER_H
