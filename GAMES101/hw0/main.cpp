#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>
#include<iostream>

// rotate the point around orign,
Eigen::Vector3f Rotate(Eigen::Vector3f point, float angle)
{
    Eigen::Matrix3f m;
    m << std::cos(angle/180.0*acos(-1)),-std::sin(angle/180.0*acos(-1)),0,
         std::sin(angle/180.0*acos(-1)),std::cos(angle/180.0*acos(-1)),0,
         0,0,1;
    
    Eigen::Vector3f ret =m*point;

    return ret;
}

// move point 
Eigen::Vector3f Move(Eigen::Vector3f point, float x,float y)
{
    Eigen::Matrix3f m;
    m << 1,0,x,
         0,1,y,
         0,0,1;
    
    Eigen::Vector3f ret =m*point;

    return ret;
}

int main(){

    // Basic Example of cpp
    std::cout << "Example of cpp \n";
    float a = 1.0, b = 2.0;
    std::cout << a << std::endl;
    std::cout << a/b << std::endl;
    std::cout << std::sqrt(b) << std::endl;
    std::cout << std::acos(-1) << std::endl;
    std::cout << std::sin(30.0/180.0*acos(-1)) << std::endl;

    // Example of vector
    std::cout << "Example of vector \n";
    // vector definition
    Eigen::Vector3f v(1.0f,2.0f,3.0f);
    Eigen::Vector3f w(1.0f,0.0f,0.0f);
    // vector output
    std::cout << "Example of output \n";
    std::cout << v << std::endl;
    // vector add
    std::cout << "Example of add \n";
    std::cout << v + w << std::endl;
    // vector scalar multiply
    std::cout << "Example of scalar multiply \n";
    std::cout << v * 3.0f << std::endl;
    std::cout << 2.0f * v << std::endl;

    // Example of matrix
    std::cout << "Example of matrix \n";
    // matrix definition
    Eigen::Matrix3f i,j;
    i << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    j << 2.0, 3.0, 1.0, 4.0, 6.0, 5.0, 9.0, 7.0, 8.0;
    // matrix output
    std::cout << "Example of output \n";
    std::cout << i << std::endl;
    // matrix add i + j
    // matrix scalar multiply i * 2.0
    // matrix multiply i * j
    // matrix multiply vector i * v
    
    std::cout<<"i+j --- \n";
    std::cout<<i+j<<std::endl;
    
    std::cout<<"i*2 --- \n";
    std::cout<<i*2<<std::endl;
    
    std::cout<<"i*j ---- \n";
    std::cout<<i*j<<std::endl;
    
    std::cout<<"i*v ---- \n";
    std::cout<<i*v<<std::endl;

    Eigen::Vector3f p1(2.0f,1.0f,1.0f);
    std::cout<<"p1 ---- \n";
    std::cout<<p1<<std::endl;

    Eigen::Vector3f p2 = Rotate(p1,45.0);
    std::cout<<"rotate 45,p2 ---- \n";
    std::cout<<p2<<std::endl;

    Eigen::Vector3f p3 = Move(p2,1,2);
    std::cout<<"p3 ---- \n";
    std::cout<<p3<<std::endl;

    return 0;
}