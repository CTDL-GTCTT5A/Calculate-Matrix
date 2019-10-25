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
	friend ostream& operator<<(ostream&, const Matrix);

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
	//Hàm gọi lại tái sử dụng
	static int Rank(double **matrix , int d , int c);

	//He phuong trinh tuyen tinh
	void TimNghiemHePTTT();


	void MainMenu();

	Matrix();

	Matrix(int n);

	~Matrix();
};

