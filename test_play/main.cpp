#include"fiveChessGame_server.h"
#include"../thread_pool/thread_pool.h"
#include"../thread_pool/list_node.h"
#include <unistd.h>
#include <vector>

using namespace std;

void func(int index)
{
	std::cout<<"Task: "<<index<<std::endl;
}

void printOther(const int& other)
{
	std::cout<<"other: "<<other<<std::endl;
}

void printIndex(const int& index)
{
	std::cout<<"index: "<<index<<std::endl;
	printOther(index);
}

int main() 
{
	// //唯一型智能指针
	// std::unique_ptr<ListNode<int>> unique_node(new ListNode<int>());
	// //std::unique_ptr<ListNode<int>> unique_node_copy = unique_node;	//无法拷贝
	// //转移所有权
	// std::unique_ptr<ListNode<int>> unique_node_copy = std::move(unique_node);
	// unique_node_copy->value = 100;
	// //测试是否拥有对象的所有权
	// std::cout<<"unique_node_copy->value: "<<(*unique_node_copy).value<<std::endl;

	// //共享型指针
	// //std::shared_ptr<ListNode<int>> list_node1(new ListNode<int>()); 
	// std::shared_ptr<ListNode<int>> list_node1
	// = std::make_shared<ListNode<int>>();

	// std::shared_ptr<ListNode<int>> list_node2 
	// = std::make_shared<ListNode<int>>();

	// //shared_ptr可以直接拷贝到weak_ptr
	// list_node1->next_node = list_node2;
	// list_node2->pre_node = list_node1;
	// list_node1->value = 1;
	// list_node2->value = 2;

	// //打印向后指针的引用计数
	// std::cout<<"use_count of list_node1->next_node: "
	// <<list_node1->getNextUseCount()<<std::endl;

	// //打印值
	// std::cout<<"list_node2 vaule : "
	// <<list_node1->next_node.lock()->value<<"\n"
	// <<"list_node1 vaule : "<< list_node2->pre_node.lock()->value<<std::endl;

	fiveChessGame *gobang_game = new fiveChessGame();
	gobang_game->play();
	delete gobang_game;
	gobang_game = NULL;


	return 0;
}