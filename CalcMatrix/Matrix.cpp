#include "Matrix.h"
#include<iostream>
#include<iomanip>
#include<sstream>
using namespace std;


Matrix::Matrix()
{
	matrix = NULL;
	cap = 0;
	row = 0;
	col = 0;
}

Matrix::Matrix(int n)
{
	matrix = new double*[n];
	for (int i = 0; i < n; i++)
	{
		matrix[i] = new double[n];
	}

}


int Matrix::GetLevel()
{
	return cap;
}
//Lấy dòng ra 
int Matrix::GetRow()
{
	return row;
}
//Lấy cột ra
int Matrix::GetCol()
{
	return col;
}

double** Matrix::GetMatrix()
{
	return matrix;
}

istream& operator>>(istream& input, Matrix &matrix)
{
	cout << "Nhap vao so dong cua ma tran : ";
	cin >> matrix.row;
	cout << "Nhap vao so cot cua ma tran : ";
	cin >> matrix.col;

	if (matrix.row == matrix.col)
		matrix.cap = matrix.row;
	matrix.matrix = new double* [matrix.row];

	for (int i = 0; i < matrix.row; i++)
	{
		matrix.matrix[i] = new double[matrix.col];
	}


	for (int i = 0; i < matrix.row; i++)
	{
		for (int j = 0; j < matrix.col; j++)
		{
			cout << "A[" << i << "][" << j << "] : =";
			cin >> matrix.matrix[i][j];
		}
	}
	return input;
}

ostream& operator<<(ostream& output , const Matrix matrix)
{
	cout << "_______________Your Matrix__________________\n";
	for (int i = 0; i < matrix.row; i++)
	{
		for (int j = 0; j < matrix.col; j++)
		{
			cout << setw(10);
			if (matrix.matrix[i][j] == (int)matrix.matrix[i][j])
				cout << fixed << setprecision(0) << matrix.matrix[i][j] << setw(12);

			else
				cout << fixed << setprecision(2) << matrix.matrix[i][j] << setw(12);
		}
		cout << endl;
	}
	return output;
}

//Tham số n = 0 khi nó là ma trận vuông
void CreateNewMatrix(double **&matrix ,int  m, int n=0)
{
	matrix = new double*[m];
	for (int i = 0; i < m; i++)
		if(n!=0)
			matrix[i] = new double[n];
		else
			matrix[i] = new double[m];
}

//Thuật toán tính ma trận cấp n như sau:
/*
Thay vì tính trực tiếp ta sẽ hạ bậc từ từ bằng đệ quy cho đến khi nó cấp 2 là điều kiện dừng
Ta hạ cấp bằng cách bỏ từng dòng i và cột 0 để tính định thức tại cấp đó
ví dụ ta có ma trận 4x4
khi ta bỏ dòng thứ 0 và cột thứ 0 thì nó ngay lập tức về ma trận 3x3
tương tự bỏ dòng thứ 1 và cột thứ 0 => ma trận 3x3
tương tự như thế cho đến khi hết dòng
Ta được 4 ma trận 3x3
Và Det(A) = A[i][0]*(-1)^i+Det(Ai)
với Ai là các ma trận ta vừa hạ cấp

Tóm lại : khi có ma trận cấp 5
-Ta hạ cấp được 5 ma trận cấp 4
-1 Ma trận cấp 4 hạ được 4 ma trận cấp 3  => 5 ma trận cấp 4 = 20 ma trận cấp 3 
- 20 ma trận cấp 3 => 60 ma trận cấp 2 => trả được kết quả


=>Cho nên khi tính với cấp n ta chỉ cần gọi đệ quy là được
*/
double Matrix::Det(double**matrix,int cap)
{
	if (row != col)
	{
		//Không phải ma trận vuông nên không thể tính
		cout << "Khong phai ma tran vuong , khong the tinh dinh thuc." << endl;
		system("pause");
		exit(0);
	}

	double det = 0, t;
	if(cap==2)
		return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
	// Ở tại mỗi dòng thì mỗi lần lấy ra một matrix con thì bỏ lần lượt từng dòng với cột 1
	for (int d = 0; d < cap; d++)
	{
		double** subMatrix;
		int dong = 0, cot = 0;
		//Khởi tạo địa chỉ cho subMatrix
		CreateNewMatrix(subMatrix, cap);
		for (int i = 0; i < cap; i++)
		{
			cot = 0;
			for (int j = 1; j < cap; j++)
			{
				if (i != d)
				{
					subMatrix[dong][cot] = matrix[i][j];
					cot++;
				}
			}
			//Kiểm tra nếu nhập được thì mới tăng dòng lên tránh tăng 2 lần
			if (i != d)
			{
				dong++;
			}

		}
		if (d % 2 == 0)
			t = 1;
		else
			t = -1;
		det += matrix[d][0] * t*Det(subMatrix, cap - 1);
		delete[] subMatrix;
	}
	return det;
}







/*
Thuật toán tìm ma trận nghịch đảo :

Ta sẽ áp dụng tính chất sau đây :

   A^(-1) = A/det|A|*Adj(A)^T
   => Ta cần tìm ma trận phụ hợp Adj của A sau đó chuyển vị nó lại (^T  = transform = chuyển vị)
   Và det|A| ta đã làm ở bài 1
   
*/


//Tìm ma trận Transform(chuyển vị) của matrix truyền vào
double**Transform(double**matrix,int dong , int cot)
{
	double**matrixtemp;
	CreateNewMatrix(matrixtemp, dong , cot);
	for (int i = 0; i < dong; i++)
	{
		for (int j = 0; j < cot; j++)
		{	

			// Dòng lệnh này là chuyển từ dòng thành cột.
			matrixtemp[j][i] = matrix[i][j];
		}
	}
	return matrixtemp;
}


//Hàm tìm ma trận phụ hợp
/*
Thuật toán tìm ma trận phụ hợp là 
Adj(A) = -1^(i+j) * Dij
Tức là ta lần lượt bỏ từng dòng và cột của ma trận A sau đó Tính định thức của ma trận sau khi bỏ dòng và cột đó
rồi lấy định thức đó nhân với -1^(dòng + cột) là ra được phần tử thứ ij(phần tử tại vị trí dòng và cột đó)

ví dụ cụ thể được thể hiện chi tiết trong bài báo cáo.

*/

//Thể hiện thuật toán
Matrix Matrix::FindAdj()
{
	if (row != col)
	{
		cout << "Khong phai ma tran vuong\n";
		return NULL;
	}
	else
	{
		Matrix temp;
		//Tạo ma trận adj mới
		CreateNewMatrix(temp.matrix, cap);
		temp.cap = cap;
		temp.row = cap;
		temp.col = cap;
		int det = 0, t;

		//Riêng ma trận cấp 2 thì bỏ dòng đi ta có ma trận cấp 1 nhưng ở hàm tính det thì điều kiện dừng là det==2 => ta phải làm trường hợp riêng
		if (cap == 2)
		{
			//Đổi chỗ a[0][0] và a[1][1]
			temp.matrix[0][0] = matrix[1][1];
			temp.matrix[1][1] = matrix[0][0];
			temp.matrix[0][1] = -1 * matrix[1][0];
			temp.matrix[1][0] = -1 * matrix[0][1];
		}
		// Ở tại mỗi dòng thì mỗi lần lấy ra một matrix con thì bỏ lần lượt từng dòng với cột 
		//Với d và c là các dòng-cột ta bỏ lần lượt
		else
		{
			for (int d = 0; d < cap; d++)
			{
				for (int c = 0; c < cap; c++)
				{
					double** subMatrix;
					int dong = 0, cot = 0;
					//Khởi tạo địa chỉ cho subMatrix
					CreateNewMatrix(subMatrix, cap);
					for (int i = 0; i < cap; i++)
					{
						//nếu i không trùng với dòng bỏ thì thực hiện
						if (i != d)
						{
							cot = 0;
							for (int j = 0; j < cap; j++)
							{
								//Nếu j không trùng với dòng bỏ thì thực hiện tìm ma trận con
								if (j != c)
								{
									subMatrix[dong][cot] = matrix[i][j];
									cot++;
								}
							}
							dong++;
						}
					}
					//Hệ số -1(i+j)
					int somu = c + d;
					int heso;
					if (somu % 2 == 0)
						heso = 1;
					else
						heso = -1;
					//Trả về ma trận phụ hợp
					temp.matrix[d][c] = heso * Det(subMatrix, cot);
					delete[] subMatrix;
				}
			}
		}
		return temp;
	}

	
	//return det;
}


Matrix Matrix::inverse()
{
	
	if (row != col)
	{
		cout << "Khong phai ma tran vuong\n";
		return NULL;
	}
	else
	{
		//Gọi hàm tìm ma trận phụ hợp
		Matrix adj = FindAdj();
		//Cấp = row vì nó là ma trận vuông
		cap = row;
		//Tranform lại ma trận phụ hợp sau đó nhân cho 1/DET|A| là ra được ma trận nghịch đảo
		adj.matrix = Transform(adj.matrix, adj.cap, adj.cap);
		if (Det(matrix, cap) == 0)
		{
			cout << "[ERROR] Khong ton tai ma tran kha nghich" << endl;
			return NULL;
		}
		else
		{
			for (int i = 0; i < cap; i++)
			{
				for (int j = 0; j < cap; j++)
				{
					//A^-1 = 1/Det|A| * Adj(A)
					adj.matrix[i][j] *= (double)1 / Det(matrix, cap);
				}
			}
		}

		return adj;
	}
	

}

Matrix Matrix::operator*(const Matrix b)
{
	if (col != b.row)
	{
		cout << "Khong the nhan(vi so cot KHAC so dong) \n";
		return NULL;
	}
	else
	{
		Matrix kq;
		//Kết quả trả về là một ma trận với cấp là : dòng của A x Cột của B
		CreateNewMatrix(kq.matrix, row, b.col);
		kq.row = row;
		kq.col = b.col;
		int c = 0;
		for (int i = 0; i < row; i++)
		{
			
			for (int j = 0; j < b.col; j++)
			{
				int sum = 0;
				for (int k = 0; k < col; k++)
				{

					sum += matrix[i][k] * b.matrix[k][j];
				}
				kq.matrix[i][c] = sum;
				c++;
			}
			c = 0;
		}
		return kq;
	}

	
}




//Swapline : hỗ trợ việc đổi dòng matrix nhiều số 0



/*
Thuật toán tìm hạng của ma trận:

Đầu tiên ta sẽ biến nó về dạng ma trận bậc thang như sau:
a b c d
0 x y z
0 0 j k
0 0 0 p

đó là một dạng của ma trận bậc thang(thật ra ma trận bậc thang có nhiều dàng)
Và hạng của nó = số dòng khác 0 của ma trận ấy.

*/



//Tìm vị trí cột đầu tiên !=0 của line 1
int FirstCol(double** matrix, int line, int col)
{
	for (int i = 0; i < col; i++)
	{
		if (matrix[line][i] != 0)
			return i;
	}
	return 0;
}
//Kiểm tra xem dòng đó có toàn số 0 không
bool isFullZero(double** matrix, int line, int col)
{
	for (int j = 0; j < col; j++)
	{
		if (matrix[line][j] != 0)
			return false;

	}
	return true;
}
//Đếm số dòng khác 0 trong matrix hỗ trợ việc tính rank
int DemSoDongKhac0(double** matrix, int row, int col)
{
	int count = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (matrix[i][j] != 0)
				break;
			else
			{
				if (j == col - 1)   //đã quét hết cột
				{
					count++;
				}
			}
		}
	}
	return row - count;
}
//Tính số lượng số 0 có trong dòng đó (từ đầu line cho đến khi gặp số !=0)
int CalcZero(double** matrix, int col, int line)
{
	int sum = 0;
	for (int j = 0; j < col; j++)
	{
		if (matrix[line][j] == 0)
			sum++;
		else
		{
			return sum;
		}
	}
	return sum;
}

void SwapLine(double** matrix, int row, int col)
{
	for (int i = 0; i < row - 1; i++)
	{
		for (int j = i + 1; j < row; j++)
		{
			if (CalcZero(matrix, col, i) > CalcZero(matrix, col, j))
				//Thực hiện swap line
			{
				for (int k = 0; k < col; k++)
				{
					swap(matrix[i][k], matrix[j][k]);
				}
			}
		}
	}
}


double**ChuyenVeBacThang(double** matrix, int row, int col)
{
	int cot = FirstCol(matrix, 0, col);  //Tìm số đầu tiên khác 0 dòng đầu tiên
	for (int i = 0; i < row - 1; i++)
	{
		for (int j = i + 1; j < row; j++)  //dòng chạy
		{
			//lấy giá trị dòng trên và dòng dưới
			int a = matrix[i][cot];
			int b = matrix[j][cot];
			for (int k = 0; k < col; k++)
			{
				//Tại mỗi cột thì trừ nhau cho mất đi giá trị đầu tiên(biến về ma trận bậc thang)
				matrix[j][k] = matrix[j][k] * a - matrix[i][k] * b;
			}

		}
		//Sau mỗi lần trừ , tránh trường hợp những dòng = 0 thì thuật toán sẽ bị sai.
		//Vì vậy sau mỗi lần trừ ta phải sort lại ma trận
		SwapLine(matrix, row, col);
		cot = FirstCol(matrix, i + 1, col);

	}
	return matrix;
}


int  Matrix::Rank()
{
	//Tạo ma trận với , mọi thao tác sẽ thực hiện trên biến matrix tạm(tránh thay đổi giá trị)
	double** matrixtemp;
	CreateNewMatrix(matrixtemp,row,col);
	//Gán bằng với matrix hiện tại của class:
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			matrixtemp[i][j] = matrix[i][j];
		}
	}
	
	//Gọi hàm thay đổi thứ tự các dòng
	SwapLine(matrixtemp, row, col);
	
	//Gọi hàm chuyển về ma trận bậc thang
	matrixtemp = ChuyenVeBacThang(matrixtemp, row, col);
	


	int Rank = DemSoDongKhac0(matrixtemp, row, col);
	delete matrixtemp;

	return Rank;

	//Bắt đầu tính hạng của ma trận = số dòng khác 0 của ma trận = số dòng - số dòng(=0);
}

int  Matrix::Rank(double** matrix , int d , int c)
{
	//Tạo ma trận với , mọi thao tác sẽ thực hiện trên biến matrix tạm(tránh thay đổi giá trị)
	double** matrixtemp;
	CreateNewMatrix(matrixtemp, d, c);
	//Gán bằng với matrix hiện tại của class:
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < c; j++)
		{
			matrixtemp[i][j] = matrix[i][j];
		}
	}

	//Gọi hàm thay đổi thứ tự các dòng
	SwapLine(matrixtemp, d, c);

	//Gọi hàm chuyển về ma trận bậc thang
	matrixtemp = ChuyenVeBacThang(matrixtemp, d, c);



	int Rank = DemSoDongKhac0(matrixtemp, d, c);
	delete matrixtemp;

	return Rank;

	//Bắt đầu tính hạng của ma trận = số dòng khác 0 của ma trận = số dòng - số dòng(=0);
}

//Hàm chuyển số qua chuỗi
string Tostring(int n)
{
	return std::to_string(n);
}





//Đây là hàm hộ trợ tìm nghiệm sau khi chuyển về ma trận bậc thang
void TimNghiem(double** matrix, int dong, int cot)
{

	int rank = DemSoDongKhac0(matrix, dong, dong);

	//_______________________________________________________________________________ TRƯỜNG HỢP HỆ VÔ NGHIỆM_______________________________________________________________________________

	if ((rank < dong && matrix[dong - 1][cot - 1] != 0) || (dong > cot-1 && matrix[dong - 1][cot - 1] != 0))
	{
		cout << "He vo nghiem" << endl;
		return;
	}
	//_____________________________________________________________________________ TRƯỜNG HỢP HỆ CÓ DUY NHẤT 1 NGHIỆM_______________________________________________________________
	else if (rank == dong)
	{
		//Mảng a dùng để lưu các nghiệm
		float* a = new float[dong];
		a[0] = (float)matrix[dong - 1][cot - 1] / matrix[dong - 1][cot - 2];
		int n = 1;
		for (int i = dong - 2; i >= 0; i--)
		{
			//Pos a là tại mỗi vị trí cần trừ để tìm x thì trừ đi những nghiệm đã có	
			int posA = 0;
			for (int j = cot - 2; j >= 0; j--)
			{
				//Nếu tại cột j không phải là một phần tử cơ sở(phần tử đầu tiên tại dòng !=0) , thì áp dụng quy tắc tìm x tính bình thường
				if (j != FirstCol(matrix, i, cot))
				{

					matrix[i][cot - 1] -= a[posA] * matrix[i][j];
					posA++;
				}
				//Sau khi đã đến vị trí cơ sở thì => nghiệm tại đó 
				else
				{
					matrix[i][cot - 1] = (float)matrix[i][cot - 1] / matrix[i][j];
					a[n++] = matrix[i][cot - 1];
				}
			}
		}
		for (int i = 0; i < n; i++)
			cout << "X" << n-i << " = "  << a[i] << "\n";
		delete[] a;
	}

	//__________________________________________________________________________________TRƯỜNG HỢP HỆ CÓ NGHIỆM TỰ DO__________________________________________________________________
	/*Để in được nghiệm tự do , ta sẽ thao tác với chuỗi khá nhiều , vì vậy để tách hệ số và  hệ số biến(ví dụ 5+6t . 5 là hệ số còn 6 là hệ số biến)
	ta sẽ phải gán cho t = 0 ; và tách ra để dễ xử lý hơn.
	Trong bài mảng a sẽ lưu giá trị hệ số tự do
	mang hesoT sẽ lưu các giá trị biến chứa tham số u1 , u2 .....
    */

	/*
	Ví dụ minh hoạ ta có ma trận sau:
	số biến : 4 
	số pt : 2
	=> Ta có ma trận , với 4 cột đầu tiên chứa biên , cột thứ 5 là vế phải sau phương trình
	1 2 17 -29  0
	0 1 -10 17  0
	0 0  0  0   0
	0 0  0  0   0
	B1 : Ban đầu ta thấy có 2 hàng trống (toàn số 0 ) => hệ có 2 nghiệm tự do(vì dòng = 4 và rank = 2 => nghiệm tự do = dòng - rank = 2)
	    lúc này ta có 2 biến tự do u1 và u2
	B2:Tại vị trí dòng đầu tiên có số khác 0 ta có 2 số -10 và 17 là 2 hệ số của biến
		Ta có mảng lưu hệ số :
		HeSo[0] = 17 và HeSo[1] = 10 ( Lưu lại nhằm xử lý , nhưng tại dòng đầu tiên này chưa cần dùng đến)

	    Lúc này  pt thứ 2 <=> x2 + (-10)u2 + 17u1 = 0
		=> biến x2 = 10u2 - 17u1;

		ở dòng đầu tiên này chưa có gì phức tạp , ta chỉ cần chuyển vế đổi dấu và nhớ chia cho hệ số của x , ta thu được biến
		x2 = 10u2 - 17u1;
		Vì để thực hiện tiếp tục với các pt trên , ta gọi thêm 1 mảng hệ số tạm , lúc này mảng sẽ lưu giá trị hệ số của biến x2
		ta lưu như sau(gọi for)
		HeSoTam[0] = -17 (Tức dòng đầu tiên , HeSoTam = HeSo
		HeSoTam[1] = 10
		dừng vòng for vì số nghiệm tự do đã =2;

	B3:Quay tiếp tục dòng trên
	tại đây ta có pt : x + 2(x2) + 17u2 - 29u1 = 0
	Lúc này mảng hệ số tiếp tục lưu 2 giá trị HeSo[0] = -29 và HeSo[1] = 17

	=> tại 2*x2 ta lấy 2 nhân từng hệ số trong HeSoTam : HeSoTam[0] =2*HeSoTam[0] = -34  và HeSoTam[1] = 2*HeSoTam[1] = 20
	sau đó cộng nó theo thứ tự để trùng nhau biến:
	HeSo[0] + HeSoTam[0] = -29+-34 = -63   , HeSo[1] + HeSoTam[1] = 37;
	=>Chia hệ số biến x và đổi dấu ta có
	x = 63u1 - 37u2;
	Đó là thuật toán.
	(Nhắm vào hệ số tạm vì nó là thứ sẽ lưu lại nghiệm trước đó ta tính)


	u1 , u2 , 10u2-17u1 , 63u1 - 37u2
	
	*/
	
	else if (rank < dong && matrix[dong - 1][cot - 1] == 0)
	{
		//Chuỗi nghiệm mục đích là để xuất ra những nghiệm mà ta tìm được
		string* ChuoiNghiem = new string[dong + 1];
		int soNghiemTuDo = 1;  //Đánh dấu nghiệm để xuất ra
		int nghiemThuN = 1;

		int slNghiem = dong - rank;
		
		//Dùng để đánh dấu các hệ số t
		float* hesoT = new float[dong + 1];
		float* hesoTam = new float[dong + 1];
		for (int i = 0; i < rank - dong; i++)
		{
			hesoTam[i] = 0;
		}
		int soluongHs = 0;
		int cotBien = cot - 2; //Thứ tự lấy nghiệm của các cột
		//Mảng a dùng để lưu các nghiệm
		float* a = new float[dong];
		//Gán các biến tự do = 0 hỗ trợ ta trong việc tách phần số và phần biến riêng
		int n = 1;

		int c;

		bool isStartZero = true;
		
		//Tìm dòng khác 0 đầu tiên
		for (int i = dong - 2; i >= 0; i--)
		{
			//Nếu dòng đó toàn số 0 thì tăng biến tự do lên 1.
			if (isFullZero(matrix, i, cot - 1) == true)
			{
				a[soNghiemTuDo++] = 0;
				n++;
			}
			else if (isFullZero(matrix, i, cot - 1) == false)
			{
				dong = i;
				if (CalcZero(matrix,cot-2,i) == cot - 2)
				{
					a[0] = (float)matrix[i][cot-1]/matrix[i][cot-2];
					ChuoiNghiem[0] = to_string(a[0]);
					isStartZero = true;
					
				}
				else
				{
					ChuoiNghiem[0] = "u1";
					a[0] = 0;
					isStartZero = false;
				}
				break;
			}

		}

	
		int StartVarCol;
		if (isStartZero == true)
		{
			for (int i = 1; i < slNghiem; i++)
			{
				//Gán chuỗi tại 1 trước vì tại 0 có thể xảy ra nhiều th đặc biệt
				ChuoiNghiem[i] = "u" + to_string(i);
				nghiemThuN++;
			}
			StartVarCol = cot - 3;
		}
		else
		{
			for (int i = 1; i < slNghiem; i++)
			{
				//Gán chuỗi tại 1 trước vì tại 0 có thể xảy ra nhiều th đặc biệt
				ChuoiNghiem[i] = "u" + to_string(i+1);
				nghiemThuN++;
			}
			//Vì biến cuối là hằng số nên không tồn tại hesoT tại đay
			StartVarCol = cot - 2;
		}




		//Tại các dòng khác 0 thì ta bắt đầu gán biến cho từng hệ số
		for (int i = dong; i >= 0; i--)
		{
			soluongHs = 0;
			//Lấy ra các hệ số mà tại đó chứa biên tự do
			for (c = StartVarCol; c >= 0; c--)
			{
				if (soluongHs == slNghiem)
					break;
				float heso = matrix[i][c];
				hesoT[soluongHs] = heso;
				hesoT[soluongHs] *= -1;
				soluongHs++;

			}
			
			//Dành cho trường hợp 1 nghiệm tự do:
			//Hệ số tại mỗi vị trí dòng sau khi đã lấy biến các dòng dưới.
			//số t sau = he số cột hiện tại x số biến t lúc trước
			//Pos a là tại mỗi vị trí cần trừ để tìm x thì trừ đi những nghiệm đã có	
			int posA = 0;

			//tongHS += matrix[i][cot - 2];
			for (int j = cot - 2; j >= 0; j--)
			{
				//Nếu tại cột j không phải là một phần tử cơ sở(phần tử đầu tiên tại dòng !=0) , thì áp dụng quy tắc tìm x tính bình thường
				if (j != FirstCol(matrix, i, cot))
				{
					matrix[i][cot - 1] -= a[posA] * matrix[i][j];
					posA++;
				}
				//Trường hợp line đó toàn 0
				//Sau khi đã đến vị trí cơ sở thì => nghiệm tại đó 
				else if (j == FirstCol(matrix, i, cot))
				{

					//Chia cho hệ số của biến cần tìm
					if (matrix[i][cot - 1] != 0)
					{
						matrix[i][cot - 1] = (float)matrix[i][cot - 1] / matrix[i][j];
					}
					a[n++] = matrix[i][cot - 1];
					//Tất nhiên phần biến cũng phải chia theo
					for (int z = 0; z < soluongHs; z++)
					{
						hesoT[z] /= (float)matrix[i][j];
					}
						int count = soluongHs;
						//Bắt đầu in giá trị , giá trị hệ số tự nhiên mà giá trị sau khi chuyển và vế phải.
						//nếu giá trị !=0 mới gán tránh thừa số
						if (matrix[i][cot - 1]!=0)
						     ChuoiNghiem[nghiemThuN] = to_string((float)matrix[i][cot - 1]);

						//Các giá trị biến cũ nhân với hệ số biến
						if (i != dong)
						{
							for (int z = 0; z < soluongHs; z++)
							{
								//Nhân lại biến với hệ số
								hesoTam[z] *= matrix[i][j + 1];
								//Chia lại cho hệ số biến cần tìm
								hesoTam[z] /= (float)matrix[i][j];
							}
						}


						//+ thêm hệ số biến
						for (int z = 0; z < count; z++)
						{
							//Khi i == dong thì chưa cần cộng thêm các hệ số biến tại vì hệ số biến mới chỉ được khởi tạo tại đây
							if (i == dong)
							{
								//Các lệnh if tránh xuất quá nhiều trường hợp thừa như 0*x làm xấu chương trình
								if(hesoT[z] > 0)
								ChuoiNghiem[nghiemThuN] += "+" + to_string(hesoT[z]) + "u" + to_string((z + 1));
								else if (hesoT[z] < 0)
								{
									ChuoiNghiem[nghiemThuN] += to_string(hesoT[z]) + "u" + to_string((z + 1));
								}
							}
							//Khi số dòng đã tăng lên lúc này ta phải cộng thêm các hệ số biến phía sau sau khi nhân hệ số cũ cho biến hệ số mới
							else
							{
								//Khi ta cộng như vậy vẫn đảm bảo thứ tự các biến được cộng (u1 + u1 , u2+u2 , ...)
								float hstemp = hesoT[z] + -hesoTam[z];
								hesoTam[z] = hstemp;
								if (hstemp < 0)
									ChuoiNghiem[nghiemThuN] += to_string(hstemp) + "u" + to_string((z + 1));
								else if(hstemp>0)
								ChuoiNghiem[nghiemThuN] += "+" + to_string(hstemp) + "u" + to_string((z + 1));
							}

						}
						nghiemThuN++;
					break;

				}

			}
			// Lưu lại hệ của các biến nhằm để trả về giá trị , sau đó nhân cùng với hệ số biến bên trên
			//Cập nhật he số tạm lần đầu tiên
			if (i == dong)
			{
				for (int z = 0; z < soluongHs; z++)
				{
					hesoTam[z] = hesoT[z];
				}
		}
		}
		for (int i = nghiemThuN-1; i >=0 ; i--)
		{
			cout << setprecision(2);
			cout << "Nghiem thu " << nghiemThuN-i<< ":"  << ChuoiNghiem[i]<<  "\n";
		}
		delete[] a;
	}
}

void Matrix::TimNghiemHePTTT()
{
	//Chuyển về ma trận bậc thang
	double** matrixtemp;
	int sobien, pt;
	cout << "Nhap vao so bien cua he phuong trinh : ";
	cin >> sobien;
	cout << "Nhap vao so phuong trinh : ";
	cin >> pt;
	
	int cloneDong0 = 0;
	if (pt < sobien)
		cloneDong0 = sobien - pt;
	//Ta sẽ tạo với số biến + 1 tại vì thêm vào giá trị của mỗi phương trình nữa
	CreateNewMatrix(matrixtemp , pt+ cloneDong0, sobien+1);


	cout << "Nhap tuan tu cac he phuong trinh : " << endl;
	//Gán bằng với matrix hiện tại của class:
	for (int i = 0; i < pt; i++)
	{
		for (int j = 0; j < sobien+1; j++)
		{
			cin >> matrixtemp[i][j];
		}
	}


	for (int i = pt; i < pt+cloneDong0; i++)
	{
		for (int j = 0; j < sobien + 1; j++)
		{
			matrixtemp[i][j] = 0;
		}
	}

	//TH Đặc biệt
	//Gọi hàm thay đổi thứ tự các dòng
	SwapLine(matrixtemp, row, col);


	//Gọi hàm chuyển về ma trận bậc thang
	matrixtemp = ChuyenVeBacThang(matrixtemp, pt+cloneDong0, sobien+1);

	//Khi quá thừa thải phương trình thì mình bỏ bớt số dòng full 0
	if (pt > sobien)
	{
		for (int i = pt-1; i >= 0; i--)
		{
			if (isFullZero(matrixtemp, i, sobien + 1))
			{
				pt--;
				if (pt == sobien)
					break;
			}
		}
	}
	//Tìm nghiệm:
	cout << "Nghiem cua phuong trinh la : " << endl;
	TimNghiem(matrixtemp, pt+cloneDong0 , sobien+1);

	
	
}


void Matrix::MainMenu()
{
	int select;
	while (1)
	{
		cout << "\n\n";
		cout << "___________________LINEAR ALGEBRA__________________" << endl;
		cout << "1.Nhap ma tran - Input" << endl;
		cout << "2.Xuat ma tran - Ouput" << endl;
		cout << "3.Tinh Dinh thuc - Determinant " << endl;
		cout << "4.Nghich dao cua ma tran - Inverse" << endl;
		cout << "5.Hang cua ma tran - Rank" << endl;
		cout << "6.Giai he phuong trinh tuyen tinh - Solve linear equations" << endl;
		cout << "\n+Your select : ";
		cin >> select;
		switch (select)
		{
		case 1:
			cin >> *this;
			break;
		case 2:
			cout << *this;
			break;
		case 3:
			//Nếu nó là 1 ma trân vuông
			if (cap != 0)
			{
				cout << "Dinh thuc cua ma tran = " << Det(matrix, cap) << endl;
			}
			else
			{
				cout << "[ERROR] Ma tran khong phai la mot ma tran vuong , khong the tinh dinh thuc" << endl;
			}
			break;
		case 4:
			if (cap != 0)
			{
				cout << "Ma tran nghich dao(Kha nghich) co dang : " << endl;
				cout << inverse();
			}
			else
			{
				cout << "[ERROR] Ma tran nghich dao phai la mot ma tran vuong." << endl;
			}
			break;
		case 5:
			cout << "Hang cua ma tran la : ";
			cout << Rank();
			break;
		case 6:
			TimNghiemHePTTT();
			break;
		default:
			break;
		}

		system("pause");
		system("cls");
	}
}

Matrix::~Matrix()
{

}
