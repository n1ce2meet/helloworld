#include <iostream>

#define tab "\t"
// #define ENABLE_PTR_WRAPPER

template <class T>
class ForwardList;

#ifdef ENABLE_PTR_WRAPPER
template <class EL>
class pEL_wrapper;
#endif

template <class T>
class Element
{
    T data_;
    Element *pNext;

public:
    Element(T data = T{}, Element *pNext = nullptr) : data_(data), pNext(pNext)
    {
        std::cout << "EConstrcutor:\t" << this << std::endl;
    }
    ~Element()
    {
        std::cout << "EDestrcutor:\t" << this << std::endl;
    }

    friend class ForwardList<T>;

#ifdef ENABLE_PTR_WRAPPER
    friend class pEL_wrapper<Element<T>>;
#endif

};

#ifdef ENABLE_PTR_WRAPPER
template <class EL>
class FL_iterator {
    EL * ptr = nullptr;
    
    // ...

    EL * operator++() {
        return ptr = ptr->pNext;
    }

    EL * operator++(int) {
        EL * temp = ptr;
        ptr = ptr->pNext;
        return temp;
    }

    // ...
};
#endif

template <class T>
class ForwardList
{
    Element<T> *head_;
    size_t size_;
    using value_type = T;

    Element<T> *advance(Element<T> *iter, size_t distance)
    {
        if (distance > size_)
            throw "Forward list out of bounds";

        for (size_t i = 0; i < distance; ++i)
        {
            iter = iter->pNext;
        }
        return iter;
    }

public:
    ForwardList() : head_(nullptr), size_(0)
    {
        std::cout << "LConstructor:\t" << this << std::endl;
    }

    ForwardList(size_t size) : ForwardList()
    {
        while (size_ != size)
        {
            this->push_front(T());
        }
    }

    ForwardList(std::initializer_list<T> il) : ForwardList()
    {
        for (T val : il)
        {
            this->push_front(val);
        }
    }

    ~ForwardList()
    {
        while (head_)
        {
            this->pop_front();
        }
        std::cout << "LDestructor:\t" << this << std::endl;
    }

    ForwardList(ForwardList<T> &&) = default;
    ForwardList(ForwardList<T> const &other) : ForwardList()
    {
        size_ = other.size_;
        for (Element<T> *iter = other.head_; iter != nullptr; iter = iter->pNext)
        {
            this->push_front(iter->data_);
        }
    }

    ForwardList<T> &operator=(ForwardList<T> &&) = default;
    ForwardList<T> &operator=(const ForwardList<T> &other)
    {
        if (this == &other)
            return *this;
        ForwardList<T> to_delete = std::move(*this);
        new (this) ForwardList(other);
        return *this;
    }

    inline void push_front(T data)
    {
        // Element<T> *New = new Element(data);
        // New->pNext = head_;
        // head_ = New;

        head_ = new Element(data, head_);

        ++this->size_;
    }

    // WITHOUT SIZE push_back
    // IMPL 1

    // void push_back(T data)
    // {
    //     if (head_ != nullptr)
    //     {
    //         Element<T> *iter = head_;
    //         while (iter->pNext)
    //         {
    //             iter = iter->pNext;
    //         }
    //         iter->pNext = new Element(data);
    //     }
    //     else
    //     {
    //         head_ = new Element(data);
    //     }
    // }

    void push_back(T data)
    {
        if (head_ == nullptr)
            head_ = new Element(data);
        else
        {
            Element<T> *iter = ForwardList::advance(head_, size_ - 1);
            iter->pNext = new Element(data);
        }

        ++this->size_;
    }

    inline void pop_front()
    {
        if (head_ == nullptr)
            return;

        Element<T> *temp = head_;
        head_ = head_->pNext;
        delete temp;

        --this->size_;
    }

    //     WITHOUT SIZE pop_back
    //     IMPL 1

    // void pop_back() {

    //     if (head_ != nullptr)
    //     {
    //         Element<T> *iter = head_;
    //         Element<T> *prev = nullptr;

    //         while (iter->pNext)
    //         {
    //             prev = iter;
    //             iter = iter->pNext;
    //         }

    //         if (prev)
    //             prev->pNext = nullptr;
    //         else
    //             head_ = nullptr;

    //         delete iter;
    //     }
    // }

    //     WITHOUT SIZE pop_back
    //     IMPL 2

    // void pop_back()
    // {
    //     if (head_ == nullptr) return;
    //     if (head_->pNext == nullptr) return this->pop_front();

    //     Element<T> * iter = head_;
    //     while(iter->pNext->pNext) iter = iter->pNext;
    //     delete iter->pNext;
    //     iter->pNext = nullptr;
    // }

    void pop_back()
    {
        if (head_ == nullptr)
            return;
        if (head_->pNext == nullptr)
            return this->pop_front();
        Element<T> *iter = ForwardList::advance(head_, size_ - 2);
        delete iter->pNext;
        iter->pNext = nullptr;

        --this->size_;
    }

    void insert(size_t index, T data)
    {
        if (index == 0 or head_ == nullptr)
            return this->push_front(data);
        Element<T> *iter = ForwardList::advance(head_, index - 1);
        // Element<T> *temp = iter->pNext;
        iter->pNext = new Element(data, iter->pNext);
        ++this->size_;
    }

    void erase(size_t index)
    {
        if (index == 0)
            return this->pop_front();
        Element<T> *iter = ForwardList::advance(head_, index - 1);
        if (iter->pNext == nullptr)
            throw "No element at given index";
        Element<T> *temp = iter->pNext->pNext;
        delete iter->pNext;
        iter->pNext = temp;
        --this->size_;
    }

    void unique() //как быстрее?
    {
        Element<T> *to_unique = this->head_;
        Element<T> *to_find = nullptr;

        for (size_t unique_idx = 0; to_unique; ++unique_idx, to_unique = to_unique->pNext)
        {
            to_find = to_unique->pNext;

            for (size_t idx_to_remove = unique_idx + 1; to_find; ++idx_to_remove)
            {
                if (to_unique->data_ == to_find->data_)
                {
                    to_find = to_find->pNext;
                    this->erase(idx_to_remove);
                    --idx_to_remove;
                }
                else
                {
                    to_find = to_find->pNext;
                }
            }
        }
    }

    void reverse()
    {
        Element<T> *prev = nullptr;
        Element<T> *next = this->head_;
        Element<T> *temp = nullptr;
        for (size_t idx = 0; idx < size_; ++idx)
        {
            temp = next->pNext;
            next->pNext = prev;
            prev = next;
            next = temp;
        }
        this->head_ = prev;
    }

    T operator[](size_t index) const
    {
        return ForwardList::advance(head_, index)->data_;
    }

    T &operator[](size_t index)
    {
        return ForwardList::advance(head_, index)->data_;
    }

    bool is_empty() const { return !head_; }

    void print() const
    {
        Element<T> *Temp = head_;

        while (Temp)
        {
            std::cout << Temp << tab << Temp->data_ << tab << Temp->pNext << std::endl;
            Temp = Temp->pNext;
        }
    }
};

// #define BASE_CHECK
// #define DESTRUCTOR_CHECK
// #define HOME_WORK_1
// #define HOME_WORK_2

int main()
{
#ifdef BASE_CHECK
    int n;
    std::cout << "Enter list size: ";
    std::cin >> n;
    ForwardList<int> list;
    list.pop_front();
    for (int i = 0; i < n; i++)
    {
        //list.push_front(rand() % 100);
        list.push_back(rand() % 100);
    }
    list.print();
    //list.push_back(123);
    //list.pop_front();
    //list.pop_back();

    int index;
    int value;
    std::cout << "Enter index INSERT element: ";
    std::cin >> index;
    std::cout << "Enter value INSERT element: ";
    std::cin >> value;

    list.insert(index, value);
    list.print();

    std::cout << "Enter index ERASE element: ";
    std::cin >> index;
    list.erase(index);
    list.print();

#endif // BASE_CHECK

    // ForwardList<int> list1;
    // list1.push_back(3);
    // list1.push_back(5);
    // list1.push_back(8);
    // list1.push_back(13);
    // list1.push_back(21);
    // list1.pop_back();
    // list1.pop_front();
    // list1.print();

#ifdef DESTRUCTOR_CHECK
    int n;
    std::cout << "Enter list size: ";
    std::cin >> n;
    ForwardList<int> list;
    for (int i = 0; i < n; i++)
    {
        list.push_front(rand() % 100);
    }
    //std::cout << "Список заполнен" << std::endl;
    list.print();
#endif // DESTRUCTOR_CHECK

#ifdef HOME_WORK_1
    int n;
    std::cout << "Enter list size: ";
    std::cin >> n;
    ForwardList<int> list(n);
    for (int i = 0; i < n; i++)
    {
        list[i] = rand() % 100;
    }
    for (int i = 0; i < n; i++)
    {
        std::cout << list[i] << tab;
    }
    std::cout << std::endl;
#endif // HOME_WORK_1

#ifdef HOME_WORK_2
    ForwardList<int> list = {3, 5, 8, 13, 3, 21, 5, 5, 6, 5, 13};
    list.unique();
    list.reverse();
    list.print();
#endif // HOME_WORK_2

std::cout << typeid(Element<int>).name() << std::endl;
}