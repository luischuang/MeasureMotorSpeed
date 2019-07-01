#pragma once
#include "fft.h"
#include "DataType.h"
#include "PhaseDifferenceCorrection.h"
//������ʵ��ת��
//Fs:����Ƶ�� N:���ݳ��� DATA:ʱ������ R:ת�Ӳ��� p:������� sMin:ת������Сֵ sMax:ת�������ֵ
double getRPM(int Fs, int N, vector<double> DATA,int R,int p,double sMin,double sMax) {
	int Delta = N / Fs;//Delta�������1Hz
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
	//�źŷ�ֵ
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


	//��ƵУ��
	ThreeTuples<double> bace ;
	PhaseDifferenceCorrection(Fs, N, DATA, maxIndex, bace);
	double baceF_correct = bace.first;//У����Ƶ��
	double baceA_correct = bace.second;//У�����ֵ
	double bacePhase_correct = bace.third;//У������λ

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
	//FshУ��
	ThreeTuples<double>FshResult ;
	PhaseDifferenceCorrection(Fs, N, DATA, Fsh_maxIndex, FshResult);
	double Fsh_correct = FshResult.first;//У����Ƶ��
	double FshA_correct = FshResult.second;//У�����ֵ
	double FshPhase_correct = FshResult.third;//У������λ

	double RPM = 60 * (Fsh_correct - baceF_correct)/R;
	return RPM;
}
//��������Ƶ
//Fs:����Ƶ�� N:���ݳ��� DATA:ʱ������
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
	//�źŷ�ֵ
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

	//��ƵУ��
	ThreeTuples<double> bace;
	PhaseDifferenceCorrection(Fs, N, DATA, maxIndex, bace);
	double baceF_correct = bace.first;//У����Ƶ��
	return baceF_correct;
}
//������ת����
//Fs:����Ƶ�� N:���ݳ��� DATA:ʱ������ R:ת�Ӳ��� p:������� sMin:ת������Сֵ sMax:ת�������ֵ 
double getMotorSlipRatio(int Fs, int N, vector<double> DATA, int R, int p, double sMin, double sMax) {
	double RPM = getRPM(Fs, N, DATA, R, p, sMin, sMax);//���ʵ��ת��
	double F = getBaseFrequency(Fs,N,DATA);//��Ƶ
	double SynchronousSpeed = 60 * F / (p / 2);//ͬ��ת��
	double slipRate = (SynchronousSpeed - RPM) / SynchronousSpeed;
	return slipRate;
}



