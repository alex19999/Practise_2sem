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

Vector3 Vector3::operator-() const{
    return Vector3(x * -1, y * -1, z * -1);
}
        
Vector3& Vector3::operator-=(const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
};
        
float Vector3::operator*(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
};
        
Vector3 Vector3::operator^(const Vector3& other) const {
    return Vector3(y * other.z - z * other.y,
                   z * other.x - x * other.z,
                   x * other.y - y * other.x);
};

Vector3& Vector3::operator*=(float scalar) {
    x *= scalar; 
    y *= scalar; 
    z *= scalar;
    return *this;
}

Vector3 operator*=(float scalar,  Vector3 vec) {
    vec *= scalar;
    return vec;
}

Vector3& Vector3::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

void Vector3::print() const {
    std::cout << x << " " << y << " " << z << "\n" << std::endl;
}

float Vector3::len() const {
    return sqrt(squareLen());
}

float Vector3::squareLen() const {
    return x * x + y * y + z * z;
    ;
}

Vector3 Vector3::get_rotated(const Vector3& axis, const float& alpha) const {
    
    if(axis.almost_equal_to_zero() || almost_equal_to_zero()) {
        std::cout << "Can't rotate null vector or around the null vector\n";
        return Vector3(0, 0, 0);
    }

    /* preparing */
    float cos_alpha = cos(alpha * PI / 180);
    float sin_alpha = sin(alpha * PI / 180);

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
    *this = get_rotated( axis, alpha);
    return *this;
}
    
Vector3& Vector3::norm() {
    if(almost_equal_to_zero()) {
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
    if(almost_equal_to_zero()) {
        std::cout << "Can't find normal to null vector\n";
        return Vector3(0, 0, 0);
    }
    if(x == 0 && y == 0) 
        return Vector3(z, 0, 0);
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
