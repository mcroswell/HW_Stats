// HW_Stats.cpp  
// Reads a CSV file of variable number of columns and rows.
// Requires:
// 1) all columns to be the same length
// 2) first row has same number of elements as data but is for description (and counting columns) only.
// 2023 (c) by Michael L. Croswell
// Can be used and modified but credit must be given.
// Speaking of credit, the formulas for std deviation were gleaned from:
// https://www.mathsisfun.com/data/standard-deviation-formulas.html
// -mlc

#include "HW_Stats.h" 
#include <assert.h>

//#define EXTRA_DEBUG 1

 using namespace std; 

class CSVReader 
{
public:
	
	CSVReader(string filename)
	{
		ifstream myStream(filename, ios::in);

		if (myStream.fail())
		{
			std::cout << "failed to open data file, " << filename << " with failbit " << myStream.failbit << endl;
		}
		else
		{
			_numColumns = countColumns(myStream);
			// tbd: Initialize vector of vectors for each columnvector and the outer construct:
			
			// tbd: call read data to fill up those vectors
			_numRows = readData(myStream);

			// Run the calculations on the columns
			// First get the averages
			_averages = averageAll();
			printVector("Averages", _averages);
			// Then the squared diffences (ea minus the avgs):
			vector<float> squaredDiffs = squaredDifferencesAll(_averages);
			printVector("Squared Diffs", squaredDiffs);
			// Finally, average the squared differences and take their squared root: 


			// Run the calculations on the columns
			myStream.close();
		}
	}

	void report() 
	{
		int col = 0;
		cout << "Column" << "\t" << " Mean" << endl;
		for (float mean : _averages)
		{
			cout << col++ << "    \t" << mean << endl;
		}
		cout << endl;
	};

private:

	vector< vector< float > > _numbers;
	vector< float > _averages;
	int _numColumns;
	int _numRows; 

	int countColumns(ifstream & aStream) {
			string firstLine;
			getline(aStream, firstLine);

			string tmp;
			size_t num = 0;
			vector<string> row = getRowItems(firstLine);
			num = row.size();
			cout << "Items from first row: " << endl;
			for (string item : row)
			{
				cout << item << " * ";
			}
			cout << endl;

			return num;
		}

	vector<string> getRowItems(string line, char delim = ',')
	{
		vector<string> rtnItems;
		size_t len = line.length();
	
		char* psz = new char[len+1]; 
		psz[len] = '\0';
		///cout << "(Line is of length " << len << " and strlen psz " << strlen(psz) << endl;
		strncpy(psz, line.c_str(),len);
	///	cout << psz << endl;
		int lastStart = 0;
		for (int i = 0; i < len; i++) {
			if (psz[i] == delim || i == len-1) {
				if (i == len - 1)
					psz[len] = 0; // We're at the end so want to include this last character...
				else
					psz[i] = 0;  // ... else we write the zero terminator on top of the comma.
				string sItem = &psz[lastStart];
				///cout << sItem << "!" << endl;
				rtnItems.push_back(sItem);
				lastStart = i + 1;
			}
		}
		delete[] psz;
		return rtnItems;
	}
	
	int readData(ifstream & aStream) { 
			string dataLine; 
			int rowNum = 0;

			// resize the vector to `R` elements of type vector<int>, each having size `C`
			//_numbers.resize(1, vector<float>(_numColumns));

			while (getline(aStream, dataLine)) {
			
				vector<string> row = getRowItems(dataLine);
				vector<float> frow;
				assert( _numColumns == row.size());
			    //Convert to numbers
				for (int i = 0; i < _numColumns; i++) {
					float f = atof(row[i].c_str());
					frow.push_back(f);
				}

				_numbers.push_back(frow);

				//cout << "data line #" << rowNum << " has " << row.size() << "items" << endl;

				rowNum++;
			}
	
			///cout << endl;
			return rowNum;
		}

	vector<float> averageAll() {
		vector<float> averages(_numColumns,0);
		for (vector<float> row : _numbers) { 
			for (int col=0; col < _numColumns; col++) {
				averages[col] += row[col];
			} 
		} 
		for (int col=0; col < _numColumns; col++) {
			averages[col] /= (float)_numRows;
		}
		return averages;
	 }
	
	vector<float> squaredDifferencesAll(const vector<float> averages) {
		vector<float> squaredDiffs(_numColumns, 0);
		for (vector<float> row : _numbers) {
			for (int col = 0; col < _numColumns; col++) {
				float diff = row[col]-averages[col];
				diff *= diff;
				squaredDiffs[col] = diff;
			}
		}
		for (int col = 0; col < _numColumns; col++) {
			squaredDiffs[col] /= (float)_numRows;
		}
		return squaredDiffs;
	}

	void printVector(string label, vector<float> vec) {
#ifdef EXTRA_DEBUG
		cout << label << endl;
		for (float v : vec) {
			cout << v << " ";
		}
		cout << endl;
#endif
	}

	};

int main()
{
	// Make file on Windows put exe in sub dirs:
	// string filename = "../../../random_numbers.csv";
	string filename = "random_numbers.csv";
	CSVReader cvsReader(filename);
	cvsReader.report();
	return 0;
}
