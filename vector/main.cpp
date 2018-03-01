#include"vector3.h"

int main() {
    Vector3 vec1(5.0, 4.0, 3.0);
    vec1.print();
    /*check len() and squareLen() methods*/
    std::cout << "len = " << vec1.len() << " square len = " << vec1.squareLen() << std::endl;
    Vector3 vec2(vec1);
    vec2.print();
    
    /*check overloaded operators*/
    auto vec3 = vec2 + vec1;
    vec3.print();
    vec3 = vec2 - vec1;
    vec3.print();
    vec2 += vec1;
    vec2.print();
    vec2 -= vec3;
    vec2.print();

}
    

