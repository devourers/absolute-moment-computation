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
	//int order = 0;
	double key, sum, priority = 0;
	//std::vector<double> sums = { 1 };
	double* sums = new double[ORDER + 1];
	Node* l, * r = nullptr;
	Node() = default;
	~Node() = default;
	Node(double key, double priority /*int order*/) : key(key), priority(priority), l(nullptr), r(nullptr)/*, order(order)*/ {
		sums[0] = 1;
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
	for (int iter_ = 0; iter_ < pwr; iter_++) {
		res *= base;
	}
	return res;
}


//Треугольник Паскаля для подсчёта биноминальных коэффициентов
std::vector<std::vector<double>> pTriangle(int n) {
	std::vector<std::vector<double>> results(n + 1);
	results[0].push_back(1);
	results[1].push_back(1);
	results[1].push_back(1);
	for (int iter_ = 2; iter_ <= n; iter_++) {
		results[iter_].push_back(1);
		for (int jiter_ = 1; jiter_ < iter_; jiter_++) {
			results[iter_].push_back(results[iter_ - 1][jiter_ - 1] + results[iter_ - 1][jiter_]);
		}
		results[iter_].push_back(1);
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
	//updateSums(l);
	//updateSums(r);

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


//удаляет вершину с ключом key из дерева t
template<int ORDER>
void erase(tNode<ORDER>& t, double key) {
	if (t->key == key) {
		merge(t, t->l, t->r);
	}
	else {
		erase(key < t->key ? t->l : t->r, key);
	}
	updateSums<ORDER>(t);
}


//объединяет два дерево в одно
template<int ORDER>
tNode<ORDER> unite(tNode<ORDER> l, tNode<ORDER> r) {
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


//функция подсчёта абсолютной величины, основанная на формуле биноминальный коэфициентов после раскрытия p-ого момента случайной величины. order - степень момента, t - дерево в котором считаем, repeater - костыль который остался от старых багов
template<int ORDER>
double countMomentFormula(int order, tNode<ORDER>& t, std::vector<double> coefs_vec) {
	double res = 0.0;
	Node<ORDER>* lesser = nullptr;
	Node<ORDER>* greater = nullptr;
	double divKey = t->sums[1] / t->sums[0];
	split<ORDER>(t, divKey, lesser, greater);
	double currDivKey = 1;
	double secCurrDivKey = power(divKey, order);
	for (int iter_ = 0; iter_ <= order; iter_ += 1) {
		if (iter_ % 2 == 0) {
			res += coefs_vec[iter_] * (currDivKey * (greater->sums[order - iter_]) + secCurrDivKey * lesser->sums[iter_]);
		}
		else {
			res -= coefs_vec[iter_] * (currDivKey * (greater->sums[order - iter_]) + secCurrDivKey * lesser->sums[iter_]);
		}
		currDivKey *= divKey;
		secCurrDivKey /= divKey;
	}
	merge<ORDER>(t, lesser, greater);

	return res;
}


//Функция подсчёта момента за два прохода, принимает на вход p-ую степень момента, начало и конец вектоора, а так же количество повторений (убрать)
double countMomentVect(const int p, std::vector<double>::const_iterator it_beg, std::vector<double>::const_iterator it_end, const int repeate = 1) {
	double result = 0.0;
	double mean = 0.0;
	int size = it_end - it_beg;
	result = 0.0;
	mean = 0.0;
	for (auto it = it_beg; it != it_end; it += 1) {
		mean += *it;
	}
	mean /= size;
	for (auto it = it_beg; it != it_end; it += 1) {
		result += power(std::abs(*it - mean), p);
	}
	return result;
}


//кастомный дестркутор дерева
/*

void deleteTree(Node* tree) {
	std::queue<Node*> qDelet;
	qDelet.push(tree);
	while (!qDelet.empty()) {
		Node curr = *qDelet.front();
		if (curr.l != nullptr) {
			qDelet.push(curr.l);
		}
		if (curr.r != nullptr) {
			qDelet.push(curr.r);
		}
		delete qDelet.front();
		qDelet.pop();
	}
}
*/

int main()
{
	for (int iter_ = 0; iter_ < 1; iter_++) {
		std::cout << iter_ + 1 << std::endl;
		double currKey = 0.0;
		double currPrior = 0.0;
		const int repeat_counter = 1;
		const int order = 4;
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
		int a = 10000;
		std::vector<std::vector<double>> coefs_vec = pTriangle(order);
		std::vector<double> sample;
		std::vector<Node<order>*> nodeSample;
		nodeSample.reserve(10001);
		sample.reserve(10000);
		std::random_device generatror;
		std::mt19937 gen(generatror());
		std::uniform_real_distribution<> dis(0.0, 1.0);
		currKey = dis(gen);
		currPrior = dis(gen);
		nodeSample[0] = new Node<order>(currKey, currPrior);
		sample[0] = currKey;
		for (int i = 0; i < a; i++) {
			currKey = dis(gen);
			currPrior = dis(gen);
			nodeSample[i + 1] = new Node<order>(currKey, currPrior);
			insert<order>(nodeSample[0], nodeSample[i + 1]);
			sample[i + 1] = currKey;

			//подсчёт времени работы с помощью формулы на струкртуре треапа
			std::chrono::high_resolution_clock::time_point t_start1 = std::chrono::high_resolution_clock::now();
			double currPlaceholder1 = countMomentFormula<order>(order, nodeSample[0], coefs_vec[order]);
			std::chrono::high_resolution_clock::time_point t_finish1 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> time_span1 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish1 - t_start1);
			myfile1 << (time_span1.count() / repeat_counter) << '\t' << currPlaceholder1 << std::endl;

			//подсчёт времени работы классического алгоритма в два прохода
			std::chrono::high_resolution_clock::time_point t_start2 = std::chrono::high_resolution_clock::now();
			double currPlaceholder2 = countMomentVect(order, sample.cbegin(), sample.begin() + i + 2, repeat_counter);
			std::chrono::high_resolution_clock::time_point t_finish2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> time_span2 = std::chrono::duration_cast<std::chrono::duration<double, std::nano>> (t_finish2 - t_start2);
			myfile2 << (time_span2.count() / repeat_counter) << '\t' << currPlaceholder2 << std::endl;

			//информация во время компиляции по поводу разности между результатами на треапе и векторе, а так же время которое понадобилось на подсчёт на данном этапе
			//std::cout << "------------------------------------------------" << std::endl << i << "| " << currPlaceholder1 << " - " << currPlaceholder2 << " = delta " << currPlaceholder1 - currPlaceholder2 << ";" << std::endl << "     time of 2pass = " << time_span2.count() / repeat_counter << " time of treap " << time_span1.count() / repeat_counter << std::endl;
		}
	}
}
