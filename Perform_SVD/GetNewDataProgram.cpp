// write_csv.cpp
// updated 8/12/16

// the purpose of this code is to demonstrate how to write data
// to a csv file in C++

// inlcude iostream and string libraries
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Eigen/SVD"
#include "Eigen/Dense"
#include "Eigen/Jacobi"
#include "Eigen/Eigen"
#include "Eigen/JacobiSVD.h"
#include "Eigen/Core"
#include "Eigen/Dense"
#include <bits/stdc++.h> 
#include<iostream>
#include <string> 

using namespace Eigen;
using namespace std;
using Eigen::MatrixXd;
using Eigen::MatrixXf;
using Eigen::Vector3f;
using namespace std;

using namespace std;

// create an ofstream for the file output (see the link on streams for
// more info)
ofstream outputFile;
ofstream fs;
// create a name for the file output
std::string filename = "FinalTrainData.csv";

ifstream inputFile;
ifstream fsinput;
vector<vector<float>> _dataInFloatReducedU;


class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str)
        {
            std::string         line;
            std::getline(str, line);

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();
            while(std::getline(lineStream, cell, ','))
            {
                m_data.push_back(cell);
            }
            // This checks for a trailing comma with no data after it.
            if (!lineStream && cell.empty())
            {
                // If there was a trailing comma then add an empty element.
                m_data.push_back("");
            }
        }
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}   

vector<vector<float>> main_test_data()
{
    std::ifstream file("test_image_dataset.csv");

    CSVRow row;
    int index=0;
    vector<vector<float>> _data;
    while(file >> row)
    {
//        std::cout << "4th Element(" << row[0] << ":" << row[1] << row[10305] << ")\n";
	//cout << row.size() << endl;
	vector<float> newData;
	for(int i=2;i<row.size();i++)
	{
		newData.push_back(stof(row[i]));
	}
	_data.push_back(newData);
	index++;
    }
	index=0;
/*	for(int i=0;i<_data.size();i++)
	{
		index = 0;
		cout << _data[i].size() << endl;

		for(int j=2;j<_data[i].size() && index<10;j++)
		{
			cout << _data[i][j] << " ";
			index++;
		}
		cout << endl;

	}

	cout << _data.size() << endl;
*/
	
	return _data;
}

MatrixXf getMatrix()
{
	cout << " Started Going For Data " << endl;
	vector<vector<float>> _d = main_test_data();
	cout << _d.size() << endl;
	cout << _d[0].size() << endl;

	int rows = _d.size();
	int cols = _d[0].size();

	MatrixXf m(rows,cols);
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
			m(i,j) = _d[i][j];
	}
	/*
	MatrixXf m(10,12);
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<12;j++)
			m(i,j) = rand() % 100 + 2;
	}
	*/
	cout << " End Going For Data " << endl;
	return m;
}

MatrixXf getNewData(MatrixXf originalData,MatrixXf U)
{
	return originalData * U;
}

MatrixXf getMatrixFromVector(vector<vector<float>> _d)
{
	cout << " Started Going For Data From Vector" << endl;
	//vector<vector<float>> _d = main_test_data();
	cout << _d.size() << endl;
	cout << _d[0].size() << endl;

	int rows = _d.size();
	int cols = _d[0].size();

	MatrixXf m(rows,cols);
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
			m(i,j) = _d[i][j];
	}
	/*
	MatrixXf m(10,12);
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<12;j++)
			m(i,j) = rand() % 100 + 2;
	}
	*/
	cout << " End Going For Data  From Vector" << endl;
	return m;
}

void generateTestData(MatrixXf testData)
{
	string ColNames = "";
	freopen("FinalTestData.csv","w",stdout);
	for(int i=-2;i<200;i++)
	{
		/**/
		if(i==-2)
			ColNames += "SampleNo,";
		else if(i==-1)
			ColNames += "ImageNumber,";
		else
			ColNames += "Column" + to_string(i+1) + ",";
	} 

	if(ColNames[ColNames.length()-1]==',')
		ColNames[ColNames.length()-1] = '\0';

	cout << ColNames << endl;

	int k_image_number=8;
	int k_sequence_number=1;

	vector<float> _dataTemp;
	//while(getline(cin,inputline)) 
	for(int i=0;i<testData.rows();i++)
	{        
		string finaldata = "";
		k_image_number += 1;

		finaldata += ( "S" + to_string(k_sequence_number) + "," + "I" + to_string(k_image_number) + "," );
		if(k_image_number%10==0)
		{
			k_image_number = 8;
			k_sequence_number++;
		}
		// Traverse through all words 
		for(int j=0;j<testData.cols();j++)
		{ 
			// Read a word 
			float word = testData(i,j); 

			// Print the read word 
			finaldata += ( to_string(word) + "," );
		} 

		if(finaldata[finaldata.length()-1]==',')
			finaldata[finaldata.length()-1] = '\0';
		finaldata += ( '\n' );

			cout << finaldata ;
	}	
}

int main(int argc, char** argv)
{
    // create and open the .csv file
    //freopen("FinalTestData.csv","w",stdout);
    freopen("N_reducedU.txt","r",stdin);

    string ColNames = "";

	for(int i=0;i<200;i++)
	{
		/**/
		if(i==0)
			ColNames += "SampleNo,";
		else if(i==1)
			ColNames += "ImageNumber,";
		else
			ColNames += "Column" + to_string(i+1) + ",";
	}    
	
if(ColNames[ColNames.length()-1]==',')
	ColNames[ColNames.length()-1] = '\0';

    // write the file headers
    //cout << ColNames << std::endl;
    
    // i is just a variable to make numbers with in the file
    string inputline="";
    // write data to the file
    //for (int counter = 0; counter <  10; counter++)
	//cout << "ABCD" << endl;
    int k_image_number=0;
    int k_sequence_number=1;
	
	vector<float> _dataTemp;
    while(getline(cin,inputline)) 
    {        
        // write the data to the output file
        //cout << inputline << std::endl;
		string finaldata = "";




	    k_image_number += 1;
		
	    finaldata += ( "S" + to_string(k_sequence_number) + "," + "I" + to_string(k_image_number) + "," );

	    if(k_image_number%8==0)
		{
			k_image_number = 0;
			k_sequence_number++;
		}

    	    istringstream ss(inputline); 
  
	    // Traverse through all words 
	    do { 
		// Read a word 
		string word; 
		ss >> word; 
	  
		// Print the read word 
		//cout << word << endl; 
		if(word!="")
		{
			finaldata += ( word + "," );
		  	_dataTemp.push_back(stof(word));
		}
		// While there is more to read 
	    } while (ss); 
		cout << _dataTemp.size() << endl;
	    _dataInFloatReducedU.push_back(_dataTemp);
	    if(finaldata[finaldata.length()-1]==',')
			finaldata[finaldata.length()-1] = '\0';
	    finaldata += ( '\n' );				
		_dataTemp.clear();
	
		//cout << finaldata << endl;
		//break;
		cout << _dataInFloatReducedU.size() << endl;

	//k++;
    }

	
	cout << _dataInFloatReducedU[0][0] ;


	// Perform Matrix Multiplication and Getting Test Reduced Data 
	
	MatrixXf testData = getMatrix();
	MatrixXf reducedU = getMatrixFromVector(_dataInFloatReducedU);
	MatrixXf newDataTest = getNewData(testData,reducedU);


	generateTestData(newDataTest);
	

    //cout << k << std::endl;
    // close the output file
    //cout.close();
    //cin.close();
    return 0;   
}
