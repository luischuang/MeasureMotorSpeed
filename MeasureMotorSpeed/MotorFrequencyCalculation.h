#pragma once
#include "fft.h"
#include "DataType.h"
#include "PhaseDifferenceCorrection.h"
//计算电机实际转速
//Fs:采样频率 N:数据长度 DATA:时域数据 R:转子槽数 p:电机极数 sMin:转差率最小值 sMax:转差率最大值
double getRPM(int Fs, int N, vector<double> DATA,int R,int p,double sMin,double sMax) {
	int Delta = N / Fs;//Delta个点代表1Hz
	Vector<double> datas;
	datas.resize(N);
	for (int i = 0; i < N; i++) {
		datas[i] = DATA[i];
	}

	Vector<complex<double>>  y1;
	y1.resize(N);
	y1 = fft(datas);
	datas.~Vector();
	vector<double> Y1;
	//信号幅值
	for (int i = 0; i < N / 2; i++) {
		Y1.push_back(getValue(y1[i]) * 2 / N);
	}
	double maxY = 0;
	int maxIndex = 0;
	for (int i = 0; i < N / 2; i++) {
		if (maxY < Y1[i]) {
			maxY = Y1[i];
			maxIndex = i;
		}
	}


	//基频校正
	ThreeTuples<double> bace ;
	PhaseDifferenceCorrection(Fs, N, DATA, maxIndex, bace);
	double baceF_correct = bace.first;//校正后频率
	double baceA_correct = bace.second;//校正后幅值
	double bacePhase_correct = bace.third;//校正后相位

	double F1 = baceF_correct;
	int Fsh_min = 0, Fsh_max = 0;

	Fsh_min = F1*(R*(1 - sMax) / (p / 2) + 1);
	Fsh_max = F1*(R*(1 - sMin) / (p / 2) + 1);
	double Fsh_maxY = 0;
	int Fsh_maxIndex = 0;
	for (int i = Fsh_min*Delta; i < Fsh_max*Delta; i++) {
		if (Fsh_maxY < Y1[i]) {
			Fsh_maxY = Y1[i];
			Fsh_maxIndex = i;
		}
	}
	y1.~Vector();
	Y1.~vector();
	//Fsh校正
	ThreeTuples<double>FshResult ;
	PhaseDifferenceCorrection(Fs, N, DATA, Fsh_maxIndex, FshResult);
	double Fsh_correct = FshResult.first;//校正后频率
	double FshA_correct = FshResult.second;//校正后幅值
	double FshPhase_correct = FshResult.third;//校正后相位

	double RPM = 60 * (Fsh_correct - baceF_correct)/R;
	return RPM;
}
//计算电机基频
//Fs:采样频率 N:数据长度 DATA:时域数据
double getBaseFrequency(int Fs, int N, vector<double> DATA) {
	Vector<double> datas;
	datas.resize(N);
	for (int i = 0; i < N; i++) {
		datas[i] = DATA[i];
	}

	Vector<complex<double>>  y1;
	y1.resize(N);
	y1 = fft(datas);
	datas.~Vector();
	vector<double> Y1;
	//信号幅值
	for (int i = 0; i < N / 2; i++) {
		Y1.push_back(getValue(y1[i]) * 2 / N);
	}
	double maxY = 0;
	int maxIndex = 0;
	for (int i = 0; i < N / 2; i++) {
		if (maxY < Y1[i]) {
			maxY = Y1[i];
			maxIndex = i;
		}
	}
	y1.~Vector();
	Y1.~vector();

	//基频校正
	ThreeTuples<double> bace;
	PhaseDifferenceCorrection(Fs, N, DATA, maxIndex, bace);
	double baceF_correct = bace.first;//校正后频率
	return baceF_correct;
}
//计算电机转差率
//Fs:采样频率 N:数据长度 DATA:时域数据 R:转子槽数 p:电机极数 sMin:转差率最小值 sMax:转差率最大值 
double getMotorSlipRatio(int Fs, int N, vector<double> DATA, int R, int p, double sMin, double sMax) {
	double RPM = getRPM(Fs, N, DATA, R, p, sMin, sMax);//电机实际转速
	double F = getBaseFrequency(Fs,N,DATA);//基频
	double SynchronousSpeed = 60 * F / (p / 2);//同步转速
	double slipRate = (SynchronousSpeed - RPM) / SynchronousSpeed;
	return slipRate;
}



