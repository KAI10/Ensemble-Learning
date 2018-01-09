/*
 * main.cpp
 *
 * Created by: Ashik <ashik@KAI10>
 * Created on: Sat, 25 Mar 2017
 */


#include <bits/stdc++.h>
#include <random>
using namespace std;

typedef long long ll;

#define mem(list, val) memset(list, (val), sizeof(list))
#define pb push_back

#include "objects.hpp"
#include "utilities.hpp"
#include "Decision_Tree_utilities.hpp"
#include "boost_utilities.hpp"

void crossValidate(vector<Example> &Examples, int k)
{
    cout << "\n########### " << k << " fold cross validation ################\n";

    int testDataSize = Examples.size()/k;
    vector<Example> data[k];

    for(int i=0; i<k; i++){
		if(i != k-1) data[i].insert(data[i].end(), Examples.begin()+i*testDataSize, Examples.begin()+(i+1)*testDataSize);
		else data[i].insert(data[i].end(), Examples.begin()+i*testDataSize, Examples.end());
	}

	double sumAccuracy = 0;

    for(int i=0; i<k; i++){

        //cout << "\n*********** part " << i+1 << " *******************\n";
        //******************* processing ***************************//

		vector<Example> Examples;
		for(int j=0; j<k; j++){
			if(j == i) continue;
			Examples.insert(Examples.end(), data[j].begin(), data[j].end());
		}

		//DT_stump *master = new DT_stump();
        //createDT_stump(master, Examples);
        adaBoost(Examples); ///Main Processing

		int true_positive = 0, false_negative = 0, false_positive = 0, true_negative = 0;

		for(int j=0; j<data[i].size(); j++){
            string prediction = boost_predict(data[i][j]); //making prediction by decision tree
            //string prediction = predict(master, data[i][j]); //making prediction by decision tree

            if(prediction == "yes" && data[i][j].getClass() == 1){
                true_positive++;
            }
            else if(prediction == "yes" && data[i][j].getClass() == 0){
                false_positive++;
            }
            else if(prediction == "no" && data[i][j].getClass() == 1){
                false_negative++;
            }
            else{
                true_negative++;
            }
        }
        
        //******************** reporting *************************//

        int condition_positive = true_positive + false_negative, condition_negative = false_positive + true_negative;
        int test_outcome_positive = true_positive + false_positive, test_outcome_negative = false_negative + true_negative;

        //cout << "SIZE: " << data[i].size() << endl;

        assert(condition_positive + condition_negative == data[i].size());
        assert(test_outcome_positive + test_outcome_negative == data[i].size());

        double accuracy = 100.0*(true_positive + true_negative)/data[i].size(); //High is GOOD

        sumAccuracy += accuracy;

        /*
        cout << "accuracy:\t" << accuracy << endl;
        */
	}

    cout << "Average accuracy:\t" << sumAccuracy/k << endl;
}

int main()
{
    //srand(time(NULL));

    vector<Example> Examples;
    readData("data_set.csv", Examples);

    //random_shuffle(Examples.begin(), Examples.end());

    //adaBoost(Examples);

    crossValidate(Examples, 5);
    crossValidate(Examples, 10);
    crossValidate(Examples, 20);
    crossValidate(Examples, Examples.size());

    return 0;
}
