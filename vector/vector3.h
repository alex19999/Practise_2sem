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
        /* constructor & destructor */
        Vector3(const float& a, const float& b, const float& c): x(a), y(b), z(c) {};
        Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z) {};
        ~Vector3() {};
        
        /* overloaded operators */
        Vector3 operator+(const Vector3& other) const;
        Vector3& operator+=(const Vector3& other);
        Vector3 operator-(const Vector3& other) const;
        Vector3& operator-();
        Vector3& operator-=(const Vector3& other);
        float operator*(const Vector3& other) const;
        Vector3 operator^(const Vector3& other) const;
        
        /* functions-members */
        Vector3& mult(const float& scaler); // multiplicate vector and scaler;
        Vector3& div(const float& scaler);  // divide vector on scaler;
        Vector3 get_normal() const;         // getting any normal to the vector;
        float len() const;                
        float squareLen() const; 
        Vector3& norm();                    // normalize vector;
        void print() const;                 // another output method;
        Vector3& rotate(const Vector3& axis, const float& alpha); // rotate this vector;
        Vector3 get_rotated(const Vector3& axis, const float& alpha) const; // returns new rotated vector;
        bool check() const { return(x == 0 && y == 0 && z == 0); }

        /* input and output */
        friend ostream& operator<<(ostream& stream, const Vector3& vec);
        friend istream& operator>>(istream& stream, Vector3& vec);


};


