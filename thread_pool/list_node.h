#ifndef LIST_NODE_H
#define LIST_NODE_H

#include <memory>


template<typename NodeType>
class ListNode
{

public:
    //std::share_ptr<ListNode<NodeType>>    pre_node;             //向前的指针
    //std::share_ptr<ListNode<NodeType>>    next_node;          //向后的指针
    std::weak_ptr<ListNode<NodeType>>    pre_node;   //向前的指针
    std::weak_ptr<ListNode<NodeType>>    next_node;  //向后的指针
    NodeType value;                                 //节点变量

public:
    ListNode()
    {
        
    }
    ~ListNode()
    {
        std::cout<<"Free list node"<<std::endl;
    }

    inline int getPreUseCount()
    {
        return pre_node.use_count();
    }

    inline int getNextUseCount()
    {
        return next_node.use_count();
    }
};

#endif