#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "fft.h"
#include "vector.h"
#include "window.h"
#include <math.h>
#include "DataType.h"
using namespace splab;
using namespace std;
double getValue(complex<double> num) {
	return sqrt(std::pow(num.real(), 2) + std::pow(num.imag(), 2));
}
//相位差校正法
//Fs:采样频率 N:数据长度 datas:时域数据 index:要进行校正的点的横坐标 result:返回的校正结果
//result.first = f_correct;//频率
//result.second = A_correct;//幅值
//result.third = phase_correct;//相位
void PhaseDifferenceCorrection(int Fs,int N, vector<double> datas,int index, ThreeTuples<double> &result) {
	double Amplitude = 1.0;
	Vector<double> window1 = hanning(N, 1.0, 1, 0);//窗1
	Vector<double> window2 = hanning(N / 2, 1.0, 1, 0);//窗2
	Vector<double> x1, x2;
	x1.resize(N);
	x2.resize(N / 2);
	for (int i = 0; i < N; i++) {
		x1[i] = datas[i] * window1[i];
	}
	for (int i = 0; i < N / 2; i++) {
		x2[i] = datas[i] * window2[i];
	}
	window1.~Vector();
	window2.~Vector();

	Vector<complex<double>>  y1, y2;
	y1.resize(N);
	y2.resize(N / 2);

	y1 = fft(x1);//对信号做N点FFT变换
	y2 = fft(x2);//对信号做N/2点FFT变换 
	x1.~Vector();
	x2.~Vector();

	vector<double> Y1, Y2;
	//第一段信号幅值
	for (int i = 0; i < N / 2; i++) {
		Y1.push_back(getValue(y1[i]) * 2 / N);
	}
	//第二段信号幅值 
	for (int i = 0; i < N / 4; i++) {
		Y2.push_back(getValue(y2[i]) * 4 / N);
	}

	int  index1 = index;
	double max1 = Y1[index1];
	complex<double> maxPoint1=y1[index1];
	//for (int i = 0; i < N / 2; i++) {
	//	if (max1 < Y1[i]) {
	//		max1 = Y1[i];
	//		maxPoint1 = y1[i];
	//		index1 = i;
	//	}
	//}
	int  index2 = index/2;
	double max2 = Y2[index2];
	complex<double> maxPoint2=y2[index2];
	//for (int i = 0; i < N / 4; i++) {
	//	if (max2 < Y2[i]) {
	//		max2 = Y2[i];
	//		maxPoint2 = y2[i];
	//		index2 = i;
	//	}
	//}
	y1.~Vector();
	y2.~Vector();
	Y1.~vector();
	Y2.~vector();
	double angle1, angle2;
	angle1 = atan2(maxPoint1.imag(), maxPoint1.real());
	angle2 = atan2(maxPoint2.imag(), maxPoint2.real());

	double A_uncorrect = max1;//未校正幅值
	double f_uncorrect = index1*Fs / N;//未校正频率
	double phase_uncorrect = angle1 * 180 / PI;//未校正相角

	double delt = fmod((angle1 - angle2), TWOPI);
	//将delt调整到（-pi,pi）之间
	if (delt<-PI)
		delt = delt + 2 * PI;
	else if (delt>PI)
		delt = delt - 2 * PI;

	double deltf = 2 * index2 - index1 - 2 * delt / PI;

	double f_correct = (index1 - deltf)*Fs / N;//校正后频率
	double A_correct = 2 / (sin(PI*deltf) / (PI*deltf))*A_uncorrect*(1 - deltf*deltf);//校正后幅值
	double phase_correct = (angle1 + deltf*PI) * 180 / PI;//校正后相位
	result.first = f_correct;
	result.second = A_correct;
	result.third = phase_correct;
}