/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) 2014 Olaf Christ 
 * email: christ_o@gmx.de
 * 
 * Tracking-by-growing-and-shrinking is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tracking-by-growing-and-shrinking is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
