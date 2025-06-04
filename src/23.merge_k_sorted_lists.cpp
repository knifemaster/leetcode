#include <iostream>
#include <vector>
#include <stack>
#include <priority_queue>


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}

};


ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    ListNode* list = new ListNode(1);


    return list

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
    ListNode* list1 = new ListNode(1, new ListNode(4, new ListNode(5)));
    ListNode* list2 = new ListNode(1, new ListNode(3, new ListNode(4)));
    ListNode* list3 = new ListNode(2, new ListNode(6));

    std::vector<ListNode*> vec_lists;
    vec_lists.push_back(list1);
    vec_lists.push_back(list2);
    vec_lists.push_back(list3);

    mergeKlists(vec_lists);



    return 0;
}

