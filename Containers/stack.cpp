#include <forward_list>

template <typename _Tp, typename Container = std::forward_list<_Tp>>
class Stack
{
    using _Self = Stack<_Tp, Container>;

    Container _data;

public:
    Stack(Container d = Container{}) : _data(d) {}
    Stack(const _Self &) = default;
    Stack(_Self &&) = default;
    _Self &operator=(_Self &&) = default;
    _Self &operator=(const _Self &) = default;

    _Tp &top()
    {
        return _data.front();
    }

    _Tp top() const
    {
        return _data.front();
    }

    void push(const _Tp & value)
    {
        return _data.push_front(value);
    }

    _Tp &&pop()
    {
        _Tp tmp = std::move(_data.front());
        _data.pop_front();
        return tmp;
    }

    std::size_t size() const
    {
        return _data.size();
    }

    bool empty() const
    {
        return _data.empty();
    }
};