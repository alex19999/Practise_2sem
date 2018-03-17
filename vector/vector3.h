#pragma once 

#include<iostream>
#include<cstdlib>
#include<limits>

using std::istream;
using std::ostream;
using std::string;

class Vector3 {
    
    private:
        
        float x;
        float y;
        float z;
    
    public:
        Vector3(float x, float y, float z): x(x), y(y), z(z) {};
        Vector3();
    
        float get_x() const { return x; }
        float get_y() const { return y; }
        float get_z() const { return z; }

		void set_x(float value) { x = value; }
		void set_y(float value) { y = value; }
		void set_z(float value) { z = value; }
        
        /* overloaded operators */
        Vector3 operator+(const Vector3& other) const;
        Vector3& operator+=(const Vector3& other);
        Vector3 operator-(const Vector3& other) const;
    
        Vector3 operator-() const;
        Vector3& operator-=(const Vector3& other);
        float operator*(const Vector3& other) const;
        Vector3 operator^(const Vector3& other) const;
        
        Vector3& operator*=(float scalar); // multiplicate vector and scaler;
        Vector3& operator/=(float scalar);  // divide vector on scaler;
        Vector3 get_normal() const;         // getting any normal to the vector;
        float len() const;                
        float squareLen() const; 
        Vector3& norm();                    // normalize vector;
        void print() const;                 // another output method;
        Vector3& rotate(const Vector3& axis, const float& alpha); // rotate this vector;
        Vector3 get_rotated(const Vector3& axis, const float& alpha) const; // returns new rotated vector;
    
        bool almost_equal_to_zero() const;
        friend Vector3 operator*=(float scalar, Vector3 vec);

        /* input and output */
        friend ostream& operator<<(ostream& stream, const Vector3& vec);
        friend istream& operator>>(istream& stream, Vector3& vec);
};


