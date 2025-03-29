class Solution {
    public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
            int carry = 0;
            ListNode* temp = new ListNode();
            ListNode* current = temp;

            while (l1 != nullptr || l2 != nullptr || carry != 0) {
                int sum = carry;
                if (l1 != nullptr) {
                    sum += l1->val;
                    l1 = l1->next;
                }

                if (l2 != nullptr) {
                    sum += l2->val;
                    l2 = l2->next;
                }
                carry = sum / 10;
                current->next = new ListNode(sum % 10);
                current = current->next;

            }
            return temp->next;
        }
};


class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *head = l1; 
        ListNode *prev = nullptr;
        int carry = 0;

        while (l1 != nullptr || l2 != nullptr) {
            int sum = carry;

            if (l1 != nullptr) {
                sum += l1->val;
            }
            if (l2 != nullptr) {
                sum += l2->val;
                if (l1 == nullptr) {
                    prev->next = new ListNode(0);
                    l1 = prev->next;
                }
            }

            carry = sum / 10;
            l1->val = sum % 10;

            prev = l1;
            l1 = l1->next;
            if (l2 != nullptr) l2 = l2->next;
        }

        if (carry > 0) {
            prev->next = new ListNode(carry);
        }

        return head;
    }
};
