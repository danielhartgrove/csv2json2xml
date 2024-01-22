// CSV2JSON

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include "DataHolder.h"

using namespace std;

/* ***************************** */
/* ERROR CODE | DESCRIPTION      */
/* ------------------------------*/
/* 0        | Success!         */
/* 1        | Unable to open    */
/* 2        | Invalid usage    */
/* 3        | Invalid type      */
/* ***************************** */

// Need to create a class for the generalized data type -- a 2D vector will be sufficient. See DataHolder.h for more info.
std::vector<std::vector<std::string>> DataHolder::data;

bool extractCSV(string path) {
    ifstream input_file(path);
    string line;

    while (getline(input_file, line)) {
        // Create a new vector for each row
        vector<string> row;

        // Use stringstream to tokenize the line into categories
        stringstream ss(line);
        string field;

        // Tokenize the line using ',' as the delimiter
        while (getline(ss, field, ',')) {
            row.push_back(field); // add field to the current row
        }

        // Add the completed row to the main data
        DataHolder::data.push_back(row);
    }

    input_file.close();
    return true;
}

bool extractJSON(string path) {
    ifstream input_file(path);
    string line;
    vector<string> categories;
    // for each line
    // dump the contents into a dictionary
    return true;
}

int convertToCSV(string path, int file_type) {
    if (file_type == 2) {
        // convert json to csv
    }

    return 0;
}

bool convertToJSON(string path, int file_type) {
    vector<string> categories;
    if (file_type == 1) {
        extractCSV(path);
        string filename = "items.json";
        ofstream outfile(filename);

        if (!outfile.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return 1;
        }
        
        // make a vector of the categories
        for (int i = 0; i <= DataHolder::getSize(); i++) {
            categories.push_back(DataHolder::data[0][i]);
        }

        outfile << "{\n";
        outfile << "\t\"items\": [\n";
        for (int i = 1; i < DataHolder::getSize(); i++) { // the 0th line is the names of the categories
            outfile << "\t\t{\n";
            for(int j = 0; j < categories.size(); j++){//for each of the categories, print the category name and the value
                
                outfile << "\t\t\t\"" << categories[j] << "\": \"" << DataHolder::data[i][j] << "\"";

                // if its at the end of the categories, then we do not need a comma
                if(j != categories.size() - 1){
                    outfile << ",\n";
                }
                else{
                    outfile << "\n";
                }
            }
            
            // if its at the end of the elements, then we do not need a comma
            if(i != DataHolder::getSize() - 1){
                outfile << "\t\t},\n";
            }
            else{
                outfile << "\t\t}\n";
            }
        }
        outfile << "\t]\n}\n";
        outfile.close();
        return true;
    }
    else if (file_type == 3) {
        // convert xml to json
        return true;
    }else{
        return false;
    }
}

int main(int argc, char* argv[]) {
    // check if the correct number of arguments are passed
    if (argc != 3) {
        // if not, then explain the usage of the program
        cerr << "Usage: " << argv[0] << " <arg1> <arg2>" << endl;
        return 2;
    }

    string path_name = argv[1];
    string convert_type = argv[2];


    ifstream file(path_name);


    // check if file exists
    if (!file.is_open()) {
        cerr << "Error: cannot open file '" << path_name << "'" << endl;
        return 1; //error code for unable to open file
    }
    file.close(); // make sure to close the file!


    int file_type = 0;

    if (path_name.substr(path_name.find_last_of(".") + 1) == "csv") {
        file_type = 1;
    }
    else if (path_name.substr(path_name.find_last_of(".") + 1) == "json") {
        file_type = 2;
    }
    else if (path_name.substr(path_name.find_last_of(".") + 1) == "xml") {
        file_type = 3;
    }
    else {
        cerr << "Error: unknown file type '" << path_name.substr(path_name.find_last_of(".") + 1) << "'" << endl;
        return 3; //error code for invalid type
    }


    map<string, int> types = { {"csv", 1},{"json", 2},{"xml", 3} };

    switch (types[convert_type]) {
    case 1:
        if (file_type != 1) {
            convertToCSV(path_name, file_type);
        }
        break;
    case 2:
        if (file_type != 2) {
            convertToJSON(path_name, file_type);
        }
        break;
        /*case 3:
            if(file_type != 3){
                convertToXML(path_name, file_type);
            }
            break;*/
    default:
        cerr << "Error: unknown conversion type '" << convert_type << "'" << endl;
        return 3; //error code for invalid type
    }

    return 0;
};

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
