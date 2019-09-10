//依赖倒置原则：依赖于抽象（接口），不要依赖于具体的实现（类），也就是针对接口编程

#include <iostream>
 
class CPU
{
    public:
        virtual void cpu_work() = 0;
        virtual ~CPU()
        {

        }
};
 
class InterCPU : public CPU
{
    public:
        void cpu_work()
        {
            std::cout << "cpu_work !" << std::endl;
        }
};
 
class HardDisk
{
    public:
        virtual void harddisk_work() = 0;
        virtual ~HardDisk()
        {

        }
};
 
class AHardDisk : public HardDisk
{
    public:
        void harddisk_work()
        {
            std::cout << "harddisk_work !" << std::endl;
        }
};
 
class Computer
{
    private:
        CPU *m_cpu;
        HardDisk * m_harddisk;
    public:
        Computer(CPU *cpu,HardDisk *harddisk)    //构造函数初始化
        {
            m_cpu = cpu;
            m_harddisk = harddisk;
        }
        void show()
        {
            m_cpu->cpu_work();         //多态
            m_harddisk->harddisk_work();
        }
};

int main()
{
    CPU *cpu = new InterCPU;             //修改时可修改此处接口
    HardDisk * harddisk = new AHardDisk;
 
    Computer *c = new Computer(cpu,harddisk);
    c->show();
 
    delete c;
    delete cpu;
    delete harddisk;
    system("pause");
    
    return 0;
}