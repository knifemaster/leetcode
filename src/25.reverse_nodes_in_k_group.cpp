#include <vector>
#include <stack>
#include <iostream>


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

void printListNode(ListNode* root) {

    if (root) {
        std::cout << root->val << std::endl;
    } else {
        return;
    }

    printListNode(root->next);

}

ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode* fast = head;

    int count = 0;

    if (fast == nullptr) {
        return nullptr;
    }

    while(fast != nullptr && count < k) {
        std::cout << fast->val;
        fast = fast->next;
        count++;
    }

    std::cout << std::endl;
    
    
    return reverseKGroup(fast, k);
        

    //} 

    //std::cout << std::endl;
    //std::cout << fast->val << std::endl;
    //std::cout << std::endl;

    return fast;
}

int main() {
    ListNode* root = new ListNode(1, new ListNode(2, new ListNode(3, new ListNode(4, new ListNode(5)))));

    reverseKGroup(root, 2);
    std::cout << std::endl;
    printListNode(root);


    return 0;

}
