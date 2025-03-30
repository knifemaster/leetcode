class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        if (nums1.size() > nums2.size()) {
            return findMedianSortedArrays(nums2, nums1); // Работаем с меньшим массивом
        }

        int m = nums1.size();
        int n = nums2.size();
        int left = 0, right = m;
        int half_len = (m + n + 1) / 2; // Округление вверх

        while (left <= right) {
            int i = (left + right) / 2; // Разбиение nums1
            int j = half_len - i;       // Соответствующее разбиение nums2

            if (i < m && nums2[j - 1] > nums1[i]) {
                left = i + 1; // Увеличиваем i (nums1[i] слишком мал)
            } else if (i > 0 && nums1[i - 1] > nums2[j]) {
                right = i - 1; // Уменьшаем i (nums1[i-1] слишком велик)
            } else {
                // Нашли правильное разбиение
                int max_left;
                if (i == 0) {
                    max_left = nums2[j - 1];
                } else if (j == 0) {
                    max_left = nums1[i - 1];
                } else {
                    max_left = max(nums1[i - 1], nums2[j - 1]);
                }

                if ((m + n) % 2 == 1) {
                    return max_left; // Нечетная длина — медиана в left_part
                }

                int min_right;
                if (i == m) {
                    min_right = nums2[j];
                } else if (j == n) {
                    min_right = nums1[i];
                } else {
                    min_right = min(nums1[i], nums2[j]);
                }

                return (max_left + min_right) / 2.0; // Четная длина — среднее
            }
        }

        return 0.0; // Сюда не дойдем, если входные данные корректны
    }
};
