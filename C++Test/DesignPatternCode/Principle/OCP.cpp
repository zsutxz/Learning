
//开放封闭原则：类的改动是通过增加代码完成，而不是修改代码
#include <iostream>
 
class BankWorker
{
	public:
	virtual void show() = 0;       //预留一个接口，用于其他类继承并实现功能
};
 
class GetMoney : public BankWorker
{
	public:
	void show()
	{
		std::cout << "GetMoney !" << std::endl;
	}
};	
 
 
class SaveMoney : public BankWorker
{
	public:
	void show()
	{
		std::cout << "SaveMoney !" << std::endl;
	}
};
 
//此时若需增加功能，则增加子类继承于基类，无需修改已有代码
class TransformMoney : public BankWorker
{
	public:
	void show()
	{
		std::cout << "TransformMoney !" << std::endl;
	}
};

int main()
{
	BankWorker *b = new SaveMoney;//定义指针对象
	b->show();
	delete b;     //及时释放
	
	b = new GetMoney;
	b->show();
	delete b;
	
	b = new TransformMoney();
	b->show();
	delete	b;
	
	system("pause");

	return 0;
}