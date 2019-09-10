// 定义：客户端不应该依赖它不需要的接口；一个类对另一个类的依赖应该建立在最小的接口上。 
// 问题由来：类A通过接口I依赖类B，类C通过接口I依赖类D，如果接口I对于类A和类B来说不是最小接口，则类B和类D必须去实现他们不需要的方法。

// 解决方案：将臃肿的接口I拆分为独立的几个接口，类A和类C分别与他们需要的接口建立依赖关系。也就是采用接口隔离原则。
#include <iostream>

using namespace std;

class I1 {
	public :
    virtual void method1()=0;
    virtual ~I1()
    {
        
    }
};

class I2 {
    public:
	virtual void method2()
    {
        cout<<("class I2 method 2")<<endl;
    }
	virtual void method3()
    {

    }
    virtual ~I2()
    {

    }

};

class I3 {
    public:
	virtual void method4()=0;
	virtual void method5()=0;
};

class A
{
	public :
    void depend1(I1 *i){
		i->method1();
	}
	void depend2(I2 *i){
		i->method2();
	}
	void depend3(I2 *i){
		i->method3();
	}
};

class B :public I1, public I2{
	public :
    virtual void method1() {
		cout<<("class B implent I1 method 1")<<endl;
	}

	void method2() {
		cout<<("class B implent I2 method 2")<<endl;
	}

	void method3() {
		cout<<("class B implent I2 method 3333")<<endl;
	}
};

class C{
	public :
    void depend1(I1 *i){
		i->method1();
	}
	
    void depend2(I3 *i){
		i->method4();
	}
	
    void depend3(I3 *i){
		i->method5();
	}
};

class D : public I1,public I3{
	public :
    void method1() {
		cout<<("class D implent I1 method 1")<<endl;
	}
	void method4() {
		cout<<("class D implent I3 method 4")<<endl;
	}
	void method5() {
		cout<<("class D implent I3 method 5")<<endl;
	}
};

int main()
{
    A *a = new A();
    B *b = new B();

    a->depend1(b);
    a->depend2(b);
    a->depend3(b);
    
    C *c = new C();
    D *d = new D();

    c->depend1(d);
    c->depend2(d);
    c->depend3(d);
    
    delete b;
    delete a;
    
    delete d;
    delete c;

    system("pause");

    return 1;
}