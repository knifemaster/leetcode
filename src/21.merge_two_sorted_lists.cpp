#include <iostream>
#include <vector>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    ListNode* result = new ListNode();
    //if (!list1 || !list2) {
        
    //}
    //vector<int> result_vec;
    result_vec.push_back();

    while(list1 || list2) {
        if (list1->val < list2->val) {
            list1 = list1->next;
        //    result = new ListNode(list1->val,)
        } else {
            list2 = list2->next;
        }    
    }

    return result;
    
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
    mergeTwoLists(list1, list2);

}
