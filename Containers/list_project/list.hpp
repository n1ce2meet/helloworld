#include <algorithm>

template <class It>
auto advance(It iter, size_t distance) -> decltype(iter.base().ptr());

template <typename _Tp>
struct _List_node
{
    _Tp data;
    _List_node *next;
    _List_node *prev;

    _List_node(_Tp somedata = _Tp(), _List_node *pNext = nullptr, _List_node *pPrev = nullptr);
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

    _List_iterator_impl(_Node *p = nullptr);
    _List_iterator_impl(const _Self &other) = default;
    _List_iterator_impl(_Self &&other) = default;
    _Self &operator=(const _Self &other) = default;
    _Self &operator=(_Self &&other) = default;
    _Self base() const;
    _Node *ptr();
    _Self &operator++();
    _Self operator++(int);
    _Self &operator--();
    _Self operator--(int);
    reference operator*() const;
    pointer operator->() const;
    operator bool() const;
};

template <class _Iterator>
class _List_reverse_iterator_impl : public std::reverse_iterator<_Iterator>
{
public:
    using std::reverse_iterator<_Iterator>::reverse_iterator;
    typename std::reverse_iterator<_Iterator>::reference operator*();
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

    List();
    List(std::initializer_list<_Tp> il);
    explicit List(size_t size);
    List(const _Self &other);
    List(_Self &&other);
    _Self operator=(const _Self &other);
    _Self operator=(_Self &&other);
    ~List();

    void push_front(value_type data);
    void push_back(value_type data);
    void pop_front();
    void pop_back();
    void insert(value_type data, size_t idx);
    void erase(size_t idx);
    bool empty() const;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    reverse_const_iterator rbegin() const;
    reverse_const_iterator rend() const;
    _Node *fasttrack(size_t idx) const;
};