#pragma once 

#include<iostream>
#include<cstdlib>

using std::istream;
using std::ostream;
using std::string;

class Vector3 {
    
    private:
    /*
    fixit: вы не предоствили способа обратиться к компонентам вектора снаружи класса.
    нужно либо делать методы вроде 
    float x() const
    {
        return x;
    }
    float& x()
    {
        return x;
    }
    либо просто сделать поля публичными.
    Второй вариант проще.
    Первый, например, удобен, если вы хотите посчитать статистику обращения к компонентам класса.
    Либо если у вас во внутреннем представлении все в метрах, а наружу вы хотите отдавать либо миллиметры, либо
    дюймы и т.п.
    Но это достаточно редкие сценарии.
    */
        float x;
        float y;
        float z;
    
    public:
        /* constructor & destructor */
        /*
        fixit:
        размер ссылки и float'а одинаковый, поэтому const float& - это скорее понты.
        Лучше просто
        Vector3(float x, float y, float z): x(x), y(y), z(z) {};
        */
        Vector3(const float& a, const float& b, const float& c): x(a), y(b), z(c) {};
    
        /*
        fixit:
        Если вы пишете сами хоть какой-то конструктор, то конструктор Vector3() компилятор
        за вас генерировать не будет. Но скорее всего он пригодится, поэтому его надо самим дописать.
        */
        /*
        fixit:
        Компилятор сам сгенерирует за вас ровно такой же конструктор копирования и деструктор.
        Их явно писать не нужно.
        
        Если вам нужно выделать память в куче, то конструктор копирования и деструктор необходимо
        написать самостоятельно.
        */
        Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z) {};
        ~Vector3() {};
        
        /* overloaded operators */
        Vector3 operator+(const Vector3& other) const;
        Vector3& operator+=(const Vector3& other);
        Vector3 operator-(const Vector3& other) const;
    
        /*
        fixit: то что унарный минус неконстантный - это нездорово.
        */
        Vector3& operator-();
        Vector3& operator-=(const Vector3& other);
        float operator*(const Vector3& other) const;
        Vector3 operator^(const Vector3& other) const;
        
        /* functions-members */
        /*
        fixit: вместо mult и div нужно перегрузить соответствующие операторы
        mult -> *=
        но нужны ещё такие, чтобы работало такое: v * 3.0 и 3.0 * v
        */
        Vector3& mult(const float& scaler); // multiplicate vector and scaler;
        Vector3& div(const float& scaler);  // divide vector on scaler;
        Vector3 get_normal() const;         // getting any normal to the vector;
        float len() const;                
        float squareLen() const; 
        Vector3& norm();                    // normalize vector;
        void print() const;                 // another output method;
        Vector3& rotate(const Vector3& axis, const float& alpha); // rotate this vector;
        Vector3 get_rotated(const Vector3& axis, const float& alpha) const; // returns new rotated vector;
    
        /*
        fixit: название несоответствует коду .... который надо в срр перенести
        не очень здорово сравнивать с нулем ... из-за погрешностей вычислений вы можете получить
        1e-10 например, а не ровно ноль. Лучше сравнивать с неким eps: fabs(x) < eps ...
        */
        bool check() const { return(x == 0 && y == 0 && z == 0); }

        /* input and output */
        friend ostream& operator<<(ostream& stream, const Vector3& vec);
        friend istream& operator>>(istream& stream, Vector3& vec);
};


