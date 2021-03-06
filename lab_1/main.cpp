#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <cmath>
#include <iterator>

using namespace std;
double eps = 1e-8;

double pow2(double x){
    return x * x;
}

template<typename T>
bool is_unique(T arr){
    auto it = arr.begin();
    for (;it != arr.end(); it ++){
        auto it1 = it + 1;
        for (;it1 != arr.end(); it1 ++){
            if (*it == *it1) return false;
        }
    }
    return true;
}

int sign(double x){
    if (x < -eps) return -1;
    if (x > eps) return 1;
    return 0;
}


class Point {
private:
    double x, y;

public:
    explicit Point(double x_ = 0, double y_ = 0): x(x_), y(y_){}

    Point(const Point& other) = default;

    Point& operator =(const Point& other)= default;

    Point(const initializer_list<double>& list){
        if (list.size() == 2){
            x = *list.begin();
            y = *(list.begin() + 1);
        }
    }

    bool operator ==(const Point& other) const{
        return x == other.x && y == other.y;
    }

    [[nodiscard]] double get_x() const{
        return x;
    }

    [[nodiscard]] double get_y() const{
        return y;
    }


    friend ostream& operator <<(ostream& s, const Point& a);
    friend istream& operator >>(istream& s, Point& a);

    Point operator -(const Point& other) const{
        return Point(x - other.x, y - other.y);
    }

    Point operator -() const{
        return Point(-x, -y);
    }

    Point operator +() const{
        return *this;
    }

    Point operator +(const Point& other) const{
        return *this - (-other);
    }

    friend double product(Point a, Point b);
};

double product(Point a, Point b){
    return a.x * b.y - a.y * b.x;
}

ostream& operator <<(ostream& s, const Point& a){
    s << a.x << " " << a.y;
    return s;
}

istream &operator>>(istream &s, Point &a) {
    s >> a.x;
    s >> a.y;
    return s;
}

double distance(const Point& a, const Point& b){
    return sqrt((a.get_x() - b.get_x())* (a.get_x() - b.get_x()) + (a.get_y() - b.get_y()) * (a.get_y() - b.get_y()));
}

class Line{
private:
    double a, b, c;
public:
    Line(const Point& p1, const Point& p2){
        a = p2.get_y() - p1.get_y();
        b = p1.get_x() - p2.get_x();
        c = p1.get_y() * p2.get_x() - p1.get_x() * p2.get_y();
    }

    Line(double a_, double b_, double c_): a(a_), b(b_), c(c_){}

    [[nodiscard]] int side(const Point& p) const{
        double res = a * p.get_x() + b * p.get_y() + c;
        if (res > eps) return 1;
        if (res < -eps) return -1;
        return 0;
    }

    [[nodiscard]] double get_a() const{
        return a;
    }

    [[nodiscard]] double get_b() const{
        return b;
    }

    [[nodiscard]] double get_c() const{
        return c;
    }

    bool operator ==(Line other){
        if (a == 0){
            return other.a == 0;
        }

        if (other.a == 0){
            return a == 0;
        }

        b /= a;
        c /= a;
        a = 1;

        other.b /= other.a;
        other.c /= other.a;
        other.a = 1;

        return abs(b - other.b) < eps && abs(c - other.c) < eps;
    }
};

bool is_parallel(const Line& l1, const Line& l2){
    return l1.get_a() * l2.get_b() - l1.get_b() * l2.get_a() == 0;
}

class PolygonalChain{
protected:

    bool is_right_figure() {
        return true;
    }

    deque<Point> points;

public:
    explicit PolygonalChain(const deque<Point>& list):points(list){
        if (!is_right_figure()) points.clear();
    }

    PolygonalChain(const PolygonalChain& other) = default;
    PolygonalChain() = default;
    explicit PolygonalChain(const Point& p){
        points.clear();
        points.push_back(p);
    }

    PolygonalChain& operator =(const PolygonalChain& line)= default;

    PolygonalChain& operator =(const Point& p){
        points.clear();
        points.push_back(p);
        return *this;
    }

    PolygonalChain(const initializer_list<Point>& list){
        points = list;
        if (!is_right_figure()) points.clear();
    }

    // nodiscard == it have to be not ignored
    [[nodiscard]] const deque<Point>& get_points() const{
        return points;
    }

    [[nodiscard]] size_t size() const{
        return points.size();
    }

    Point& operator [](size_t i){
        return points[i];
    }

    void push_back(const Point& p){
        points.push_back(p);
    }

    void push_front(const Point& p){
        points.push_front(p);
    }

    const Point& back(){
        return points.back();
    }

    const Point& front(){
        return points.front();
    }

    void pop_back(){
        points.pop_back();
    }

    void pop_front(){
        points.pop_front();
    }

    [[nodiscard]] bool empty() const{
        return points.empty();
    }

    [[nodiscard]] virtual double get_perimeter() const{
        if (empty()) return 0;

        double res = 0;
        for (int i = 0; i < size() - 1; i ++){
            res += distance(points[i], points[i + 1]);
        }
        return res;
    }

    void clear(){
        points.clear();
    }

};

class ClosedPolygonalChain: public PolygonalChain{
public:
    explicit ClosedPolygonalChain(const Point& point):PolygonalChain(point){}

    ClosedPolygonalChain() = default;

    ClosedPolygonalChain(const ClosedPolygonalChain& other) = default;

    ClosedPolygonalChain(const initializer_list<Point>& list): PolygonalChain(list){}

    explicit ClosedPolygonalChain(const deque<Point>& list): PolygonalChain(list){}

    [[nodiscard]] double get_perimeter() const override{
        if (empty()) return 0;
        return PolygonalChain::get_perimeter() + distance(points.front(), points.back());
    }

    friend class Polygon;
    friend class RegularPolygon;
    friend class Triangle;
    friend class Trapezoid;
};

class Polygon{
protected:
    ClosedPolygonalChain chain;

    int angle_side = -2;

    bool is_right_figure(){
        for (int i = 0; i < size(); i ++){
            Point b = chain.points[i], a = chain.points[(i + 1) % size()], c = chain.points[(i + 2) % size()];
            Point ab = a - b, bc = c - b;
            int k = sign(product(ab, bc));

            if (i == 0) angle_side = k;
            else {
                if (angle_side != k) return false;
            }
        }
        return true;
    }
public:

    Polygon() = default;

    Polygon(const Polygon& other) = default;

    Polygon(const initializer_list<Point>& list):chain(list){
        if (!is_right_figure()) chain.clear();
    }

    explicit Polygon(const Point& point): chain(point) {
        if (!is_right_figure()) chain.clear();
    }

    explicit Polygon(const deque<Point>& list): chain(list){
        if (!is_right_figure()) chain.clear();
    }

    int size(){
        return chain.size();
    }

    virtual double get_square(){
        double res = 0;
        for (int i = 0; i < size(); i ++){
            res += chain.points[i].get_x() * chain.points[(i + 1) % size()].get_y();
        }

        for (int i = 0; i < size(); i ++){
            res -= chain.points[i].get_y() * chain.points[(i + 1) % size()].get_x();
        }

        res = abs(res) / 2;

        return res;
    }

    vector<double> get_sides(){
        vector<double> res;
        res.reserve(chain.points.size());
        for (int i = 0; i < size(); i ++){
            res.push_back(distance(chain.points[i], chain.points[(i + 1) % chain.points.size()]));
        }

        return res;
    }

    vector<Line> get_sides_lines(){
        vector<Line> res;
        res.reserve(chain.points.size());
        for (int i = 0; i < chain.points.size(); i ++)
            res.emplace_back(chain.points[i], chain.points[(i + 1) % chain.points.size()]);
        return res;
    }

    Point& operator [](int i){
        return chain.points[i];
    }

    [[nodiscard]] const deque<Point>& get_points() const{
        return chain.get_points();
    }

    [[nodiscard]] double get_perimeter() const{
        return chain.get_perimeter();
    }

    void clear(){
        chain.clear();
    }
};

class Triangle: public Polygon{
protected:
    bool is_right_figure(){
        if (!Polygon::is_right_figure()) return false;
        if (size() != 3) return false;
        return true;
    }

public:
    Triangle(){
        for (int i = 0; i < 3; i ++) chain.push_back(Point());
    }

    Triangle(const Triangle& other) = default;

    Triangle(const initializer_list<Point>& list):Polygon(list){
        if (!is_right_figure()) chain.clear();
    }

    explicit Triangle(const deque<Point>& list):Polygon(list){
        if (!is_right_figure()) chain.clear();
    }

    double get_angle(int i){
        vector<double> sides = get_sides();

        double across_side = sides[(i + 1) % size()];
        double near_side1 = sides[i], near_side2 = sides[(i + 2) % size()];

        // across_side^2 = near_side1^2 + near_side2^2 - 2 * near_side1 * near_side2 * angle_cos
        double angle_cos = (pow2(near_side1) + pow2(near_side2) - pow2(across_side))/(2 * near_side1 * near_side2);
        return acos(angle_cos);
    }

    vector<double> get_angles(){
        vector<double> res = {get_angle(0), get_angle(1), get_angle(2)};
        return res;
    }

    bool has_right_angle(){
        for (auto ang: get_angles())
            if (abs(ang - M_PI / 2) < eps) return true;
        return false;
    }
};

class Trapezoid: public Polygon{
private:
    bool is_right_figure(){
        if (!Polygon::is_right_figure()) return false;
        auto sides = get_sides_lines();
        Line l1 = sides[1], l2 = sides[3];
        return is_parallel(l1, l2);
    }
public:
    Trapezoid(){
        chain.points.clear();
        for (int i = 0; i < 4; i ++) chain.push_back(Point());
    }

    Trapezoid(const Trapezoid& other) = default;

    Trapezoid(const initializer_list<Point>& list):Polygon(list){
        if (!is_right_figure()) chain.clear();
    }

    explicit Trapezoid(const deque<Point>& list):Polygon(list){
        if (!is_right_figure()) chain.clear();
    }


    double get_height(){
        if (!is_right_figure()) return 0;

        auto sides = get_sides_lines();
        Line l1 = sides[1], l2 = sides[3];

        double nw_c1 = 0, nw_c2 = 0;

        if (l1.get_a() == 0){
            nw_c1 = l1.get_c() / l1.get_b();
            nw_c2 = l2.get_c() / l2.get_b();
        }
        else{
            nw_c1 = l1.get_c() / l1.get_a();
            nw_c2 = l2.get_c() / l2.get_a();
        }

        return abs(nw_c2 - nw_c1);
    }
};

class RegularPolygon: public Polygon{
protected:
    bool is_right_figure(){
        if (!Polygon::is_right_figure()) return false;
        if (size() <= 2) return true;
        auto sides = get_sides();
        for (int i = 1; i < sides.size(); i ++){
            if (sides[i] != sides[0]) return false;
        }
        return true;
    }
public:
    RegularPolygon() = default;

    RegularPolygon(const RegularPolygon& other) = default;

    RegularPolygon(const initializer_list<Point>& list):Polygon(list){
        if (!is_right_figure()) chain.clear();
    }

    explicit RegularPolygon(const Point& point): Polygon(point) {
        if (!is_right_figure()) chain.clear();
    }

    explicit RegularPolygon(const deque<Point>& list):Polygon(list){
        if (!is_right_figure()) chain.clear();
    }
};

class TestingSystem{
public:
    static void PolygonalChainTest(){
        int n;
        cout << "Write points number: " << endl;
        cin >> n;

        PolygonalChain chain;
        if (n != 0){
            cout << "Write points coordinates" << endl;
            for (int i = 0; i < n; i ++){
                Point a;
                cin >> a;
                chain.push_back(a);
            }
        }

        cout << "Perimeter value is " << chain.get_perimeter();
    }

    static void ClosedPolygonalChainTest(){
        int n;
        cout << "Write points number: " << endl;
        cin >> n;

        ClosedPolygonalChain chain;
        if (n != 0){
            cout << "Write points coordinates" << endl;
            for (int i = 0; i < n; i ++){
                Point a;
                cin >> a;
                chain.push_back(a);
            }
        }

        cout << "Perimeter value is " << chain.get_perimeter();
    }

    static void PolygonTest(){
        Polygon figure = {Point(0, 0), Point(2, 0), Point(2, 2), Point(0, 2)};

        cout << "Perimeter value is " << figure.get_perimeter() << endl;
        cout << "Square value is " << figure.get_square();
    }

    static void TriangleTest(){
        Triangle tr = {{0, 0}, {0, 1}, {1, 1}};

        auto angles = tr.get_angles();
        for (auto angle: angles) cout << angle * 180 / M_PI << " " << endl;

        if (tr.has_right_angle()) cout << "This triangle has right angle" << endl;

        cout << "Square is " << tr.get_square() << endl;
    }

    static void TrapezoidTest(){
        Trapezoid trap = {{0, 0}, {-6, 6}, {6, 6}, {2, 0}};
        cout << "Height is " << trap.get_height() << endl;
    }

    static void RegularPolygonTest(){
        RegularPolygon fig;
        fig = {{0, 0}, {1, 0}, {1, 1}, {2, 1}};
        cout << "Points: " << endl;
        for (auto& p: fig.get_points()) cout << p << endl;
        cout << "Perimeter is " << fig.get_perimeter() << endl;
        cout << "Square is " << fig.get_square() << endl;
    }

    static void PolymorphismTest(){
        vector<Polygon> figures = {Triangle({{0, 0}, {0, 1}, {1, 1}}), Trapezoid({{0, 0}, {0, 1}, {1, 1}, {1, 0}}), RegularPolygon({{0, 0}})};
        double sum_square = 0;
        for (auto& figure : figures) sum_square += figure.get_square();
        cout << sum_square << endl;
    }
};


int main() {
    TestingSystem::PolymorphismTest();


}
