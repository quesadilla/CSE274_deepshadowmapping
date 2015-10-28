
#include <math.h>

#include "Matrix4.h"

Matrix4::Matrix4()
{
  // column number
  for (int i=0; i<4; ++i)
  {
	// row number
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
} 

Matrix4::Matrix4(double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;
	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;
	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;
	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}



Matrix4& Matrix4::operator=(const Matrix4& m2)
{
  if (this != &m2)
  {
	// column number
    for (int i=0; i<4; ++i)
    {
	  // row number
      for (int j=0; j<4; ++j)
      {
        m[i][j] = m2.m[i][j];
      }
    }
  }
  return *this;
}

Matrix4 Matrix4::operator*(Matrix4& m2)
{
	Matrix4 product = Matrix4();
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			for (int k = 0; k < 4; k++)
			{
				// m goes across the row, while m2 goes down a column
				product.m[i][j] += m[i][k] * m2.m[k][j];
			}
		}
	}
	return product;
}

Vector4 Matrix4::operator*(Vector4& v)
{
	double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
	x = v.getX() * m[0][0] + v.getY() * m[0][1] + v.getZ() * m[0][2] + v.getW() * m[0][3];
	y = v.getX() * m[1][0] + v.getY() * m[1][1] + v.getZ() * m[1][2] + v.getW() * m[1][3];
	z = v.getX() * m[2][0] + v.getY() * m[2][1] + v.getZ() * m[2][2] + v.getW() * m[2][3];
	w = v.getX() * m[3][0] + v.getY() * m[3][1] + v.getZ() * m[3][2] + v.getW() * m[3][3];
	return Vector4(x,y,z,w);
}

Vector3 Matrix4::operator*(Vector3& v)
{
	double x = 0.0, y = 0.0, z = 0.0;
	x = v.getX() * m[0][0] + v.getY() * m[0][1] + v.getZ() * m[0][2];
	y = v.getX() * m[1][0] + v.getY() * m[1][1] + v.getZ() * m[1][2];
	z = v.getX() * m[2][0] + v.getY() * m[2][1] + v.getZ() * m[2][2];
	return Vector3(x, y, z);
}

// return pointer to matrix elements
double* Matrix4::getPointer()
{
  return &m[0][0];
}

// set matrix to identity matrix
Matrix4 Matrix4::identity()
{
  Matrix4 I = Matrix4();

  // column number
  for (int i=0; i<4; ++i)
  {
	// row number
    for (int j=0; j<4; ++j)
    {
      if (i==j) 
        I.m[i][j] = 1.0; 
      else 
        I.m[i][j] = 0.0;
    }
  }
  return I;
}

// transpose the matrix (mirror at diagonal)
Matrix4 Matrix4::transpose()
{
  Matrix4 temp;
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      temp.m[j][i] = m[i][j];
    }
  }
  return temp; 
}

// Creates a rotation matrix which rotates about the x axis.
// angle is expected in degrees
Matrix4 Matrix4::makeRotateX(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	Matrix4 R = identity();
	R.m[1][1] = cos(angle);
	R.m[2][1] = sin(angle);
	R.m[1][2] = -sin(angle);
	R.m[2][2] = cos(angle);
	return R;
}

// Creates a rotation matrix which rotates about the y axis.
// angle is expected in degrees
Matrix4 Matrix4::makeRotateY(double angle)
{
  angle = angle / 180.0 * M_PI;  // convert from degrees to radians
  Matrix4 R = identity();
	R.m[0][0] = cos(angle);
	R.m[0][2] = sin(angle);
	R.m[2][0] = -sin(angle);
	R.m[2][2] = cos(angle);
	return R;
}

// Creates a rotation matrix which rotates about the z axis.
// angle is expected in degrees
Matrix4 Matrix4::makeRotateZ(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	Matrix4 R = identity();
	R.m[0][0] = cos(angle);
	R.m[1][0] = sin(angle);
	R.m[0][1] = -sin(angle);
	R.m[1][1] = cos(angle);
	return R;
}

Matrix4 Matrix4::makeRotate(double angle, Vector3& axis)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	Vector3 v = axis.normalize();
	double sin = std::sin(angle);
	double cos = std::cos(angle);
	double x = v.getX(), y = v.getY(), z = v.getZ();
	Matrix4 R = identity();
	R.m[0][0] = cos + x * x * (1 - cos);
	R.m[0][1] = x * y * (1 - cos) - (z * sin);
	//R.m[0][2] = y * z * (1 - cos) - x * sin;
	R.m[0][2] = x * z * (1 - cos) + (y * sin);

	R.m[1][0] = y * x * (1 - cos) + (z * sin);
	R.m[1][1] = cos + y * y * (1 - cos);
	R.m[1][2] = y * z * (1 - cos) - (x * sin);

	R.m[2][0] = z * x * (1 - cos) - (y * sin);
	R.m[2][1] = z * y * (1 - cos) + (x * sin);
	R.m[2][2] = cos + z * z * (1 - cos);
	return R.transpose();
}

Matrix4 Matrix4::makeScale(double sx, double sy, double sz)
{
	Matrix4 S = identity();
	S.m[0][0] = sx;
	S.m[1][1] = sy;
	S.m[2][2] = sz;
	return S;
}

Matrix4 Matrix4::makeTranslate(double tx, double ty, double tz)
{
	Matrix4 T = identity();
	T.m[0][3] = tx;
	T.m[1][3] = ty;
	T.m[2][3] = tz;
	return T;
}

Matrix4 Matrix4::makeTranslate(Vector3 v)
{
	return makeTranslate(v.getX(), v.getY(), v.getZ());
}

/*Matrix4 Matrix4::addTranslate(double tx, double ty, double tz)
{

}*/

void Matrix4::print(std::string comment)
{

	std::cout << comment << std::endl;
	// column number
	for (int i = 0; i < 4; ++i)
	{
		// row number
		for (int j = 0; j < 4; ++j)
		{
			std::cout << m[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Matrix4::setX(Vector3 xVect)
{

	m[0][0] = xVect.getX();
	m[1][0] = xVect.getY();
	m[2][0] = xVect.getZ();
}

void Matrix4::setY(Vector3 yVect)
{
	m[0][1] = yVect.getX();
	m[1][1] = yVect.getY();
	m[2][1] = yVect.getZ();
}

void Matrix4::setZ(Vector3 zVect)
{
	m[0][2] = zVect.getX();
	m[1][2] = zVect.getY();
	m[2][2] = zVect.getZ();
}

double * Matrix4::getRow(int index)
{
	double currRow[4];

	if (index > -1 && index < 4)
	{
		currRow[0] = m[index][0];
		currRow[1] = m[index][1];
		currRow[2] = m[index][2];
		currRow[3] = m[index][3];

		return currRow;
	}
	else
	{
		return nullptr;
	}
}

double * Matrix4::getCol(int index)
{
	double currCol[4];

	if (index > -1 && index < 4)
	{
		currCol[0] = m[0][index];
		currCol[1] = m[1][index];
		currCol[2] = m[2][index];
		currCol[3] = m[3][index];

		return currCol;
	}
	else
	{
		return nullptr;
	}
}
