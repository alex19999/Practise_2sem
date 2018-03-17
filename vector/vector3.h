#pragma once 

#include<iostream>
#include<cstdlib>
#include<limits>

#define ACCURANCY 2
/*
    accuracy
    вы не используете эту константу. лучше удалить совсем.
*/

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
        /*
        fixit: А каким образом можно задать значение компонент кроме как пересоздать вектор заново?
        */
        
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
        /*
        переопределенный operator << лучше тем, что работает и для записи в файл,
        а print только на экран пишет
        */
        void print() const;                 // another output method;
        Vector3& rotate(const Vector3& axis, const float& alpha); // rotate this vector;
        Vector3 get_rotated(const Vector3& axis, const float& alpha) const; // returns new rotated vector;
    
        /*
        1) код ниже рассчитан на то, что компоненты вектора от 0 до 1 мо модулю.
        здесь http://ru.cppreference.com/w/cpp/types/numeric_limits/epsilon общий случай
        2) реализацию надо в cpp перенести
        */
        bool almost_equal_to_zero() const { 
            float epsilon = std::numeric_limits<float>::epsilon();
            if(x < epsilon && x > -epsilon)
                if(y < epsilon && y > -epsilon)
                    if(z < epsilon && z > -epsilon)
                        return true;
            return false;
            }

        /* input and output */
        friend ostream& operator<<(ostream& stream, const Vector3& vec);
        friend istream& operator>>(istream& stream, Vector3& vec);
};


