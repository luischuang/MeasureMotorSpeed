// MeasureMotorSpeed.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "window.h"
#include "vector.h"
#include "DataType.h"
#include "fft.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <math.h>
#include "PhaseDifferenceCorrection.h"
#include "MotorFrequencyCalculation.h"
#include "FailureFrequencyCalculation.h"
using namespace std;
using namespace splab;

void readData(string file,vector<double> &data) {
	string s;
	ifstream infile(file.c_str());
	if (!infile) {
		cerr << "Error opening file";
		exit(EXIT_FAILURE);
	}
	while (getline(infile, s)) {
		data.push_back(atof(s.c_str()));
	}
	infile.close();
	infile.clear();
}


int main()
{
	string dir = "D:\\Data\\20190107-1s50波\\断条1根\\正转\\负载0.2\\10s_data\\v4\\";
	cout << "id, BaceFrequency, RPM" << endl;
	for (int i = 1; i <= 6; i++) {
		string filename = dir + to_string(i) + ".txt";
		int Fs = 3900;
		int N = 39000;
		vector<double> datas;
		readData(filename, datas);
		//double *result = new double[3];
		//memset(result, 0, 3 * sizeof(double));
		//PhaseDifferenceCorrection(Fs, N, datas, 50, result);
		//cout << "f_correct:" << result[0] << endl;
		//cout << "A_correct:" << result[1] << endl;
		//cout << "phase_correct:" << result[2] << endl;
		double BaceFrequency = getBaseFrequency(Fs,N,datas);
		
		double RPM = getRPM(Fs, N, datas, 22, 4, 0.01, 0.06);

		vector<double> fault;
		getFailureFrequency(Fs, N, datas, 22, 4, 0.01, 0.06, fault);
		cout << i << ", " << BaceFrequency << ", " << RPM << "fault:" << fault[0] << endl;

	}




	//int Fs = 3900;
	//int N = 39000;
	//vector<double> datas;
	//readData("10s_1-10.txt", datas);

	//ThreeTuples<double> result;
	//PhaseDifferenceCorrection(Fs, N, datas, 500, result);
	//cout << "f_correct:" << result.first << endl;
	//cout << "A_correct:" << result.second << endl;
	//cout << "phase_correct:" << result.third << endl;

	//double RPM = getRPM(Fs, N, datas, 22, 4, 0.01, 0.06);
	//cout << "RPM:" << RPM << endl;

	//TwoTuples<ThreeTuples<double>> FreOFBrokenBar;
	//getFreOfBrokenBar(Fs, N, datas, 22, 4, 0.01, 0.06, FreOFBrokenBar);

	system("pause");
    return 0;
}
