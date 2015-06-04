#ifndef FVECTOR_H
#define FVECTOR_H

#define PI 3.1415926535897932384626433832795f
#define DEGREE PI / 180.0f

class FVector3 { //Float 3d-vect, normally used
public:
    FVector3(float x = 0.0, float y = 0.0, float z = 0.0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float length() const;
    FVector3 normalize() const;

    FVector3 cross(const FVector3 &other) const;    
    FVector3 operator+ (const FVector3 &other) const;
    FVector3 operator- (const FVector3 &other) const;
    float operator* (const FVector3 &other) const;
    FVector3 operator* (const float r) const;
    FVector3 operator/ (const float r) const;

    void addBy(const FVector3 &other);
    FVector3 & operator= (const FVector3 &other);
    void set(float x = 0.0, float y = 0.0, float z = 0.0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    float x, y, z;
};

#endif