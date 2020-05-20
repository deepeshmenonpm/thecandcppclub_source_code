#include <iostream>
#include <list>
#include <string>

using namespace std;

template <typename T, int N>
class Hash_Table
{
public:
    enum class error_codes{invalid_key_type = 0, invalid_key = 1};
private:
    list<T> h_tables[N];
    string hash_type;
    
    int hash_key(T val)
    {
        int key = -1;
        unsigned int h = 0U;
        unsigned int g = 0U;
        
        if(hash_type.compare("integer hash") == 0)
        {
            int *temp_val_ptr = reinterpret_cast<int *>(&val);
            int temp_val = *temp_val_ptr;
            if(temp_val < 0){temp_val *= -1;}
            key = temp_val % N;
        }
        else
        {
            // string hash uses PJW hash algorithm 
            // in UNIX ELF string hashing 
            string *temp = reinterpret_cast<string*>(&val);
            char *str = const_cast<char *>(temp->c_str());
            char *p;
            int int_eq = 0;
            
            for(p = str; *p; ++p)
            {
                int_eq = static_cast<int>(*p);
                h = (h << 4) + int_eq; 
                g = h & 0xF0000000U;
                
                if(g != 0x0U) // i.e. high nibble is set 
                {
                    h ^= (g >> 24);
                }
                
                h &= ~g;
            }
            
            key = static_cast<int>(h) % N;
        }
        
        return key;
        
    }
    
public:
    Hash_Table()
    {
        hash_type = "integer hash";
        constexpr bool integer_type = is_same<T, int>::value;
        constexpr bool string_type = is_same<T, string>::value;
        
        // check for key type. class invariance 
        if(   (integer_type == false) 
            &&(string_type == false) )
        {
            cout << "invalid key type" << endl;
            throw error_codes::invalid_key_type;
        }
        
        if(string_type == true)
        {
            hash_type = "string hash";
        }
        
        cout << hash_type << " has been created" << endl;
    }
    
    ~Hash_Table()
    {
        cout << hash_type << " has been destroyed" << endl;
    }
    
    int insert_value(T val)
    {
        int status = -1;
        int key = hash_key(val);
        
        if(key != -1)
        {
            // found a valid key
            h_tables[key].push_back(val);
            status = 0;
        }
        
        return status;
        
    }
    
    int delete_value(T val)
    {
        int status = -1;
        int key = hash_key(val);
        
        if(key != -1)
        {
            auto itr_start = h_tables[key].begin();
            auto itr_end = h_tables[key].end();
            
            for(auto itr = itr_start; itr != itr_end; ++itr)
            {
                if(*itr == val)
                {
                    // found the val
                    h_tables[key].erase(itr);
                    status = 0;
                    break;
                }
            }
        }
        
        return status;
    }
};

int main()
{
    int unsorted_array[] = {5, 2, 6, 7, 1, 10, 11, 4, 8};
    int status = 0;
    
    cout << "\nTesting integer hash..." << endl;
    Hash_Table<int, 5> ht_1;
    
    for (auto val : unsorted_array)
    {
        status = ht_1.insert_value(val);
        if(status == 0)
        {
            cout << "Val : " << val << " added into hash table" << endl;
        }
        else
        {
            cout << "Val : " << val << " not added into hash table" << endl;
        }
    }
    
    cout << "\nTesting deletion..." << endl;
    //delete 2
    if( 0 == ht_1.delete_value(2))
    {
        cout << "2 has been deleted from hash table" << endl;
    }
    else
    {
        cout << "2 not deleted from hash table" << endl;
    }
    
    //delete 10
    if( 0 == ht_1.delete_value(10))
    {
        cout << "10 has been deleted from hash table" << endl;
    }
    else
    {
        cout << "10 not deleted from hash table" << endl;
    }
    
    //delete 10 again
    if( 0 == ht_1.delete_value(10))
    {
        cout << "10 has been deleted from hash table" << endl;
    }
    else
    {
        cout << "10 not deleted from hash table" << endl;
    }
    
    // Now testing string hash
    cout << "\nTesting string hash..." << endl;
    Hash_Table<string, 5> ht_2;
    
    if(0 == ht_2.insert_value("Hello_String_1"))
    {
        cout << "Hello_String_1 added into Hash_Table" << endl;
    }
    else
    {
        cout << "Hello_String_1 is not added into Hash_Table" << endl;
    }
    
    if(0 ==    ht_2.insert_value("Hello_Another_String_1"))
    {
        cout << "Hello_Another_String_1 is added into Hash_Table" << endl;
    }
    else
    {
        cout << "Hello_Another_String_1 is deleted from Hash_Table" << endl;
    }
    
    cout << "\nTesting deletion..." << endl;
    
    // delete both strings 
    if(0 == ht_2.delete_value("Hello_String_1"))
    {
        cout << "Hello_String_1 is deleted from Hash_Table" << endl;
    }
    else
    {
        cout << "Hello_String_1 is not deleted from Hash_Table" << endl;
    }
    
    if(0 ==    ht_2.delete_value("Hello_Another_String_1"))
    {
        cout << "Hello_Another_String_1 is deleted from Hash_Table " << endl;
    }
    else
    {
        cout << "Hello_Another_String_1 is not deleted from Hash_Table" << endl;
    }
    
    cout << "\nMain ends" << endl;
    
    return 0;
}


