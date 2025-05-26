#include <iostream>
#include <vector>
#include <print>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {

    if (list1 == nullptr) {
        return list2;
    }

    if (list2 == nullptr) {
        return list1;
    }

    if (list1->val <= list2->val) {
        list1->next = mergeTwoLists(list1->next, list2);
        return list1;
    } else {
        list2->next = mergeTwoLists(list1, list2->next);
        return list2;
    }


    /*
    while(list1) {
        if (list1->val < list2->val) {
            std::cout << list1->val << std::endl;
            std::cout << list2->val << std::endl;
            list1 = list1->next;
            result.push_back(list1->val);
        
        } else {
            std::cout << list1->val << std::endl;
            std::cout << list2->val << std::endl;
            list2 = list2->next;
            result.push_back(list2->val);
        }    
    */
    //}

    //std::print("{}", result);

    //return result1;
    
}

void printListNode(ListNode* root) {
    
    if (root) {
        std::cout << root->val << " ";
    } else {
        return;
    }

    printListNode(root->next);

}

int main() {
    ListNode* list1 = new ListNode(1, new ListNode(2, new ListNode(4)));
    ListNode* list2 = new ListNode(1, new ListNode(3, new ListNode(4)));


    printListNode(list1);
    std::cout << std::endl;
    printListNode(list2);
    ListNode* res = mergeTwoLists(list1, list2);

    printListNode(res);

}
