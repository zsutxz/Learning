#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Element;

class Visitor
{
public:
     virtual void VisitConcreteElementA(Element *pElementA) = 0;
     virtual void VisitConcreteElementB(Element *pElementB) = 0;
     virtual ~Visitor()
     {

     }
};
 
class ConcreteVisitor1 : public Visitor
{
public:
     void VisitConcreteElementA(Element *pElementA);
     void VisitConcreteElementB(Element *pElementB);
};
 
void ConcreteVisitor1::VisitConcreteElementA(Element *pElementA)
{
     // 现在根据传进来的pElementA，可以对ConcreteElementA中的element进行操作
     cout<<"in concreteVistor1,op ConcreteElementA"<<endl;
}
 
void ConcreteVisitor1::VisitConcreteElementB(Element *pElementB)
{
    // 现在根据传进来的pElementB，可以对ConcreteElementB中的element进行操作
     cout<<"in concreteVistor1,op ConcreteElementB"<<endl;
}
 
class ConcreteVisitor2 : public Visitor
{
public:
     void VisitConcreteElementA(Element *pElementA);
     void VisitConcreteElementB(Element *pElementB);
};
 
void ConcreteVisitor2::VisitConcreteElementA(Element *pElementA)
{
     // ...
    cout<<"in concreteVistor2 333,op ConcreteElementA"<<endl;

}
 
void ConcreteVisitor2::VisitConcreteElementB(Element *pElementB)
{
     // ...
    cout<<"in concreteVistor2 CCC,op ConcreteElementB"<<endl;

}
 
// Element object
class Element
{
public:
     virtual void Accept(Visitor *pVisitor) = 0;
     virtual ~Element()
     {

     }
};
 
class ConcreteElementA : public Element
{
public:
     void Accept(Visitor *pVisitor);
};
 
void ConcreteElementA::Accept(Visitor *pVisitor)
{
     pVisitor->VisitConcreteElementA(this);
}
 
class ConcreteElementB : public Element
{
public:
     void Accept(Visitor *pVisitor);
};
 
void ConcreteElementB::Accept(Visitor *pVisitor)
{
     pVisitor->VisitConcreteElementB(this);
}
 
// ObjectStructure类，能枚举它的元素，可以提供一个高层的接口以允许访问者访问它的元素
class ObjectStructure
{
public:
     void Attach(Element *pElement);
     void Detach(Element *pElement);
     void Accept(Visitor *pVisitor);
 
private:
     vector<Element *> elements;
};
 
void ObjectStructure::Attach(Element *pElement)
{
     elements.push_back(pElement);
}
 
void ObjectStructure::Detach(Element *pElement)
{
     vector<Element *>::iterator it = find(elements.begin(), elements.end(), pElement);
     if (it != elements.end())
     {
          elements.erase(it);
     }
}
 
void ObjectStructure::Accept(Visitor *pVisitor)
{
     // 为每一个element设置visitor，进行对应的操作
     for (vector<Element *>::const_iterator it = elements.begin(); it != elements.end(); ++it)
     {
          (*it)->Accept(pVisitor);
     }
}
 
int main()
{
     ObjectStructure *pObject = new ObjectStructure;
 
     Element *pElementA = new ConcreteElementA;
     Element *pElementB = new ConcreteElementB;
 
     pObject->Attach(pElementA);
     pObject->Attach(pElementB);
 
     ConcreteVisitor1 *pVisitor1 = new ConcreteVisitor1;
     ConcreteVisitor2 *pVisitor2 = new ConcreteVisitor2;
 
     pObject->Accept(pVisitor1);
     pObject->Accept(pVisitor2);
 
     if (pVisitor2) delete pVisitor2;
     if (pVisitor1) delete pVisitor1;
     if (pElementB) delete pElementB;
     if (pElementA) delete pElementA;
     if (pObject) delete pObject;
    
    system("pause");
    return 0;
}
