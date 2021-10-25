/*
	Salei, Roya

	October 25th, 2020

	CS3310
	Project 1
*/
#include "stdafx.h"


void read_matrices_from_file(int** a, int** b, int n, int size);
void matrix_builder();
void multiply_iterative(int** a, int** b, int** r, int n);
void display_matrix(int** a, int** b, int** r, int n);
void add_matrices(int** a, int** b, int** r, int n);
void multiply_divide_conquer(int** a, int** b, int** r, int n);
void subtract_matrices(int** a, int** b, int** c, int n);
void multiply_strassen(int** a, int** b, int** r, int n);


int main()
{
	//matrix_builder();
	
	int max_size = 16;  // size can be changed from 2,4,8,16,32,64,128,256
	int size = 2;


	std::ofstream algo_time("times.csv", std::ios::app);
	for (size; size < max_size; size *= 2)
	{
		
		int** m1 = new int* [size];
		int** m2 = new int* [size];
		int** r = new int* [size];
		for (int i = 0; i < size; ++i)
		{
			m1[i] = new int[size];
			m2[i] = new int[size];
			r[i] = new int[size];
			for (int j = 0; j < size; ++j)
			{
				m1[i][j] = 0;
				m2[i][j] = 0;
				r[i][j] = 0;
			}
		}

		int line_number = int(log2(double(size)));

		read_matrices_from_file(m1, m2, line_number, size);

		std::cout << "  Matrices Before Multiplication: " << std::endl;
		display_matrix(m1, m2, r, size);

		double iterative_time = 0;
		double divide_conquer_time = 0;
		double strassen_time = 0;

		for (int i = 0; i < 1; i++)
		{
			auto start_iterative = std::chrono::high_resolution_clock::now();
			multiply_iterative(m1, m2, r, size);
			auto finish_iterative = std::chrono::high_resolution_clock::now();
			double t1 = std::chrono::duration_cast<std::chrono::microseconds>(finish_iterative - start_iterative).count();
			//iterative_time += t1;
			std::cout << "  Iterative Algo " << std::endl;
			display_matrix(m1, m2, r, size);

			auto start_divide_conquer = std::chrono::high_resolution_clock::now();
			multiply_divide_conquer(m1, m2, r, size);
			auto finish_divide_conquer = std::chrono::high_resolution_clock::now();
			double t2 = std::chrono::duration_cast<std::chrono::microseconds>(finish_divide_conquer - start_divide_conquer).count();
			//algo_time << t2 << ",";
			//divide_conquer_time += t2;
			std::cout << "  Divide and Conquer Algo " << std::endl;
			display_matrix(m1, m2, r, size);

			auto start_strassen = std::chrono::high_resolution_clock::now();
			multiply_strassen(m1, m2, r, size);
			auto finish_strassen = std::chrono::high_resolution_clock::now();
			double t3 = std::chrono::duration_cast<std::chrono::microseconds>(finish_strassen - start_strassen).count();
			//strassen_time += t3;
			//algo_time << t3 << ",";
			std::cout << "  Strassen Algo " << std::endl;
			display_matrix(m1, m2, r, size);
		}
		//algo_time << std::endl;
		//algo_time << (iterative_time / 20) << ",";
		//algo_time << (divide_conquer_time / 20) << ",";
		//algo_time << (strassen_time / 20) << std::endl;

		for (int i = 0; i < size; i++)
		{
			delete[]m1[i]; m1[i] = nullptr;
			delete[]m2[i]; m2[i] = nullptr;
			delete[]r[i]; r[i] = nullptr;
		}
		delete[]m1; delete[]m2; delete[]r;
		m1 = nullptr; m2 = nullptr; r = nullptr;
	}
	
	
	algo_time.close();
	std::cout << std::endl;
	system("pause");
	return 0;
}

void matrix_builder()
{
	int n[] = { 2, 4, 8, 16, 32, 64, 128, 256 };

	std::ofstream Matrix("matrices.csv");

	for (int i = 0; i < 8; i++)
	{
		int size = n[i];
		for (int j = 0; j < size; j++)
		{
			Matrix << rand() % (100) + 1 << ",";
		}
		Matrix << "\n";
	}
	Matrix.close();
}

void read_matrices_from_file(int** a, int** b, int n, int size)
{
	bool found = false;
	std::ifstream myFile("matrices.csv");
	std::string line;
	if (myFile.is_open()) 
	{
		while (!myFile.eof() && !found)
		{
			for (int i = 1; getline(myFile, line) && i < 9; i++)
				if (i == n)
				{
					char c;
					int num, j=0;
					std::istringstream ss(line);
					
					while (ss >> num >> c && c == ',')
					{
						a[0][j] = num;
						b[0][j] = num;
						j++;
					}
					found = true;
					if (found) 
						break;
				}
		}
		myFile.close();
	}
	else
		throw std::runtime_error("Could not open file");

	// fill up the rest of the matrix
	for (int row = 1; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			a[row][col] = a[0][col];
			b[row][col] = b[0][col];

		}
	}
}


void multiply_iterative(int** a, int** b, int** r, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			r[i][j] = 0;
			for (int k = 0; k < n; k++)
			{
				r[i][j] += a[k][j] * b[j][k];
			}
		}
	}
}

void add_matrices(int** a, int** b, int** r, int n) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			r[i][j] = a[i][j] + b[i][j];
		}
}

void display_matrix(int** a, int** b, int** r, int n)
{
	std::cout << "\n  Matrix 1 \t Matrix 2 \tProduct Matrix " << std::endl;
	std::cout << "  -----------\t----------\t----------------" << std::endl;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			
			std::cout <<" "<< a[i][j] << " ";
		}
		std::cout << "\t";
		for (int j = 0; j < n; ++j)
		{

			std::cout << " " << b[i][j] << " ";
		}
		std::cout << "\t";
		for (int j = 0; j < n; ++j)
		{

			std::cout << " " << r[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void multiply_divide_conquer(int** a, int** b, int** r, int n) 
{
	int mid = n / 2;
	if (n == 1) {
		r[0][0] = a[0][0] * b[0][0];
	}
	else if (n == 2)
	{
		r[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]);
		r[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]);
		r[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]);
		r[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]);
	}
	else {
		int** a11 = new int* [mid];
		int** a12 = new int* [mid];
		int** a21 = new int* [mid];
		int** a22 = new int* [mid];

		int** b11 = new int* [mid];
		int** b12 = new int* [mid];
		int** b21 = new int* [mid];
		int** b22 = new int* [mid];

		int** r11 = new int* [mid];
		int** r12 = new int* [mid];
		int** r21 = new int* [mid];
		int** r22 = new int* [mid];

		int** matrix1 = new int* [mid];
		int** matrix2 = new int* [mid];

		for (int i = 0; i < mid; i++) {
			a11[i] = new int[mid];
			a12[i] = new int[mid];
			a21[i] = new int[mid];
			a22[i] = new int[mid];

			b11[i] = new int[mid];
			b12[i] = new int[mid];
			b21[i] = new int[mid];
			b22[i] = new int[mid];

			r11[i] = new int[mid];
			r12[i] = new int[mid];
			r21[i] = new int[mid];
			r22[i] = new int[mid];

			matrix1[i] = new int[mid];
			matrix2[i] = new int[mid];
		}

		for (int i = 0; i < mid; i++)
			for (int j = 0; j < mid; j++) {
				a11[i][j] = a[i][j];
				a12[i][j] = a[i][j + mid];
				a21[i][j] = a[i + mid][j];
				a22[i][j] = a[i + mid][j + mid];

				b11[i][j] = b[i][j];
				b12[i][j] = b[i][j + mid];
				b21[i][j] = b[i + mid][j];
				b22[i][j] = b[i + mid][j + mid];
			}

		multiply_divide_conquer(a11, b11, matrix1, mid);
		multiply_divide_conquer(a12, b21, matrix2, mid);
		add_matrices(matrix1, matrix2, r11, mid);

		multiply_divide_conquer(a11, b12, matrix1, mid);
		multiply_divide_conquer(a12, b22, matrix2, mid);
		add_matrices(matrix1, matrix2, r12, mid);

		multiply_divide_conquer(a21, b11, matrix1, mid);
		multiply_divide_conquer(a22, b21, matrix2, mid);
		add_matrices(matrix1, matrix2, r21, mid);

		multiply_divide_conquer(a21, b12, matrix1, mid);
		multiply_divide_conquer(a22, b22, matrix2, mid);
		add_matrices(matrix1, matrix2, r22, mid);


		for (int i = 0; i < mid; i++)
			for (int j = 0; j < mid; j++) {
				r[i][j] = r11[i][j];
				r[i][j + mid] = r12[i][j];
				r[i + mid][j] = r21[i][j];
				r[i + mid][j + mid] = r22[i][j];
			}



		for (int i = 0; i < mid; i++) {
			delete[] a11[i]; delete[] a12[i]; delete[] a21[i]; delete[] a22[i];			
			delete[] b11[i]; delete[] b12[i]; delete[] b21[i]; delete[] b22[i];
			delete[] r11[i]; delete[] r12[i]; delete[] r21[i]; delete[] r22[i];

			a11[i] = nullptr; a12[i] = nullptr; a21[i] = nullptr; a22[i] = nullptr;			
			b11[i] = nullptr; b12[i] = nullptr; b21[i] = nullptr; b22[i] = nullptr;			
			r11[i] = nullptr; r12[i] = nullptr; r21[i] = nullptr; r22[i] = nullptr;
			
			delete[] matrix1[i]; delete[] matrix2[i]; 
			matrix1[i] = nullptr; matrix2[i] = nullptr;
		}
		delete[]a11; delete[]a12; delete[]a21; delete[]a22;
		delete[]b11; delete[]b12; delete[]b21; delete[]b22;
		delete[]r11; delete[]r12; delete[]r21; delete[]r22;
		delete[]matrix1; delete[]matrix2;

		a11 = nullptr; a12 = nullptr; a21 = nullptr; a22 = nullptr;
		b11 = nullptr; b12 = nullptr; b21 = nullptr; b22 = nullptr;
		r11 = nullptr; r12 = nullptr; r21 = nullptr; r22 = nullptr;
		matrix1 = nullptr; matrix2 = nullptr;
	}

}

void subtract_matrices(int** a, int** b, int** c, int n) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			c[i][j] = a[i][j] - b[i][j];
		}
}

void multiply_strassen(int** a, int** b, int** r, int n) 
{
	int mid = n / 2;
	if (n == 1) {
		r[0][0] = a[0][0] * b[0][0];
	}
	else if (n == 2)
	{
		r[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]);
		r[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]);
		r[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]);
		r[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]);
	}
	else {
		int** a11 = new int* [mid];
		int** a12 = new int* [mid];
		int** a21 = new int* [mid];
		int** a22 = new int* [mid];

		int** b11 = new int* [mid];
		int** b12 = new int* [mid];
		int** b21 = new int* [mid];
		int** b22 = new int* [mid];

		int** r11 = new int* [mid];
		int** r12 = new int* [mid];
		int** r21 = new int* [mid];
		int** r22 = new int* [mid];

		int** matrix1 = new int* [mid];
		int** matrix2 = new int* [mid];

		int** m1 = new int* [mid];
		int** m2 = new int* [mid];
		int** m3 = new int* [mid];
		int** m4 = new int* [mid];
		int** m5 = new int* [mid];
		int** m6 = new int* [mid];
		int** m7 = new int* [mid];

		for (int i = 0; i < mid; i++) {
			a11[i] = new int[mid];
			a12[i] = new int[mid];
			a21[i] = new int[mid];
			a22[i] = new int[mid];

			b11[i] = new int[mid];
			b12[i] = new int[mid];
			b21[i] = new int[mid];
			b22[i] = new int[mid];

			r11[i] = new int[mid];
			r12[i] = new int[mid];
			r21[i] = new int[mid];
			r22[i] = new int[mid];

			matrix1[i] = new int[mid];
			matrix2[i] = new int[mid];

			m1[i] = new int[mid];
			m2[i] = new int[mid];
			m3[i] = new int[mid];
			m4[i] = new int[mid];
			m5[i] = new int[mid];
			m6[i] = new int[mid];
			m7[i] = new int[mid];
		}

		for (int i = 0; i < mid; i++)
			for (int j = 0; j < mid; j++) {
				a11[i][j] = a[i][j];
				a12[i][j] = a[i][j + mid];
				a21[i][j] = a[i + mid][j];
				a22[i][j] = a[i + mid][j + mid];

				b11[i][j] = b[i][j];
				b12[i][j] = b[i][j + mid];
				b21[i][j] = b[i + mid][j];
				b22[i][j] = b[i + mid][j + mid];
			}


		add_matrices(a11, a22, matrix1, mid); // a11 + a22
		add_matrices(b11, b22, matrix2, mid); // b11 + b22
		multiply_strassen(matrix1, matrix2, m1, mid); //m1 = (a11 + a22) (b11 + b22)

		add_matrices(a21, a22, matrix1, mid); // a21 + a22
		multiply_strassen(matrix1, b11, m2, mid); //m2 = (a21 + a22) b11

		subtract_matrices(b12, b22, matrix1, mid); // b12 - b22
		multiply_strassen(a11, matrix1, m3, mid); //m3 = a11 (b12 - b22)

		subtract_matrices(b21, b11, matrix1, mid); // b21 - b11
		multiply_strassen(a22, matrix1, m4, mid); //m4 = a22 (b21 - b11)

		add_matrices(a11, a12, matrix1, mid); // a11 + a12
		multiply_strassen(matrix1, b22, m5, mid); //m5 = (a11 + a12) b22

		subtract_matrices(a21, a11, matrix1, mid); // a21 - a11
		add_matrices(b11, b12, matrix2, mid); // b11 + b12
		multiply_strassen(matrix1, matrix2, m6, mid); //m6 = (a21 - a11) (b11 + b12)

		subtract_matrices(a12, a22, matrix1, mid); // a12 - a22
		add_matrices(b21, b22, matrix2, mid); // b21 + b22
		multiply_strassen(matrix1, matrix2, m7, mid); //m7 = (a12 - a22) (b21 + b22)

		add_matrices(m1, m4, matrix1, mid); // m1 + m4
		add_matrices(matrix1, m7, matrix2, mid); // m1 + m4 + m7
		subtract_matrices(matrix2, m5, r11, mid);// c11 = m1 + m4 - m5 + m7
		add_matrices(m3, m5, r12, mid); // c12 = m3 + m5
		add_matrices(m2, m4, r21, mid); // c21 = m2 + m4
		add_matrices(m1, m3, matrix1, mid); // m1 + m3
		add_matrices(matrix1, m6, matrix2, mid); //m1 + m3 + m6
		subtract_matrices(matrix2, m2, r22, mid); // c22 = m1 - m2 + m3 + m6

		for (int i = 0; i < mid; i++)
			for (int j = 0; j < mid; j++) {
				r[i][j] = r11[i][j];
				r[i][j + mid] = r12[i][j];
				r[i + mid][j] = r21[i][j];
				r[i + mid][j + mid] = r22[i][j];
			}



		for (int i = 0; i < mid; i++) 
		{
			delete[] a11[i]; delete[] a12[i]; delete[] a21[i]; delete[] a22[i];
			delete[] b11[i]; delete[] b12[i]; delete[] b21[i]; delete[] b22[i];
			delete[] r11[i]; delete[] r12[i]; delete[] r21[i]; delete[] r22[i];

			a11[i] = nullptr; a12[i] = nullptr; a21[i] = nullptr; a22[i] = nullptr;
			b11[i] = nullptr; b12[i] = nullptr; b21[i] = nullptr; b22[i] = nullptr;
			r11[i] = nullptr; r12[i] = nullptr; r21[i] = nullptr; r22[i] = nullptr;

			delete[] matrix1[i]; delete[] matrix2[i];
			matrix1[i] = nullptr; matrix2[i] = nullptr;
		}
		delete[]a11; delete[]a12; delete[]a21; delete[]a22;
		delete[]b11; delete[]b12; delete[]b21; delete[]b22;
		delete[]r11; delete[]r12; delete[]r21; delete[]r22;
		delete[]matrix1; delete[]matrix2;

		a11 = nullptr; a12 = nullptr; a21 = nullptr; a22 = nullptr;
		b11 = nullptr; b12 = nullptr; b21 = nullptr; b22 = nullptr;
		r11 = nullptr; r12 = nullptr; r21 = nullptr; r22 = nullptr;
		matrix1 = nullptr; matrix2 = nullptr;
	}
}