/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    void visitList(ListNode* root, std::vector<int>& vec) {
        if (!root) return;
        vec.push_back(root->val);
        visitList(root->next, vec); 
    }

    ListNode* mergeKLists(std::vector<ListNode*>& lists) {
        std::vector<int> res;
        
        if (lists.empty()) {
            return nullptr;
        }

        for (auto list : lists) {
            visitList(list, res);
        }

        if (res.empty()) {
            return nullptr;
        }

        std::sort(res.begin(), res.end());

        ListNode* result = new ListNode(res[0]);
        ListNode* current = result;

        for (size_t i = 1; i < res.size(); ++i) {
            current->next = new ListNode(res[i]);
            current = current->next;
        }

        return result;
    }
};
