#include <iostream>;
#include <random>;
#include <iterator>
#include <vector>

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
	double res = (lesser->nodes + greater->nodes) * divKey * divKey - 2 * divKey * (lesser->sum + greater->sum) + (lesser->sqauredSum + greater->sqauredSum);
	merge(t, lesser, greater);
	return res;
}


double countMomentVect(const int p, std::vector<double>::const_iterator it_beg, std::vector<double>::const_iterator it_end, const int repeate = 1) {
	double result = 0.0;
	double mean = 0.0;
	int size = it_end - it_beg;
	for (int i_repeate = 0; i_repeate < repeate; i_repeate += 1) {
		result = 0.0;
		mean = 0.0;
		for (auto it = it_beg; it != it_end; it += 1) {
			mean += *it;
		}
		mean /= size;
		for (auto it = it_beg; it != it_end; it += 1) {
			result += std::pow(std::abs(*it - mean), p);
		}
	}
	return result;
}


int main()
{
	double currKey;
	double currPrior;
	double realSum = 0;
	double squRealSum = 0;
	int a = 1000;
	std::vector <double> sample(a+1);
	currKey = rand() % a;
	currKey /= a;
	realSum += currKey;
	squRealSum += currKey * currKey;
	currPrior = rand() % 100;
	currPrior /= 100;
	node* tree = new node(currKey, currPrior);
	sample[0] = currKey;
	for (int i = 0; i < a; i++) {
		currKey = rand() % a;
		currKey /= (a/10);
		realSum += currKey;
		squRealSum += currKey * currKey;
		currPrior = rand() % (i + 100);
		currPrior /= (i + 100);
		node* curr = new node(currKey, currPrior);
		insert(tree, curr);
		sample[i + 1] = currKey;
		std::cout << i + 1 << " alg " << countMoment(tree) << std::endl;
		std::cout << i + 1 << " real " << countMomentVect(2, sample.cbegin(), sample.begin()+i+2, 1) << std::endl;
		std::cout << "/////////////" << std::endl;
	}
}