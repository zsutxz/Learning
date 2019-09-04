/*
* 关键代码：在抽象类实现通用接口，细节变化在子类实现。
*/

#include <iostream>
using namespace std;

class Computer
{
public:
    void product()
    {
        installCpu();
        installRam();
        installGraphicsCard();
    }

    virtual ~Computer()
    {

    }

protected:
    virtual void installCpu() = 0;
    virtual void installRam() = 0;
    virtual void installGraphicsCard() = 0;
};

class ComputerA : public Computer
{
protected:
    void installCpu() override
    {
        cout << "ComputerA install Inter Core i5" << endl;
    }

    void installRam() override
    {
        cout << "ComputerA install 2G Ram" << endl;
    }

    void installGraphicsCard() override
    {
        cout << "ComputerA install Gtx940 GraphicsCard" << endl;
    }
};

class ComputerB : public Computer
{
protected:
    void installCpu() override
    {
        cout << "ComputerB install Inter Core i7" << endl;
    }

    void installRam() override
    {
        cout << "ComputerB install 4G Ram" << endl;
    }

    void installGraphicsCard() override
    {
        cout << "ComputerB install Gtx960 GraphicsCard" << endl;
    }
};

int main()
{
    Computer* c1 = new ComputerA();
    c1->product();
 
    delete c1;
    c1 = nullptr;

    system("pause");

    return 0;
}