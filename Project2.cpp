#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

//IMPORTANT: Program is generalized as best as possible. However, cannot generalize the declaration of
//           the 2D array declared in main() labeled as "data" (array size must be a constant).
//           So, to test either the small data set or the large data set, you must comment out one of the 
//           data array declarations.
//           There are 2 declarations:
//                  double data[300][11] represents the small data set
//                  double data[500][101] represents the large data set
//           The default test data set is the small one. To test the large data set, comment out the current
//           declaration for data and uncomment the other declaration for data.
//IMPORTANT: Must also change the values for data[][] when it is passed into a function, i.e. in kCrossValidation
//           and forwardSelection + backwardsElimination.

int testDataRows = 999;     //used to generalize program to work for both small and large test data set
int testDataColumns = 999;  //used to generalize program to work for both small and large test data set

double kCrossValidation(double data[][11], int currentFeatures[], int featureToAdd) {  //returns accuracy of set of features
    double dataCopy[testDataRows][testDataColumns]; //copy of data[][] to be used for classification
    double num_correct_classification = 0;          //number of correct classifications (max 300)
    double object_to_classify_label;                //the class of the object being classified
    double object_to_classify[testDataColumns - 1]; //the features of the object being classified
    double nearest_neighbor_distance;               //nearest neighbor's distance to object being classified
    int nearest_neighbor_location;                  //nearest neighbor's row location
    double nearest_neighbor_label;                  //nearest neighbor's class
    double distance = 0;                            //used to determine nearest neighbor
    double sum = 0;                                 //used to calculate distance
    double accuracy;                                //accuracy obtained using a given set of features
    int check = 0;                                  //used to check if a feature needs to be set to 0

    for (int x = 0; x < testDataRows - 1; ++x) {    //creates a copy of data[][]
        for (int y = 0; y < testDataColumns; ++y) {
            dataCopy[x][y] = data[x][y];
        }
    }

    for (int a = 1; a < testDataColumns; ++a) {             //if a feature is not in the set of currentFeatures
        check = 0;                                          //and is not featureToAdd, set it to 0 for every instance.
        for (int b = 0; b < testDataColumns - 1; ++b) {
            if (currentFeatures[b] == a) {
                check = 1;
            }
        }
        if (check == 0) {  
            if (a != featureToAdd) {
                for (int c = 0; c < testDataRows - 1; ++c) {
                    dataCopy[c][a] = 0;
                }
            }
        }
    }

    for (int i = 0; i < testDataRows - 1; ++i) {            //nearest neighbor algorithm; test every instance (row)
        object_to_classify_label = dataCopy[i][0];          //store actual class of object being classified
        for (int j = 1; j < testDataColumns; ++j) {         
            object_to_classify[j - 1] = dataCopy[i][j];     //store features of object being classified
        }
        nearest_neighbor_distance = 99999999;
        nearest_neighbor_location = 99999999;
        for (int k = 0; k < testDataRows - 1; ++k) {        //check distance between object being classified and every other object
            sum = 0;
            if (k != i) {
                for (int L = 1; L < testDataColumns; ++L) {
                    sum = sum + pow(dataCopy[i][L] - dataCopy[k][L], 2);    //distance = sqrt((object1.feature1-object2.feature1)^2)
                }                                                           // +...+(object1.featureN-object2.featureN)^2)
                distance = sqrt(sum);
                if (distance < nearest_neighbor_distance) {
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_location = k;
                    nearest_neighbor_label = dataCopy[nearest_neighbor_location][0];
                }
            }
        }
        if (object_to_classify_label == nearest_neighbor_label) {   //if nearest neighbor's class is actual class of object
            ++num_correct_classification;                           //being classified, increment number of correct classifications.
        }
    }
    accuracy = num_correct_classification / testDataRows;
    return accuracy;             //accuracy obtained by using a specific subset of features
}

void forwardSelection(double data[][11]) {       //***CHANGE VALUE IN 2ND BRACKET BASED ON WHICH TEST YOU USE***
    int currentFeatures[testDataColumns - 1];     //current set of features, initialized w/ 999s
    int featureToAdd;            //feature that provides best accuracy when added to current set of features
    double bestAccuracy;         //used to keep track of which feature subset provides the best accuracy
    double accuracy;             //the accuracy that a feature provides when added to current set of features
    double realBestAccuracy = 0; //used to keep track of the ACTUAL best accuracy
    vector<int> bestFeatureSet;  //used to keep track of set of features that provides greatest overall accuracy
    int check = 0;               //used to check if featured is already in current set before considering adding it
    int check2 = 1;              //used to help with output format

    cout << endl << "This dataset has " << testDataColumns - 1 << " features (not including the class attribute) with "
    << testDataRows << " instances." << endl << endl << "Beginning search." << endl << endl;

    for (int w = 0; w < testDataColumns - 1; ++w) {     //initializing currentFeatures array
        currentFeatures[w] = 999;
    }

    accuracy = kCrossValidation(data, currentFeatures, 0);  //accuracy of empty subset
    cout << "On level 0 of the search tree:" << endl;
    cout << "Using feature(s) {} gives accuracy of " << accuracy << endl << endl;

    for (int i = 1; i < testDataColumns; ++i) {      
        cout << "On level " << i << " of the search tree:" << endl;
        featureToAdd = 0;
        bestAccuracy = 0;
        for (int k = 1; k < testDataColumns; ++k) {
            check = 0;
            check2 = 1;
            if (currentFeatures[0] != 999) {        //if feature k is already in currentFeatures, don't consider adding it
                for (int j = 0; j < testDataColumns; ++j) {
                    if (k == currentFeatures[j]) {
                        check = 1;
                    }
                }
            }
            if (check == 0) {                       //obtain accuracy after adding feature k to currentFeatures
                accuracy = kCrossValidation(data, currentFeatures, k);
                currentFeatures[i - 1] = k;
                cout << "Using feature(s) {";
                for (int m = 0; m < testDataColumns - 1; ++m) {
                    if (currentFeatures[m] != 999) {
                        if (check2 == i) {
                            cout << currentFeatures[m];
                        }
                        else {
                            cout << currentFeatures[m] << ", ";
                            ++check2;
                        }
                    }
                }
                cout << "} gives accuracy of " << accuracy << endl;
                currentFeatures[i - 1] = 999;
                if (accuracy > bestAccuracy) {      //if a subset's accuracy is best out of every other subset in the level,
                    bestAccuracy = accuracy;        //update bestAccuracy and add feature k to currentFeatures (for real)
                    featureToAdd = k;
                }
            }
        }
        if (bestAccuracy > realBestAccuracy) {      //if subset's accuracy is best out of every other subset in EVERY level,
            realBestAccuracy = bestAccuracy;        //update realBestAccuracy and add feature k to the bestFeatureSet
            bestFeatureSet.push_back(featureToAdd);
        }
        if (bestAccuracy < realBestAccuracy && i != testDataColumns - 1) {  //if adding a feature results in a lower accuracy
            cout << "Warning: adding feature " << featureToAdd << 
            " results in a decreased accuracy. Continuing search." << endl;
        }
        currentFeatures[i - 1] = featureToAdd;      //adding feature k to currentFeatures
        cout << "Adding feature " << featureToAdd << " was best with an accuracy of " 
        << bestAccuracy << ". Added feature " << featureToAdd << " to current set of features" << endl << endl;
    }
    cout << "Search completed. The best feature subset is {";
    for (int n = 0; n < bestFeatureSet.size(); ++n) {       //output the actual overall best feature subset
        if (n == bestFeatureSet.size() - 1) {
            cout << bestFeatureSet.at(n);
        }
        else {
            cout << bestFeatureSet.at(n) << ", ";
        }
    }
    cout << "} with an accuracy of " << realBestAccuracy << ".";     //output the actual overall best accuracy

    return;
}

void backwardElimination (double data[][11]) {   //***CHANGE VALUE IN 2ND BRACKET BASED ON WHICH TEST YOU USE***
    int currentFeatures[testDataColumns - 1];     //current set of features, initialized w/ every feature
    int featureToRemove;         //feature that provides best accuracy when removed from current set of features
    double bestAccuracy;         //used to keep track of which feature subset provides the best accuracy
    double accuracy;             //the accuracy that a feature provides when removed from current set of features
    double realBestAccuracy = 0; //used to keep track of the ACTUAL best accuracy
    vector<int> bestFeatureSet;  //used to keep track of set of features that provides greatest overall accuracy
    vector<int> storage;         //helps keep track of which features should be added to bestFeatureSet
    int check = 0;               //used to check if featured is already not in current set before considering removing it
    int check2 = 1;              //used for output formatting purposes

    cout << endl << "This dataset has " << testDataColumns - 1 << " features (not including the class attribute) with "
    << testDataRows << " instances." << endl << endl << "Beginning search." << endl << endl;

    for (int w = 0; w < testDataColumns - 1; ++w) {     //initializing currentFeatures array w/ every feature
        currentFeatures[w] = w + 1;
    }

    for (int i = testDataColumns - 1; i > 0; --i) {     //format similar to forward selection, but with different indexes
        cout << "On level " << i << " of the search tree:" << endl;
        featureToRemove = 0;
        bestAccuracy = 0;
        for (int k = testDataColumns - 1; k > 0; --k) {
            check = 0;
            check2 = 1;
            if (currentFeatures[k - 1] == 0) {       //if a feature is already removed, don't consider removing it
                check = 1;
            }
            if (check == 0) {                        //obtain accuracy achieved by removing a specific feature
                currentFeatures[k - 1] = 0;
                accuracy = kCrossValidation(data, currentFeatures, 999);
                cout << "Using feature(s) {";
                for (int m = 0; m < testDataColumns - 1; ++m) {
                    if (currentFeatures[m] != 0) {
                        if (check2 == i - 1) {
                            cout << currentFeatures[m];
                        }
                        else {
                            cout << currentFeatures[m] << ", ";
                            ++check2;
                        }
                    }
                }
                cout << "} gives accuracy of " << accuracy << endl;
                currentFeatures[k - 1] = k;
                if (accuracy > bestAccuracy) {      //if a subset's accuracy is greater than that of
                    bestAccuracy = accuracy;        //every other subset in the same level
                    featureToRemove = k;            //update bestAccuracy and remove feature k from currentFeatures
                }
            }
        }
        if (bestAccuracy > realBestAccuracy) {      //if a subset's accuracy is greater than that of
            realBestAccuracy = bestAccuracy;        //every other subset in EVERY level
            storage.push_back(featureToRemove);     //update realBestAccuracy and storage
        }
        if (bestAccuracy < realBestAccuracy && i != 1) {    //if removing a feature results in a decrease in accuracy
            cout << "Warning: removing feature " << featureToRemove << 
            " results in a decreased accuracy. Continuing search." << endl;
        }
        currentFeatures[featureToRemove - 1] = 0;       //remove feature k from currentFeatures
        cout << "Removing feature " << featureToRemove << " was best with an accuracy of " 
        << bestAccuracy << ". Removed feature " << featureToRemove << " from current set of features" << endl << endl;
    }
    cout << "Search completed. The best feature subset is {";
    int check3 = 0;
    for (int a = 0; a < testDataColumns - 1; ++a) {     //store the actual best feature subset in bestFeatureSet
        check3 = 0;
        for (int b = 0; b < storage.size(); ++b) {
            if (storage.at(b) != a + 1) {
                ++check3;
            }
            if (check3 == storage.size()) {
                bestFeatureSet.push_back(a + 1);
            }
        }
    }
    for (int n = 0; n < bestFeatureSet.size(); ++n) {   //output the actual overall best feature subset
        if (n == bestFeatureSet.size() - 1) {
            cout << bestFeatureSet.at(n);
        }
        else {
            cout << bestFeatureSet.at(n) << ", ";
        }
    }
    cout << "} with an accuracy of " << realBestAccuracy << ".";    //output the actual overall best accuracy

    return;
}

int main() {
    int algorithm;      //used to determine which feature search algorithm to use
    int testFile;       //used to determine which data set file to use
    double input;       //takes in input from data set files
    int x = 0;          //used as row index for data[][]
    int y = 0;          //used as column index for data[][]
    double data[300][11];       //small data set    IMPORTANT: uncomment the test you want to use and comment the other one
    //double data[500][101];    //large data set               (default is small data set)

    cout << "Feature Selection with Nearest Neighbor" << endl << endl;

    cout << "Type the number of the file you want to test." << endl;
    cout << "1) CS170_SMALLtestdata__74.txt" << endl;
    cout << "2) CS170_largetestdata_48.txt" << endl;

    cin >> testFile;
    
    cout << endl << "Type the number of the algorithm you want to run." << endl;
    cout << "1) Forward Selection" << endl;
    cout << "2) Backward Elimination" << endl;
    
    cin >> algorithm;
    
    ifstream inFile;
    if (testFile == 1) {
        inFile.open("C:\\Users\\Chris\\Downloads\\CS170_SMALLtestdata__74.txt");             //small = 10 features
        //inFile.open("C:\\Users\\Chris\\Downloads\\CS170_small_special_testdata__96.txt");    //extra test
        //inFile.open("C:\\Users\\Chris\\Downloads\\CS170_small_special_testdata__95.txt");    //extra test
        testDataRows = 300;
        testDataColumns = 11;
    }
    else if (testFile == 2) {
        inFile.open("C:\\Users\\Chris\\Downloads\\CS170_largetestdata__48.txt"); //large = 100 features
        testDataRows = 500;
        testDataColumns = 101;
    }
    
    if (!inFile) {
        cout << "Unable to open file";
        exit(1);
    }

    if (testFile == 1) {
        while (inFile >> input) {
            data[x][y] = input;
            ++y;
            if (y == 11) {
                ++x;
                y = 0;
            }
        }
    }
    else if (testFile == 2) {
        while (inFile >> input) {
            data[x][y] = input;
            ++y;
            if (y == 101) {
                ++x;
                y = 0;
            }
        }
    }
    
    inFile.close();
    
    if (algorithm == 1) {
        forwardSelection(data);
    }
    else if (algorithm == 2) {
        backwardElimination(data);
    }

    return 0;
}
