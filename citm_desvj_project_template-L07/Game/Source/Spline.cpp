#include "Spline.h"

Spline::Spline() {}

Spline::Spline(double totalTime)
{
	this->totalTime = totalTime;
	this->elapsedTime = 0;
	this->bFinished = true;
}

Spline::~Spline()
{
	points.Clear();
}

iPoint Spline::GetSplinePoint(double t, bool looped)
{
	int p0, p1, p2, p3;

	if (!looped)
	{
		p1 = (int)t + 1;
		p2 = p1 + 1;
		p3 = p2 + 1;
		p0 = p1 - 1;
	}
	else
	{
		p1 = (int)t;
		p2 = (p1 + 1) % points.Count();
		p3 = (p2 + 1) % points.Count();
		p0 = p1 >= 1 ? p1 - 1 : points.Count() - 1;
	}

	t = t - (int)t;

	double tt = t * t;
	double ttt = tt * t;

	double q1 = -ttt + 2.0f * tt - t;
	double q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
	double q3 = -3.0f * ttt + 4.0f * tt + t;
	double q4 = ttt - tt;

	double tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
	double ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

	return { (int)tx, (int)ty };
}

double Spline::TrackTime(double dt)
{
	if (elapsedTime < totalTime - 0.1 )
	{
		elapsedTime += (dt / 1000);
		return (elapsedTime / totalTime) * (points.Count() - 3);
	}
	else
	{
		bFinished = true;
		elapsedTime = 0;
		return 0;
	}
}