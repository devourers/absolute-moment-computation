#include <iostream>;
#include <random>;
#include <iterator>
#include <vector>;
#include <fstream>;
#include <chrono>;
#include <math.h>;

struct Node {
	int order;
	double key, sum, priority;
	std::vector<double> sums;
	Node* l, *r;
	Node() { }
	Node(double key, double priority, int order) : key(key), priority(priority), l(nullptr), r(nullptr), order(order), sums({ key }) {
		sums.resize(order+1);
		sums[0] = 1;
		for (int i = 1; i < order; i += 1) {
			sums[i] = sums[i-1]*key;
		}
	}
};
typedef Node* tNode;


double power(double base, int pwr) {
	double res = 1.0;
	for (int iter_ = 0; iter_ < pwr; iter_++) {
		res *= base;
	}
	return res;
}

double** pascalTriangle(int n) {
	double** results = new double*[n + 1];
	results[0] = new double [1];
	results[1] = new double [2];
	results[0][0] = 1;
	results[1][0] = 1;
	results[1][1] = 1;
	for (int iter_ = 2; iter_ < n + 1; iter_++) {
		results[iter_] = new double[iter_ + 1];
		results[iter_][0] = 1;
		for (int jiter_ = 1; jiter_ < iter_; jiter_++) {
			results[iter_][jiter_] = results[iter_ - 1][jiter_ - 1] + results[iter_ - 1][jiter_];
		}
		results[iter_][iter_] = 1;
	}
	return results;
}

void updateSums(tNode t) {
	if (t != nullptr) {
		for (int iter_ = 0; iter_ < t->sums.size(); iter_ += 1) {
			t->sums[iter_] = power(t->key, iter_);
		}
		if (t->l != nullptr) {
			for (int iter_ = 0; iter_ < t->sums.size(); iter_ += 1) {
				t->sums[iter_] += t->l->sums[iter_];
			}
		}
		if (t->r != nullptr) {
			for (int iter_ = 0; iter_ < t->sums.size(); iter_ += 1) {
				t->sums[iter_] += t->r->sums[iter_];
			}
		}
	}
	else {}
}

void split(tNode t, double key, tNode& l, tNode& r) {
	if (!t)
		l = r = nullptr;
	else if (key < t->key){
		split(t->l, key, l, t->l);
		r = t;
	}
	else{
		split(t->r, key, t->r, r);
		l = t;
	}
	updateSums(l);
	updateSums(r);

}

void insert(tNode& t, tNode it) {
	if (!t) {
		t = it;
	}
	else if (it->priority > t->priority){
		split(t, it->key, it->l, it->r);
		t = it;
	}
	else {
		insert(it->key < t->key ? t->l : t->r, it);
	}
	updateSums(t);
}

void merge(tNode& t, tNode l, tNode r) {
	if (!l || !r) {
		t = l ? l : r;
	}
	else if (l->priority > r->priority) {
		merge(l->r, l->r, r);
		t = l;
	}
	else {
		merge(r->l, l, r->l);
		t = r;
	}
	updateSums(t);
}

void erase(tNode& t, double key) {
	if (t->key == key) {
		merge(t, t->l, t->r);
	}
	else {
		erase(key < t->key ? t->l : t->r, key);
	}
	updateSums(t);
}

tNode unite(tNode l, tNode r) {
	if (!l || !r) {
		return l ? l : r;
	}
	if (l->priority < r->priority) { 
		std::swap(l, r); 
	}
	tNode lt, rt;
	split(r, l->key, lt, rt);
	l->l = unite(l->l, lt);
	l->r = unite(l->r, rt);
	return l;
}

double countMomentFormula(int order, tNode& t, const int repeater) {
	double res = 0.0;
	Node* lesser;
	Node* greater;
	double** coefs = pascalTriangle(order);
	double divKey = t->sums[1] / t->sums[0];
	split(t, divKey, lesser, greater);
	for (int iter_ = 0; iter_ <= order; iter_ += 1) {
		res += power(-1, iter_) * coefs[order][iter_] * (power(divKey, iter_)*(greater->sums[order-iter_]) + power(divKey, order-iter_)*lesser->sums[iter_]);
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
			result += power(std::abs(*it - mean), p);
		}
	}
	return result;
}


int main()
{
	double currKey;
	double currPrior;
	const int repeat_counter = 1;
	int order = 11;
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
	Node* tree = new Node(currKey, currPrior, order);
	sample[0] = currKey;
	for (int i = 0; i < a; i++) {
		currKey = rand() % a;
		currKey /= (a / 10);
		currPrior = rand() % (i + 100);
		currPrior /= (i + 100);
		Node* curr = new Node(currKey, currPrior, order);
		insert(tree, curr);
		sample[i + 1] = currKey;

		//counting with treaps
		std::chrono::high_resolution_clock::time_point t_start1 = std::chrono::high_resolution_clock::now();
		double currPlaceholder1 = countMomentFormula(11, tree, repeat_counter);
		std::chrono::high_resolution_clock::time_point t_finish1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span1 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish1 - t_start1);
		myfile1 << (time_span1.count() / repeat_counter) << '\t' << currPlaceholder1 << std::endl;

		//counting with vector 
		std::chrono::high_resolution_clock::time_point t_start2 = std::chrono::high_resolution_clock::now();
		double currPlaceholder2 = countMomentVect(11, sample.cbegin(), sample.begin() + i + 2, repeat_counter);
		std::chrono::high_resolution_clock::time_point t_finish2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span2 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish2 - t_start2);
		myfile2 << (time_span2.count() / repeat_counter) << '\t' << currPlaceholder2 << std::endl;

		std::cout << "------------------------------------------------" << std::endl << i << "| " << currPlaceholder1 << " - " << currPlaceholder2 << " = delta " << currPlaceholder1 - currPlaceholder2 << ";" << std::endl << "     time of 2pass = " << time_span2.count() / repeat_counter << " time of treap " << time_span1.count() / repeat_counter << std::endl;
	}
	std::cout << "!done" << std::endl;
}