vector<string> Features;

random_device rd;
uniform_real_distribution<double> rng(0.0, 1.0);

void readData(string filename, vector<Example> &Examples)
{
    ifstream fin;
    fin.open(filename);

    string line;
    getline(fin, line);

    istringstream ss(line);

    string token;
    while(getline(ss, token, ',')){
        Features.push_back(token);
    }

    Example temp;

    while(getline(fin, line)){
        ss.clear();
        ss.str(line);
        while(getline(ss,token,',')) temp.exp.push_back(stoi(token));
        Examples.push_back(temp);
        temp.exp.clear();
    }

    fin.close();
}

int getPositiveCount(vector<Example> Exp)
{
    int ret = 0;
    for(int i=0; i<Exp.size(); i++){
        if(Exp[i].getClass() == 1) ret++;
    }
    return ret;
}

void getFilteredExamples(vector<Example>Exp, int feature, int fVal, vector<Example>&ret)
{
    for(int i=0; i<Exp.size(); i++){
        if(Exp[i].getFeatureVal(feature) == fVal){
            ret.push_back(Exp[i]);
        }
    }
}

double getInformationGain(vector<Example> Exp, int feature)
{
    int siz = Exp.size(), pos = getPositiveCount(Exp), neg = siz-pos;
    double curEntropy = -log2(1.0*pos/siz)*pos/siz - log2(1.0*neg/siz)*neg/siz;

    double childEntropy = 0;
    for(int i=1; i<=10; i++){
        vector<Example> temp;
        getFilteredExamples(Exp, feature, i, temp);
        siz = temp.size(), pos = getPositiveCount(temp), neg = siz-pos;

        if(pos == 0 || neg == 0);
        else childEntropy -= (log2(1.0*pos/siz)*pos/siz + log2(1.0*neg/siz)*neg/siz)*siz/Exp.size();
    }

    return curEntropy - childEntropy;
}


double getRandomDouble()
{
    //return (double)rand()/RAND_MAX;
    return rng(rd);
}

int pickExampleIndex(vector<double> &CW)
{
    double num = getRandomDouble();
    vector<double>:: iterator it = lower_bound(CW.begin(), CW.end(), num);
    return (it - CW.begin());
}

bool mismatch(string prediction, Example test){
    return (prediction == "yes" && test.getClass() == 0) || (prediction == "no" && test.getClass() == 1);
}

