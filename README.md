# hash-table

hash table seperate chaining 

<b> Note </b>: <br>
To use the table for custom class definitions you must inject your custom class into the std::hash definition, here's an example
```
class Foo{
    public:
    std::string bar;
    
    Foo(const std::string& str){
        this->bar = str;
    }
}

template <>
struct std::hash<Foo>{
    std::size_t operator()(const Foo& foo){
        return std::hash<std::string>{}(foo.bar);
    }
}

```
