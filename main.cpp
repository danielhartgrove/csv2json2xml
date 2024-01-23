// CSV2JSON

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <filesystem>
#include <algorithm>
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
std::vector<std::string> DataHolder::categories;

string getFileName(string path, string suffix){
         // Find the last occurrence of the dot
        size_t dotIndex = path.find_last_of('.');
        size_t slashIndex = path.find_last_of('/');
        string filename = "";
        string catname = "items";
        // Check if dot is found and it's not the first character
        if (dotIndex != std::string::npos && dotIndex > 0) {
            // Replace the file extension with ".json"
             filename = path.substr(0, dotIndex) + "." + suffix;
        }else{
            printf("Error: Invalid file type\n");
            exit(3); //error code for invalid type
        }


        return filename;
        
}

string getCatName(string path){
         // Find the last occurrence of the dot
        size_t dotIndex = path.find_last_of('.');
        size_t slashIndex = path.find_last_of('/');
        string catname = "items";
        // Check if dot is found and it's not the first character
        if (dotIndex != std::string::npos && dotIndex > 0) {
              if (slashIndex != std::string::npos && dotIndex != std::string::npos && dotIndex > slashIndex) {
                // Extract the substring between the last slash and the last dot
                catname = path.substr(slashIndex + 1, dotIndex - slashIndex - 1);
            }
        }else{
            printf("Error: Invalid file type\n");
            exit(3); //error code for invalid type
        }
        return catname;
}

bool extractCSV(string path) {
    ifstream input_file(path);
    string line;
    int count = 0;

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
        if(count == 0){
            DataHolder::categories = row;
            count++; // no use in incrementing count each time as it is only relevant for the first check. Useless computation.
        }else{
            DataHolder::data.push_back(row);
        }
        
    }

    input_file.close();
    DataHolder:: printAll();
    return true;
}

// Having a spot of bother with extracting the JSON data
bool extractJSON(string path) {
    ifstream input_file(path);
    string line;
    vector<string> categories;

    while (getline(input_file, line)) {
    
        // Remove unwanted characters from the line
        line.erase(remove_if(line.begin(), line.end(), [](char c) {return c == '{' || c == '}' || c == '\"' || c == '[' || c == ']' || c == '\t' || c == '\n' || c == ',';}), line.end());

        if (line != "") {
            // Create a new vector for each row
            vector<string> row;

            // Use stringstream to tokenize the line into categories
            stringstream ss(line);
            string field;


            // Tokenize the line using ':' as the delimiter
            int count = 0;

            while (getline(ss, field, ':')) {
                // Skip the first token if it matches any category in the vector
                if ((count == 0 && !DataHolder::isCategory(field))) {
                    DataHolder::categories.push_back(field);
                } else if(count != 0){
                    row.push_back(field); // add field to the current row
                }
                count++;
            }

            if (count == 1) {
                row.pop_back();
            }

            // Add the completed row to the main data
            DataHolder::data.push_back(row);
        }
    }

    DataHolder::processJSON();  // the json data is separated by newlines when in the data class so I need to match up the data with the categories in a single row
    input_file.close();
    return true;
}

bool convertToCSV(string path, int file_type) {
    if (file_type != 1) {
        extractJSON(path);
        string filename = getFileName(path, "csv");

        ofstream outfile(filename);

        if (!outfile.is_open()) {
            printf("Error opening file: %s\n", filename);
            return 1;
        }

        for (int i = 0; i < DataHolder::categories.size(); i++) {
            outfile << DataHolder::categories[i];
            // Check if it's at the end of the categories, then no need for a comma
            if (i != DataHolder::categories.size() - 1) {
                outfile << ",";
            } else {
                outfile << "\n";
            }
        }

        for (int i = 1; i < DataHolder::getDataSize(); i++) {
            for (int j = 0; j < DataHolder::categories.size(); j++) {
                outfile << DataHolder::data[i][j];

                // Check if it's at the end of the categories, then no need for a comma
                if (j != DataHolder::categories.size() - 1) {
                    outfile << ",";
                } else {
                    outfile << "\n";
                }
            }
        }

        outfile.close();
        printf("Success!\n");

        return true;
    }

    return false;
}

bool convertToJSON(string path, int file_type) {
    if (file_type != 2) {
        extractCSV(path);
        
        string filename = getFileName(path, "json");
        string catname = getCatName(path);

        ofstream outfile(filename);

        if (!outfile.is_open()) {
            printf("Error opening file: %s\n", filename);
            return 3;
        }
    

        outfile << "{\n";
        outfile << "\t\""<< catname << "\": [\n";
        
        for (int i = 0; i < DataHolder::getDataSize(); i++) {
            outfile << "\t\t{\n";

            for (int j = 0; j < DataHolder::categories.size(); j++) {
                outfile << "\t\t\t\"" << DataHolder::categories[j] << "\": \"" << DataHolder::data[i][j] << "\"";

                // Check if it's at the end of the categories, then no need for a comma
                if (j != DataHolder::categories.size() - 1) {
                    outfile << ",\n";
                } else {
                    outfile << "\n";
                }
            }

            // Check if it's at the end of the elements, then no need for a comma
            if (i != DataHolder::getDataSize() - 1) {
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

    return false;
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