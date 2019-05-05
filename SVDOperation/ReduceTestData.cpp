#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <vector>
#include <string>

#include "Eigen/SVD"
#include "Eigen/Dense"
#include "Eigen/Jacobi"
#include "Eigen/Eigen"
#include "Eigen/JacobiSVD.h"
#include "Eigen/Core"
#include <bits/stdc++.h> 
#include<iostream>

using namespace Eigen;
using namespace std;
using Eigen::MatrixXd;
using Eigen::MatrixXf;
using Eigen::Vector3f;
using namespace std;

using namespace std;

int THREADS = 4;

/*
 * A class to create and write data in a csv file.
 */
class CSVWriter
{
	std::string fileName;
	std::string delimeter;
	int linesCount;
 
public:
	CSVWriter(std::string filename, std::string delm = ",") :
			fileName(filename), delimeter(delm), linesCount(0)
	{}
	/*
	 * Member function to store a range as comma seperated value
	 */
	template<typename T>
	void addDatainRow(T first, T last);
};
 
/*
 * This Function accepts a range and appends all the elements in the range
 * to the last row, seperated by delimeter (Default is comma)
 */
template<typename T>
void CSVWriter::addDatainRow(T first, T last)
{
	std::fstream file;
	// Open the file in truncate mode if first line else in Append Mode
	//file.open(fileName, std::ios::out | (linesCount ? std::ios::app : std::ios::trunc));
	file.open(fileName.c_str());
 
	// Iterate over the range and add each lement to file seperated by delimeter.
	for (; first != last; )
	{
		file << *first;
		if (++first != last)
			file << delimeter;
	}
	file << "\n";
	linesCount++;
 
	// Close the file
	file.close();
}


bool checkSpaces(string temp_value)
{
	int flag = 0;


	#pragma omp parallel for reduction(+:flag) num_threads(THREADS)
	for(int i = 0;i<temp_value.length();i++){
		if(temp_value[i]!=' ')
		{
			flag += 1;
		}	
	}

	if(flag>0)
		return true;
	
	
	return false;

	// while(temp_value[icc]!='\0')
	// {
	// 	if(temp_value[icc]!=' ')
	// 	{
	// 		flag = true;
	// 		break;
	// 	}	
	// 	icc++;
	// } 
	// return flag;
}

vector<string> split(string str, string token){
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
		string temp_value = str.substr(0,index);
		bool flag = checkSpaces(temp_value);

		if(flag == true)
	            result.push_back(temp_value);
            str = str.substr(index+token.size());
		 temp_value = str;
            if(str.size()==0)
		{
			flag = checkSpaces(str);
			if(flag == true)
				result.push_back(str);
		}
        }else{
		bool flag = checkSpaces(str);
		if(flag == true)
            		result.push_back(str);
            str = "";
        }
    }
    return result;
}

int debugging = 0;

MatrixXf getTestData(vector<string> _d)
{
	MatrixXf mDummy;
	if(_d.size() <=0 )
	{
		cout << "************Problem In Test Data Set " << endl;
		return mDummy;
	}

	// removing Subject and Image Number 
	int sizeofNumberofColumns = split(_d[0],",").size()-2;

	MatrixXf m(_d.size(),sizeofNumberofColumns);

	cout << "Analyse Test Data " << endl;
		#pragma omp parallel for num_threads(THREADS)
	for(int i=0;i<_d.size();i++)
	{
		vector<string> _eachRowData = split(_d[i],",");
		#pragma omp parallel for num_threads(THREADS)
		for(int k=2;k<_eachRowData.size();k++)
		{
			m(i,k-2) = atof(_eachRowData[k].c_str());
		}		
	}

	if(debugging == 1)
	{

		cout << "Dimesion of Test Matrix " << endl;
		cout << m.rows() << endl;
		cout << m.cols() << endl;

		for(int i=0;i<m.rows();i++)
		{
			cout << m(i,0) << " : " << m(i,m.cols()-1) << endl;
			if(i>5)
				break;
		}
	}

	return m;
}

MatrixXf getVData(vector<string> _d,int reducedDimension)
{
	MatrixXf mDummy;
	if(_d.size() <=0 || reducedDimension>=_d[0].size())
	{
		cout << "************Problem In V Data Set " << endl;
		return mDummy;
	}

	// removing Subject and Image Number 
	int sizeofNumberofColumns = split(_d[0]," ").size();

	if(debugging == 1)
	{



		cout << "Number of Columns From V File" << endl;
		cout << sizeofNumberofColumns << endl;		
	}

	if(reducedDimension>=sizeofNumberofColumns)
	{
		cout << "************Problem In Reduced Dimension Data Set " << endl;
		return mDummy;
	}

	sizeofNumberofColumns = min(sizeofNumberofColumns,reducedDimension);

	MatrixXf m(_d.size(),sizeofNumberofColumns);

	cout << "Analyse V Data " << endl;

	#pragma omp parallel for num_threads(THREADS)
	for(int i=0;i<_d.size();i++)
	{
		vector<string> _eachRowData = split(_d[i]," ");
		#pragma omp parallel for num_threads(THREADS)
		for(int k=0;k<sizeofNumberofColumns;k++)
		{
			m(i,k) = atof(_eachRowData[k].c_str());
			
		}
		if(i%5000==0)	
			cout << i << "row Completed " << endl;	
	}

	if(debugging == 1)
	{
		cout << "Dimesion of V Matrix " << endl;
		cout << m.rows() << endl;
		cout << m.cols() << endl;

		for(int i=0;i<m.rows();i++)
		{
			cout << m(i,0) << " : " << m(i,m.cols()-1) << endl;
			if(i>5)
				break;
		}
	}

	return m;
}
      

MatrixXf getReducedTestDataSet(MatrixXf testDataSet,MatrixXf getVVectorData)
{
	return testDataSet * getVVectorData;
}

int main(int argc,char** argv){

	
	if(argc < 7)
	{
		cout << "Total Number of Parameters should be Four" << endl;
		cout << "Params : ./a.out InputFile #ColumnConsidered VFile PathForDestinationNewDataSet #PhotoFrEachSubject StartIndexofPhoto" << endl;
		return 0;	
	}

    std::ifstream inFile;
    cout << argv[1] << endl;
    inFile.open(argv[1]); //open the input file
    
    	cout << argv[3] << endl;
	std::ifstream inputVFile(argv[3]);
    
	cout << "Total Number of Column Taken For Reduction"<<endl;    
    	cout << argv[2] << endl;
    	
	cout << "#Of Photos For Each Subject"<<endl;    
    	cout << argv[5] << endl;

	cout << "#Starting INdex of Photo"<<endl;    
    	cout << argv[6] << endl;
    	
    	string destination(argv[4]);
    	cout << "Destination Path " << endl;
    	cout << destination << endl;

	//-------------------------------------------------- Reading Input File 
    //std::stringstream strStream;
    //strStream << inFile.rdbuf(); //read the file
    //std::string str = strStream.str(); //str holds the content of the file

    //	std::cout << str << std::endl; //you can do anything with the string!!!
	/**/
		cout << "Start Reading Input Test File " << endl;	
		vector<string> _data;
		for( std::string line; getline( inFile, line ); )
		{
			//cout << line << endl;
			_data.push_back(line);
			//break;
		}

		cout << _data.size() << endl;	
	
	
	//-------------------------------------------------- Reading V File 
		cout << "Start Reading V File " << endl;
		vector<string> _dataVFile;
		for( std::string line; getline( inputVFile, line ); )
		{
			//cout << line << endl;
			_dataVFile.push_back(line);
			//break;
		}
				
		cout << _dataVFile.size() << endl;	
		
	MatrixXf TestDataFromFile = getTestData(_data);
	MatrixXf GetVData = getVData(_dataVFile,atoi(argv[2]));
	MatrixXf reducedTestDataPostSVD = getReducedTestDataSet(TestDataFromFile,GetVData);

	if(debugging == 1)
	{
		cout << " Test Data Dimension " << TestDataFromFile.rows() << " : " << TestDataFromFile.cols() << endl;
		cout << " GetVData Data Dimension " << GetVData.rows() << " : " << GetVData.cols() << endl;
		cout << " Reduced Data Set Dimension " << reducedTestDataPostSVD.rows() << " : " << reducedTestDataPostSVD.cols() << endl;
	}

	

		
	//CSVWriter writer(destination);	
	
	
	int subjectNumber = 1;
	int photoNumber = atoi(argv[6]);

	ofstream fout;

	fout.open(destination.c_str(),ios::app);
	
	for(int i=0;i<reducedTestDataPostSVD.rows() ; i++)
	{
		string final_string = "";
		vector<string> _nested_data;
		


		string sValue = "";

		ostringstream str1;
		str1 << subjectNumber;
		sValue = str1.str();	

		//_nested_data.push_back("S"+sValue);

		ostringstream str2;
		str2 << photoNumber;
		string sNew = str2.str();	

		//_nested_data.push_back("I"+sNew);

		fout << "S"+sValue << ',';
		fout << "I"+sNew << ',';
		
		photoNumber++;
		
		if(photoNumber%(atoi(argv[5])+1) == 0)
		{
			photoNumber = atoi(argv[6]);
			subjectNumber++;
		}
		
		for(int k=0;k<reducedTestDataPostSVD.cols(); k++)
		{

			float tempValue = reducedTestDataPostSVD(i,k);

			//cout << "tempValue" << endl;
			//cout << tempValue << endl;
			
			std::ostringstream ss;
			ss << tempValue;
			std::string s(ss.str());
			//cout << "s tempValue" << endl;
			//cout << s << endl;
			
			//_nested_data.push_back(s);

			fout << s << ',';

		}

		fout << "\n";

		if(debugging == 0)
		{
			cout << " No of Column in Test Data " << endl;
			cout << _nested_data.size() << endl;
			/*
			for(int k1=0;k1<_nested_data.size();k1++)
			{
				cout << _nested_data[k1] << "  " ;
			}
			//break;
			*/

		}

		//writer.addDatainRow(_nested_data.begin(), _nested_data.end());
		
	}

	fout.close();

	cout << "Completed Writing New Data Set " << endl;
	
	//cout << final_string << endl;
	/*string strName(argv[1]);
	cout << strName << endl;
	freopen(strName.append(".csv").c_str(),"w",stdout);
	cout << final_string << endl;
	*/
      //myfile.close();

	return 0;
}
