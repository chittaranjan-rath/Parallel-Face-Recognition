// write_csv.cpp
// updated 8/12/16

// the purpose of this code is to demonstrate how to write data
// to a csv file in C++

// inlcude iostream and string libraries
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// create an ofstream for the file output (see the link on streams for
// more info)
ofstream outputFile;
ofstream fs;
// create a name for the file output
std::string filename = "FinalTrainData.csv";

ifstream inputFile;
ifstream fsinput;

int main(int argc, char** argv)
{
    // create and open the .csv file
    freopen("FinalTrainData.csv","w",stdout);
    freopen("N_newData.txt","r",stdin);

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
    cout << ColNames << std::endl;
    
    // i is just a variable to make numbers with in the file
    string inputline="";
    // write data to the file
    //for (int counter = 0; counter <  10; counter++)
	//cout << "ABCD" << endl;
    int k_image_number=0;
    int k_sequence_number=1;

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
		//cout << word ; 
		finaldata += ( word + "," );
	  
		// While there is more to read 
	    } while (ss); 

	    if(finaldata[finaldata.length()-1]==',')
			finaldata[finaldata.length()-1] = '\0';
	    finaldata += ( '\n' );

		cout << finaldata ;

	//k++;
    }
    //cout << k << std::endl;
    // close the output file
    //cout.close();
    //cin.close();
    return 0;   
}
