#ifndef VECTOR3_tpp
#define VECTOR3_tpp

#include <cmath>

#define DEGREE 3.1415926 / 180

template<typename T>
class Vector3 {
public:
    T length() const;
    Vector3 normalize() const;

    Vector3 cross(const Vector3 &other) const;  // cross product
    Vector3 operator+ (const Vector3 &other) const;
    Vector3 operator- (const Vector3 &other) const;
    T operator* (const Vector3 &other) const;
    Vector3 operator* (const T r) const;
    Vector3 operator/ (const T r) const;

    void addBy(const Vector3 &other);  // add another vector to it
    void set(T x = 0, T y = 0, T z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    T x, y, z;
};

template<typename T>
T Vector3<T>::length() const {
    return (T)(sqrt(x * x + y * y + z * z));
}

template<typename T>
Vector3<T> Vector3<T>::normalize() const {
    T len = length();
    if (fabs(len - 0) < 1e-6) {
        return Vector3<T>();
    } else {
        Vector3<T> res;
        res.set(x / len, y / len, z / len);
        return res;
    }
}

template<typename T>
Vector3<T> Vector3<T>::cross(const Vector3<T> &v) const {
    Vector3<T> res;
    res.set(y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    return res;
}

template<typename T>
Vector3<T> Vector3<T>::operator+ (const Vector3<T> &v) const {
    Vector3<T> res;
    res.set(x + v.x, y + v.y, z + v.z);
    return res;
}

template<typename T>
Vector3<T> Vector3<T>::operator- (const Vector3<T> &v) const {
    Vector3<T> res;
    res.set(x - v.x, y - v.y, z - v.z);
    return res;
}

template<typename T>
T Vector3<T>::operator* (const Vector3<T> &v) const {
    return x * v.x + y * v.y + z * v.z;
}

template<typename T>
Vector3<T> Vector3<T>::operator* (T r) const {
    Vector3<T> res;
    res.set(x * r, y * r, z * r);
    return res;
}

template<typename T>
Vector3<T> Vector3<T>::operator/ (T r) const {
    Vector3<T> res;
    res.set(x / r, y / r, z / r);
    return res;
}

template<typename T>
void Vector3<T>::addBy(const Vector3<T> &other) {
    x += other.x;
    y += other.y;
    z += other.z;
}

typedef Vector3<float> FVector3;
typedef Vector3<int> IVector3;

#endif
