#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <print>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}

};


void printListNode(ListNode* root) {
    
    if (root) {
        std::cout << root->val << " ";
    } else {
        return;
    }

    printListNode(root->next);

}


std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
i
void visitList(ListNode* root) {
    if (root) {
        heap.push(root->val);
    } else {
        return;
    }

    visitList(root->next);

}

ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    ListNode* result = new ListNode(1);

    std::vector<int> values;
    values.reserve(lists.size());
    
    //std::priority_queue<int> heap;
    
    for (auto list : lists) {
        //printListNode(list);
        visitList(list);
    }
        std::cout << std::endl;



    return result;

}



int main() {
    ListNode* list1 = new ListNode(1, new ListNode(4, new ListNode(5)));
    ListNode* list2 = new ListNode(1, new ListNode(3, new ListNode(4)));
    ListNode* list3 = new ListNode(2, new ListNode(6));

    std::vector<ListNode*> vec_lists;
    vec_lists.push_back(list1);
    vec_lists.push_back(list2);
    vec_lists.push_back(list3);

    mergeKLists(vec_lists);

    ListNode* res;

    //std::print("{}", heap);
    std::cout << std::endl;
    while (!heap.empty()) {
        std::cout << heap.top() << " ";
       // res->next = new ListNode(heap.top());
        heap.pop();
    }
    return 0;
}

