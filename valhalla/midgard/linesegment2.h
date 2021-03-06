#ifndef VALHALLA_MIDGARD_LINESEGMENT2_H_
#define VALHALLA_MIDGARD_LINESEGMENT2_H_

#include <math.h>
#include <vector>

#include <valhalla/midgard/point2.h>
#include <valhalla/midgard/vector2.h>

namespace valhalla{
namespace midgard{

/**
 * Line segment in 2D
 * @author  David W. Nesbitt
 */
class LineSegment2 {
 public:
  /**
   * Default constructor.
   */
  LineSegment2();

  /**
   * Constructor given 2 points.
   * @param   p1    First point of the segment.
   * @param   p2    Second point of the segment.
   */
  LineSegment2(const Point2& p1, const Point2& p2);

  /**
   * Get the first point of the segment.
   * @return  Returns first point of the segment.
   */
  Point2 a() const;

  /**
   * Get the second point of the segment.
   * @return  Returns second point of the segment.
   */
  Point2 b() const;

/**
   * Finds the distance squared of a specified point from the line segment
   * and the closest point on the segment to the specified point.
   * @param   p        Test point.
   * @param   closest  (Return) Closest point on the segment to c.
   * @return  Returns the distance squared from pt to the closest point on
   *          the segment.
  */
  float DistanceSquared(const Point2& p, Point2& closest) const;

  /**
   * Finds the distance of a specified point from the line segment
   * and the closest point on the segment to the specified point.
   * @param   p        Test point.
   * @param   closest  (Return) Closest point on the segment to c.
   * @return  Returns the distance from p to the closest point on
   *          the segment.
  */
  float Distance(const Point2& p, Point2& closest) const;

  /**
   * Determines if the current segment intersects the specified segment.
   * If an intersect occurs the intersectPt is determined.  Note: the
   * case where the lines overlap is not considered.
   * @param   segment      Segment to determine intersection with.
   * @param   intersect    (OUT) Intersection point.
   * @return  Returns true if an intersection exists, false if not.
   */
  bool Intersect(const LineSegment2& segment, Point2& intersect) const;

  /**
   * Tests if a point is to left, right, or on the line segment
   * @param    p   Point to test
   * @return   Returns >0 for point to the left, < 0 for point to the right,
   *           and 0 for a point on the line
   */
  float IsLeft(const Point2& p) const;

 private:
  Point2 a_;
  Point2 b_;
};


}
}

#endif  // VALHALLA_MIDGARD_LINESEGMENT2_H_
