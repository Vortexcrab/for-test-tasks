#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>
#include <tuple>
#include <iomanip>


using namespace std;

// constants
const double PI = 3.14159265358979323846;

// абстрактный класс для кривых
class Curve {
public:
    virtual tuple<double, double, double> pointAt(double t) const = 0;
    virtual tuple<double, double, double> derivativeAt(double t) const = 0;
    virtual string getType() const = 0; // тип кривой
    virtual ~Curve() = default;
};

// окружность
class Circle : public Curve {
public:
    explicit Circle(double r) : radius(r) {}

    tuple<double, double, double> pointAt(double t) const override {
        return { radius * cos(t), radius * sin(t), 0.0 };
    }

    tuple<double, double, double> derivativeAt(double t) const override {
        return { -radius * sin(t), radius * cos(t), 0.0 };
    }

    string getType() const override {
        return "Circle";
    }

    double getRadius() const {
        return radius;
    }

private:
    double radius;
};

// эллипс
class Ellipse : public Curve {
public:
    Ellipse(double rx, double ry) : radiusX(rx), radiusY(ry) {}

    tuple<double, double, double> pointAt(double t) const override {
        return { radiusX * cos(t), radiusY * sin(t), 0.0 };
    }

    tuple<double, double, double> derivativeAt(double t) const override {
        return { -radiusX * sin(t), radiusY * cos(t), 0.0 };
    }

    string getType() const override {
        return "Ellipse";
    }

private:
    double radiusX, radiusY;
};

// спираль
class Spiral : public Curve {
public:
    Spiral(double r, double step) : radius(r), step(step) {}

    tuple<double, double, double> pointAt(double t) const override {
        return { radius * cos(t), radius * sin(t), step * t };
    }

    tuple<double, double, double> derivativeAt(double t) const override {
        return { -radius * sin(t), radius * cos(t), step };
    }

    string getType() const override {
        return "Spiral";
    }

private:
    double radius, step;
};

// rng
double random_double(double min, double max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

int main() {
    // вектор для хранения объектов кривых
    vector<Curve*> curves;

    // заполнение вектора случайными кривыми
    for (int i = 0; i < 10; ++i) {
        int type = rand() % 3;  // случайно выбираем тип кривой(0 - окружность, 1 - эллипс, 2 - спираль)

        if (type == 0) {  // окружность
            double radius = random_double(1.0, 10.0);
            curves.push_back(new Circle(radius));
        }
        else if (type == 1) {  // эллипс
            double rx = random_double(1.0, 10.0);
            double ry = random_double(1.0, 10.0);
            curves.push_back(new Ellipse(rx, ry));
        }
        else if (type == 2) {  // спираль
            double radius = random_double(1.0, 10.0);
            double step = random_double(0.1, 1.0);
            curves.push_back(new Spiral(radius, step));
        }
    }

    // вывод координат точек и производных всех кривых при t = PI/4
    cout << fixed << setprecision(3);
    cout << "Curve Details at t = PI/4:\n\n";
    double t = PI / 4.0;

    for (const auto& curve : curves) {
        auto point = curve->pointAt(t);
        auto derivative = curve->derivativeAt(t);

        cout << "Type: " << curve->getType() << "\n";
        cout << "Point: (" << get<0>(point) << ", " << get<1>(point) << ", " << get<2>(point) << ")\n";
        cout << "Derivative: (" << get<0>(derivative) << ", " << get<1>(derivative) << ", " << get<2>(derivative) << ")\n";
        cout << "---------------------------------\n";
    }

    // второй контейнер для окружностей
    vector<Circle*> circles;

    // копируем указатели на окружности из первого контейнера
    for (const auto& curve : curves) {
        if (auto* circle = dynamic_cast<Circle*>(curve)) {
            circles.push_back(circle);
        }
    }

    // вывод радиусов окружностей до сортировки
    cout << "\nCircles before sorting:\n";
    for (const auto& circle : circles) {
        cout << "Circle with radius: " << circle->getRadius() << "\n";
    }

    // сортировка второго контейнера
    sort(circles.begin(), circles.end(), [](Circle* a, Circle* b) {
        return a->getRadius() < b->getRadius();
        });

    // вывод радиусов окружностей после сортировки
    cout << "\nCircles after sorting (by radius):\n";
    for (const auto& circle : circles) {
        cout << "Circle with radius: " << circle->getRadius() << "\n";
    }

    // вычисление общей суммы радиусов окружностей
    double totalRadius = 0.0;
    for (const auto& circle : circles) {
        totalRadius += circle->getRadius();
    }

    cout << "\nTotal sum of radii: " << totalRadius << "\n";

    for (auto* curve : curves) {
        delete curve;
    }

    return 0;
}
