#pragma once
#include "fft.h"
#include "DataType.h"
#include "PhaseDifferenceCorrection.h"
#include "MotorFrequencyCalculation.h"



//��ȡ�����������Ƶ��
//Fs:����Ƶ�� N:���ݳ��� frequency:Ƶ������ s:���ת���� F_base:��Ƶ����ԴƵ�ʣ� space:Ƶ�������С
double getFreOfBrokenBar(int Fs, int N, vector<double> frequency, int s, int F_base,double space) {
	//double s = getMotorSlipRatio(Fs, N, DATA, R, p, sMin, sMax);//���ת����
	//double F = getBaseFrequency(Fs, N, DATA);//��Ƶ
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


//�����������Ƶ�ʵķ�ֵ
//Fs:����Ƶ�� N:���ݳ��� data:ʱ������ R:ת�Ӳ��� p:������� sMin:ת������Сֵ sMax:ת�������ֵ result:��������Ƶ�ʵ�ֵ
void getFailureFrequency(int Fs, int N, vector<double> data, int R, int p, double sMin, double sMax, vector<double>& result) {
	double s = getMotorSlipRatio(Fs, N, data, R, p, sMin, sMax);//���ת����
	double F = getBaseFrequency(Fs, N, data);//��Ƶ

	Vector<double> datas;
	datas.resize(N);
	for (int i = 0; i < N; i++) {
		datas[i] = data[i];
	}
	Vector<complex<double>>  y;
	y.resize(N);
	y = fft(datas);
	datas.~Vector();
	vector<double> Y;//Ƶ������
	//�źŷ�ֵ
	for (int i = 0; i < N / 2; i++) {
		Y.push_back(getValue(y[i]) * 2 / N);
	}
	double FreOfBrokenBar = getFreOfBrokenBar(Fs, N, Y, s, F, 1);
	result.push_back(FreOfBrokenBar);


}