# Random-Forest-Classifier
C++ implementation of a random forest classifier with reduced error pruning methods

Implemented Algorithms:
1.	ID3 Decision Tree Algorithm
2.	Reduced Error Pruning 
3.	Random Forest Algorithm


**ID3 Decision Tree Algorithm**

Measures of performance:

•	Time taken: 238 ms

•	Accuracy on testing data: 81.75%

•	Precision on testing data: 63.04%

•	Recall on testing data: 54.99%

•	F measure: 58.74%

ID3 with Reduced Error Pruning

We halved the training data set, and used the first half for training the decision tree and the second half as a validation set to prune the tree.

Measures of performance:

•	Time taken: 141 ms

•	Accuracy on testing data pre pruning: 80.85%

•	Tree size pre pruning: 14,861 nodes

•	Accuracy on testing data post pruning: 81.90%

•	Tree size post pruning: 927 nodes

•	Precision on testing data: 70.05%

•	Recall on testing data: 40.87%

•	F measure: 51.62%

Random Forest Algorithm

For the random forest we used a 100 decision trees to expand the variance we cover. Sample for attribute’s random sampling was taken as square root of total number of attributes.

Measures of performance:

•	Time taken: 10019ms

•	Accuracy on testing data: 83.34%

•	Precision on testing data: 68.76%

•	Recall on testing data: 54.03%

•	F measure: 60.51%

