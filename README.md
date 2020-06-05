# Streams
This is a very simple implementation of java streams in C++.

---

**Requirments:**

In order to use the stream with your own container, you have to make sure that your container is iterable.
You are also allowed to specify which container type should be used in order to manage the stream behind the scenes. 
For example if you would like the stream to be implemented using a list, then define it as follows:

`Stream<Type, std::list>`

If no value is specified here (second template parameter), then the default value `std::vector` is selected.

***Example:***

```
std::vector<int> vec = {1, 2, 3, 4, 5};
std::cout << Stream(vec).map([](const auto& elem) {
    return elem + 1;
}).reduce(0, [](const auto& elm1, const auto& elm2) {
    return elm1 + elm2;
}) << std::endl;
```

Here the vector `vec` is mapped to a new vector `{2, 3, 4, 5, 6}`. Then, the reduce function is calculating the sum of the new vector.
The result is being printed to the standard output.

