//============================================================================
// Name        : ID3.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <ctime>
#include "datautil.h"
#include "tree.h"

using namespace std;

/**
 * Prints the tree
 * @param head: root node of the tree
 * @param distinct_attr_values: contains the number of different values an attribute can take
 */
void printTree(TreeNode *head, map<int, int> distinct_attr_values) {
	if (head->leaf == false) {
		cout << head->attr_id << endl;
		for (int t = 0; t < distinct_attr_values[head->attr_id]; ++t) {
			printTree(head->next[t], distinct_attr_values);
		}
	} else {
		cout << "value = " << head->value << endl;
	}
}

/**
 * Function to get the size of a Tree
 * @param head: root node of the tree
 * @param distinct_attr_values: contains the number of different values an attribute can take
 * @return integer value representing the number of nodes in the tree
 */

int treeSize(TreeNode *head, map<int, int> distinct_attr_values) {

	if (head == NULL)
		return 0;
	else if (head->leaf == true)
		return 1;
	else {
		int count = 0;
		int n = distinct_attr_values[head->attr_id];
		for (int i = 0; i < n; i++)
			count += treeSize(head->next[i], distinct_attr_values);
		return count;
	}

}

/**
 * Function to get the number of distinct values an attribute can take
 * @param v: Complete dataset with datapoints having the attributes in question
 * @param attr: specifies which attribute
 * @return integer value containing number of distinct values attribute takes
 */

int get_distinct_attr_values(vector<struct Data> &v, int attr) {
	set<int> points;
	for (unsigned i = 0; i < v.size(); ++i) {
		points.insert(get_attr_value(attr, v[i]));
	}
	return points.size();
}

/**
 * Function to predict the value of new datapoint using the ID3 tree
 * @param d: Datapoint
 * @param head: The root node of the ID3 tree
 * @return boolean value representing the target attribute (In this case income >50K or not)
 */

bool predicted_value(struct Data &d, TreeNode *head) {
	if (head->leaf == true)
		return head->value;
	return predicted_value(d, head->next[get_attr_value(head->attr_id, d)]);
}

/**
 *Function to predict value given data point using Random Forest (mode of values predicted by member trees)
 * @param d: Datapoint
 * @param forest: Vector of root nodes of the different trees in the forest
 * @return boolean value representing the target attribute (In this case income >50K or not)
 */

bool predicted_value(struct Data &d, vector<TreeNode*> forest) {
	vector<TreeNode*>::iterator it = forest.begin();
	int forest_result = 0;
	for (; it != forest.end(); it++) {

		if (predicted_value(d, *it))
			forest_result++;
		else
			forest_result--;
	}
	if (forest_result > 0)
		return true;
	else
		return false;
}

/**
 * Function to get the accuracy of a given ID3 tree with respect to a dataset
 * @param v: Dataset vector
 * @param head: root node for ID3 tree
 * @param target_attr: Attribute to be predicted
 * @return Percentage accuracy of Decision tree
 */

double accuracy(vector<struct Data> v, TreeNode *head, int target_attr) {
	int correct = 0;
	for (unsigned i = 0; i < v.size(); ++i) {
		if (get_attr_value(target_attr, v[i]) == predicted_value(v[i], head)) {
			correct++;
		}
	}
	return (double) correct / v.size();
}

/**
 * Function to get accuracy of given random forest
 * @param v: Dataset vector
 * @param forest: forest vector (vector of root nodes of multiple trees)
 * @param target_attr: attribute to be predicted
 * @return Percentage accuracy of random forest on given data
 */

double accuracy(vector<struct Data> v, vector<TreeNode*> forest,
		int target_attr) {
	int correct = 0;
	for (unsigned i = 0; i < v.size(); ++i) {
		if (get_attr_value(target_attr, v[i])
				== predicted_value(v[i], forest)) {
			correct++;
		}
	}
	return (double) correct / v.size();
}


/**
 * Function to get the precision of a Decision tree
 * @param v: Dataset
 * @param head: root node of decision tree
 * @param target_attr: attribute to be predicted
 * @return percentage precision of tree on given data
 */

double precision(vector<struct Data> v, TreeNode *head, int target_attr) {
	int true_positive = 0;
	int true_negative = 0;
	int false_positive = 0;
	int false_negative = 0;
	for (unsigned i = 0; i < v.size(); ++i) {
		if (get_attr_value(target_attr, v[i]) == predicted_value(v[i], head)
				&& get_attr_value(target_attr, v[i]) == 1) {
			true_positive++;
		} else if (get_attr_value(target_attr, v[i])
				== predicted_value(v[i], head)
				&& get_attr_value(target_attr, v[i]) == 0) {
			true_negative++;
		} else if (get_attr_value(target_attr, v[i])
				!= predicted_value(v[i], head)
				&& get_attr_value(target_attr, v[i]) == 0) {
			false_positive++;
		} else if (get_attr_value(target_attr, v[i])
				!= predicted_value(v[i], head)
				&& get_attr_value(target_attr, v[i]) == 1) {
			false_negative++;
		}
	}
	return (double) true_positive / (true_positive + false_positive);

}

/**
 *Function to get the precision of a Random Forest
 * @param v: Dataset
 * @param forest: Forest vector containing root nodes of trees
 * @param target_attr: attribute to be predicted
 * @return percentage precision of random forest on given data
 */

double precision(vector<struct Data> v, vector<TreeNode*> forest,
		int target_attr) {
	int true_positive = 0;
	int true_negative = 0;
	int false_positive = 0;
	int false_negative = 0;
	for (unsigned i = 0; i < v.size(); ++i) {
		if (get_attr_value(target_attr, v[i]) == predicted_value(v[i], forest)
				&& get_attr_value(target_attr, v[i]) == 1) {
			true_positive++;
		} else if (get_attr_value(target_attr, v[i])
				== predicted_value(v[i], forest)
				&& get_attr_value(target_attr, v[i]) == 0) {
			true_negative++;
		} else if (get_attr_value(target_attr, v[i])
				!= predicted_value(v[i], forest)
				&& get_attr_value(target_attr, v[i]) == 0) {
			false_positive++;
		} else if (get_attr_value(target_attr, v[i])
				!= predicted_value(v[i], forest)
				&& get_attr_value(target_attr, v[i]) == 1) {
			false_negative++;
		}
	}
	return (double) true_positive / (true_positive + false_positive);
}

/**
 *Function to get recall of a decision tree for given dataset
 * @param v: Dataset
 * @param head: root node of Decision tree
 * @param target_attr: attribute to be predicted
 * @return percentage recall of decision tree for the dataset
 */

double recall(vector<struct Data> v, TreeNode *head, int target_attr) {
	int true_positive = 0;
	int true_negative = 0;
	int false_positive = 0;
	int false_negative = 0;
	for (unsigned i = 0; i < v.size(); ++i) {
		if (get_attr_value(target_attr, v[i]) == predicted_value(v[i], head)
				&& get_attr_value(target_attr, v[i]) == 1) {
			true_positive++;
		} else if (get_attr_value(target_attr, v[i])
				== predicted_value(v[i], head)
				&& get_attr_value(target_attr, v[i]) == 0) {
			true_negative++;
		} else if (get_attr_value(target_attr, v[i])
				!= predicted_value(v[i], head)
				&& get_attr_value(target_attr, v[i]) == 0) {
			false_positive++;
		} else if (get_attr_value(target_attr, v[i])
				!= predicted_value(v[i], head)
				&& get_attr_value(target_attr, v[i]) == 1) {
			false_negative++;
		}
	}
	return (double) true_positive / (true_positive + false_negative);

}

/**
 * Function to get recall of a random forest for given dataset
 * @param v: Dataset
 * @param forest: Forest vector containing root nodes of trees
 * @param target_attr: attribute to be predicted
 * @return percentage recall of random forest for the dataset
 */

double recall(vector<struct Data> v, vector<TreeNode*> forest,
		int target_attr) {
	int true_positive = 0;
	int true_negative = 0;
	int false_positive = 0;
	int false_negative = 0;
	for (unsigned i = 0; i < v.size(); ++i) {
		if (get_attr_value(target_attr, v[i]) == predicted_value(v[i], forest)
				&& get_attr_value(target_attr, v[i]) == 1) {
			true_positive++;
		} else if (get_attr_value(target_attr, v[i])
				== predicted_value(v[i], forest)
				&& get_attr_value(target_attr, v[i]) == 0) {
			true_negative++;
		} else if (get_attr_value(target_attr, v[i])
				!= predicted_value(v[i], forest)
				&& get_attr_value(target_attr, v[i]) == 0) {
			false_positive++;
		} else if (get_attr_value(target_attr, v[i])
				!= predicted_value(v[i], forest)
				&& get_attr_value(target_attr, v[i]) == 1) {
			false_negative++;
		}
	}
	return (double) true_positive / (true_positive + false_negative);

}

/**
 * Driver function for random forest
 * @param data_test: Test Dataset, to print accuracies of individual trees(for analysis purpose)
 * @param data: Training Dataset
 * @param target_attr: Attribute to predict
 * @param distinct_attr_values: Map containing number of distinct values an attribute can take for each attribute
 * @param n: number of decision trees to have in the forest
 * @return vector of root nodes of the trees in the forest
 */

vector<TreeNode*> executeRandomForest(vector<struct Data> data_test,
		vector<struct Data> data, int target_attr,
		map<int, int> distinct_attr_values, int n) {
	vector<TreeNode*> v;

	for (int i = 0; i < n; i++) {
		TreeNode *head = new TreeNode();
		randomForest(data, target_attr, distinct_attr_values, head,
				i + time(NULL));
		cout << "Accuracy for tree " << i << " with root attribute "
				<< head->attr_id << " is:"
				<< accuracy(data_test, head, target_attr) << endl;
		v.push_back(head);
	}
	return v;
}


/**
 *Function to prune an existing decision tree using reduced error pruning
 * @param curr: pointer to current node in traversal
 * @param head: pointer to root node of decision tree
 * @param valid_set: validation data set (used to test accuracy change)
 * @param distinct_attr_values: map containing number of distinct values an attribute can take for each attribute
 * @param target_attr: attribute to be predicted
 */

void pruning(TreeNode *curr, TreeNode *head, vector<struct Data> valid_set,
		map<int, int> distinct_attr_values, int target_attr) {

	if (curr == NULL)
		return;
	if (curr->leaf == true)
		return;

	int n = distinct_attr_values[curr->attr_id];
	for (int i = 0; i < n; i++) {

		pruning(curr->next[i], head, valid_set, distinct_attr_values,
				target_attr);
	}
	for (int i = 0; i < n; i++) {

		if (curr->next[i]->leaf == false) {

			double curr_acc = accuracy(valid_set, head, target_attr);
			curr->next[i]->leaf = true;
			double post_acc = accuracy(valid_set, head, target_attr);

			if (curr_acc > post_acc) {
				curr->next[i]->leaf = false;
			}
		}
		i++;
	}
}

//Driver function to run the program

int main(int argc, char** argv) {

	int x;

	auto training_data = get_dataset(
			"data\\adult.data");

	auto testing_data = get_dataset(
			"data\\adult.test");

	map<int, int> distinct_attr_values;

	for (int i = 0; i < 14; ++i) {
		distinct_attr_values[i] = get_distinct_attr_values(training_data, i);

	}
	distinct_attr_values[13] = 41;


	TreeNode *head = new TreeNode();

	vector<TreeNode*> v;

//	auto train_data_prune = get_dataset(
//			"C:\\Users\\Binki\\workspaceC\\ID3\\data\\adult_train.data");

//	auto valid_data_prune = get_dataset(
//			"C:\\Users\\Binki\\workspaceC\\ID3\\data\\adult_validate.data");

	vector<struct Data> train_data_prune;
	vector<struct Data> valid_data_prune;

	unsigned cut = 1.0 / 2.0 * training_data.size();

	for (unsigned i = 0; i < cut; ++i) {
		train_data_prune.push_back(training_data[i]);
	}
	for (unsigned i = cut; i < v.size(); ++i) {
		valid_data_prune.push_back(training_data[i]);
	}

	std::clock_t start;


	while (1) {

		cout << "Enter \n1.1 for ID3\n"
				"2.2 for ID3 with Pruning with a 1/2 cut\n"
				"3.3 for Random Forest\n"
				"4.4 for exit." << endl;

		cin >> x;

		if (x == 1) {

			start = std::clock();

			id3(training_data, 14, distinct_attr_values, head);

			std::cout << "Time: "
					<< (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000)
					<< " ms" << std::endl;

			cout << "accuracy on testing data ="
					<< accuracy(testing_data, head, 14) << endl;

			double _precision = precision(testing_data, head, 14);
			double _recall = recall(testing_data, head, 14);

			cout << "precision on testing data ="
					<< _precision << endl;

			cout << "recall on testing data =" << _recall
					<< endl;

			cout<< "F measure: " << 2*(_precision*_recall)/(_precision+_recall)<<endl;


		}
		if (x == 2) {

			start = std::clock();

			id3(train_data_prune, 14, distinct_attr_values, head);

			cout << "Tree size pre pruning="
					<< treeSize(head, distinct_attr_values) << endl;

			cout << "accuracy on testing data pre-pruning ="
					<< accuracy(testing_data, head, 14) << endl;

			pruning(head, head, valid_data_prune, distinct_attr_values, 14);
			std::cout << "Time: "
					<< (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000)
					<< " ms" << std::endl;

			cout << "accuracy on testing data post pruning="
					<< accuracy(testing_data, head, 14) << endl;

			double _precision = precision(testing_data, head, 14);
			double _recall = recall(testing_data, head, 14);

			cout << "precision on testing data ="
					<< _precision << endl;

			cout << "recall on testing data =" << _recall
					<< endl;

			cout<< "F measure: " << 2*(_precision*_recall)/(_precision+_recall)<<endl;


			cout << "Tree size post pruning="
					<< treeSize(head, distinct_attr_values) << endl;

		}
		if (x == 3) {

			start = std::clock();

			v = executeRandomForest(testing_data, training_data, 14,
					distinct_attr_values, 100);
			std::cout << "Time: "
					<< (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000)
					<< " ms" << std::endl;

			cout << "accuracy on testing data = "
					<< accuracy(testing_data, v, 14) << endl;

			double _precision = precision(testing_data, v, 14);
			double _recall = recall(testing_data, v, 14);

			cout << "precision on testing data ="
					<< _precision << endl;

			cout << "recall on testing data =" << _recall
					<< endl;

			cout<< "F measure: " << 2*(_precision*_recall)/(_precision+_recall)<<endl;
		}

		if (x == 4) {
			break;
		}

	}

	cout << "end";
}

