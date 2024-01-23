#pragma once
#include <vector>
#include <string>

using namespace std;

class DataHolder{
public:
	static vector<vector<string>> data;
	static vector<string> categories;

	static int getDataSize() {
		return data.size();
	}

	static bool isCategory(string s){
		for (int i = 0; i < categories.size(); i++){
			if (categories[i] == s){
				return true;
			}
		}
		return false;
	}

	static bool processJSON(){
		removeFirstCategory();
		removeDataBlanks();
		// Calculate the number of lines needed
		int rows = data.size() / categories.size();
		int cols = categories.size();

		std::vector<std::vector<string>> newData(rows, std::vector<string>(cols));

		// Populate the 2D vector from the original vector
		int index = 0;
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				newData[i][j] = data[index++][0];
			}
		}
		data = newData;
    	return true;
	}

	static bool removeFirstCategory(){
		if (categories.size() > 0){
			categories.erase(categories.begin());
			return true;
		}
		return false;
	}

	static bool removeDataBlanks(){
		for (int i = 0; i < data.size(); i++){
			if(data[i][0] == " "){
				data.erase(data.begin() + i);
				i--;
			}	
		}
		return true;
	}

	static bool printAll(){
		printf("Categories:\n");
		for (int i = 0; i < categories.size(); i++){
			printf("%s ", categories[i].c_str());
		}
		printf("\n\nData:\n");
		for (int i = 0; i < data.size(); i++){
			for (int j = 0; j < data[i].size(); j++){
				printf("%s, ", data[i][j].c_str());
			}
			printf("\n");
		}
		return true;
	}

};

