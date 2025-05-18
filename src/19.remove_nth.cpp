#include <iostream>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* findNthFromEnd(ListNode* head, int n) {
    if (!head || n <= 0) return nullptr;

    ListNode *fast = head, *slow = head;

    for (int i = 0; i < n; ++i) {
        if (!fast) return nullptr;  
        fast = fast->next;
    }

    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }

    return slow; 
}

int main() {
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);
    head->next->next->next->next = new ListNode(5);

    int n = 3; 
    ListNode* result = findNthFromEnd(head, n);

    if (result) {
        std::cout << n << "-й элемент с конца: " << result->val << std::endl;  // 3
    } else {
        std::cout << "Нет такого элемента!" << std::endl;
    }

    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}
