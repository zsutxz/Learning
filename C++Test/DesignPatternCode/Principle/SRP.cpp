//不要存在多于一个导致类变更的原因。通俗的说，即一个类只负责一项职责。
// 遵循单一职责原的优点有：
// 可以降低类的复杂度，一个类只负责一项职责，其逻辑肯定要比负责多项职责简单的多；
// 提高类的可读性，提高系统的可维护性；
// 变更引起的风险降低，变更是必然的，如果单一职责原则遵守的好，当修改一个功能时，可以显著降低对其他功能的影响。

#include <iostream>
 
class Animal
{
    public:
        virtual void breathe(std::string animal)=0;
        virtual ~Animal()
        {

        }
};

class Terrestrial : public Animal
{
	public:
        void breathe(std::string animal)
        {
            std::cout<<animal + " breathe!" << std::endl;
	    }
};

class Aquatic : public Animal
{
    public:
        void breathe(std::string animal)
        {
            std::cout << animal + " breathe in water!" << std::endl;
	    }
};

int main()
{
    Terrestrial *terrestrial = new Terrestrial();
    
    terrestrial->breathe("cow");
    terrestrial->breathe("sheep");
    terrestrial->breathe("pig");
    
    Aquatic *aquatic = new Aquatic;
    aquatic->breathe("fish");
    
    delete terrestrial;
    delete aquatic;

    system("pause");
    
    return 1;
}		