#include <iostream>;
#include <random>;

struct node {
	double key, sum;
	double priority;
	node* l, *r;
	node() { }
	node(double key, double priority) : key(key), priority(priority), l(NULL), r(NULL), sum(NULL) { }
};
typedef node* tnode;

void updateSum(tnode& t) {
	if (t->l == NULL && t->r == NULL) {
		t->sum = t->key;
	}
	else if (t->l == NULL)
	{
		t->sum = t->key + t->r->sum;
	}
	else if (t->l == NULL)
	{
		t->sum = t->key + t->r->sum;
	}
	else if (t->r == NULL) {
		t->sum = t->key + t->l->sum;
	}
	else
	{
		t->sum = t->key + t->l->sum + t->r->sum;
	}

}

void split(tnode t, int key, tnode& l, tnode& r) {
	if (!t)
		l = r = NULL;
	else if (key < t->key)
		split(t->l, key, l, t->l), r = t;
	else
		split(t->r, key, t->r, r), l = t;
		
}

void insert(tnode& t, tnode it) {
	if (!t)
		t = it;
	else if (it->priority > t->priority)
		split(t, it->key, it->l, it->r), t = it;
	else
		insert(it->key < t->key ? t->l : t->r, it);
	updateSum(t);
}

void merge(tnode& t, tnode l, tnode r) {
	if (!l || !r)
		t = l ? l : r;
	else if (l->priority > r->priority)
		merge(l->r, l->r, r), t = l;
	else
		merge(r->l, l, r->l), t = r;
	updateSum(t);
}

void erase(tnode& t, int key) {
	if (t->key == key)
		merge(t, t->l, t->r);
	else
		erase(key < t->key ? t->l : t->r, key);
	updateSum(t);
}

tnode unite(tnode l, tnode r) {
	if (!l || !r)  return l ? l : r;
	if (l->priority < r->priority)  std::swap(l, r);
	tnode lt, rt;
	split(r, l->key, lt, rt);
	l->l = unite(l->l, lt);
	l->r = unite(l->r, rt);
	updateSum(l);
	return l;
}

int main()
{
	double currKey;
	double currPrior;
	int a = 1000;
	currKey = rand() % a;
	currKey /= a;
	currPrior = rand() % 100;
	currPrior /= 100;
	node* tree = new node(currKey, currPrior);
	for (int i = 0; i < a; i++) {
		currKey = rand() % a;
		currKey /= a;
		currPrior = rand() % (i+100);
		currPrior /= (i + 100);
		node* curr = new node(currKey, currPrior);
		insert(tree, curr);
	}
	//node* ae = new node(100, 1);
	//node* b = new node(150, 2);
	//node* c = new node(125, 3);
	//node* d = new node(75, 0.1);
	//node* e = new node(200, 2.5);
	//node* t = unite(ae, b);
	//insert(t, c);
	//insert(t, d);
	//insert(t, e);
	std::cout << tree->key << std::endl;
	std::cout << tree->l->key << std::endl;
	std::cout << tree->l->l->key << std::endl;
	std::cout << tree->r->key << std::endl;
	std::cout << tree->sum << std::endl;
}

