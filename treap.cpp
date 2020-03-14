#include <iostream>;
#include <random>;

struct node {
	double key, sum, priority, sqauredSum;
	int nodes;
	node* l, * r;
	node() { }
	node(double key, double priority) : key(key), priority(priority), l(nullptr), r(nullptr), sum(key), sqauredSum(key*key), nodes(1) { }
};
typedef node* tnode;

void updateSum(tnode t)
{
	if (t == nullptr) {
		
}
	else if (t->l == nullptr && t->r == nullptr) {
		t->sum = t->key;
		t->sqauredSum = t->key * t->key;
		t->nodes = 1;
	}
	else if (t->l == nullptr)
	{
		t->sum = t->key + t->r->sum;
		t->sqauredSum = t->key * t->key + t->r->sqauredSum;
		t->nodes = 1 + t->r->nodes;
	}
	else if (t->r == nullptr) {
		t->sum = t->key + t->l->sum;
		t->sqauredSum = t->key * t->key + t->l->sqauredSum;
		t->nodes = 1 + t->l->nodes;
	}
	else
	{
		t->sum = t->key + t->l->sum + t->r->sum;
		t->sqauredSum = t->key * t->key + t->r->sqauredSum + t->l->sqauredSum;
		t->nodes = 1 + t->r->nodes + t->l->nodes;
	}

}

void split(tnode t, double key, tnode& l, tnode& r) {
	if (!t)
		l = r = NULL;
	else if (key < t->key)
		split(t->l, key, l, t->l), r = t;
	else
		split(t->r, key, t->r, r), l = t;
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

void erase(tnode& t, double key) {
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
	return l;
}

double countMoment(tnode& t) {
	node* lesser;
	node* greater;
	double divKey = t->sum / t->nodes;
	split(t, divKey, lesser, greater);
	double res = t->nodes * divKey * divKey - 2 * divKey * t->sum + t->sqauredSum;
	merge(t, lesser, greater);
	return res;
}


int main()
{
	double currKey;
	double currPrior;
	double realSum = 0;
	double squRealSum = 0;
	int a = 1000;
	currKey = rand() % a;
	currKey /= a;
	realSum += currKey;
	squRealSum += currKey * currKey;
	currPrior = rand() % 100;
	currPrior /= 100;
	node* tree = new node(currKey, currPrior);
	std::cout << currKey << std::endl;
	for (int i = 0; i < a; i++) {
		currKey = rand() % a;
		currKey /= (a/10);
		realSum += currKey;
		squRealSum += currKey * currKey;
		currPrior = rand() % (i + 100);
		currPrior /= (i + 100);
		node* curr = new node(currKey, currPrior);
		//std::cout << currKey << std::endl;
		insert(tree, curr);
		std::cout << i + 1 << " " << countMoment(tree) << std::endl;
	}
	std::cout << "key " << tree->key << std::endl;
	std::cout<< "alg " << tree->sum << std::endl;
	std::cout << "real " << realSum << std::endl;
	std::cout << "alg squr " << tree->sqauredSum << std::endl;
	std::cout << "real " << squRealSum << std::endl;
	std::cout << "nodes " << tree->nodes << std::endl;
	std::cout << "alg moment " << countMoment(tree) << std::endl;
}
