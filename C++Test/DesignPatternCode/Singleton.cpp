#include <iostream>
#include <mutex>

using namespace std;

mutex mt;

class Singleton
{
public:
    static Singleton* getInstance();
    static Singleton* getInstance2();
    static Singleton& getInstance3();

    void show()
    {
        cout<<"hello wo11112223334444555666777888rld!"<<endl;
    }

private:
    Singleton(){}                                    //构造函数私有
    Singleton(const Singleton&)= delete;            //明确拒绝
    Singleton& operator=(const Singleton&) = delete; //明确拒绝

    static Singleton* m_pSingleton;
    static Singleton* m_pSingleton2;
        
    // GC 机制
    class GC
    {
    public:
        ~GC()
        {
            // 可以在这里销毁所有的资源，例如：db 连接、文件句柄等
            if (m_pSingleton != NULL) {
                cout << "Here destroy the m_pSingleton..." << endl;
                delete m_pSingleton;
                m_pSingleton = NULL;
            }
        }
        static GC gc;  // 用于释放单例
    };
};

Singleton* Singleton::m_pSingleton = NULL;
Singleton* Singleton::getInstance()
{
    if(m_pSingleton == NULL)
    {
        mt.lock();
        if(m_pSingleton == NULL)
        {
            m_pSingleton = new Singleton();
        }
        mt.unlock();
    }
    return m_pSingleton;
}

Singleton* Singleton::m_pSingleton2 = new Singleton();
Singleton* Singleton::getInstance2()
{
    return m_pSingleton2;
}

Singleton& Singleton::getInstance3()
{
    static Singleton singleton;
    return singleton;
}

Singleton::GC Singleton::GC::gc; // 重要

int main()
{
    //Singleton::getInstance3().show();

    Singleton *pSingleton1 = Singleton::getInstance();
    pSingleton1->show();
    Singleton *pSingleton2 = Singleton::getInstance();

    cout << (pSingleton1 == pSingleton2) << endl;

    system("pause");

    return 0;
}