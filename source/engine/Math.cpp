#include "Math.h"

Matrix4x4::Matrix4x4(Matrix_Init_Type n_initType )
{
	for( int i = 0; i < 4; i++)
	{
		for( int j = 0; j < 4; j++ )
		{
			m_matrix[i][j] = 0.0;
		}
	}

	if( n_initType == Matrix_Init_Type_Identity )
	{
		for( int i = 0; i < 4; i++ )
			m_matrix[i][i] = 1.0;
	}
}

double* Matrix4x4::operator[](const int n_index)
{
	return m_matrix[n_index];
}

double Matrix4x4::Get_Const(int n_i, int n_j ) const
{
	return m_matrix[n_i][n_j];
}

std::ostream& operator<< (std::ostream &out, const Matrix4x4 &n_obj)
{
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			out << n_obj.m_matrix[i][j] << " ";
		}
		out << std::endl;
	}
	return out;
}

Matrix4x4 operator*(const Matrix4x4 &l, const Matrix4x4 &r)
{
	Matrix4x4 tmp_new(Matrix_Init_Type_Empty);

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				tmp_new[i][j] += l.Get_Const(i,k) * r.Get_Const(k,j);
			}
		}
	}
	return tmp_new;
}
