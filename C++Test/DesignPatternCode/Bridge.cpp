/*
* 关键代码：将现实独立出来，抽象类依赖现实类。
* 以下示例中，将各类App、各类手机独立开来，实现各种App和各种手机的自由桥接。
*/
#include <iostream>

using namespace std;

//抽象App类，提供接口
class App
{
public:
    // App()
    // {
    //     m_value = 0;
    // }
    // App(int v)
    // {
    //     m_value = v;
    // }
    // //拷贝构造函数
    // App(const App& c)
    // {
    //     m_value = c.m_value;
    // }

    virtual ~App(){ cout << "~App()" << endl; }
    virtual void run() = 0;
private:
    int m_value;
};

//具体的App实现类
class GameApp:public App
{
public:
    // //拷贝构造函数
    // GameApp(const GameApp& c):App(c)
    // {
    // }
    // GameApp():App()
    // {
    // }
    void run()
    {
        cout << "GameApp2 Running" << endl;
    }
};

//具体的App实现类
class TranslateApp:public App
{
public:
    void run()
    {
        cout << "TranslateApp2 Running" << endl;
    }
};

//抽象手机类，提供接口
class MobilePhone
{
public:
    MobilePhone(App* app):m_app(app){}
    virtual ~MobilePhone(){ cout << "~MobilePhone()" << endl;}
    virtual void appRun(App* app) = 0;  //实现App与手机的桥接
    virtual void run() = 0;

protected:
    App *m_app;
};

//具体的手机实现类
class XiaoMi:public MobilePhone
{
public:
    XiaoMi(App* app):MobilePhone(app){}
    void appRun(App* app)
    {
        cout << "XiaoMi: ";
        app->run();
    }
    
    void run()
    {
        m_app->run();
    }
};

//具体的手机实现类
class HuaWei:public MobilePhone
{
public:
    HuaWei(App* app):MobilePhone(app){}
    void appRun(App* app)
    {
        cout << "HuaWei: ";
        app->run();
    }

    void run()
    {
        m_app->run();
    }
};

int main()
{
    App* gameApp = new GameApp;
    App* translateApp = new TranslateApp;
    MobilePhone* mi = new XiaoMi(gameApp);
    MobilePhone* hua = new HuaWei(translateApp);
    mi->appRun(gameApp);
    mi->appRun(translateApp);
    mi->run();
    hua->appRun(gameApp);
    hua->appRun(translateApp);
    hua->run();

    delete hua;
    hua = nullptr;
    delete mi;
    mi = nullptr;
    delete gameApp;
    gameApp = nullptr;
    delete translateApp;
    translateApp = nullptr;

    system("pause");

    return 0;
}
