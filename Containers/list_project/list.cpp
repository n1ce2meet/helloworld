#include "list.hpp"

template <class It>
auto advance(It iter, size_t distance) -> decltype(iter.base().ptr())
{
    for (; distance; ++iter, --distance);
    return iter.base().ptr();
}

template <class _Tp>
_List_node<_Tp>::_List_node(_Tp somedata, _List_node *pNext, _List_node *pPrev) : data(somedata), next(pNext), prev(pPrev) {}

template <class _Tp, class _Node>
_List_iterator_impl<_Tp, _Node>::_List_iterator_impl(_Node *p) : _p(p) {}

template <class _Tp, class _Node>
typename _List_iterator_impl<_Tp, _Node>::_Self
_List_iterator_impl<_Tp, _Node>::base() const
{
    return *this;
}

template <class _Tp, class _Node>
_Node *
_List_iterator_impl<_Tp, _Node>::ptr()
{
    return _p;
}

template <class _Tp, class _Node>
typename _List_iterator_impl<_Tp, _Node>::_Self &
_List_iterator_impl<_Tp, _Node>::operator++()
{
    _p = _p->next;
    return *this;
}

template <class _Tp, class _Node>
typename _List_iterator_impl<_Tp, _Node>::_Self
_List_iterator_impl<_Tp, _Node>::operator++(int)
{
    pointer res = _p;
    _p = _p->next;
    return res;
}

template <class _Tp, class _Node>
typename _List_iterator_impl<_Tp, _Node>::_Self &
_List_iterator_impl<_Tp, _Node>::operator--()
{
    _p = _p->prev;
    return *this;
}

template <class _Tp, class _Node>
typename _List_iterator_impl<_Tp, _Node>::_Self
_List_iterator_impl<_Tp, _Node>::operator--(int)
{
    pointer res = _p;
    _p = _p->prev;
    return res;
}

template <class _Tp, class _Node>
typename _List_iterator_impl<_Tp, _Node>::reference
_List_iterator_impl<_Tp, _Node>::operator*() const
{
    return _p->data;
}

template <class _Tp, class _Node>
typename _List_iterator_impl<_Tp, _Node>::pointer
_List_iterator_impl<_Tp, _Node>::operator->() const
{
    return &(_p->data);
}

template <class _Tp, class _Node>
_List_iterator_impl<_Tp, _Node>::operator bool() const
{
    return _p;
}

template <class _Iterator>
typename std::reverse_iterator<_Iterator>::reference
_List_reverse_iterator_impl<_Iterator>::operator*()
{
    return *std::reverse_iterator<_Iterator>::current;
}

template <typename _Tp>
List<_Tp>::List() : _head(nullptr), _tail(nullptr), _list_size(0) {}

template <typename _Tp>
List<_Tp>::List(std::initializer_list<_Tp> il) : List()
{
    for (auto &&i : il)
        this->push_back(i);
}

template <typename _Tp>
List<_Tp>::List(size_t size) : List()
{
    for (size_t i = 0; i < size; ++i)
        this->push_back(_Tp());
}

template <typename _Tp>
List<_Tp>::List(const _Self &other) : List()
{
    for (auto &&i : other)
    {
        this->push_back(i);
    }
}

template <typename _Tp>
List<_Tp>::List(_Self &&other)
{
    this->_head = other._head;
    this->_tail = other._tail;
    this->_list_size = other._list_size;

    other._head = nullptr;
    other._tail = nullptr;
    other._list_size = 0;
}

template <typename _Tp>
List<_Tp>::~List()
{
    while (!this->empty())
        this->pop_back();
}

template <typename _Tp>
typename List<_Tp>::_Self 
List<_Tp>::operator=(const _Self &other)
{
    if (this == &other)
        return *this;

    _Self to_delete = std::move(*this);
    new (this) List(other);
    return *this;
}

template <typename _Tp>
typename List<_Tp>::_Self
List<_Tp>::operator=(_Self &&other)
{
    if (this == &other)
        return *this;

    _Self to_delete = std::move(*this);
    new (this) List(std::move(other));
    return *this;
}

template <typename _Tp>
void 
List<_Tp>::push_front(value_type data)
{
    _head = new _Node(data, _head, nullptr);
    if (_tail == nullptr)
        _tail = _head;
    else
        _head->next->prev = _head;

    ++_list_size;
}

template <typename _Tp>
void
List<_Tp>::push_back(value_type data)
{
    _tail = new _Node(data, nullptr, _tail);
    if (_head == nullptr)
        _head = _tail;
    else
        _tail->prev->next = _tail;

    ++_list_size;
}

template <typename _Tp>
void
List<_Tp>::pop_front()
{
    if (_head == nullptr)
        return;

    _Node *tmp = _head;
    _head = _head->next;
    if (_head)
        _head->prev = nullptr;
    else
        _tail = _head;

    delete tmp;
    --_list_size;
}

template <typename _Tp>
void
List<_Tp>::pop_back()
{
    if (_tail == nullptr)
        return;

    _Node *tmp = _tail;
    _tail = _tail->prev;
    if (_tail)
        _tail->next = nullptr;
    else
        _head = _tail;

    delete tmp;
    --_list_size;
}

template <typename _Tp>
void
List<_Tp>::insert(value_type data, size_t idx)
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

template <typename _Tp>
void
List<_Tp>::erase(size_t idx)
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

template <typename _Tp>
bool
List<_Tp>::empty() const
{
    return _head == nullptr;
}

template <typename _Tp>
typename List<_Tp>::iterator
List<_Tp>::begin()
{
    return iterator(_head);
}

template <typename _Tp>
typename List<_Tp>::iterator
List<_Tp>::end()
{
    return iterator();
}

template <typename _Tp>
typename List<_Tp>::const_iterator
List<_Tp>::begin() const
{
    return const_iterator(_head);
}

template <typename _Tp>
typename List<_Tp>::const_iterator
List<_Tp>::end() const
{
    return const_iterator();
}

template <typename _Tp>
typename List<_Tp>::reverse_iterator
List<_Tp>::rbegin()
{
    return reverse_iterator(iterator(_tail));
}

template <typename _Tp>
typename List<_Tp>::reverse_iterator
List<_Tp>::rend()
{
    return reverse_iterator(nullptr);
}

template <typename _Tp>
typename List<_Tp>::reverse_const_iterator
List<_Tp>::rbegin() const
{
    return reverse_const_iterator(const_iterator(_tail));
}

template <typename _Tp>
typename List<_Tp>::reverse_const_iterator
List<_Tp>::rend() const
{
    return reverse_const_iterator(nullptr);
}

template <class _Tp>
typename List<_Tp>::_Node *
List<_Tp>::fasttrack(size_t idx) const
{
    if (idx > _list_size)
        throw "List out of bounds";

    if (idx > (_list_size / 2))
        return advance(this->rbegin(), idx);
    else
        return advance(this->begin(), idx);
}