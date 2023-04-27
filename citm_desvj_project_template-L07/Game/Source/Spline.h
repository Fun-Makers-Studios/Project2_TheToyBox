#pragma once

#include "Point.h"
#include "List.h"

class Spline
{
public:
	Spline();
	Spline(double totalTime);
	~Spline();

	iPoint GetSplinePoint(double t, bool looped = false);
	double TrackTime(double dt);

	double GetElapsedTime() const { return elapsedTime; }
	void SetElapsedTime(double elapsedTime) { this->elapsedTime = elapsedTime; }

	List<iPoint> points;
	int selectedPoint = 0;

private:

	double elapsedTime = 0;
	double totalTime = 0;
	bool bFinished = true;
};