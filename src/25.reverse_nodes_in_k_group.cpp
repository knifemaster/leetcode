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
    ListNode* next_head = nullptr;
    int count = 0;

    if (fast == nullptr) {
        return nullptr;
    }

    if (head == nullptr || k == 1) {
        return head;
    }

    while(fast != nullptr && count < k) {
        std::cout << fast->val;
        fast = fast->next;
        count++;
    }

    if (count == k) {
        std::cout << "count equals k" << std::endl;
        next_head = reverseKGroup(fast, k);
        fast = head;
        ListNode* prev = nullptr;
        ListNode* temp_next = nullptr;
        count = 0;

        while (count < k) {
            temp_next = fast->next;
            fast->next = prev;
            prev = fast;
            fast = temp_next;
            count++;
        }

        head->next = next_head;
        return prev;
    } else {
        return head;
    }

    std::cout << std::endl;
    
    
    //return reverseKGroup(fast, k);
        

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
