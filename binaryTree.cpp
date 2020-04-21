#include <iostream>
#include <vector>
#include <random>>
#include <string>

struct Node {
	int key;
	int left;
	int right;
	Node(int key, int right, int left) : key(key), right(right), left(left) {};
	Node(int key) : key(key), right(NULL), left(NULL) {};
	std::string toString() {
		return std::to_string(key);
	}
	
};

typedef Node * tNode;

class Tree {
public:
	tNode root;
	std::vector<tNode> Nodes;

	Tree() {};

	Tree(tNode root) {
		this->root = root;
		this->Nodes.push_back(root);
	}

	void push(tNode t, Tree r) {
		if (this->root == nullptr) {
			this->root = t;
			this->Nodes.push_back(t);
			this->root->left == NULL;
			this->root->right == NULL;
		}
		else {
			if (root->key < t->key) {
				this->Nodes.push_back(t);


			}
		}

	}

	void coutTree() {
		for (int i = 0; i < Nodes.size()-1; i++) {
			std::cout << Nodes[i]->toString() << " -> " << Nodes[Nodes[i]->left]->toString() << ";" << std::endl;
			std::cout << Nodes[i]->toString() << " -> " << Nodes[Nodes[i]->right]->toString() << ";" << std::endl;
		}
	}

};
int main()
{
	//Node* start = new Node(50);
	Tree t;
	//std::cout << t.root->toString() << std::endl;
	for (int j = 0; j < 10; j++) {
		Node* a = new Node(rand() % 100);
		t.push(t.root, a);
	}

	for (int i = 0; i < t.Nodes.size()-1; i++) {
		std::cout << t.Nodes[i]->toString() << std::endl;
	}
	std::cout << t.Nodes.size() << std::endl;
	std::cout << t.root->toString() << std::endl;
	t.coutTree();
	
	int i;
	std::cin >> i;

}


