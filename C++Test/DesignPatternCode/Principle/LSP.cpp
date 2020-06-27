// 里氏替换原则通俗的来讲就是：子类可以扩展父类的功能，但不能改变父类原有的功能。它包含以下4层含义：

 //   子类可以实现父类的抽象方法，但不能覆盖父类的非抽象方法。
 //   子类中可以增加自己特有的方法。
 //   当子类的方法重载父类的方法时，方法的前置条件（即方法的形参）要比父类方法的输入参数更宽松。
 //   当子类的方法实现父类的抽象方法时，方法的后置条件（即方法的返回值）要比父类更严格。
#include <iostream>

using namespace std;

class A{
	public :
    virtual ~A()
    {

    }
    int func1(int a, int b){
		return a-b;
	}
};

class B :public A
{
	public :

    int func1(int a, int b){
		return a+b;
	}
	
    int func2(int a, int b){
		return func1(a,b)+100;
	}
};

int main()
{
    A *a = new A();
    cout<<"a,100-50="<<a->func1(100,50)<<endl;
    cout<<"a,100-80="<<a->func1(100,80)<<endl;

    B *b = new B();
    cout<<"b,100-50="<<b->func1(100,50)<<endl;
    cout<<"b,100-80="<<b->func1(100,80)<<endl;
    cout<<"b,100+20+100="<<b->func2(100,20)<<endl;

    delete a;
    delete b;
    
    system("pause");
    
    return 0;
} 