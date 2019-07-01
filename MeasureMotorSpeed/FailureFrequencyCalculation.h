#pragma once
#include "fft.h"
#include "DataType.h"
#include "PhaseDifferenceCorrection.h"
#include "MotorFrequencyCalculation.h"



//获取电机断条故障频率
//Fs:采样频率 N:数据长度 frequency:频域数据 s:电机转差率 F_base:基频（电源频率） space:频率区间大小
double getFreOfBrokenBar(int Fs, int N, vector<double> frequency, int s, int F_base,double space) {
	//double s = getMotorSlipRatio(Fs, N, DATA, R, p, sMin, sMax);//电机转差率
	//double F = getBaseFrequency(Fs, N, DATA);//基频
	double sum1 = 0, sum2 = 0;

	double f1 = (1 - 2 * s)*F_base;
	int f1_index1 = (f1 - space) * N / Fs;
	int f1_index2 = (f1 + space) * N / Fs;
	for (int i = f1_index1; i <= f1_index2; i++) {
		sum1 += frequency[i];
	}
	sum1 = sum1 / (f1_index2 - f1_index1 + 1)* (N / Fs);

	double f2 = (1 + 2 * s)*F_base;
	int f2_index1 = (f2 - space) * N / Fs;
	int f2_index2 = (f2 + space) * N / Fs;
	for (int i = f2_index1; i <= f2_index2; i++) {
		sum2 += frequency[i];
	}
	sum2 = sum2 / (f1_index2 - f1_index1 + 1)* (N / Fs);

	double sum = (sum1 + sum2) / 2;
	return sum;
}


//计算各个故障频率的幅值
//Fs:采样频率 N:数据长度 data:时域数据 R:转子槽数 p:电机极数 sMin:转差率最小值 sMax:转差率最大值 result:各个故障频率的值
void getFailureFrequency(int Fs, int N, vector<double> data, int R, int p, double sMin, double sMax, vector<double>& result) {
	double s = getMotorSlipRatio(Fs, N, data, R, p, sMin, sMax);//电机转差率
	double F = getBaseFrequency(Fs, N, data);//基频

	Vector<double> datas;
	datas.resize(N);
	for (int i = 0; i < N; i++) {
		datas[i] = data[i];
	}
	Vector<complex<double>>  y;
	y.resize(N);
	y = fft(datas);
	datas.~Vector();
	vector<double> Y;//频域数据
	//信号幅值
	for (int i = 0; i < N / 2; i++) {
		Y.push_back(getValue(y[i]) * 2 / N);
	}
	double FreOfBrokenBar = getFreOfBrokenBar(Fs, N, Y, s, F, 1);
	result.push_back(FreOfBrokenBar);


}