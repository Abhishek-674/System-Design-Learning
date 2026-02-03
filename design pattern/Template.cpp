/*
Client
  |
  v
MLTrainingTemplate
+ trainPipeline()
+ loadData()        (pure virtual)
+ preprocessData()  (pure virtual)
+ trainModel()      (pure virtual)
+ evaluateModel()   (pure virtual)
+ saveModel()       (concrete)
        ▲
   ┌────┴─────────------─┐
   |                     |
LinearRegressionTraining   NeuralNetworkTraining

*/

#include <iostream>
using namespace std;

class MLTrainingTemplate {
public:
    // Template Method
    void trainPipeline() {
        loadData();
        preprocessData();
        trainModel();
        evaluateModel();
        saveModel();
    }

    virtual ~MLTrainingTemplate() {}

protected:
    virtual void loadData() = 0;
    virtual void preprocessData() = 0;
    virtual void trainModel() = 0;
    virtual void evaluateModel() = 0;

    // Common step
    void saveModel() {
        cout << "Saving trained model" << endl;
    }
};
class LinearRegressionTraining : public MLTrainingTemplate {
protected:
    void loadData() override {
        cout << "Loading CSV data" << endl;
    }

    void preprocessData() override {
        cout << "Normalizing features" << endl;
    }

    void trainModel() override {
        cout << "Training Linear Regression model" << endl;
    }

    void evaluateModel() override {
        cout << "Evaluating using MSE" << endl;
    }
};
class NeuralNetworkTraining : public MLTrainingTemplate {
protected:
    void loadData() override {
        cout << "Loading image dataset" << endl;
    }

    void preprocessData() override {
        cout << "Augmenting images" << endl;
    }

    void trainModel() override {
        cout << "Training Neural Network" << endl;
    }

    void evaluateModel() override {
        cout << "Evaluating using Accuracy" << endl;
    }
};
int main() {
    MLTrainingTemplate* trainer;

    trainer = new LinearRegressionTraining();
    trainer->trainPipeline();
    delete trainer;

    cout << "-------------------" << endl;

    trainer = new NeuralNetworkTraining();
    trainer->trainPipeline();
    delete trainer;

    return 0;
}

/*
use when
You want to enforce execution order
Steps must run in a specific sequence
Subclasses should not change the order
hower the step implementtion can be changed
*/