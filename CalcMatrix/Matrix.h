#pragma once
#include<iostream>
using namespace std;
class Matrix
{
private:
	//Matrix : ma trận dùng chính trong chương trình
	double **matrix;
	//cấp , dòng và cột của matrix
	int cap, row, col;
public:
	//Lấy cấp ra dùng
	int GetLevel();
	//Lấy dòng ra 
	int GetRow();
	//Lấy cột ra
	int GetCol();
	//Lấy ra địa chỉ của ma trận
	double** GetMatrix();

	friend istream& operator>>(istream&, Matrix&);
	friend ostream& operator<<(ostream&, Matrix);

	//Tính định thức
	double Det(double**matrix,int cap);


	//Ma trận phụ hợp
	Matrix FindAdj();

	//Ma trận nghịch đảo
	Matrix inverse();

	//Dùng để nhân 2 ma trận
	Matrix operator*(const Matrix b);

	//Hạng của ma trận
	int Rank();

	//He phuong trinh tuyen tinh
	void InRaNghiem();


	void MainMenu();

	Matrix();

	Matrix(int n);

	~Matrix();
};

