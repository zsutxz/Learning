// 定义：一个对象应该对其他对象保持最少的了解。

// 问题由来：类与类之间的关系越密切，耦合度越大，当一个类发生改变时，对另一个类的影响也越大。

// 解决方案：尽量降低类与类之间的耦合。

#include<iostream>
#include<string>
#include<vector>
using namespace std;
  
class AbstractBuilding
{
public:
    virtual void sale() = 0;
    virtual string getQuality() = 0;
    virtual ~AbstractBuilding()
    {

    }
  
};
  
class BuildingA :public AbstractBuilding
{
public:
    BuildingA()
    {
        mQulity = "high";
    }
    
    virtual void sale()
    {
        cout << "building A " << mQulity << " sale!" << endl;
    }
    
    virtual string getQuality()
    {
        return mQulity;
    }
    
public:
    string mQulity;
};
  
  
class BuildingB :public AbstractBuilding
{
public:
  
    BuildingB()
    {
        mQulity = "low";
    }
    
    virtual void sale()
    {
        cout << "building B " << mQulity << " sale!" << endl;
    }
    
    virtual string getQuality()
    {
        return mQulity;
    }
  
public:
    string mQulity;
};
  
// 中介类
class Mediator
{
public:
    Mediator()
    {
        AbstractBuilding* building = new BuildingA;
        vBuilding.push_back(building);
        building = new BuildingB;
        vBuilding.push_back(building);
    }
    
    // 对外提供接口
    AbstractBuilding* findMyBuilding(string quality)
    {
        for (vector<AbstractBuilding*>::iterator it = vBuilding.begin();
        it != vBuilding.end(); it++)
        {
            if ((*it)->getQuality() == quality)
            {
                return *it;
            }
        }
    
        return NULL;
    }
  
 ~Mediator()
 {
    for (vector<AbstractBuilding*>::iterator it = vBuilding.begin();
    it != vBuilding.end(); it++)
    {
        if (*it != NULL)
        delete *it;
    }
 }
public:
    vector<AbstractBuilding*> vBuilding;
};
  
  
void test01()
{
    Mediator* mediator = new Mediator;
    
    AbstractBuilding* building = mediator->findMyBuilding("low");
    
    if (building != NULL)
    {
        building->sale();
    }
    else
    {
        cout << "no building" << endl;
    }
    
}
  
int main()
{
    test01();
    printf("test print: %d",12.000);

    system("pause");
}