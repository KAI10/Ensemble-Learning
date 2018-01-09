void createDT_stump(DT_stump* node, vector<Example> &Examples)
{
    int siz = Examples.size(), pos = getPositiveCount(Examples), neg = siz-pos;
    if(pos == 0){
        node->set("no");
        return;
    }
    if(neg == 0){
        node->set("yes");
        return;
    }

    string prevMajorityLabel = (pos >= neg)? "yes": "no";

    double maxInfoGain = getInformationGain(Examples, 0);
    int bestFeatureIndex = 0;

    for(int i=1; i<Features.size()-1; i++){
        double infoGain = getInformationGain(Examples, i);
        if(infoGain > maxInfoGain){
            maxInfoGain = infoGain;
            bestFeatureIndex = i;
        }
    }

    //cout << bestFeatureIndex << ' ' << Features[bestFeatureIndex] << endl;
    node->set(Features[bestFeatureIndex], bestFeatureIndex);

    for(int i=1; i<=10; i++){
        vector<Example> temp;
        getFilteredExamples(Examples, bestFeatureIndex, i, temp);

        int tsiz = temp.size(), tpos = getPositiveCount(temp), tneg = tsiz - tpos;
        string majorityLabel = (tpos >= tneg)? "yes": "no";

        if(tsiz == 0) node->child.push_back(prevMajorityLabel);
        else node->child.push_back(majorityLabel);
    }
}

string predict(DT_stump *node, Example test)
{
    if(node->getLabel() == "yes" || node->getLabel() == "no") return node->getLabel();
    return node->child[test.getFeatureVal(node->getFeatureIndex())-1];
}
