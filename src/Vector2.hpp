#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>

#define DEGREE 3.1415926 / 180

template<typename T>
class Vector2 {
public:
    T length() const;
    Vector2 normalize() const;

    Vector2 operator+ (const Vector2 &other) const;
    Vector2 operator- (const Vector2 &other) const;
    T operator* (const Vector2 &other) const;
    Vector2 operator* (const T r) const;
    Vector2 operator/ (const T r) const;

    void addBy(const Vector2 &other);  // add another vector to it
    void set(T x = 0, T y = 0) {
        this->x = x;
        this->y = y;
    }

    T x, y;
};

template<typename T>
T Vector2<T>::length() const {
    return (T)(sqrt(x * x + y * y));
}

template<typename T>
Vector2<T> Vector2<T>::normalize() const {
    T len = length();
    if (fabs(len - 0) < 1e-6) {
        return Vector2<T>();
    } else {
        Vector2<T> res;
        res.set(x / len, y / len);
        return res;
    }
}

template<typename T>
Vector2<T> Vector2<T>::operator+ (const Vector2<T> &v) const {
    Vector2<T> res;
    res.set(x + v.x, y + v.y);
    return res;
}

template<typename T>
Vector2<T> Vector2<T>::operator- (const Vector2<T> &v) const {
    Vector2<T> res;
    res.set(x - v.x, y - v.y);
    return res;
}

template<typename T>
T Vector2<T>::operator* (const Vector2<T> &v) const {
    return x * v.x + y * v.y;
}

template<typename T>
Vector2<T> Vector2<T>::operator* (T r) const {
    Vector2<T> res;
    res.set(x * r, y * r);
    return res;
}

template<typename T>
Vector2<T> Vector2<T>::operator/ (T r) const {
    Vector2<T> res;
    res.set(x / r, y / r);
    return res;
}

template<typename T>
void Vector2<T>::addBy(const Vector2<T> &other) {
    x += other.x;
    y += other.y;
}

typedef Vector2<float> FVector2;
typedef Vector2<int> IVector2;

#endif
