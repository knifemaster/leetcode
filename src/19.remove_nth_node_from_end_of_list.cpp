#include <iostream>
#include <print>
#include <vector>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode* removeNthFromEnd(ListNode* head, int n) {

}

void visitNode(ListNode* head, int count_nodes, int step, ListNode*& prev, ListNode*& temp, ListNode*& prev_delete) {
    if (head == nullptr) {
        return;
    }

    if (count_nodes % step == 0) {
        prev = head;//head->next;
        prev_delete = temp;        
        std::cout << "step" << count_nodes << std::endl;
        std::cout << "values" << head->val << std::endl;
    }

    temp = head;

    count_nodes++;
    //std::cout << head->val << std::endl;
    //std::cout << "node n: " << count_nodes << std::endl;
    visitNode(head->next, count_nodes, step, prev, temp, prev_delete);
}

void printAll(ListNode* root) {
    if (root == nullptr) {
        return;
    }
    std::cout << "deleted" << root->val << std::endl;
    printAll(root->next);
}

int main() {

    ListNode* root = new ListNode(1, new ListNode(2, new ListNode(3, new ListNode(4, new ListNode(5)))));
    ListNode* prev = new ListNode();
    ListNode* temp = new ListNode();
    ListNode* prev_delete = new ListNode();
    visitNode(root, 1, 3, prev, temp, prev_delete);

    std::cout << "last element" <<prev->val << std::endl;
    std::cout << "prev_delete" << prev_delete->val << std::endl;
    prev_delete->next = prev->next;

    printAll(root);

    return 0;
}
