// CSV2JSON

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <filesystem>
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
        if(line==""){
            break;
        }
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

bool convertToCSV(string path, int file_type) {
    if (file_type == 2) {
        // convert json to csv
    }

    return true;
}

bool convertToJSON(string path, int file_type) {
    vector<string> categories;
    if (file_type == 1) {
        extractCSV(path);
        string filename = "items.json";
        ofstream outfile(filename);

        if (!outfile.is_open()) {
            printf("Error opening file: %s\n", filename);
            return 1;
        }

        
        for (int i = 0; i < DataHolder::data[0].size(); i++){
            categories.push_back(DataHolder::data[0][i]);
        }

        int count = 0;

        outfile << "{\n";
        outfile << "\t\"items\": [\n";
        for (int i = 1; i < DataHolder::getSize(); i++) {
            outfile << "\t\t{\n";

            // Use a range-based for loop for better readability
            for (int j = 0; j < categories.size(); j++) {
                outfile << "\t\t\t\"" << categories[j] << "\": \"" << DataHolder::data[i][j] << "\"";

                // Check if it's at the end of the categories, then no need for a comma
                if (j != categories.size() - 1) {
                    outfile << ",\n";
                } else {
                    outfile << "\n";
                }
            }

            // Check if it's at the end of the elements, then no need for a comma
            if (i != DataHolder::getSize() - 1) {
                outfile << "\t\t},\n";
            } else {
                outfile << "\t\t}\n";
            }
        }

        outfile << "\t]\n}\n";
        outfile.close();
        printf("Success!\n");
        return true;
    }
    else if (file_type == 3) {
        // convert xml to json
        return true;
    }else{
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    // check if the correct number of arguments are passed
    if (argc != 3) {
        // if not, then explain the usage of the program
        printf("Error: Invalid usage\n");
        printf("Correct Usage: %s <arg1> <arg2>", argv[0]);
        exit(2);
    }

    string path_name = argv[1];
    string convert_type = argv[2];
    
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
        file_type = 0;
        /* THIS CODE IS BROKEN IF THE FILE EXTENSION IS INCORRECT */
        printf("Error: Invalid file type\n");
        exit(3); //error code for invalid type
         /* THIS CODE IS BROKEN IF THE FILE EXTENSION IS INCORRECT */
    }
   
    ifstream f;
    f.open(path_name);

    if(!f.is_open()){
        printf("Error: unable to open file '%s'\n", path_name.c_str());
        exit(1); //error code for unable to open file
    }
    f.close();

    if(convert_type == "csv"){
        if (file_type != 1) {
            convertToCSV(path_name, file_type);
        }else{
            printf("Error: file type is already CSV\n");
            exit(3); //error code for invalid type
        }
    }
    else if (convert_type == "json") {
        if (file_type != 2) {

            convertToJSON(path_name, file_type);
        }else{
            printf("Error: file type is already JSON\n");
            exit(3); //error code for invalid type
        }
    }
    else if (convert_type == "xml") {
        if (file_type != 3) {
            //convertToXML(path_name, file_type);
        }else{
            printf("Error: file type is already XML\n");
            exit(3); //error code for invalid type
        }
    }else{
        printf("Error: Invalid conversion type\n");
        exit(3); //error code for invalid type
    }

    return 0;
};