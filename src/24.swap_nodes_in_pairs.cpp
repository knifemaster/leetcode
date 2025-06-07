#include <iostream>
#include <vector>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

void printListNode(ListNode* root) {
    if (root) {
        std::cout << root->val << " ";
        printListNode(root->next);
    } else {
        std::cout << std::endl;
    }
}

void visitListNode(ListNode* fast, ListNode* slow) {
    if (fast == nullptr || slow == nullptr) {
        return;
    }

    int temp = fast->val;
    fast->val = slow->val;
    slow->val = temp;

    if (fast->next && fast->next->next) {
        visitListNode(fast->next->next, fast->next);
    }
}

int main() {
    ListNode *root = new ListNode(1, new ListNode(2, new ListNode(3, new ListNode(4))));
    std::cout << "Before: ";
    printListNode(root);

    visitListNode(root->next, root);

    std::cout << "After: ";
    printListNode(root);
   return 0;
}
