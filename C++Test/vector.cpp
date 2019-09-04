#include <iostream>
#include <vector>
#include <array>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <functional>

using namespace std;

int main()
{
    array<int, 10> arrayDemo = { 1,2,3,4,6,8 };
    cout << "arrayDemo:" << std::endl;
    for(auto it:arrayDemo)
    {
        cout << it << std::endl;
    }
    int arrayDemoSize = sizeof(arrayDemo);
    cout << "arrayDemo size:" << arrayDemoSize << endl;
    
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
    
    system("pause");
   
    return 0;
}   