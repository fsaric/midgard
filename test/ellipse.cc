#include "valhalla/midgard/ellipse.h"

#include "test.h"

#include <vector>

#include "valhalla/midgard/point2.h"
#include "valhalla/midgard/vector2.h"
#include "valhalla/midgard/linesegment2.h"
#include "valhalla/midgard/aabb2.h"

using namespace std;
using namespace valhalla::midgard;

namespace {

void TryLineIntersection(const Ellipse& a, const LineSegment2& line, const uint32_t expected) {
  Point2 p1, p2;
  if (a.Intersect(line, p1, p2) != expected) {
    throw runtime_error("Ellipse: LineSegment intersect test failed: expected: " +
                        std::to_string(expected));
  }
}

void TestLineIntersection() {
  Ellipse e({1,1}, {5,-1}, 45.0f);

  // TODO - could check intersect points
  TryLineIntersection(e, LineSegment2({-5,5}, {5,5}), 0);
  TryLineIntersection(e, LineSegment2({-5,0}, {5,0}), 2);
  TryLineIntersection(e, LineSegment2({3,0}, {3,5}), 1);
}

void TryAABBIntersection(const Ellipse& a, const AABB2& box, const IntersectCase expected) {
  if (a.DoesIntersect(box) != expected) {
    throw runtime_error("Ellipse AABB2 does intersect test failed: expected: " +
                        std::to_string(expected));
  }
}

void TestAABBIntersection() {
  Ellipse e({1,1}, {5,-1}, 45.0f);

  // Test for ellipse containing rectangle
  TryAABBIntersection(e, AABB2({2.5f,-0.5f},{3.5f,0.5f}), IntersectCase::kContains);

  // Test for ellipse within rectangle
  TryAABBIntersection(e, AABB2({-2,-2},{6,3}), IntersectCase::kWithin);

  // Test for rectangle outside ellipse
  TryAABBIntersection(e, AABB2({-2,-2},{0,3}), IntersectCase::kOutside);

  // Test for rectangle intersecting ellipse
  TryAABBIntersection(e, AABB2({1,0},{3,1}), IntersectCase::kIntersects);
}


void TryContains(const Ellipse& a, const Point2& pt, const bool expected) {
  if (a.Contains(pt) != expected) {
    throw runtime_error("Ellipse Contains test failed: expected: " +
                        std::to_string(expected));
  }
}

void TestContains() {
  Ellipse e({1,1}, {5,-1}, 45.0f);
  TryContains(e, {3,0}, true);
  TryContains(e, {1,0}, false);
  TryContains(e, {5,0}, false);
  TryContains(e, {3,1}, true);
  TryContains(e, {3,-1}, true);
}

}

int main() {
  test::suite suite("ellipse");

  // Tests if line segments intersect ellipse
  suite.test(TEST_CASE(TestLineIntersection));

  // Tests if line segments intersect ellipse
   suite.test(TEST_CASE(TestAABBIntersection));

  // Test if points contained within ellipse
  suite.test(TEST_CASE(TestContains));

  return suite.tear_down();
}
