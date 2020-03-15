#include <iostream>;
#include <random>;
#include <iterator>
#include <vector>;
#include <fstream>;
#include <chrono>;
#include <math.h>;

struct node {
	int order;
	double key, sum, priority, sqauredSum, cubeSum;
	std::vector<double> sums;
	node* l, *r;
	node() { }
	node(double key, double priority, int order) : key(key), priority(priority), l(nullptr), r(nullptr), order(order), sums({ key }) {
		sums.resize(order+1);
		for (int i = 0; i < order; i += 1) {
			sums[i] = std::pow(key, i);
		}
	}
};
typedef node* tnode;

void updateSum(tnode t)
{
	if (t == nullptr) {

	}
	else if (t->l == nullptr && t->r == nullptr) {
		for (int iter_ = 0; iter_ < t->sums.size(); iter_ += 1) {
			t->sums[iter_] = std::pow(t->key, iter_);
		}
	}
	else if (t->l == nullptr)
	{
		for (int iter_ = 0; iter_ < t->sums.size(); iter_ += 1) {
			t->sums[iter_] = std::pow(t->key, iter_) + t->r->sums[iter_];
		}
	}
	else if (t->r == nullptr) {
		for (int iter_ = 0; iter_ < t->sums.size(); iter_ += 1) {
			t->sums[iter_] = std::pow(t->key, iter_) + t->l->sums[iter_];
		}
	}
	else
	{
		for (int iter_ = 0; iter_ < t->sums.size(); iter_ += 1) {
			t->sums[iter_] = std::pow(t->key, iter_) + t->r->sums[iter_] + t->l->sums[iter_];
		}
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

double countMoment(tnode& t, const int repeat_counter = 1) {
	double res = 0.0;
	node* lesser;
	node* greater;
	double divKey = t->sums[1] / t->sums[0];
	/*split(t, divKey, lesser, greater);
	std::cout << "///lesser///" << std::endl;
	std::cout << " sums[0]" << lesser->sums[0] << std::endl;
	std::cout << " sums[1]" << lesser->sums[1] << std::endl;
	std::cout << " sums[2]" << lesser->sums[2] << std::endl;
	std::cout << "         " << std::endl;
	std::cout << "///greater///" << std::endl;
	std::cout << " sums[0]" << greater->sums[0] << std::endl;
	std::cout << " sums[1]" << greater->sums[1] << std::endl;
	std::cout << " sums[2]" << greater->sums[2] << std::endl;*/
	//for (int i_rep = 0; i_rep < repeat_counter; i_rep += 1) {
	split(t, divKey, lesser, greater);
	//PLAIN FORMULAS FOR ORDER 2 AND 3

	//res = (lesser->nodes + greater->nodes) * std::pow(divKey, 2) - 2 * divKey * (lesser->sum + greater->sum) + (lesser->sqauredSum + greater->sqauredSum);
	res = (greater->sums[3] - lesser->sums[3]) + 3 * std::pow(divKey, 2) * (greater->sums[1] - lesser->sums[1]) + 3 * divKey * (lesser->sums[2] - greater->sums[2]) + std::pow(divKey, 3) * (lesser->sums[0] - greater->sums[0]);
	merge(t, lesser, greater);
	//}

	return res;
}

int factorial(int n) {
	if (n == 0) {
		return 1;
	}
	else
	{
		return factorial(n - 1) * n;
	}
}

int binominalCoef(int n, int k) {
	return factorial(n) / (factorial(k) * factorial(n - k));
}

double countMomentFormula(int order, tnode& t, const int repeater) {
	double res = 0.0;
	node* lesser;
	node* greater;
	double divKey = t->sums[1] / t->sums[0];
	split(t, divKey, lesser, greater);
	for (int iter_ = 0; iter_ <= order; iter_ += 1) {
		res += std::pow(-1, iter_) * binominalCoef(order, iter_) * (std::pow(divKey, iter_)*greater->sums[order-iter_] + std::pow(divKey, order-iter_)*lesser->sums[iter_]);
	}
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
	const int repeat_counter = 1;
	int order = 20;
	std::ofstream myfile1;
	std::ofstream myfile2;
	myfile1.open("treap_results_new.txt");
	myfile2.open("2pass_results.txt");
	int a = 100000;
	std::vector <double> sample(a + 1);
	currKey = rand() % a;
	currKey /= a;
	currPrior = rand() % 100;
	currPrior /= 100;
	node* tree = new node(currKey, currPrior, order);
	sample[0] = currKey;
	for (int i = 0; i < a; i++) {
		currKey = rand() % a;
		currKey /= (a / 10);
		currPrior = rand() % (i + 100);
		currPrior /= (i + 100);
		node* curr = new node(currKey, currPrior, order);
		insert(tree, curr);
		sample[i + 1] = currKey;


		/*
		//counting with treaps classic
		std::chrono::high_resolution_clock::time_point t_start1 = std::chrono::high_resolution_clock::now();
		double currPlaceholder1 = countMoment(tree, repeat_counter);
		std::chrono::high_resolution_clock::time_point t_finish1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span1 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish1 - t_start1);
		myfile1 << (time_span1.count() / repeat_counter) << '\t' << currPlaceholder1 << std::endl;
		*/

		//counting with treaps for any order
		std::chrono::high_resolution_clock::time_point t_start1 = std::chrono::high_resolution_clock::now();
		double currPlaceholder1 = countMomentFormula(20, tree, repeat_counter);
		std::chrono::high_resolution_clock::time_point t_finish1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span1 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish1 - t_start1);
		myfile1 << (time_span1.count() / repeat_counter) << '\t' << currPlaceholder1 << std::endl;

		//counting with 2pass 
		std::chrono::high_resolution_clock::time_point t_start2 = std::chrono::high_resolution_clock::now();
		double currPlaceholder2 = countMomentVect(20, sample.cbegin(), sample.begin() + i + 2, repeat_counter);
		std::chrono::high_resolution_clock::time_point t_finish2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span2 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish2 - t_start2);
		myfile2 << (time_span2.count() / repeat_counter) << '\t' << currPlaceholder2 << std::endl;
	}
	std::cout << "!done" << std::endl;
}