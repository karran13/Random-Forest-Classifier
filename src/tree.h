/*
 * tree.h
 *
 *  Created on: Sep 25, 2017
 *      Author: Binki
 */

#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <set>
#include <map>
#include "datautil.h"


struct TreeNode {
	int attr_id;
	bool leaf;
	bool value;
	struct TreeNode **next;
};


double gain(std::vector<struct Data> &v, int attr_id);

//double entropy(const std::vector<struct Data> &v);

void id3(std::vector<struct Data> data, int target_attr, std::map<int, int> distinct_attr_values, struct TreeNode *node);


void randomForest(std::vector<struct Data> data, int target_attr, std::map<int, int> distinct_attr_values, struct TreeNode *node,int i);


#endif /* TREE_H_ */
