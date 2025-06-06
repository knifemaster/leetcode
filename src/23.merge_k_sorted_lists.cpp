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

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <print>

class Solution {
public:
    //std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
    void visitList(ListNode* root) {
        if (root) {
            heap.push(root->val);
        } else {
            return;
        }

        visitList(root->next);

    }

    ListNode* mergeKLists(std::vector<ListNode*>& lists) {

    
        if (lists.empty()) {
            return nullptr;
        }

        for (auto list : lists) {
            visitList(list);
        }

        ListNode* result;
        ListNode* current;
        
        if (!heap.empty()) {
            result = new ListNode(heap.top());
            current = result;
            heap.pop();
        } else {
            return nullptr;
        }

        while (!heap.empty()) {
                std::cout << heap.top() << " ";
                current->next = new ListNode(heap.top());
                current = current->next;
                heap.pop();
        }

        return result;
    }



};
