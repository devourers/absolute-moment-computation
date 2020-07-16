#include <iostream>
#include <random>
#include <iterator>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <cmath>
#include <queue>


//структура вершины-дерева, нулевой элемент суммы -- количество вершин в поддереве, а так sums хранит сумму всех степеней вплоть до n (\sum k^0, \sum k^1, \sum k^2, ...) для поддерева
template<int ORDER>
struct Node {
	double key = 0.0;
	double priority = 0.0;
	double sums[ORDER + 1];
	Node* l = nullptr;
	Node* r = nullptr;

	Node() {
		for (int iter_ = 0; iter_ <= ORDER; iter_++) {
			sums[iter_] = 0.0;
		}
	}

	~Node() = default;

	Node(double key, double priority) : key(key), priority(priority), l(nullptr), r(nullptr) {
		sums[0] = 1.0;
		for (int i = 1; i <= ORDER; i += 1) {
			sums[i] = sums[i - 1] * key;
		}
	}
};

template<int ORDER>
using tNode = Node<ORDER>*;


//функция степени, которая работает быстрее и стабильнее std::pow
double power(double base, int pwr) {
	double res = 1.0;
	while (pwr) {
		if (pwr & 1) {
			res *= base;
		}
		base *= base;
		pwr >>= 1;
	}
	return res;
}


//Треугольник Паскаля для подсчёта биноминальных коэффициентов
std::vector<std::vector<double>> pTriangle(int n) {
	std::vector<std::vector<double>> results(n + 1);
	results[0].push_back(1.0);
	results[1].push_back(1.0);
	results[1].push_back(1.0);
	for (int iter_ = 2; iter_ <= n; iter_++) {
		results[iter_].push_back(1.0);
		for (int jiter_ = 1; jiter_ < iter_; jiter_++) {
			results[iter_].push_back(results[iter_ - 1][jiter_ - 1] + results[iter_ - 1][jiter_]);
		}
		results[iter_].push_back(1.0);
	}
	return results;
}


//обновляет все n сумм (в зависимости от заданного порядка дерева).
template<int ORDER>
void updateSums(tNode<ORDER> t) {
	if (t != nullptr) {
		t->sums[0] = 1;
		for (int iter_ = 1; iter_ <= ORDER; iter_ += 1) {
			t->sums[iter_] = t->sums[iter_ - 1] * t->key;
		}
		if (t->l != nullptr) {
			for (int iter_ = 0; iter_ <= ORDER; iter_ += 1) {
				t->sums[iter_] += t->l->sums[iter_];
			}
		}
		if (t->r != nullptr) {
			for (int iter_ = 0; iter_ <= ORDER; iter_ += 1) {
				t->sums[iter_] += t->r->sums[iter_];
			}
		}
	}
	else {}
}


//сплитит дерево t в два дерево l и r по заданному ключу
template<int ORDER>
void split(tNode<ORDER> t, double key, tNode<ORDER>& l, tNode<ORDER>& r) {
	if (!t)
		l = r = nullptr;
	else if (key < t->key) {
		split(t->l, key, l, t->l);
		r = t;
		updateSums<ORDER>(r);
	}
	else {
		split(t->r, key, t->r, r);
		l = t;
		updateSums<ORDER>(l);
	}
}


//вставляет в дерево t вершину it
template<int ORDER>
void insert(tNode<ORDER>& t, tNode<ORDER> it) {
	if (!t) {
		t = it;
	}
	else if (it->priority > t->priority) {
		split(t, it->key, it->l, it->r);
		t = it;
	}
	else {
		insert(it->key < t->key ? t->l : t->r, it);
	}
	updateSums<ORDER>(t);
}


//мержит деревья l и r в дерево t
template<int ORDER>
void merge(tNode<ORDER>& t, tNode<ORDER> l, tNode<ORDER> r) {
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
	updateSums<ORDER>(t);
}

//Функция подсчёта момента за два прохода, принимает на вход p-ую степень момента, начало и конец вектора
double countMomentVect(const int p, std::vector<double>::const_iterator it_beg, std::vector<double>::const_iterator it_end) {
	double result = 0.0;
	double mean = 0.0;
	int size = it_end - it_beg;
	for (auto it = it_beg; it != it_end; it++) {
		mean += *it;
	}
	mean /= size;
	for (auto it = it_beg; it != it_end; it++) {
		result += power(std::fabs(*it - mean), p);
	}
	return result;
}


double* countSmartMomentVect(const int p, double prev_mean, double new_value, int curr_place, std::vector<double> curr_sample) {
	std::cout << prev_mean << std::endl;
	std::cout << curr_place << std::endl;
	std::cout << new_value << std::endl;
	double result = 0.0;
	double mean = (prev_mean * curr_place + new_value) / (curr_place + 1.0);
	std::cout << mean << std::endl;
	for (int iter_ = 0; iter_ < curr_place; iter_++){
		result += std::fabs(power((curr_sample[iter_] - mean), p));
	}
	result += std::fabs(power((new_value - mean), p));
	double res[2];
	res[0] = result;
	res[1] = mean;
	return res;
}

//вспомогательная функция для поиска меньшего дерева из split(mean)
template<int ORDER>
void traverse(tNode<ORDER> node, double* accumulator, double mean) {
	if (node == nullptr){
	}
	else {
		if (node->key > mean) {
			if (node->l != nullptr) {
				traverse(node->l, accumulator, mean);
			}
			else {}
		}
		else {
			double k_i = 1.0;
			for (int iter_ = 0; iter_ <= ORDER; iter_++) {
				if (node->l != nullptr) {
					accumulator[iter_] += node->l->sums[iter_];
				}
				accumulator[iter_] += k_i;
				k_i *= node->key;
			}
			traverse(node->r, accumulator, mean);
		}
	}
}

//фунция подсчёта момента по заданной дерамиде
template<int ORDER>
double smartCountMomentTreap(tNode<ORDER> root, const std::vector<double>& coefs_vec) {
	double res = 0.0;
	double mean_value = root->sums[1] / root->sums[0];
	double lesserSums[ORDER + 1];

	for (int iter_ = 0; iter_ <= ORDER; iter_++) {
		lesserSums[iter_] = 0;
	}
	traverse(root, lesserSums, mean_value);
	double currDivKey = 1.0;
	double secCurrDivKey = power(mean_value, ORDER);
	for (int iter_ = 0; iter_ <= ORDER; iter_++) {
		double a = coefs_vec[iter_] * (currDivKey * (root->sums[ORDER - iter_] - lesserSums[ORDER - iter_]) + secCurrDivKey * lesserSums[iter_]);
		if (iter_ % 2 == 1) {
			a *= -1.0;
		}
		currDivKey *= mean_value;
		secCurrDivKey /= mean_value;
		res += a;
	}
	return res;
}


//MISC FUNCTIONS

double countNonAbsoulteMomentVect(const int p, std::vector<double>::const_iterator it_beg, std::vector<double>::const_iterator it_end) {
	double result = 0.0;
	double mean = 0.0;
	int size = it_end - it_beg;
	for (auto it = it_beg; it != it_end; it++) {
		mean += *it;
	}
	mean /= size;
	for (auto it = it_beg; it != it_end; it++) {
		result += power(*it - mean, p);
	}
	return result;
}

template<int ORDER>
double countNonAbsoluteMomentTreap(tNode<ORDER> root, const std::vector<double>& coefs_vec) {
	double mean = root->sums[1] / root->sums[0];
	double res = 0.0;
	return std::fabs(root->sums[ORDER]);
	double CurrDivKey = power(mean, ORDER);
	for (int iter_ = 0; iter_ <= ORDER; iter_++) {
		double a = coefs_vec[iter_] * CurrDivKey * root->sums[iter_];
		if (iter_ % 2 == 1) {
			a *= -1.0;
		}
		CurrDivKey /= mean;
		res += a;
	}
	return res;
}

template<int ORDER>
double countNonCentralAbsouluteMomentTreap(tNode<ORDER> root) {
	return std::fabs(root->sums[ORDER]);
}

int main()
{
	int rep_cout = 21;
	for (int iter_ = 0; iter_ < rep_cout; iter_++) {
		std::cout << iter_ + 1 << "/" << rep_cout << " in progress." << std::endl;
		double currKey = 0.0;
		double currPrior = 0.0;
		const int order = 1;
		int check = 0;
		std::ofstream myfile1;
		std::ofstream myfile2;
		std::string treap_res_text = "txt_output/treap_results_new";
		treap_res_text += std::to_string(iter_);
		treap_res_text += ".txt";
		std::string doublepass_res_text = "txt_output/2pass_results";
		doublepass_res_text += std::to_string(iter_);
		doublepass_res_text += ".txt";
		myfile1.open(treap_res_text);
		myfile2.open(doublepass_res_text);

		size_t a = 1000;
		std::vector<std::vector<double>> coefs_vec = pTriangle(order);
		std::vector<double> sample;
		sample.resize(a);

		std::mt19937 gen(1337);
		//std::normal_distribution<double> dis(0.0, 1.0);
		std::uniform_real_distribution<double> dis(0.0, 1.0);

		std::vector<Node<order> > nodeSample;
		nodeSample.resize(a);

		currKey = dis(gen);
		currPrior = dis(gen);

		nodeSample[0] = Node<order>(currKey, currPrior);
		sample[0] = currKey;

		tNode<order> root = &(nodeSample[0]);
		double curr_mean = sample[0];

		for (size_t i = 1; i < a; i++) {
			currKey = dis(gen);
			currPrior = dis(gen);



			nodeSample[i] = Node<order>(currKey, currPrior);
			//insert<order>(root, &(nodeSample[i]));


			//подсчёт времени работы с помощью формулы на струкртуре треапа
			auto t_start1 = std::chrono::high_resolution_clock::now();
			insert<order>(root, &(nodeSample[i]));
			double currPlaceholder1 = smartCountMomentTreap<order>(root, coefs_vec[order]);
			auto t_finish1 = std::chrono::high_resolution_clock::now();
			auto time_span1 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(t_finish1 - t_start1);
			myfile1 << time_span1.count() << '\t' << currPlaceholder1 << std::endl;


			/*
			//подсчёт времени работы классического алгоритма в два прохода
			auto t_start2 = std::chrono::high_resolution_clock::now();
			double currPlaceholder2 = countMomentVect(order, sample.cbegin(), sample.cbegin() + i + 1);
			auto t_finish2 = std::chrono::high_resolution_clock::now();
			auto time_span2 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish2 - t_start2);
			myfile2 << time_span2.count() << '\t' << currPlaceholder2 << std::endl;
			*/

			//подсчёт времени работы классического алгоритма в два прохода
			auto t_start2 = std::chrono::high_resolution_clock::now();
			double* currPlaceholder2 = countSmartMomentVect(order, curr_mean,  currKey, i, sample);
			auto t_finish2 = std::chrono::high_resolution_clock::now();
			auto time_span2 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish2 - t_start2);
			myfile2 << time_span2.count() << '\t' << currPlaceholder2[0] << std::endl;

			sample[i] = currKey;
			curr_mean = currPlaceholder2[1];

			//информация во время компиляции по поводу разности между результатами на треапе и векторе, а так же время которое понадобилось на подсчёт на данном этапе
			std::cout << "------------------------------------------------" << std::endl;
			std::cout << i << "| " << currPlaceholder1 << " - " << currPlaceholder2[0] << " = delta " << currPlaceholder1 - currPlaceholder2[0] << ";" << std::endl;
			std::cout<< "     time of 2pass = " << time_span2.count() << " time of treap = " << time_span1.count() << std::endl;
			std::cin >> check;
		}
		std::cout << iter_ + 1 << "/" << rep_cout << " done." << std::endl;
	}
}
