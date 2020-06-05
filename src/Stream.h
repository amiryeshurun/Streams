//
// Created by Amir Yeshurun on 2020-05-03.
//

#ifndef STREAMS_STREAM_H
#define STREAMS_STREAM_H

#include <memory>
#include <functional>

template < typename  T, template<typename...> typename Container = std::vector>
class Stream {
private:
    Container<T> container;
public:
    Stream();
    template< template<typename...> typename InputContainer >
    Stream(const InputContainer<T>& inputContainer);

    Stream<T, Container>& add(const T& val);
    Stream<T, Container>& add(T&& val);

    template< typename Function >
    Stream<std::invoke_result_t<Function, const T&>, Container> map(Function&& mapper);
    template< template<typename...> typename OutputContainer = std::vector >
    OutputContainer<T> collect();

    void forEach(std::function<void(const T&)> consumer);
    T reduce(const T& startValue, std::function<T(const T&, const T&)> reducer);
    T sum(std::function<const T&(const T&)> mapper = [](const T& elm){
        return elm;
    });

    Stream<T, Container>& apply(std::function<void(T&)> consumer);
    Stream<T, Container>& filter(std::function<bool(const T&)> predicate);
    Stream<int, Container> mapToInt(std::function<int(const T&)> mapper);
};

template < typename  T, template<typename...> typename Container >
Stream<T, Container>::Stream() {}

template < typename  T, template<typename...> typename Container >
template< template<typename...> typename InputContainer >
Stream<T, Container>::Stream(const InputContainer<T>& inputContainer) {
    for(const auto& elm : inputContainer)
        container.push_back(elm);
}

template < typename  T, template<typename...> typename Container >
Stream<T, Container>& Stream<T, Container>::add(const T& val) {
    container.push_back(val);
    return *this;
}

template < typename  T, template<typename...> typename Container >
Stream<T, Container>& Stream<T, Container>::add(T&& val) {
    container.push_back(std::move(val));
    return *this;
}

template < typename  T, template<typename...> typename Container >
Stream<T, Container>& Stream<T, Container>::apply(std::function<void(T&)> consumer) {
    for(auto& elm : container)
        consumer(elm);
    return *this;
}

template < typename  T, template<typename...> typename Container >
Stream<T, Container>& Stream<T, Container>::filter(std::function<bool(const T&)> predicate) {
    for(typename Container<T>::iterator it = container.begin(); it != container.end();) {
        if(predicate(*it))
            it = container.erase(it);
        else
            it++;
        if(it == container.end())
            break;
    }
    return *this;
}

template < typename  T, template<typename...> typename Container >
template< typename Function >
Stream<std::invoke_result_t<Function, const T&>, Container> Stream<T, Container>::map(Function&& mapper) {
    decltype(map(mapper)) next;
    for(const auto& elm : container)
        next.add(mapper(elm));
    return next;
}

template < typename  T, template<typename...> typename Container >
void Stream<T, Container>::forEach(std::function<void(const T&)> consumer) {
    for(const auto& elm : container)
        consumer(elm);
}

template < typename  T, template<typename...> typename Container >
Stream<int, Container> Stream<T, Container>::mapToInt(std::function<int(const T&)> mapper) {
    decltype(mapToInt(mapper)) next;
    for(const auto& elm : container)
        next.add(mapper(elm));
    return next;
}

template < typename  T, template<typename...> typename Container >
T Stream<T, Container>::sum(std::function<const T&(const T&)> mapper) {
    auto it = container.begin();
    T sum = mapper(*it);
    for(; it != container.end(); it++)
        sum += mapper(*it);
    return sum;
}

template < typename  T, template<typename...> typename Container >
T Stream<T, Container>::reduce(const T& startValue, std::function<T(const T&, const T&)> reducer) {
    T curr = startValue;
    for(const auto& elm : container)
        curr = reducer(curr, elm);
    return curr;
}

template < typename  T, template<typename...> typename Container >
template< template<typename...> typename OutputContainer >
OutputContainer<T> Stream<T, Container>::collect()  {
    OutputContainer<T> out;
    for(auto&& elm : container)
        out.push_back(elm);
    return out;
}

#endif //STREAMS_STREAM_H