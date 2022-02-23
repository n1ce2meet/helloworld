#include <iostream>
#include "list.cpp"
#define tab '\t';

template <typename It>
void print(It start, It end)
{
    for (; start != end; ++start)
    {
        std::cerr << *start << tab;
    }
    std::cout << std::endl;
}

#define BASE_CHECK

int main()
{
#ifdef BASE_CHECK
    int n;
    std::cout << "Введите размер списка: ";
    std::cin >> n;
    List<int> list;
    for (int i = 0; i < n; i++)
    {
        // list.push_front(rand() % 100);
        list.push_back(rand() % 100);
    }
    print(list.begin(), list.end());
    print(list.rbegin(), list.rend());

    List<int> list2 = list;

    int index;
    int value;
    std::cout << "Введите индекс добавляемого элемента: ";
    std::cin >> index;
    std::cout << "Введите значение добавляемого элемента: ";
    std::cin >> value;
    list.insert(value, index);
    list.erase(2);
    print(list.begin(), list.end());
    print(list.rbegin(), list.rend());
#endif // BASE_CHECK

// List<int> list = { 3, 5, 8, 13, 21 };
// for(int i:list) std::cout << i << tab; std::cout << std::endl;
}