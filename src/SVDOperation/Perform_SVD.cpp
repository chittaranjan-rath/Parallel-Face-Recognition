#include "Eigen/SVD"
#include "Eigen/Dense"
#include "Eigen/Jacobi"
#include "Eigen/Eigen"
#include "Eigen/JacobiSVD.h"
#include "Eigen/Core"
#include "omp.h"
#include <bits/stdc++.h> 
#include<iostream>

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace Eigen;
using namespace std;
using Eigen::MatrixXd;
using Eigen::MatrixXf;
using Eigen::Vector3f;
using namespace std;



int THREADS = 4;

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
/*
void writeToFile(string fileName,string content)
{
  ofstream myfile;
  myfile.open(fileName,ios::app);
  myfile << content;
  myfile.close();
}
*/
vector<vector<float> > main_data()
{
    std::ifstream file("toy_train.csv");

    CSVRow row;
    int index=0;
    vector<vector<float> > _data;
    while(file >> row)
    {
//        std::cout << "4th Element(" << row[0] << ":" << row[1] << row[10305] << ")\n";
//	cout << row.size() << endl;
	vector<float> newData(row.size()-2,0.0);
	#pragma omp parallel for num_threads(THREADS)
	for(int i=2;i<row.size();i++)
	{
		newData[i-2] = atof(row[i].c_str());
		// newData.push_back(atof(row[i].c_str()));
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
int debugValue = 0;
MatrixXf getMatrix()
{
	if(debugValue == 0)
	{
		vector<vector<float> > _d = main_data();
		cout << "AAAA" << endl;
		cout << _d.size() << endl;
		cout << _d[0].size() << endl;
		
		//MatrixXf m(320,10304);
			
		MatrixXf m(_d.size(),_d[0].size());

		#pragma omp collapse(2)
		for(int i=0;i<_d.size();i++)
		{
			for(int j=0;j<_d[0].size();j++)
				m(i,j) = _d[i][j];
		}
		return m;
	}
	else
	{
		MatrixXf m(10,12);
		for(int i=0;i<10;i++)
		{
			for(int j=0;j<12;j++)
				m(i,j) = rand() % 100 + 2;
		}
		cout << " COmpleted Data Matrix" << endl;
		return m;
	}
	
}

int ColumnTillFound = -1;

MatrixXf getCumulativeValue(MatrixXf eigenValueVector)
{
	int sizeofeigen = eigenValueVector.rows();
	cout << sizeofeigen << endl;
	float cumulative_sum = 0;
	MatrixXf SigMaVector(sizeofeigen,sizeofeigen);

	#pragma omp parallel for reduction(+:cumulative_sum) num_threads(THREADS)
	for(int i=0;i<eigenValueVector.rows();i++)
	{
		cumulative_sum += eigenValueVector(i,0);
		SigMaVector(i,i)=eigenValueVector(i,0);
	}

	#pragma omp collapse(2)
	for(int i=0;i<sizeofeigen;i++)
		for(int j=0;j<sizeofeigen;j++)
			if(i!=j)
				SigMaVector(i,j) = 0;
	
	
	MatrixXf percentageVector(eigenValueVector.rows(),1);

	// #pragma omp parallel for
	for(int i=0;i<eigenValueVector.rows();i++)
	{
		if(i==0)
			percentageVector(i,0) = (eigenValueVector(i,0)/cumulative_sum);
		else
		{
			percentageVector(i,0) = (eigenValueVector(i,0)/cumulative_sum) + percentageVector(i-1,0);
	
			if(percentageVector(i,0)>0.90 && ColumnTillFound==-1)
			{
				// #pragma omp critical
				ColumnTillFound = i;
			}
		}
		
	}
	//cout << "Cumulative Completetd	:	" << ColumnTillFound << endl;



	// freopen("CumulativePercentage.txt","w",stdout);
	// cout << percentageVector << endl;
	return SigMaVector;
}

MatrixXf reducedSigma(MatrixXf value,int reducedDimension)
{
	//Consideinrg 200 vectors
	MatrixXf reducedSigma(value.rows(),reducedDimension);
	#pragma omp collapse(2)
	for(int i=0;i<value.rows();i++)
	{
		for(int j=0;j<reducedDimension;j++)
		{
			reducedSigma(i,j) = value(i,j);
		}		
	}
	return reducedSigma;
}

MatrixXf getNewData(MatrixXf sigmaVector,MatrixXf U)
{
	return U * sigmaVector;
}



int main()
{

	double t = omp_get_wtime();	
	//MatrixXf m = MatrixXf::Random(500,10000);
	MatrixXf m = getMatrix();
	

	//MatrixXf m(500,10100);

	//cout << "Here is the matrix m:" << endl << m << endl;
	JacobiSVD<MatrixXf> svd(m, ComputeThinU | ComputeThinV);
	//JacobiSVD<MatrixXf> svd(m);
	cout << "Its singular values are:" << endl;// << svd.singularValues() << endl;
	cout << "Its left singular vectors are the columns of the thin U matrix:" << endl;// << svd.matrixU() << endl;
	cout << "Its right singular vectors are the columns of the thin V matrix:" << endl;// << svd.matrixV() << endl;
	//Vector3f rhs(1, 0, 0);
	//cout << "Now consider this rhs vector:" << endl << rhs << endl;
	//cout << "A least-squares solution of m*x = rhs is:" << endl << svd.solve(rhs) << endl;

	
	MatrixXf SigMaVector = getCumulativeValue(svd.singularValues());
	ColumnTillFound = ColumnTillFound!=-1 ? ColumnTillFound : 10; 
	MatrixXf reducedSigMaVector = reducedSigma(SigMaVector,ColumnTillFound);
	MatrixXf U = svd.matrixU();
	MatrixXf newData = getNewData(reducedSigMaVector,U);
	
	double f = omp_get_wtime();
	// printf("Time: %f\n",f-t);

	freopen("Singular_Values.txt","w",stdout);
	cout << svd.singularValues() << endl;
	
	freopen("V_Values.txt","w",stdout);
	cout << svd.matrixV() << endl;
	freopen("U_Values.txt","w",stdout);
	cout << U << endl;
	/*
	*/
	freopen("NoofColumnsTaken.txt","w",stdout);
	cout << ColumnTillFound << endl;

	freopen("Input_Data.txt","w",stdout);
	cout << m << endl;
	freopen("SigMaVector.txt","w",stdout);
	cout << SigMaVector << endl;
	freopen("reducedSigMaVector.txt","w",stdout);
	cout << reducedSigMaVector << endl;

	freopen("newData.txt","w",stdout);
	cout << newData << endl;

	
	

	return 0;
}



/*
// define a custom template binary functor
template<typename Scalar> struct MakeComplexOp {
  EIGEN_EMPTY_STRUCT_CTOR(MakeComplexOp)
  typedef complex<Scalar> result_type;
  complex<Scalar> operator()(const Scalar& a, const Scalar& b) const { return complex<Scalar>(a,b); }
};
int main(int, char**)
{
  Matrix4d m1 = Matrix4d::Random(), m2 = Matrix4d::Random();
  cout << m1.binaryExpr(m2, MakeComplexOp<double>()) << endl;
  return 0;
}

*/
