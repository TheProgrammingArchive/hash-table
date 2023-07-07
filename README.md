# hash-table
### Hash Table (seperate chaining)

<b> Note </b>: <br>
To use the table for custom class definitions you must inject your custom class into the std::hash definition, here's an example

```
class Foo{
    public:
    std::string bar;
    
    Foo(const std::string& str){
        this->bar = str;
    }
};

template <>
struct std::hash<Foo>{
    std::size_t operator()(const Foo& foo) const{
        return std::hash<std::string>{}(foo.bar);
    }
};

```

<b> Usage: </b> <br>
```
HashTable<std::string, int> table = {{"A", 1}, {"B", 3}};
table.update({"C", 5});
table.update({"A", 0});
table.remove("A");

for (const std::string& key : table.get_keys()){
    std::cout << key << " : " << table.get(key) << '\n';
}

