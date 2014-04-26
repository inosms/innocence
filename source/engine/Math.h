#ifndef _MATH_H_
#define _MATH_H_

#include <ostream>

enum Matrix_Init_Type
{
	Matrix_Init_Type_Empty,
	Matrix_Init_Type_Identity
};

class Matrix4x4
{
public:
	double m_matrix[4][4];

	Matrix4x4(Matrix_Init_Type n_initType = Matrix_Init_Type_Empty);

	double* operator[](const int n_index);
	double Get_Const(int n_i, int n_j ) const;

	friend std::ostream& operator<<(std::ostream  &out, const Matrix4x4 &n_obj);
	friend Matrix4x4 operator*(const Matrix4x4 &l,const Matrix4x4 &r);

};

#endif