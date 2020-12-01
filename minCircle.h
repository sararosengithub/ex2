#include "anomaly_detection_util.h"
#include "math.h"
#include "vector"
#include "random"


class Circle {
public:
    float r;
    Point c;

    Circle() : r(0), c(0, 0) {};

    Circle(float r, Point c) : r(r), c(c) {}

    float getr() {
        return this->r;
    }

    Point getc() {
        return this->c;
    }
};

float dist(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

Circle from_one_point(Point p1) { return Circle(0, p1); }

Circle from_two_points(Point p1, Point p2) {
    Point center = new Point((p1.x + p2.x / 2), (p1.y + p2.y) / 2);
    float radius = dist(p1, p2)/2;
    return Circle(radius, center);
}

Circle from_three_points(Point p1, Point p2, Point p3) {
}

bool point_in_circle(Point p, Circle c) {
    return (dist(p,c.getc())<= c.getr());
}

Circle Welzls(std::vector<Point> p, int sizeofp, std::vector<Point> r, int sizeofr) {
    if (sizeofp == 1 | sizeofr <= 3) {
        switch (sizeofr) {
            case (0):
                return null; //what should i do in this case?
                break;
            case (1):
                return from_one_point(r[0]);
                break;
            case (2):
                return from_two_points(r[0], r[1]);
                break;
            case (3):
                return from_three_points(r[0], r[1], r[2]);
                break;
        }
    }
    std::random_shuffle(p.begin(), p.end());
    Point p_rand = p.front();
    p.erase(0);
    Circle D = Welzls(p, sizeofp - 1, r, sizeofr);
    if (point_in_circle(p_rand, D)) { return D; }
    r.push_back(p_rand);
    return Welzls(p, sizeofp - 1, r, sizeofr + 1);

}