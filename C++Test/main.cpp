#include <iostream>
#include <vector>
#include <functional>

using namespace std;

auto AddTest(int a, int b) 
{
    cout<<"AddTest"<<endl;
    return a + b;
}

class TestAdd
{
public:
    int Add(int a,int b)
    {
        cout<<"TestAdd::Add"<<endl;
        return a+b;
    }
};

class Test
{
public:
    void TestWork(int index)
    {
        std::cout << "TestWork 1" << std::endl;
    }
    void TestWork(int * index)
    {
        std::cout << "TestWork 2" << std::endl;
    }
        
    //typedef int(*FunType)(int,int);

    void Add(function<int(int,int)> fun, int a,int b)
    {
        int sum = fun(a,b);
        cout<<"in Test add sum:"<<sum<<endl;
    }
};

vector<int> getVector()
{
    return {1,2,3,4,5,6};
}

int main() {
    
    auto add = [](int a ,int b )->int{
        return a+b;
    };

    int lamdaRet = add(1,2);
    cout<<"lamda ret:"<<lamdaRet<<endl;

    vector<int> v = getVector();
    cout<<v[0]<<v[1]<<v.size()<<endl;
    
    Test test;    
    TestAdd testAdd;
    test.Add(bind(&TestAdd::Add,testAdd,placeholders::_1,placeholders::_2) ,1,2);

    test.Add([](int a,int b)->int{
        cout<<"landa add fun"<<endl;
        return a+b;
    },1,2);
    
    vector<string> msg {"Hello", "C++", "World", "from", "VS Coddfdfddfe!"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
   
    long double ld = 3.141759877;
    int c(ld);
    auto ret = AddTest(222,124);
    cout<<"c:"<<c<<" ret:"<<ret<<endl;
    
    const int t = 10;
    constexpr int a = t+20;
    cout<<"a:"<<a<<endl;
 
     system("pause");
   
    return 0;
}