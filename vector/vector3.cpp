#include"vector3.h"
#include<valarray>

#define PI 3.14159265

Vector3& Vector3::mult(const float& scaler) {
    x = x * scaler; 
    y = y * scaler; 
    z = z * scaler;
    return *this;
}

Vector3& Vector3::div(const float& scaler) {
    x = x / scaler;
    y = y / scaler;
    z = z / scaler;
    return *this;
}

void Vector3::print() {
    std::cout << x << " " << y << " " << z << "\n" << std::endl;
}

float Vector3::len() const {
    return sqrt(x * x +
                y * y +
                z * z);
}

float Vector3::squareLen() const {
    return (x * x + y * y + z * z);
}

Vector3 Vector3::getRotated(const Vector3& axis, const float& alpha) {
    float first_el;
    float second_el;
    float third_el;
    
    /* preparing */
    float cos_alpha = cos(alpha * PI / 180);
    float sin_alpha = sin(alpha * PI / 180);

    /* computing new elements */
    first_el = (cos_alpha + (1 - cos_alpha) * axis.x * axis.x) * x
             + ((1 - cos_alpha) * axis.x * axis.y - sin_alpha * axis.z) * y
             + ((1 - cos_alpha) * axis.x * axis.z + sin_alpha * axis.y) * z;
    
    second_el  = ((1 - cos_alpha) * axis.x * axis.y + sin_alpha * axis.z) * x
               + (cos_alpha + (1 - cos_alpha) * axis.y * axis.y) * y
               + ((1 - cos_alpha) * axis.z * axis.y - sin_alpha * axis.x) * z;

    third_el = ((1 - cos_alpha) * axis.x * axis.z - sin_alpha * axis.y) * x
             + ((1 - cos_alpha) * axis.z * axis.y + sin_alpha * axis.x) * y
             + (cos_alpha + (1 - cos_alpha) * axis.z * axis.z) * z;

    /* constructor */
    return Vector3(first_el, second_el, third_el);
}


Vector3& Vector3::rotate(const Vector3& axis, const float& alpha) {
    *this = this->getRotated( axis, alpha);
    return *this;
}
    
Vector3& Vector3::norm() {
    x = x / this->len();
    y = y / this->len();
    z = z / this->len();
    return *this;
}

