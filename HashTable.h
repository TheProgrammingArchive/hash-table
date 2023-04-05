#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <iostream>
#include <list>
#include <vector>

template <typename T>
int hashObject(const T& object){
    std::hash<T> hash_;
    return std::abs(static_cast<int>(hash_(object)));
}

template <typename T, typename E>
class Entry{
    public:
    T key;
    E value;
    int hash_code;

    Entry(const T& key, const E& value){
        this->key = key;
        this->value = value;
        this->hash_code = hashObject(key);
    }

    bool operator==(const Entry& e){
        if (this->hash_code == e.hash_code){
            return true;
        }
        return false;
    }
};

template <typename T, typename E>
class HashTable{
    private:
    int table_size, curr_kv_pairs, threshold = 0;
    float load_factor = 0;
    std::list<Entry<T, E>>** hash_table = nullptr;
    std::vector<T> keys = {};

    int hash_index(const T& object){
        return hashObject(object) % table_size;
    }

    E& handle_not_found(E* default_, const std::string& err_message){
        if (default_ == nullptr){
            throw std::out_of_range(err_message);
        }
        else{
            return *default_;
        }
    }

    void resize_table(){
        std::list<Entry<T, E>>** hash_table_new = new std::list<Entry<T, E>>*[table_size * 2]{nullptr};
        this->threshold = static_cast<int>(this->table_size * this->threshold * 2);

        for (int i = 0; i < table_size; i++){
            if (hash_table[i] != nullptr){
                if (hash_table[i]->size() != 0){
                    for (Entry<T, E> entry : *hash_table[i]){
                        int index_new = hashObject(entry.key) % (table_size * 2);
                        if (hash_table_new[index_new] == nullptr){
                            hash_table_new[index_new] = new std::list<Entry<T, E>>;
                        }
                        hash_table_new[index_new]->push_back(entry);
                    }
                }
            }
        }

        for (int i = 0; i < table_size; i++){
            if (hash_table[i] != nullptr){
                hash_table[i]->clear();
                hash_table[i] = nullptr;
            }
        }
        hash_table = nullptr;

        hash_table = hash_table_new;
        this->table_size = table_size * 2;
        this->threshold = static_cast<int>(table_size * load_factor);
    }

    public:
    HashTable(int table_size = 16, float load_factor = 0.75){
        this->table_size = table_size;
        this->load_factor = load_factor;
        this->threshold = static_cast<int>(table_size * load_factor);
        this->hash_table = new std::list<Entry<T, E>>*[table_size]{nullptr};
    }

    HashTable(const std::initializer_list<Entry<T, E>>& list) : HashTable(){
        for (const Entry<T, E>& entry : list){
            update(entry);
        }
    }

    ~HashTable(){
        for (int i = 0; i < table_size; i++){
            if (hash_table[i] != nullptr){
                hash_table[i]->clear();
                hash_table[i] = nullptr;
            }
        }
        delete[] hash_table;
    }    

    const std::vector<T>& get_keys(){
        return keys;
    }

    int size(){
        return curr_kv_pairs;
    }


    const E& get(const T& key, E* default_ = nullptr){
        int index = hash_index(key);
        std::list<Entry<T, E>>* list_ = hash_table[index];

        if (list_ == nullptr){
            return handle_not_found(default_, "");
        }

        for (Entry<T, E>& entry : *list_){
            if (entry.key == key){
                return entry.value;
            } 
        }
        
        return handle_not_found(default_, "KeyError");
    }

    void update(const Entry<T, E>& entry){
        int index = hash_index(entry.key);
        if (hash_table[index] == nullptr){
            hash_table[index] = new std::list<Entry<T, E>>;
        }

        bool found = false;
        for (Entry<T, E>& entry_ : *hash_table[index]){
            if (entry_.key == entry.key){
                entry_.value = entry.value;
                found = true;
                break;
            }
        }
        
        if (!found){
            if (curr_kv_pairs == threshold){
                resize_table();
                int index = hash_index(entry.key);
                if (hash_table[index] == nullptr){
                    hash_table[index] = new std::list<Entry<T, E>>;
                }
            }
            
            hash_table[index]->push_back(entry);
            curr_kv_pairs += 1;
            keys.push_back(entry.key);
        }
    }

    E& remove(const T& key, E* default_ = nullptr){
        if (curr_kv_pairs == 0){
            return handle_not_found(default_, "Remove invoked on empty hash table");
        }

        int index = hash_index(key);
        if (hash_table[index] == nullptr){
            return handle_not_found(default_, "KeyError");
        }

        for (Entry<T, E>& entry : *hash_table[index]){
            if (entry.key == key){
                hash_table[index]->remove(entry);

                int pos = 0;
                for (int x = 0; x < keys.size(); x++){
                    if (keys[x] == key){
                        pos = x;
                        break;
                    }
                }
                keys.erase(keys.begin() + pos);

                if (hash_table[index]->size() == 0){
                    hash_table[index] = nullptr;
                }
                
                curr_kv_pairs = curr_kv_pairs - 1;
                return entry.value;
            }
        }
            
        return handle_not_found(default_, "Key Error");
    }

    Entry<T, E>& pop_item(){
        if (curr_kv_pairs == 0){
            throw std::out_of_range("Empty");
        }

        T rem_key = keys[keys.size() - 1];
        int table_index = hash_index(rem_key);

        E req = remove(rem_key);
        Entry<T, E> *entry = new Entry<T, E>{rem_key, req};

        return *entry;
    }
};

#endif // HASHTABLE_H