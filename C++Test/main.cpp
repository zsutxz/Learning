#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <functional>
#include <thread>

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

extern int thread_main(void);
extern int thread2_main(void);
extern int test_thread(void);
extern int ptr_main();

vector<int> getVector()
{
    return {1,2,3,4,5,6};
}

int main() {
    
    // auto add = [](int a ,int b )->int{
    //     return a+b;
    // };

    // int lamdaRet = add(1,2);
    // cout<<"lamda ret:"<<lamdaRet<<endl;

    vector<int> v = getVector();
    cout<<v[0]<<v[1]<<v.size()<<endl;

    // while(1);
    
    // Test test;    
    // TestAdd testAdd;
    // test.Add(bind(&TestAdd::Add,testAdd,placeholders::_1,placeholders::_2) ,1,2);

    // test.Add([](int a,int b)->int{
    //     cout<<"landa add fun"<<endl;
    //     return a+b;
    // },1,2);
    
    //ptr_main();
    //thread2_main();
    //test_thread();
    
    //return 1;
    
    std::cout << 2334 << std::endl;
    std::cout << 34566 << std::endl;
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

    while(1);

    std::array<int, 10> arrayDemo = { 1,2,3,4,6,8 };
    std::cout << "arrayDemo:" << std::endl;
    for(auto it:arrayDemo)
    {
        std::cout << it << std::endl;
    }
    int arrayDemoSize = sizeof(arrayDemo);
    std::cout << "arrayDemo size:" << arrayDemoSize << std::endl;
    
    Test * test1 = new Test();
    test1->TestWork(1);
    test1->TestWork(nullptr);
    delete test1;

    forward_list<int> numbers = {1,2,3,4,5,6,7};
    for (auto n :numbers)
    {
        cout<<n<<endl;
    }

    numbers.remove(4);
    cout<<"numbsers after remove:"<<endl;

    for (auto n:numbers)
    {
        cout<<n<<endl;
    }

    unordered_map<string,string> mymap=
    {
        {"house","maison"},
        { "apple","pomme" },
        { "tree","arbre" },
        { "book","livre" },
        { "door","porte" },
        { "grapefruit","pamplemousse" }
    };

    int n = mymap.bucket_count();
    
    cout<<"mymap has "<<n<<" buckets\n";

    int i = 0;

    for(auto it = mymap.begin();it!=mymap.end();it++)
    {
        cout << "bucket #" << i++ << " contains: ";
        cout<<"["<<it->first<<":"<<it->second<<"] "<<endl;
    }

    unordered_set<int> unorder_set;
    unorder_set.insert(8);
    unorder_set.insert(5);
    unorder_set.insert(6);
    unorder_set.insert(3);
    unorder_set.insert(100);
    
    cout<<"unorder_set:"<<endl;
    for(auto itor:unorder_set)
    {
        cout<<itor<<endl;
    }
    
    set<int> tempset;
    tempset.insert(3);
    tempset.insert(5);
    tempset.insert(5);
    tempset.insert(8);
    tempset.insert(7);
    tempset.insert(7);

    cout<<"set:"<<endl;

    for(auto itor:tempset)
    {
        cout<<itor<<endl;
    }
    
    //while(1);
   
    return 0;
}