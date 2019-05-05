#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <vector>
#include <string>
#include <bits/stdc++.h> 
using namespace std;


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
		bool flag = false;int icc = 0;
		while(temp_value[icc]!='\0')
		{
			if(temp_value[icc]!=' ')
			{
				flag = true;
				break;
			}	
			icc++;
		} 
		return flag;
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

      

int main(int argc,char** argv){


	if(argc < 4)
	{
		cout << "Total Number of Parameters should be Three" << endl;
		cout << "Params : ./a.out NameOFInputFile #PhotoInEachSubject PathForDestinationNewDataSet" << endl;
		return 0;	
	}

    std::ifstream inFile;
    cout << argv[1] << endl;
    inFile.open(argv[1]); //open the input file
    
	cout << "Total Number of Photos For Each Subject"<<endl;    
    	cout << argv[2] << endl;


    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
    std::string str = strStream.str(); //str holds the content of the file

    //	std::cout << str << std::endl; //you can do anything with the string!!!
	vector<string> _data = split(str,"\n");
	cout << _data.size() << endl;	
		
	string destination(argv[3]);

	cout << destination << endl;

	CSVWriter writer(destination);	
	
	/*std::ofstream fs;
	std::ofstream myfile;
	string strName(argv[1]);
	std::string nameTemp = "TestData.csv";
       fs.open(myfile,nameTemp);
	*/
	
	int subjectNumber = 1;
	int photoNumber = 1;

	ofstream fout;

	fout.open(destination.c_str(),ios::app);
	
	for(int i=0;i<_data.size() ; i++)
	{
		string final_string = "";
		vector<string> _nested_data = split(_data[i]," ");
		
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
		
		if(photoNumber%(atoi(argv[2])+1) == 0)
		{
			photoNumber = 1;
			subjectNumber++;
		}
		
		for(int i=0;i<_nested_data.size();i++)
		{
			fout << _nested_data[i] << ',';
		}		
		fout << "\n";
		//writer.addDatainRow(_nested_data.begin(), _nested_data.end());
		
	}
	fout.close();

	return 0;
}
