#include <memory>
#include <iostream>

using namespace std;

class TestB;

class TestA
{
    public :
        TestA()
        {
            cout<<"TestA()"<<endl;            
        }

        void ReferTestB(weak_ptr<TestB> test_ptr)
        {
            m_testB_ptr = test_ptr;
        }
        void TestWork()
        {
            cout<<"~TestA::TestWork()"<<endl;
        }
        ~TestA()
        {
            cout<<"~TestA"<<endl;
        }
        private :
        weak_ptr<TestB> m_testB_ptr;  //TestB的职能指针
};

class TestB
{
public:
    TestB()
    {
        cout<<"TestB()"<<endl;
    }
    
    void ReferTestA(weak_ptr<TestA> test_ptr)
    {
        m_testA_ptr = test_ptr;
    }
    
    void TestWork()
    {
        std::cout << "~TestB::TestWork()" << std::endl;
    }

    ~TestB()
    {
        shared_ptr<TestA> temp = m_testA_ptr.lock();
        temp->TestWork();
        cout<<"2 ref a:"<<temp.use_count()<<endl;
        cout<<"~TestB()"<<endl;
    }
    weak_ptr<TestA> m_testA_ptr; //TestA的智能指针
};

int main()
{
    shared_ptr<TestA> ptr_a = make_shared<TestA>();
    shared_ptr<TestB> ptr_b = make_shared<TestB>();

    ptr_a->ReferTestB(ptr_b);
    ptr_b->ReferTestA(ptr_a);
    cout<<"1 ref a:"<<ptr_a.use_count()<<endl;
    cout<<"1 ref b:"<<ptr_a.use_count()<<endl;

    shared_ptr<TestA> p1 = make_shared<TestA>();
    cout<<"1 ref:"<<p1.use_count()<<endl;
    {
        shared_ptr<TestA> p2 = p1;
        cout<<"2 ref:"<<p1.use_count()<<endl;
    }

    cout<<"3 ref:"<<p1.use_count()<<endl;
    
    system("pause");
    
    return 0;
}