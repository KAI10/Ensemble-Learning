#define T 20

vector<DT_stump*> stumps;
vector<double> stump_weight;

string boost_predict(Example test)
{
    double ans = 0.0;
    for(int i=0; i<stumps.size(); i++){
        if(predict(stumps[i], test) == "yes") ans += stump_weight[i];
        else ans -= stump_weight[i];
    }
    return (ans > 0)? "yes": "no";
}

void adaBoost(vector<Example> &Examples)
{
    stumps.clear();
    stump_weight.clear();

    ///initial weight assignement
    vector<double> W(Examples.size(), 1.0/Examples.size()), CW(Examples.size());
    CW[0] = W[0]; for(int i=1; i<CW.size(); i++) CW[i] = CW[i-1] + W[i];

    for(int t=0; t<T; t++){
        //cout << "iteration: " << t+1 << endl;

        vector<Example> workExamples(Examples.size());
        for(int i=0; i<workExamples.size(); i++){
            int index = pickExampleIndex(CW);
            workExamples[i] = Examples[index];
        }

        DT_stump *temp = new DT_stump();
        createDT_stump(temp, workExamples);

        double errorVal = 0.0;
        for(int i=0; i<Examples.size(); i++){
            string prediction = predict(temp, Examples[i]);
            //cout << prediction << ' ' << Examples[i].getClass() << endl;
            if(mismatch(prediction, Examples[i])) errorVal += W[i];
        }

        //cout << "*****************************\n";

        ///cout << errorVal*100.0 << endl;
        if(errorVal > 0.5){
            //cout << "0.5 jhamela\n";
            break;
        }
        if(errorVal == 0){
            //cout << temp->feature_name << endl;
            //for(int i=1; i<=10; i++) cout << temp->child[i-1] << ' ';
            //cout << "\n0 equal Jhamela\n";
            break;
        }

        double alphat = 0.5*log((1-errorVal)/errorVal);

        ///update and normalize weight
        double sum = 0.0;
        for(int i=0; i<Examples.size(); i++){
            string prediction = predict(temp, Examples[i]);
            if(mismatch(prediction, Examples[i])) W[i] *= exp(alphat);
            else W[i] *= exp(-alphat);
            sum += W[i];
        }

        for(int i=0; i<Examples.size(); i++) W[i] /= sum;
        CW[0] = W[0]; for(int i=1; i<Examples.size(); i++) CW[i] = CW[i-1] + W[i];

        stumps.push_back(temp);
        stump_weight.push_back(alphat);

        ///check if all train data is correctly classified
        /*
        int wrong = 0;
        for(int i=0; i<Examples.size(); i++){
            string prediction = boost_predict(Examples[i]);
            if(mismatch(prediction, Examples[i])) wrong += 1;
        }

        //cout << wrong*100.0/Examples.size() << endl;
        if(wrong == 0){
            cout << "YO DONE\n";
            break;
        }
        */
    }
}
