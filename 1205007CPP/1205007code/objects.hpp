struct Example{
    vector<int> exp;

    int getFeatureVal(int featureIndex) {return exp[featureIndex];}
    int getClass(){return exp[exp.size()-1]; }
};

struct DT_stump{
    string feature_name;
    int featureIndex;

    vector<string> child;

    void set(string label){
        feature_name = label;
    }
    void set(string label, int index){
        feature_name = label;
        featureIndex = index;
    }

    string getLabel(){
		return feature_name;
	}
    int getFeatureIndex(){
		return featureIndex;
	}
};
