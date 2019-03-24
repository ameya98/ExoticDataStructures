/*
    A B+ Tree Implementation

    Author: Ameya Daigavane
*/

#include <vector>
#include <iostream>
#include <cassert>
#include <deque>
#include <utility>

using namespace std;

template <typename T>
class TreeNode {
    deque<T> keys;
    size_t max_size;
    bool is_child;
};

template <typename T>
class DataNode : public TreeNode<T>{
    deque<int> data;

    public:
    DataNode<T> (size_t DataNodeSize){
        this -> max_size = DataNodeSize;
        this -> is_child = true;
    }
};

template <typename T>
class IndexNode : public TreeNode<T>{
    deque<TreeNode<T> *> children;

    public:
    IndexNode<T> (size_t IndexNodeSize){
        this -> max_size = IndexNodeSize;
        this -> is_child = false;
    }
};

template <typename T>
class BPlusTree {

    IndexNode<T> * root;
    deque<DataNode<T> *> leaves;
    size_t IndexNodeSize;
    size_t DataNodeSize;

    public:
    BPlusTree(size_t IndexNodeSize, size_t DataNodeSize){
        this -> root = new IndexNode<T>(IndexNodeSize);
        this -> IndexNodeSize = IndexNodeSize;
        this -> DataNodeSize = DataNodeSize;
    }

    void insert_helper(T key, TreeNode<T> * curr){
        deque<T>& keys = curr -> keys;
        deque<TreeNode<T> *>& children = curr -> children;

        if(key < keys.front()){
            insert_helper(key, children.front());
        } else if (key > keys.back()){
            insert_helper(key, children.back());
        } else {
            for(size_t i = 0; i < keys.size(); ++i){
                if(keys[i] > key){
                    insert_helper(key, children[i]);
                }
            }
        }
    }

    void insert(T key){
        insert_helper(key, root);
    }

    TreeNode<T> * search(T key){

    }

    bool count(T key){
        return (search(key) != nullptr);
    }

    ~BPlusTree(){
        delete_mem(root);
    }

    /* Delete memory allocated recursively. */
    delete_mem(TreeNode<T> * curr){
        deque<TreeNode<T> *>& children = curr -> children;

        for(size_t i = 0; i < children.size(); ++i){
            delete_mem(children[i]);
        }

        delete(curr);
    }
};

int main(){

}
