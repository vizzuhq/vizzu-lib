#include "bubblechart_v2.h"

#include "base/io/log.h"

using namespace Vizzu;
using namespace Geom;
using namespace Vizzu::Charts;

BubbleChartV2::BubbleChartV2(const std::vector<double> &sizes,
    Boundary,
    const Geom::Rect &rect)
{
	for (auto j = 0u; j < sizes.size(); j++)
		radiuses.push_back({j, sizes[j]});

	std::sort(radiuses.begin(),
	    radiuses.end(),
	    [](const RadiusRecord &a, const RadiusRecord &b)
	    {
		    return b.value < a.value;
	    });

	generate();
	normalize(rect);

	std::sort(data.begin(),
	    data.end(),
	    [](const DataRecord &a, const DataRecord &b)
	    {
		    return a.index < b.index;
	    });
}

void BubbleChartV2::generate()
{
	int numberOfBubbles = radiuses.size();
	data.reserve(numberOfBubbles);

	double relativeBubbleFactor = 1;
	if (numberOfBubbles > 0) relativeBubbleFactor = radiuses[0].value;

	int touchingBubbleIndex = 0;
	for (int bubbleIndex = 0; bubbleIndex < (int)radiuses.size();
	     bubbleIndex++) {
		try {
			// bubble sized normalized so their area is relative to
			// the weight given
			double relativeBubbleRadius = std::sqrt(
			    radiuses[bubbleIndex].value / relativeBubbleFactor);
			// Bubble 0 goes in the middle
			Point relativeBubbleCenter = Point{0, 0};
			if (bubbleIndex == 1) {
				// Bubble 1 goes to the right from Bubble 0
				relativeBubbleCenter =
				    Point{1 + relativeBubbleRadius, 0};
			}
			else if (bubbleIndex > 1) {
				// Bubble n should touch Bubble n-1 and the bubble
				// touched by Bubble n-1
				relativeBubbleCenter =
				    touchingBubbleCenter(data[bubbleIndex - 1].circle,
				        data[touchingBubbleIndex].circle,
				        relativeBubbleRadius);

				int nextTouchingBubbleIndex = touchingBubbleIndex;
				// check if there really is room for it - doesn't it
				// clash with the bubbles consecutive to the touched
				// one?
				for (int touchCandidateIndex =
				         touchingBubbleIndex + 1;
				     touchCandidateIndex
				     < std::min(touchingBubbleIndex + 4,
				         bubbleIndex - 3);
				     touchCandidateIndex++) {
					double d_C_Tc = distanceBetweenCircles(
					    data[touchCandidateIndex].circle,
					    Circle{relativeBubbleCenter,
					        relativeBubbleRadius});

					double d_Bm1_Tc = distanceBetweenCircles(
					    data[bubbleIndex - 1].circle,
					    data[touchCandidateIndex].circle);

					double side = calculateSide(
					    data[bubbleIndex - 1].circle.center,
					    data[touchCandidateIndex].circle.center,
					    relativeBubbleCenter);

					// not only looking for intersection but also to
					// check if possible upcoming bubbles will have
					// their room
					if (d_C_Tc < 0
					    || (d_Bm1_Tc < 2 * relativeBubbleRadius
					        && side > 0)) {
						relativeBubbleCenter = touchingBubbleCenter(
						    data[bubbleIndex - 1].circle,
						    data[touchCandidateIndex].circle,
						    relativeBubbleRadius);
						nextTouchingBubbleIndex = touchCandidateIndex;
					}
				}
				touchingBubbleIndex = nextTouchingBubbleIndex;
			}
			Circle bubble =
			    Circle{relativeBubbleCenter, relativeBubbleRadius};
			data.push_back({radiuses[bubbleIndex].index, bubble});
		}
		catch (std::exception &e) {
			IO::log() << "Bubblechart: " << e.what();
		}
	}
}

Point BubbleChartV2::touchingBubbleCenter(Circle circle1,
    Circle circle2,
    double radius3)
{
	circle1.radius += radius3;
	circle2.radius += radius3;
	return circle1.intersection(circle2);
}

double BubbleChartV2::distanceBetweenCircles(const Circle &circle1,
    const Circle &circle2)
{
	return circle1.signedDistance(circle2);
}

double BubbleChartV2::calculateSide(const Point &point1,
    const Point &point2,
    const Point &point3)
{
	// actually cross product of vectors P2->P1 and P3->P1
	// the sign of returned value tells which side of P2->P1 P3 is on
	Point d12 = point1 - point2;
	Point d13 = point1 - point3;
	return d12 ^ d13;
}
