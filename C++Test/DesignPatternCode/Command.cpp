#include <iostream>
#include <list>
using namespace std;

// 司机
class Reciever
{
public:
    //接单
    void takeOrder()
    {
         std::cout << "Take order..." << std::endl;
    }
    //收款
    void receipt()
    {
       std::cout << "Receipt..." << std::endl; 
    } 
};

// 提供执行命令的接口
class Command
{
public:
    Command()
    {
    }
    
    //添加接受者
    void addReciever(Reciever *rev)
    {
        m_pRecievers.push_back(rev);
    }

    // 删除接收者
    void deletereciever(Reciever *rev)
    {
        m_pRecievers.remove(rev);
    }

    // 执行命令
    virtual void execute()
    {
    }  
    virtual ~Command()
    {
    }
protected:
    //Reciever *m_pReciever;

    std::list<Reciever *> m_pRecievers;  // 接收者队列
};

// 命令的持有者
class Invoker
{
public:
    Invoker()
    {
        
    }
    //添加命令
    void addCmd(Command *cmd)
    {
        m_cmds.push_back(cmd);
    }

    // 删除命令
    void deleteCmd(Command *cmd)
    {
        m_cmds.remove(cmd);
    }
    
    //执行命令
    void notify()
    {
        std::list<Command *>::iterator it = m_cmds.begin();
        while (it != m_cmds.end()) {
            (*it)->execute();
            ++it;
        }
    }

private:
    std::list<Command *> m_cmds;  // 命令队列
};


// 打车
class TakeCommand : public Command
{
public:
    TakeCommand()
    {
    }
    void execute()
    {
        std::cout << "Take command22..." << std::endl;
        
        std::list<Reciever *>::iterator it = m_pRecievers.begin();
        while (it != m_pRecievers.end()) {
            (*it)->takeOrder();
            ++it;
        }
    }
    ~TakeCommand()
    {
    }
};

// 付款
class PayCommand : public Command
{
public:
    PayCommand()
    {

    }
    void execute()
    {
        std::cout << "Pay command..." << std::endl;
       
        std::list<Reciever *>::iterator it = m_pRecievers.begin();
        while (it != m_pRecievers.end()) {
            (*it)->takeOrder();
            ++it;
        }

    }
};

int main()
{
    Reciever *rev = new Reciever();
    Command *cmd1 = new TakeCommand();
    cmd1->addReciever(rev);
    Command *cmd2 = new PayCommand();
    cmd2->addReciever(rev);
 
    Invoker inv;
    inv.addCmd(cmd1);
    inv.addCmd(cmd2);
    inv.notify();

    delete cmd1;
    delete cmd2;
    delete rev;

    system("pause");

    return 0;
}
