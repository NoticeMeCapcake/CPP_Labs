#include <iostream>
#include <cstdarg>
// #include <string>
#include <vector>

using namespace std;

enum Func_type { CANON, X, Y };

struct Point {
    double x, y;
    Point(const double _x, const double _y) {
        x = _x;
        y = _y;
    }
};

bool is_polygon_convex(const int count, ...) {
    if (count <= 3) return true;
    va_list args;
    va_start(args, count);
    vector<Point> points;
    for (int i = 0; i < count; i++) {
        points.push_back(va_arg(args, Point));
    }
    points.push_back(points[0]);
    double coef_a = 0, coef_b = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        Func_type type;
        Point point1 = points[i], point2 = points[i + 1];

        if (point1.x == point2.x) type = X;
        else if (point1.y == point2.y) type = Y;
        else {
            type = CANON;
            coef_a = (point2.y - point1.y) / (point2.x - point1.x);
            coef_b = point1.y - coef_a * point1.x;
        }

        int minus_cnt = 0, plus_cnt = 0;
        double result;
        for (int j = 0; j < points.size(); j++) {
            if (j == i or j == i + 1) continue;

            if (type == CANON)
                result = points[j].y - coef_a * points[j].x - coef_b;
            else if (type == X)
                result = point1.x - points[j].x;
            else
                result = point1.y - points[j].y;

            if (result < 0) minus_cnt++;
            else if (result > 0) plus_cnt++;
        }
        if (minus_cnt * plus_cnt) {
            return false;
        }
    }
    va_end(args);
    return true;
}

int main() {
    Point first_p(0, 0);
    Point second_p(5, 0);
    Point third_p(5, 5); // 1 0.8
    Point fourth_p(0, 5);
    if (is_polygon_convex(4, first_p, second_p, third_p, fourth_p))
        cout << "Polygon is convex" << endl;
    else
        cout << "Polygon is not convex" << endl;
    return 0;
}
