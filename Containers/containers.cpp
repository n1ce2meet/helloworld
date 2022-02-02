#include <iostream>

#define tab "\t"

template <class T>
class ForwardList;

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
};

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

    ~ForwardList()
    {
        while (head_)
        {
            this->pop_front();
        }
        std::cout << "LDestructor:\t" << this << std::endl;
    }

    ForwardList(ForwardList<T> &&) = default;
    ForwardList(ForwardList<T> const &other) : ForwardList() , size_(other.size_)
    {
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
        Element<T> *New = new Element(data);
        New->pNext = head_;
        head_ = New;

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

    void push_back(T data) {
        if (head_ == nullptr) head_ = new Element(data);
        else {
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

    void pop_back() {
        if (head_ == nullptr) return;
        if (head_->pNext == nullptr) return this->pop_front();
        Element<T> * iter = ForwardList::advance(head_, size - 2);
        delete iter->pNext;
        iter->pNext = nullptr;

        --this->size_;
    }

    void insert(size_t index, T data)
    {
        if (index == 0 or head_ == nullptr)
            return this->push_front(data);
        Element<T> *iter = ForwardList::advance(head_, index - 1);
        Element<T> *temp = iter->pNext;
        iter->pNext = new Element(data, temp);
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

int main()
{
    ForwardList<int> l{};
    for (size_t i = 0; i < 12; ++i)
    {
        l.push_front(i);
    }

    ForwardList<int> p{};
    for (size_t i = 12; i < 24; ++i)
    {
        p.push_front(i);
    }

    l = p;

    l.print();

    l.erase(5);
    l.insert(7, 7);
    l.pop_back();
    l.pop_front();
    l.push_front(521);
    l.push_back(24);

    l.print();
}