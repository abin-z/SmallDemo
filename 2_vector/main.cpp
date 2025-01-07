#include <iostream>
#include <vector>
#include <algorithm>

/// @brief 打印vector函数
/// @tparam T 
/// @param vec 
template <typename T>
void printVector(const std::vector<T>& vec) {
    for (const T& v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

int main() {
    // 构造一个空的 vector
    std::vector<int> vec1;
    std::cout << "vec1 size: " << vec1.size() << ", capacity: " << vec1.capacity() << std::endl;

    // 使用初始值填充 vector
    std::vector<int> vec2(5, 10); // 初始化5个元素，值为10
    std::cout << "vec2: ";
    printVector(vec2);

    // 使用数组初始化
    int arr[] = {1, 2, 3, 4, 5};
    std::vector<int> vec3(arr, arr + sizeof(arr) / sizeof(arr[0]));
    std::cout << "vec3: ";
    printVector(vec3);

    // 添加元素
    vec1.push_back(30);
    std::cout << "vec1 after push_back: ";
    printVector(vec1);

    // 删除最后一个元素
    vec1.pop_back();
    std::cout << "vec1 after pop_back: ";
    printVector(vec1);

    // 访问元素
    std::cout << "First element of vec2: " << vec2.front() << std::endl;
    std::cout << "Last element of vec3: " << vec3.back() << std::endl;

    // 使用下标访问
    vec2[2] = 50;
    std::cout << "vec2 after modifying index 2: ";
    printVector(vec2);

    // 迭代器访问
    std::cout << "vec3 elements using iterator: ";
    for (auto it = vec3.begin(); it != vec3.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 插入元素
    vec2.insert(vec2.begin() + 2, 99);  // 在索引 2 处插入 99
    std::cout << "vec2 after insert: ";
    printVector(vec2);

    // 删除元素
    vec2.erase(vec2.begin() + 1);  // 删除索引 1 的元素
    std::cout << "vec2 after erase: ";
    printVector(vec2);

    // 清空所有元素
    vec2.clear();
    std::cout << "vec2 after clear, size: " << vec2.size() << std::endl;

    // 检查是否为空
    if (vec2.empty()) {
        std::cout << "vec2 is empty." << std::endl;
    }

    // 重新填充 vec1
    vec1 = {1, 2, 3, 4, 5};
    std::cout << "vec1 before resizing: ";
    printVector(vec1);

    // 改变大小
    vec1.resize(8, 0);  // 扩展到8个元素，新增元素初始化为 0
    std::cout << "vec1 after resizing: ";
    printVector(vec1);

    // 交换两个 vector
    std::vector<int> vec4 = {10, 20, 30};
    std::cout << "vec4 before swap: ";
    printVector(vec4);

    vec1.swap(vec4);
    std::cout << "vec1 after swap: ";
    printVector(vec1);
    std::cout << "vec4 after swap: ";
    printVector(vec4);

    // 排序
    std::sort(vec1.begin(), vec1.end());
    std::cout << "vec1 after sort: ";
    printVector(vec1);

    return 0;
}
