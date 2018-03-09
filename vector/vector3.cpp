#include"vector3.h"
#include<valarray>

#define PI 3.14159265

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
};

Vector3& Vector3::operator+=(const Vector3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
};

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
};

/*
fixit:
неконстантный вариант назовите invert
унарный минус должен быть константным
*/
Vector3& Vector3::operator-() {
    x = -x;
    y = -y;
    z = -z;
    return *this;
}
        
Vector3& Vector3::operator-=(const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
};
        
float Vector3::operator*(const Vector3& other) const {
    /*
    скобки лишние
    */
    return (x * other.x + y * other.y + z * other.z);
};
        
Vector3 Vector3::operator^(const Vector3& other) const {
    return Vector3(y * other.z - z * other.y,
                   z * other.x - x * other.z,
                   x * other.y - y * other.x);
};

Vector3& Vector3::mult(const float& scaler) {
    /*
    1) const float& -> просто float
    2) x *= scalar;
    */
    x = x * scaler; 
    y = y * scaler; 
    z = z * scaler;
    return *this;
}

Vector3& Vector3::div(const float& scaler) {
    /*
    x /= scalar;
    */
    x = x / scaler;
    y = y / scaler;
    z = z / scaler;
    return *this;
}

void Vector3::print() const {
    std::cout << x << " " << y << " " << z << "\n" << std::endl;
}

float Vector3::len() const {
    return sqrt(x * x +
                y * y +
                z * z);
}

float Vector3::squareLen() const {
    /*
    fixit: наоборот надо len через squareLen считать, чтобы не было ошибок в округлении
    */
    return len() * len();
}

Vector3 Vector3::get_rotated(const Vector3& axis, const float& alpha) const {
    
    if(axis.check() || check()) {
        std::cout << "Can't rotate null vector or around the null vector\n";
        return Vector3(0, 0, 0);
    }

    /* preparing */
    float cos_alpha = cos(alpha * PI / 180);
    float sin_alpha = sin(alpha * PI / 180);

    /*
    покомпонентные операции выглядят жестко ... я тут в закромах нашел такой вариант
    
  void Rotate(const Vector3 &axis, const T sinAng, const T cosAng)
  {
    Vector3<T> self = *this;
    Vector3<T> x = self - axis * (axis * self);
    Vector3<T> y = x ^ axis;
    Vector3<T> delta = x * cosAng + y * sinAng - x;
    self += delta;
    *this = self;
  }

  void  Rotate(const Vector3 &axis, const T angle)
  {
    Rotate(axis, sin(angle), cos(angle));
  }
    */
    
    /* computing new elements */
    auto first_el = (cos_alpha + (1 - cos_alpha) * axis.x * axis.x) * x
             + ((1 - cos_alpha) * axis.x * axis.y - sin_alpha * axis.z) * y
             + ((1 - cos_alpha) * axis.x * axis.z + sin_alpha * axis.y) * z;
    
    auto second_el  = ((1 - cos_alpha) * axis.x * axis.y + sin_alpha * axis.z) * x
               + (cos_alpha + (1 - cos_alpha) * axis.y * axis.y) * y
               + ((1 - cos_alpha) * axis.z * axis.y - sin_alpha * axis.x) * z;

    auto third_el = ((1 - cos_alpha) * axis.x * axis.z - sin_alpha * axis.y) * x
             + ((1 - cos_alpha) * axis.z * axis.y + sin_alpha * axis.x) * y
             + (cos_alpha + (1 - cos_alpha) * axis.z * axis.z) * z;

    /* constructor */
    return Vector3(first_el, second_el, third_el);
}


Vector3& Vector3::rotate(const Vector3& axis, const float& alpha) {
    /*
    можно так
    *this = get_rotated(axis, alpha);
    */
    *this = this->get_rotated( axis, alpha);
    return *this;
}
    
Vector3& Vector3::norm() {
    if(check()) {
        std::cout << "Can't normalize null vector\n";
        x = 0;
        y = 0;
        z = 0;
    } else {
        x = x / this->len();
        y = y / this->len();
        z = z / this->len();
    }
    return *this;
}

Vector3 Vector3::get_normal() const {
    if(check()) {
        std::cout << "Can't find normal to null vector\n";
        return Vector3(0, 0, 0);
    }
    if(x == 0 && y == 0) 
        return Vector3(z, 0, 0);
    /*
    чуть странно. я б не так написал. можно на семинаре обсудить.
    */
    return Vector3(-y, x, 0);
}

ostream& operator<<(ostream& stream, const Vector3& vec) {
    stream << vec.x << " " << vec.y << " " << vec.z;
    return stream;
}

istream& operator>>(istream& stream, Vector3& vec) {
    stream >> vec.x >> vec.y >> vec.z;
    return stream;
};
