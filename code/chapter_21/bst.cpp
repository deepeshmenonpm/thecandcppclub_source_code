#include <iostream>
#include <vector>

using namespace std;

class bst
{
private:
    struct node
    {
        int key;
        int data; // optional. Not used in this example.
        node* left_node;
        node* right_node;
        ~node()
        {
            cout << "node is deleted" << endl;
        }
    };
    
    node *root;
    
    node* create_new_node(node** this_node, int key)
    {
        if(*this_node == nullptr)
        {
            // found a free node 
            *this_node = new node;
            (*this_node)->left_node = nullptr;
            (*this_node)->right_node = nullptr;
            
            return *this_node;
        }
        
        // start traversing 
        if(key == (*this_node)->key)
        {
            return nullptr; // cannot create duplicate key 
        }
        else if(key < (*this_node)->key)
        {
            // traverse left 
            return create_new_node(&((*this_node)->left_node), key);
        }
        else if(key > (*this_node)->key)
        {
            // traverse right 
            return create_new_node(&((*this_node)->right_node), key);
        }
        
    }
    
    void print_keys(node* this_node)
    {
        if(this_node != nullptr)
        {
            print_keys(this_node->left_node);    // start traversing to the left to print 
                                                 // in ascending order
            cout << "key : " << this_node->key << endl; // print this node key 
            print_keys(this_node->right_node);  // now, traverse in the right to print
                                                // the higher keys in order 
        }
        
        return;
    }
    
    node* find_min_value_node(node *this_node)
    {
        node *temp_node = this_node;
        
        if( this_node->left_node != nullptr )
        {
            temp_node = find_min_value_node(this_node->left_node);
        }
        
        return temp_node;
    }
    
    void delete_this_key(node **this_node, int key, int& status)
    {
        // step 1 : find the node
        if(*this_node == nullptr)
        {
            // reached a null-node 
            cout << "Key not found. Cannot be deleted" << endl;
            status = -1;
            return;
        }
        
        if(key == (*this_node)->key)
        {
            // found our node 
            status = 0;
            // now, check the children
            // case 1 - There are no children. Then simply delete this node 
            if(    ( (*this_node)->left_node == nullptr ) 
                && ( (*this_node)->right_node == nullptr ) )
            {
                delete *this_node;
                *this_node = nullptr;
            }
            // case 2 - this_node has one right child 
            else if( (*this_node)->left_node == nullptr )
            {
                // copy the right node byte by byte to this node 
                // and delete the right node 
                // i.e. move the right node to this node 
                node *temp = (*this_node)->right_node;
                (*this_node)->key = (*this_node)->right_node->key;
                (*this_node)->left_node = (*this_node)->right_node->left_node;
                (*this_node)->right_node = (*this_node)->right_node->right_node;                
                // Now, delete the original right node, as it is moved to this node
                delete temp;           
            }
            // case 3 - this_node has one left child 
            else if( (*this_node)->right_node == nullptr )
            {
                // copy the left node byte by byte to this node 
                // and delete the left_node node 
                // i.e. move the left_node node to this node 
                node *temp = (*this_node)->left_node;
                (*this_node)->key = (*this_node)->left_node->key;
                (*this_node)->left_node = (*this_node)->left_node->left_node;
                (*this_node)->right_node = (*this_node)->left_node->right_node;                
                // Now, delete the original left node, as it is moved to this node
                delete temp;                     
            }
            // case 4 - this node has two children both left and right 
            else 
            {
                node *temp = find_min_value_node( (*this_node)->right_node );
                // found a node with minimum value and with no left nodes 
                // copy the key of minimum node to this node and delete that temp node
                (*this_node)->key = temp->key;
                // clean the right node link properly
                // check whether the temp node is actually this_node's right node
                // in that case this_node's right_node should be set to nullptr
                if((*this_node)->key == (*this_node)->right_node->key)
                {
                    // check if this temp node is a leaf node
                    if(   (temp->left_node == nullptr )
                        &&(temp->right_node == nullptr) )
                    {
                        (*this_node)->right_node = nullptr;
                    }
                }
                
                int temp_status = 0;
                delete_this_key(&temp, temp->key, temp_status);
                
            }
        }
        else if(key < (*this_node)->key)
        {
            // traverse left 
            delete_this_key(&((*this_node)->left_node), key, status);
        }
        else if(key > (*this_node)->key)
        {
            // traverse right 
            delete_this_key(&((*this_node)->right_node), key, status);
        }
        
        return;
    }
    
    void get_all_nodes(vector<node*>& nodes, node* this_node)
    {
        if(this_node != nullptr)
        {
            get_all_nodes(nodes, this_node->left_node);    // start traversing to the left to print 
                                                           // in ascending order
            nodes.push_back(this_node);
            get_all_nodes(nodes, this_node->right_node);  // now, traverse in the right to print
                                                          // the higher keys in order 
        }
        
        return;
    }
    
public:
    bst():root{nullptr}{}
    ~bst()
    {
        // get the nodes one by one in to a vector and delete them 
        vector<node*> nodes;
        get_all_nodes(nodes, root);
        
        for (auto this_node : nodes)
        {
            delete this_node;
        }
        
        cout << "\nbst is destroyed" << endl;
    }
    
    int insert(int key)
    {
        int status = -1;
        node* new_node = create_new_node(&root, key);
        
        if(new_node != nullptr)
        {
            // a new node is created 
            new_node->key = key;
            status = 0;
        }
        
        return status;
    }
    
    void print()
    {
        cout << "\nprinting keys..." << endl;
        print_keys(root);
        cout << "printing complete\n" << endl;
    }
    
    int delete_key(int key)
    {    
        int status = 0;
        delete_this_key(&root, key, status);
        return status;
    }
    
};


int main()
{
    bst bst_1;
    // '1', '5' and '2' are duplicates 
    int unsorted_array[] = {5, 2, 7, 9, 10, 1, 6, 12, 3, 1, 5, 2}; 
    int status = 0;
    for (auto val : unsorted_array)
    {
        status = bst_1.insert(val);
        if(status == 0)
        {
            cout << "val : " << val << " inserted" << endl;
        }
        else
        {
            cout << "val : " << val << " not inserted" << endl;
        }
    }
    
    bst_1.print();
    
    // delete 1
    status = bst_1.delete_key(1);
    if(status == 0)
    {
        cout << "Val : 1 deleted" << endl; 
    }
    else
    {
        cout << "Val : 1 not deleted" << endl;
    }
    
    bst_1.print();
    
    // delete 9
    status = bst_1.delete_key(9);
    if(status == 0)
    {
        cout << "Val : 9 deleted" << endl; 
    }
    else
    {
        cout << "Val : 9 not deleted" << endl;
    }
    
    bst_1.print();
    
    // delete 7
    status = bst_1.delete_key(7);
    if(status == 0)
    {
        cout << "Val : 7 deleted" << endl; 
    }
    else
    {
        cout << "Val : 7 not deleted" << endl;
    }
    
    bst_1.print();
    
    // delete 10
    status = bst_1.delete_key(10);
    if(status == 0)
    {
        cout << "Val : 10 deleted" << endl; 
    }
    else
    {
        cout << "Val : 10 not deleted" << endl;
    }
    
    bst_1.print();
    
    // try to delete 1 again 
    status = bst_1.delete_key(1);
    if(status == 0)
    {
        cout << "Val : 1 deleted" << endl; 
    }
    else
    {
        cout << "Val : 1 not deleted" << endl;
    }
    
    bst_1.print();
    
    // try to delete 7 again 
    status = bst_1.delete_key(7);
    if(status == 0)
    {
        cout << "Val : 7 deleted" << endl; 
    }
    else
    {
        cout << "Val : 7 not deleted" << endl;
    }
    
    bst_1.print();
    
    cout << "Main ends" << endl;
    
    return 0;
}


