#include <iostream>
#include <vector>



template<int ORDER>
struct Node {
    double key = 0;
    double priority = 0;
    double* SUMS = new double[ORDER + 1];
    int rightIndex = -1;
    int leftIndex = -1;

    Node() = default;
    ~Node() = default;
    explicit Node(double key, int priority) : key(key), priority(priority) { };
    explicit Node(double key, double priority, int leftIndex, int rightIndex) : key(key), priority(priority), leftIndex(leftIndex), rightIndex(rightIndex) {
        SUMS[0] = 1.0;
        for (int iter_ = 1; iter_ <= ORDER; iter_++) {
            SUMS[iter_] = SUMS[iter_ - 1] * key;
        }
    }
};

template<int ORDER>
class Tree {
public:
    Tree() = default;
    ~Tree() = default;
    int size = 0;
    int root = -1;
    int value_generator = 0;
    std::vector<Node<ORDER>> tree;

    Tree& operator =(const Tree& Tree_from) {
        if (Tree_from != this) {
            root = Tree_from.root;
            size = Tree_from.size;
            tree = Tree_from.tree;
            value_generator = Tree_from.value_generator;
            return *this;
        }
    }

    void updateSums(int currIndex) {
        if (currIndex != -1) {
            for (int iter_ = 0; iter_ <= ORDER; iter_++) {
                double rightSum = tree[currIndex].rightIndex != -1 ? tree[tree[currIndex].rightIndex].SUMS[iter_] : 0;
                double leftSum = tree[currIndex].leftIndex != -1 ? tree[tree[currIndex].leftIndex].SUMS[iter_] : 0;
                tree[currIndex].SUMS[iter_] = (leftSum + rightSum + tree[currIndex].SUMS[iter_]);

            }
            std::cout << "Updated to ";
            for (int i = 0; i <= ORDER; i++) {
                std::cout << tree[currIndex].SUMS[i] << " ";
            }
            std::cout << std::endl;

            std::cout << "Current root sums ";
            for (int i = 0; i <= ORDER; i++) {
                std::cout << tree[root].SUMS[i] << " ";
            }
            std::cout << std::endl;
        }

    }


    void split(int& currNodeIdx, double key, int& leftIndex, int& rightIndex) {
        if (currNodeIdx == -1) {
            leftIndex = rightIndex = -1;
        }
        else if (tree[currNodeIdx].key <= key) {
            split(tree[currNodeIdx].rightIndex, key, tree[currNodeIdx].rightIndex, rightIndex);
            leftIndex = currNodeIdx;
        }
        else {
            split(tree[currNodeIdx].leftIndex, key, leftIndex, tree[currNodeIdx].leftIndex);
            rightIndex = currNodeIdx;
        }
        //std::cout << "/////////UPDATE IN SPLIT///////////" << std::endl;
        updateSums(currNodeIdx);
        //std::cout << "/////////UPDATE IN SPLIT///////////" << std::endl;
    }


    void merge(int& currNodeIndex, int& left, int& right) {
        if (left == -1 || right == -1) {
            currNodeIndex = left != -1 ? left : right;
        }
        else if (tree[left].key < tree[right].key) {
            merge(tree[left].rightIndex, tree[left].rightIndex, right);
            currNodeIndex = left;
        }
        else {
            merge(tree[right].leftIndex, left, tree[right].leftIndex);
            currNodeIndex = right;
        }
        updateSums(currNodeIndex);
    }


    void insert(double key) {
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "start insert key = " << key << std::endl;
        double value = ++value_generator;
        ++size;

        if (root == -1) {
            Node<ORDER> newRoot = Node<ORDER>(key, value, -1, -1);
            tree.push_back(newRoot);
            root = 0;
            std::cout << "empty tree inserted key " << key << std::endl;
            std::cout << "end insert" << std::endl;
            std::cout << "-------------------------------------------" << std::endl;
            return;
        }

        int nodeIndex = root;
        int parentIndex = -1;

        while (nodeIndex != -1) {
            parentIndex = nodeIndex;
            if (key < tree[nodeIndex].key) {
                nodeIndex = tree[nodeIndex].leftIndex;
            }
            else {
                nodeIndex = tree[nodeIndex].rightIndex;
            }
        }

        int leftTreeIndex = -1;
        int rightTreeIndex = -1;

        split(nodeIndex, key, leftTreeIndex, rightTreeIndex);

        Node<ORDER> newNode = Node<ORDER>(key, value, -1, -1);
        tree.push_back(newNode);
        int newNodeIndex = tree.size() - 1;

        std::cout << "Current root sums ";
        for (int i = 0; i <= ORDER; i++) {
            std::cout << tree[root].SUMS[i] << " ";
        }
        std::cout << std::endl;
        if (parentIndex == -1) {
            root = newNodeIndex;
            std::cout << "inserted key " << key << " first if " << std::endl;
            updateSums(root);
            std::cout << "if update done" << std::endl;
        }

        else if (key < tree[parentIndex].key) {
            tree[parentIndex].leftIndex = newNodeIndex;
            std::cout << "inserted key " << key << " second if " << std::endl;
            updateSums(parentIndex);
            //updateSums(newNodeIndex);
            std::cout << "if update done" << std::endl;
        }
        else {
            tree[parentIndex].rightIndex = newNodeIndex;
            std::cout << "inserted key " << key << " third if " << std::endl;
            //updateSums(parentIndex);
            std::cout << "if update done" << std::endl;
        }

        tree[newNodeIndex].leftIndex = leftTreeIndex;
        tree[newNodeIndex].rightIndex = rightTreeIndex;
        std::cout << "/////////////////////////////UPDATES//////////////////" << std::endl;
        std::cout << "///////////newNode update insert ///////////////////////" << std::endl;
        //updateSums(newNodeIndex);
        std::cout << "//////////parent update insert //////////////////////" << std::endl;
        std::cout << "parent is " << tree[parentIndex].key << std::endl;
        //updateSums(parentIndex);
        updateSums(root);
        std::cout << "///////////////////////////////////////////////////////" << std::endl;
        std::cout << "Current root sums ";
        for (int i = 0; i <= ORDER; i++) {
            std::cout << tree[root].SUMS[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "end insert" << std::endl;
        std::cout << "-------------------------------------------" << std::endl;
    }

    Tree(const Tree& tree_from) {
        root = tree_from.root;
        size = tree_from.size;
        tree = tree_from.tree;
        value_generator = tree_from.value_generator;
    }

};

int main()
{
    Tree<2> test = Tree<2>();
    test.insert(2);
    test.insert(1);
    test.insert(4);
    test.insert(3);
    test.insert(-1);
    std::cout << test.tree[test.root].key << std::endl;
    std::cout << test.tree[test.tree[test.root].leftIndex].key << std::endl;
    std::cout << test.tree[test.tree[test.root].rightIndex].key << std::endl;
    std::cout << test.tree[test.tree[test.tree[test.root].rightIndex].leftIndex].key << std::endl;
    std::cout << "total points " << test.tree[test.root].SUMS[0] << std::endl;
    std::cout << "sum of keys " << test.tree[test.root].SUMS[1] << std::endl;
    std::cout << "sum of squares of keys " << test.tree[test.root].SUMS[2] << std::endl;
    std::cout << test.size << std::endl;
}

