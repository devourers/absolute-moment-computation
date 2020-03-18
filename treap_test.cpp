#include <iostream>;
#include <random>;
#include <iterator>
#include <vector>;
#include <array>;
#include <fstream>;
#include <chrono>;
#include <math.h>;

template<int order_>

struct Node {
	double key_ = 0;
	double priority_ = 0;
	std::array<double, order_> sums_;
	Node* l_;
	Node* r_;
	Node() = default;
	Node(const Node&) = default;
	Node& operator=(const Node&) = delete;
	~Node() = default;

	Node(const double key, const double priority) : key_(key), priority_(priority)
	{
		sums_[0] = 1;
		sums_[1] = key_;
		for (int i = 2; i < order_; i += 1) {
			sums_[i] = sums_[i-1] * sums_[1];
		}
	}
};	

template<int order_>
void updateSums(Node<order_> t)
{
	if (t == nullptr) {

	}
	else if (t->l_ == nullptr && t->r_ == nullptr) {
		for (int iter_ = 0; iter_ < t->sums_.size(); iter_ += 1) {
			t->sums_[iter_] = std::pow(t->key_, iter_);
		}
	}
	else if (t->l_ == nullptr)
	{
		for (int iter_ = 0; iter_ < t->sums_.size(); iter_ += 1) {
			t->sums_[iter_] = std::pow(t->key_, iter_) + t->r_->sums_[iter_];
		}
	}
	else if (t->r_ == nullptr) {
		for (int iter_ = 0; iter_ < t->sums_.size(); iter_ += 1) {
			t->sums_[iter_] = std::pow(t->key_, iter_) + t->l_->sums_[iter_];
		}
	}
	else
	{
		for (int iter_ = 0; iter_ < t->sums_.size(); iter_ += 1) {
			t->sums_[iter_] = std::pow(t->key_, iter_) + t->r_->sums_[iter_] + t->l_->sums_[iter_];
		}
	}

}
template<int order_>
void split(Node<order_> t, double key, Node<order_>& l, Node<order_>& r) {
	if (!t)
		l = r = NULL;
	else if (key < t->key_)
		split(t->l_, key, l, t->l_), r = t;
	else
		split(t->r_, key, t->r_, r), l = t;
	updateSums(l);
	updateSums(r);

}
template<int order_>
void insert(Node<order_>& t, Node<order_> it) {
	if (!t)
		t = it;
	else if (it->priority_ > t->priority_) {
		split(t, it->key_, it->l_, it->r_);
		t = it;
	}
	else {
		insert(it->key_ < t->key_ ? t->l_ : t->r_, it);
	}
	updateSums(t);
}

template<int order_>
void merge(Node<order_>& t, Node<order_> l, Node<order_> r) {
	if (!l || !r)
		t = l ? l : r;
	else if (l->priority_ > r->priority_)
		merge(l->r_, l->r_, r), t = l;
	else
		merge(r->l_, l, r->l_), t = r;
	updateSums(t);
}

template<int order_>
void erase(Node<order_>& t, double key) {
	if (t->key_ == key)
		merge(t, t->l_, t->r_);
	else
		erase(key < t->key_ ? t->l_ : t->r_, key);
	updateSums(t);
}

template<int order_>
Node<order_> unite(Node<order_> l, Node<order_> r) {
	if (!l || !r)  return l ? l : r;
	if (l->priority_ < r->priority_)  std::swap(l, r);
	Node lt, rt;
	split(r, l->key_, lt, rt);
	l->l_ = unite(l->l_, lt);
	l->r_ = unite(l->r_, rt);
	return l;
}
template<int order_>
double countMoment(Node<order_>& t, const int repeat_counter = 1) {
	double res = 0.0;
	Node* lesser;
	Node* greater;
	double divKey = t->sums_[1] / t->sums_[0];
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

	//res = (lesser->Nodes + greater->Nodes) * std::pow(divKey, 2) - 2 * divKey * (lesser->sum + greater->sum) + (lesser->sqauredSum + greater->sqauredSum);
	res = (greater->sums_[3] - lesser->sums_[3]) + 3 * std::pow(divKey, 2) * (greater->sums_[1] - lesser->sums_[1]) + 3 * divKey * (lesser->sums_[2] - greater->sums_[2]) + std::pow(divKey, 3) * (lesser->sums_[0] - greater->sums_[0]);
	merge(t, lesser, greater);
	//}

	return res;
}

long int factorial(int n) {
	if (n == 0) {
		return 1;
	}
	else
	{
		return factorial(n - 1) * n;
	}
}

long int binominalCoef(int n, int k) {
	return factorial(n) / (factorial(k) * factorial(n - k));
}

template<int order_>
double countMomentFormula(int order, Node<order_>* t, const int repeater) {
	double res = 0.0;
	Node<order_> lesser;
	Node<order_> greater;
	double divKey = t->sums_[1] / t->sums_[0];
	split(t, divKey, lesser, greater);
	for (int iter_ = 0; iter_ <= order; iter_ += 1) {
		res += std::pow(-1, iter_) * binominalCoef(order, iter_) * (std::pow(divKey, iter_) * greater->sums_[order - iter_] + std::pow(divKey, order - iter_) * lesser->sums_[iter_]);
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
	const int order = 40;
	std::ofstream myfile1;
	std::ofstream myfile2;
	myfile1.open("treap_results_new.txt");
	myfile2.open("2pass_results.txt");
	int a = 10000;
	std::vector <double> sample(a + 1);
	currKey = rand() % a;
	currKey /= a;
	currPrior = rand() % 100;
	currPrior /= 100;
	Node<order>* tree = new Node<order>(currKey, currPrior);
	sample[0] = currKey;
	for (int i = 0; i < a; i++) {
		currKey = rand() % a;
		currKey /= a;
		currPrior = rand() % (i + 100);
		currPrior /= (i + 100);
		Node<order> curr = Node<order>(currKey, currPrior);
		insert(*tree, curr);
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
		double currPlaceholder1 = countMomentFormula(10, tree, repeat_counter);
		std::chrono::high_resolution_clock::time_point t_finish1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span1 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish1 - t_start1);
		myfile1 << (time_span1.count() / repeat_counter) << '\t' << currPlaceholder1 << std::endl;

		//counting with 2pass 
		std::chrono::high_resolution_clock::time_point t_start2 = std::chrono::high_resolution_clock::now();
		double currPlaceholder2 = countMomentVect(10, sample.cbegin(), sample.begin() + i + 2, repeat_counter);
		std::chrono::high_resolution_clock::time_point t_finish2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span2 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish2 - t_start2);
		myfile2 << (time_span2.count() / repeat_counter) << '\t' << currPlaceholder2 << std::endl;

		std::cout << i << " delta " << currPlaceholder1 << " - "<< currPlaceholder2 << " = " << currPlaceholder1-currPlaceholder2 << std::endl;
	}
	std::cout << "!done" << std::endl;
}