/*
 * tree.cpp
 *
 *  Created on: Sep 25, 2017
 *      Author: Binki
 */

#include "tree.h"
#include "datautil.h"
#include <cmath>
#include <map>
#include <iostream>
#include<time.h>

using namespace std;

/**
 * Function to calculate entropy over a given set of data
 * @param v: Dataset
 * @param target_attr: Target attribute which classifies as positive or negative example
 * @return value of entropy for given dataset based on given target attribute
 */

double calc_entropy(const vector<struct Data > &v, int target_attr){
	int ntrue = 0, nfalse = 0;
	for(unsigned i = 0; i < v.size(); ++i){
		if(get_attr_value(target_attr, v[i]))
			ntrue++;
		else
			nfalse++;
	}
	double ptrue = (double)ntrue / (ntrue + nfalse);
	double pfalse = (double)nfalse / (ntrue + nfalse);

	double entropy = 0;
	if(ptrue != 0)
		entropy = -ptrue * log2(ptrue);
	if(pfalse != 0)
		entropy = -pfalse * log2(pfalse);
	return entropy;

}

/**
 * Function to calculate information gain over a given dataset for a certain attribute
 * @param v: Dataset
 * @param attr_id: attribute for which to calculate information gain
 * @param target_attr: target attribute which determines positive or negative example
 * @return value of information gain for given dataset over given attribute and target attribute
 */

double gain(vector<struct Data> &v, int attr_id, int target_attr) {
	map<int, vector<struct Data > > m;

	for(unsigned i = 0; i < v.size(); ++i)
		m[get_attr_value(attr_id, v[i])].push_back(v[i]);

	double entr = calc_entropy(v, target_attr);
	for(auto i = m.begin(); i != m.end(); ++i){
		double entropy = calc_entropy(i->second, target_attr);
		entr -= (double)i->second.size() / v.size() * entropy;
	}
	return entr;
}

/**
 *Function to test if all values for target attribute in a given dataset equal given test value
 * @param v: Dataset
 * @param test: value to be tested for
 * @param target_attr: index of target attribute
 * @return true if all points in dataset evaluate to the test value for the target attribute else fase
 */

bool test_all(vector<struct Data> &v, bool test, int target_attr) {
	for(unsigned i = 0; i < v.size(); ++i) {
		if(get_attr_value(target_attr, v[i]) != test)
			return false;
	}
	return true;
}

/**
 * Function to get the most common value for target attribute in a dataset
 * @param v: Dataset
 * @param target_attr: index of target attribute
 * @return value of target attribute which is most common in the dataset
 */

bool most_common(vector<struct Data> &v, int target_attr) {
	int ntrue = 0, nfalse = 0;
	for(unsigned i = 0; i < v.size(); ++i) {
		if(get_attr_value(target_attr, v[i]))
			ntrue++;
		else
			nfalse++;
	}
	if(ntrue > nfalse)
		return true;
	else
		return false;
}


/**
 *Driver function for ID3 Algorithm to construct decision tree
 * @param data: Training dataset
 * @param target_attr: index of attribute to be predicted
 * @param distinct_attr_values: map containing number of distinct values an attribute can take for each attribute
 * @param node: root node from which decision tree is to be built
 */

void id3(vector<struct Data> data, int target_attr, std::map<int, int> distinct_attr_values, struct TreeNode *node) {
//	struct TreeNode *node = new struct TreeNode();

	// if all are positive
	if(test_all(data, true, target_attr)){
		node->leaf = true;
		node->value = true;
//		cout << "all positive" << endl;
		return;
	}
	// if all are negative
	if(test_all(data, false, target_attr)){
		node->leaf = true;
		node->value = false;
//		cout << "all negative" << endl;
		return;
	}

	if(distinct_attr_values.empty()) {
		node->leaf = true;
		node->value = most_common(data, target_attr);
		return;
	}

	node->leaf = false;
	node->value=most_common(data,target_attr);

	double max_gain = 0.0; int max_gain_attr = distinct_attr_values.begin()->first;
	for(auto i = distinct_attr_values.begin(); i != distinct_attr_values.end(); ++i) {
		double g = gain(data, i->first, target_attr);

//		cout << "g = " << g ;
//		cout << " i->first = " << i->first ;
//		cout << endl;

		if(g > max_gain){
			max_gain = g;
			max_gain_attr = i->first;
		}
	}

	node->attr_id = max_gain_attr;
//	cout << "max_gain_attr " << max_gain_attr << endl;
	int n = distinct_attr_values[max_gain_attr];
	node->next = new TreeNode *[n+1];
	for(int i = 0; i <= n; ++i) {
		TreeNode *new_node = new TreeNode();
		node->next[i] = new_node;
//		cout << "max gain attr,  i (v(i)) = " << max_gain_attr << " "<< i << endl;
//		cout << "distinct_attr_values[max_gain_attr] = " << n << endl;

		// new_data = subset that have the value vi (i) for A (max_gain_attr)
		vector<struct Data> new_data;
		for(unsigned j = 0; j < data.size(); ++j) {
			if(get_attr_value(max_gain_attr, data[j]) == i)
			new_data.push_back(data[j]);
		}

		if(new_data.empty()){
//			cout << "new_data_empty" << endl;
			new_node->leaf = true;
			new_node->value = most_common(data, target_attr);
		}
		else {
//			cout << "new_data not empty" <<endl;
			distinct_attr_values.erase(max_gain_attr);
			id3(new_data, target_attr, distinct_attr_values, new_node);
//			cout << "return from recursive call" << endl;
		}
	}
}

/**
 * Function to construct decision trees as required by random forest method
 * @param data: Training dataset
 * @param target_attr: index of attribute to be predicted
 * @param distinct_attr_values: map containing number of distinct values an attribute can take for each attribute
 * @param node: root node from which randomized decision tree is to be built
 * @param i: variable to help randomize the seed variable for rand() function at each stage
 */

void randomForest(vector<struct Data> data, int target_attr, std::map<int, int> distinct_attr_values, struct TreeNode *node,int i) {
//	struct TreeNode *node = new struct TreeNode();

	// if all are positive
	if(test_all(data, true, target_attr)){
		node->leaf = true;
		node->value = true;
//		cout << "all positive" << endl;
		return;
	}
	// if all are negative
	if(test_all(data, false, target_attr)){
		node->leaf = true;
		node->value = false;
//		cout << "all negative" << endl;
		return;
	}

	if(distinct_attr_values.empty()) {
		node->leaf = true;
		node->value = most_common(data, target_attr);
		return;
	}

	node->leaf = false;

	node->value=most_common(data,target_attr);


	int size=distinct_attr_values.size();
	int sample_size=(int) sqrt(size);

	map <int,int> random_distinct_attr_values;

//	cout<<size<<endl;

	map<int,int>::iterator it=distinct_attr_values.begin();
	map<int,int>::iterator it_random=random_distinct_attr_values.begin();

	srand(i);

	while(random_distinct_attr_values.size()<sample_size) {
		int k=rand()%size;
//		cout<<k<<endl;
		std::advance(it,k);
		random_distinct_attr_values[it->first]=it->second;
		it=distinct_attr_values.begin();
	}

//	cout<<random_distinct_attr_values.size()<<endl;

	double max_gain = 0.0; int max_gain_attr = random_distinct_attr_values.begin()->first;
	for(auto i = random_distinct_attr_values.begin(); i != random_distinct_attr_values.end(); ++i) {
		double g = gain(data, i->first, target_attr);

//		cout << "g = " << g ;
//		cout << " i->first = " << i->first ;
//		cout << endl;

		if(g > max_gain){
			max_gain = g;
			max_gain_attr = i->first;
		}
	}

	node->attr_id = max_gain_attr;
//	cout << "max_gain_attr " << max_gain_attr << endl;
	int n = random_distinct_attr_values[max_gain_attr];
	node->next = new TreeNode *[n+1];
	for(int i = 0; i <= n; ++i) {
		TreeNode *new_node = new TreeNode();
		node->next[i] = new_node;
//		cout << "max gain attr,  i (v(i)) = " << max_gain_attr << " "<< i << endl;
//		cout << "distinct_attr_values[max_gain_attr] = " << n << endl;

		// new_data = subset that have the value vi (i) for A (max_gain_attr)
		vector<struct Data> new_data;
		for(unsigned j = 0; j < data.size(); ++j) {
			if(get_attr_value(max_gain_attr, data[j]) == i)
			new_data.push_back(data[j]);
		}

		if(new_data.empty()){
//			cout << "new_data_empty" << endl;
			new_node->leaf = true;
			new_node->value = most_common(data, target_attr);
		}
		else {
//			cout << "new_data not empty" <<endl;
			distinct_attr_values.erase(max_gain_attr);
			randomForest(new_data, target_attr, distinct_attr_values, new_node,i+rand());
//			cout << "return from recursive call" << endl;
		}
	}
}



