#include <iostream>;

struct node {
	int key, sum;
	double priority;
	node* l, * r;
	node() { }
	node(int key, int priority) : key(key), priority(priority), l(NULL), r(NULL) { }
};
typedef node* tnode;

void updateSum(tnode t) {
	if (t == nullptr) {
		t->sum == 0;
	}
	else if (t->l == NULL && t->r == NULL) {
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
	updateSum(t);
	updateSum(l);
	updateSum(r);
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
	node* a = new node(100, 1);
	node* b = new node(150, 2);
	node* c = new node(125, 3);
	node* d = new node(75, 0.1);
	node* e = new node(200, 2.5);
	node* t = unite(a, b);
	insert(t, c);
	insert(t, d);
	insert(t, e);
	std::cout << t->key << std::endl;
	std::cout << t->l->key << std::endl;
	std::cout << t->r->key << std::endl;
	std::cout << t->sum << std::endl;
    std::cout << "Hello World!\n";
}


