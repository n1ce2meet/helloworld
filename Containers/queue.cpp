#include <list>

template <typename _Tp, typename Container = std::list<_Tp>>
class Queue
{
    using _Self = Queue<_Tp, Container>;

    Container _data;

public:
    Queue(Container d = Container{}) : _data(d) {}
    Queue(const _Self &) = default;
    Queue(_Self &&) = default;
    _Self &operator=(_Self &&) = default;
    _Self &operator=(const _Self &) = default;

    void push(const _Tp& value) { 
        _data.push_front(value);
    }

    _Tp && pop() {
        _Tp tmp = std::move(_data.back());
        _data.pop_back();
        return tmp;
    }

    _Tp & front() {
        return _data.front();
    }

    _Tp front() const {
        return _data.front();
    }

    _Tp & back() {
        return _data.back();
    }

    _Tp back() const {
        return _data.back();
    }

    std::size_t size() const {
        return _data.size();
    }

    bool empty() const {
        return _data.empty();
    }
};