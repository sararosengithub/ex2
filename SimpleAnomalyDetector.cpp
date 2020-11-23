
#include "SimpleAnomalyDetector.h"
#include "list"
#include "anomaly_detection_util.h"
#include "map"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub
    //CORRLATION = 0.9*1.1;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    // TODO Auto-generated destructor stub
    float correlation;
    float cor[] = {0, 0};
    vector<float> x, y;
    //get names of columns:
    std::vector<string> cn = ts.getColumnNames();
    //iterate over all combinations of pairs in timeseries:
    for (int i = 0; i < cn.size() - 1; i++) {
        cor[0]=0; cor[1]=0;
        for(int j=0;j<cn.size();j++){
            if(i==j)continue;
       // for (int j = i + 1; j < cn.size(); j++) {
            x = ts.getColumn(cn[i]);
            //  std::list<float> feature1(x.begin(), x.end());
            y = ts.getColumn(cn[j]);
            // std::list<float> feature2(x.begin(), x.end());
            //check correlation of specific pair:
            correlation = pearson(&x[0], &y[0], x.size());
            //if this is the most correlated feature(highest pearson)
            // to element i, save the element and the pearson value
            if (abs(correlation) > cor[1]) {
                cor[0] = j;
                cor[1] = correlation;
            }
        } //create a struct of correlated features
        if(cor[0]<i)continue;
        string f1 = cn[i], f2 = cn[cor[0]];
        //calculate linear regression:
        auto feature1 = ts.getColumn(cn[i]);
        auto feature2 = ts.getColumn(cn[cor[0]]);
        int size1 = feature1.size();

        Point **p = new Point*[size1];
       // Point *ps[size1];
        for (int k = 0; k < size1; k++)
            p[k] = new Point(feature1[k], feature2[k]);

        Line lin_reg = linear_reg(p, size1);
        float threshold = 0;
        for (int k = 0; k < size1; k++) {
            if (dev(*p[k], lin_reg) > threshold)
                threshold = dev(*p[k], lin_reg);
        }
        correlatedFeatures newcf(f1, f2, cor[1], lin_reg, threshold*1.1);
        cf.push_back(newcf);
        for (int j = 0; j <size1; ++j) {
            delete p[j];
        }delete []p;
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    // TODO Auto-generated destructor stub
    vector<AnomalyReport> reports;
    string f1, f2;
    Point *p;
    float x, y;
    //iterate over all correlated features in cf:
    for (int i=0; i < cf.size(); i++) {
        if (abs(cf.at(i).corrlation) > 0.99) {
            //get name of correlated features:
            f1 = cf.at(i).feature1;
            f2 = cf.at(i).feature2;
            for (int t = 0; t < ts.getNumOfRows(); t++) {
                x = ts.getValue(t, f1);
                y = ts.getValue(t, f2);
                p = new Point(x, y);
                if (dev(*p, cf.at(i).lin_reg) > cf.at(i).threshold) {
                    reports.push_back( AnomalyReport(f1 + "-" + f2, (long)t+1));
                }   delete p;
            }
        }
    }
    return reports;
}

