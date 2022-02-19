#include <algorithm>
typedef __PTRDIFF_TYPE__ ptrdiff_t;

template <typename _Tp>
struct _List_node
{
    _Tp data;
    _List_node *next;
    _List_node *prev;

    _List_node(_Tp somedata = _Tp(), _List_node *pNext = nullptr, _List_node *pPrev = nullptr)
        : data(somedata), next(pNext), prev(pPrev) {}
};

template <class _Tp, class _Node = _List_node<_Tp>>
class _List_iterator_impl
{
    _Node *_p;

public:
    using _Self = _List_iterator_impl<_Tp, _Node>;

    using difference_type = ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = _Tp;
    using reference = _Tp &;
    using pointer = _Tp *;

    _List_iterator_impl(_Node *p = nullptr) : _p(p) {}
    _List_iterator_impl(const _Self &other) = default;
    _List_iterator_impl(_Self &&other) = default;
    _Self &operator=(const _Self &other) = default;
    _Self &operator=(_Self &&other) = default;

    _Self base() const
    {
        return *this;
    }

    _Node *ptr()
    {
        return _p;
    }

    _Self &operator++()
    {
        _p = _p->next;
        return *this;
    }

    _Self operator++(int)
    {
        pointer res = _p;
        _p = _p->next;
        return res;
    }

    _Self &operator--()
    {
        _p = _p->prev;
        return *this;
    }

    _Self operator--(int)
    {
        pointer res = _p;
        _p = _p->prev;
        return res;
    }

    reference operator*() const
    {
        return _p->data;
    }

    pointer operator->() const
    {
        return &(_p->data);
    }

    operator bool() const
    {
        return _p;
    }
};

template <class _Iterator>
class _List_reverse_iterator_impl : public std::reverse_iterator<_Iterator> {
public:

    using std::reverse_iterator<_Iterator>::reverse_iterator;

    typename std::reverse_iterator<_Iterator>::reference operator*() {
        return *std::reverse_iterator<_Iterator>::current;
    }
};

template <class _Tp>
class List
{
    using _Self = List<_Tp>;
    using _Node = _List_node<_Tp>;

    _Node *_head;
    _Node *_tail;
    size_t _list_size;

public:
    using value_type = _Tp;
    using iterator = _List_iterator_impl<_Tp>;
    using const_iterator = _List_iterator_impl<const _Tp, _List_node<_Tp>>;
    using reverse_iterator = _List_reverse_iterator_impl<iterator>;
    using reverse_const_iterator = _List_reverse_iterator_impl<const_iterator>;

    List() : _head(nullptr), _tail(nullptr), _list_size(0) {}

    List(std::initializer_list<_Tp> il) : List()
    {
        for (auto &&i : il)
            this->push_back(i);
    }

    explicit List(size_t size) : List()
    {
        for (size_t i = 0; i < size; ++i)
            this->push_back(_Tp());
    }

    List(const _Self &other) : List()
    {
        for (auto &&i : other)
        {
            this->push_back(i);
        }
    }

    List(_Self &&other)
    {
        this->_head = other._head;
        this->_tail = other._tail;
        this->_list_size = other._list_size;

        other._head = nullptr;
        other._tail = nullptr;
        other._list_size = 0;
    }

    _Self operator=(const _Self &other)
    {
        if (this == &other)
            return *this;

        _Self to_delete = std::move(*this);
        new (this) List(other);
        return *this;
    }

    _Self operator=(_Self &&other)
    {
        if (this == &other)
            return *this;

        _Self to_delete = std::move(*this);
        new (this) List(std::move(other));
        return *this;
    }

    ~List()
    {
        while (!this->empty())
            this->pop_back();
    }

    void push_front(value_type data)
    {
        _head = new _Node(data, _head, nullptr);
        if (_tail == nullptr)
            _tail = _head;
        else
            _head->next->prev = _head;

        ++_list_size;
    }

    void push_back(value_type data)
    {
        _tail = new _Node(data, nullptr, _tail);
        if (_head == nullptr)
            _head = _tail;
        else
            _tail->prev->next = _tail;

        ++_list_size;
    }

    void pop_front()
    {
        if (_head == nullptr)
            return;

        _Node * tmp = _head;
        _head = _head->next;
        if (_head)
            _head->prev = nullptr;
        else
            _tail = _head;

        delete tmp;
        --_list_size;
    }

    void pop_back()
    {
        if (_tail == nullptr)
            return;

        _Node * tmp = _tail;
        _tail = _tail->prev;
        if (_tail)
            _tail->next = nullptr;
        else
            _head = _tail;

        delete tmp;
        --_list_size;
    }

    void insert(value_type data, size_t idx)
    {
        if (idx == 0)
            return push_front(data);
        if (idx == _list_size - 1)
            return push_back(data);
        _Node *prev_tmp = fasttrack(idx - 1);
        _Node *next_tmp = prev_tmp->next;
        prev_tmp->next = new _Node(data, next_tmp, prev_tmp);
        next_tmp->prev = prev_tmp->next;

        ++_list_size;
    }

    void erase(size_t idx)
    {   
        if (idx == 0)
            return pop_front();
        if (idx == _list_size - 1)
            return pop_back();
        _Node *prev_tmp = fasttrack(idx - 1);
        _Node *next_tmp = prev_tmp->next->next;
        delete prev_tmp->next;
        prev_tmp->next = next_tmp;
        next_tmp->prev = prev_tmp;

        --_list_size;
    }

    bool empty() const
    {
        return _head == nullptr;
    }

    iterator begin()
    {
        return iterator(_head);
    }

    iterator end()
    {
        return iterator();
    }

    const_iterator begin() const
    {
        return const_iterator(_head);
    }

    const_iterator end() const
    {
        return const_iterator();
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(iterator(_tail));
    }

    reverse_iterator rend()
    {
        return reverse_iterator(nullptr);
    }

    reverse_const_iterator rbegin() const
    {
        return reverse_const_iterator(const_iterator(_tail));
    }

    reverse_const_iterator rend() const
    {
        return reverse_const_iterator(nullptr);
    }

    template <class It>
    static auto advance(It iter, size_t distance) -> decltype(iter.base().ptr())
    {
        for (; distance; ++iter, --distance);
        return iter.base().ptr();
    }

    _Node *fasttrack(size_t idx) const
    {
        if (idx > _list_size)
            throw "List out of bounds";

        if (idx > (_list_size / 2))
            return List::advance(this->rbegin(), idx);
        else
            return List::advance(this->begin(), idx);
    }
};

#include <iostream>
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