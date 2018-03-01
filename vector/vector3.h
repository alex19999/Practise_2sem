#pragma once 

#include<iostream>
#include<cstdlib>

using std::istream;
using std::ostream;
using std::string;

class Vector3 {
    
    private:
        float x;
        float y;
        float z;
    
    public:
        Vector3(const float& a, const float& b, const float& c): x(a), y(b), z(c) {};
        Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z) {};
        ~Vector3() {};

        /*friend ostream& operator<<(ostream& stream, const Vector3& vec) {
            stream << vec.x << " " << vec.y << " " << vec.z;
            return stream;
        };

        friend istream& operator>>(istream& stream, const Vector3& vec) {
            stream >> vec.x >> vec.y >> vec.z;
            return stream;
        };*/

        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        };

        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        };

        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        };
        
        Vector3& operator-=(const Vector3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        };
        
        float operator*(const Vector3& other) const {
            return (x * other.x + y * other.y + z * other.z);
        };
        
        Vector3 operator^(const Vector3& other) const {
            return Vector3(y * other.z - z * other.y,
                           z * other.x - x * other.z,
                           x * other.y - y * other.x);
        };

        Vector3& mult(const float& scaler);
        Vector3& div(const float& scaler);
        Vector3 get_normal() const;
        float len() const;
        float squareLen() const;
        Vector3& norm();
        void print();
        //унарный минус Vector2 operator-() const;
        Vector3& rotate(const Vector3& axis, const float& alpha);
        Vector3 getRotated(const Vector3& axis, const float& alpha);

};
