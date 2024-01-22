#pragma once
#include <vector>
#include <string>

using namespace std;

class DataHolder{
public:
	static vector<vector<string>> data;

	static int getSize() {
		return data.size();
	}
};

