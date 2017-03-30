#include "Matrix4.h"

namespace EngineTest
{
	namespace Math
	{
		Matrix4::Matrix4()
		{
			for (int i = 0; i < 16; i++)
				_elements[i] = 0.0f;
		}
		Matrix4::Matrix4(float diagonal)
		{
			for (int i = 0; i < 16; i++)
				_elements[i] = 0.0f;
			_elements[0] = _elements[5] = _elements[10] = _elements[15] = diagonal; 
		}

		Matrix4 Matrix4::Identitiy()
		{
			return Matrix4(1.0f);
		}
		Matrix4 Matrix4::Orthographic(double left, double right, double top, double bottom, double near, double far)
		{
			Matrix4 result(1.0f);

			result._elements[0] = (float)(2.0 / (right - left));
			result._elements[5] = (float)(2.0 / (top - bottom));
			result._elements[10] = (float)(2.0 / (near - far));
			result._elements[12] = (float)((left + right) / (left - right));
			result._elements[13] = (float)((bottom + top) / (bottom - top));
			result._elements[14] = (float)((far + near) / (far - near));

			return result;
		}
		Matrix4 Matrix4::Perspective(double fov, double aspectRatio, double near, double far)
		{
			Matrix4 result(1.0f);
			
			double q = 1.0 / tan(ToRadians(0.5 * fov));
			double a = q / aspectRatio;
			double b = (near + far) / (near - far);
			double c = (2.0 * near * far) / (near - far);

			result._elements[0] = (float)a;
			result._elements[5] = (float)q;
			result._elements[10] = (float)b;
			result._elements[11] = -1.0f;
			result._elements[14] = (float)c;

			return result;
		}
		Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
		{
			Matrix4 result(1.0f);

			Vector3 f = (at - eye).NormalizeEquals();
			Vector3 s = f.Cross(up.Normalize());
			Vector3 u = s.Cross(f);


			result._elements[0] = s._x;
			result._elements[4] = s._y;
			result._elements[8] = s._z;

			result._elements[1] = u._x;
			result._elements[5] = u._y;
			result._elements[9] = u._z;

			result._elements[2] = -f._x;
			result._elements[6] = -f._y;
			result._elements[10] = -f._z;

			return result * Matrix4::Translate(Vector3(-eye._x, -eye._y, -eye._z));
		}
		Matrix4 Matrix4::Translate(const Vector3& translate)
		{
			Matrix4 result(1.0f);

			result._elements[12] = translate._x;
			result._elements[13] = translate._y;
			result._elements[14] = translate._z;

			return result;
		}
		Matrix4 Matrix4::Scale(const Vector3& scale)
		{
			Matrix4 result(1.0f);

			result._elements[0] = scale._x;
			result._elements[5] = scale._y;
			result._elements[10] = scale._z;

			return result;
		}
		Matrix4 Matrix4::Rotate(const double angle, const Vector3& axis)
		{
			double r = ToRadians(angle);
			double c = cos(r);
			double omc = 1.0 - c;
			double s = sin(r);

			double x = (double)axis._x;
			double y = (double)axis._y;
			double z = (double)axis._z;

			Matrix4 result(1.0f);

			result._elements[0] =		(float)(x * x * omc + c);
			result._elements[1] =		(float)(x * y * omc + z * s);
			result._elements[2] =		(float)(x * z * omc - y * s);

			result._elements[4] =		(float)(y * x * omc - z * s);
			result._elements[5] =		(float)(y * y * omc + c);
			result._elements[6] =		(float)(y * z * omc + x * s);

			result._elements[8] =		(float)(z * x * omc - y * s);
			result._elements[9] =		(float)(z * y * omc - x * s);
			result._elements[10] =		(float)(z * z * omc + c);

			return result;
		}
		Matrix4 Matrix4::Multiply(const Matrix4& matrix) const
		{
			Matrix4 result;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					float sum = 0.0f;
					for (int e = 0; e < 4; e++)
					{
						sum += _elements[x + e * 4] * matrix._elements[e + y * 4];
					}
					result._elements[x + y * 4] = sum;
				}
			}
			return result;
		}
		Matrix4& Matrix4::MultiplyEquals(const Matrix4& matrix)
		{
			float data[16];
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					float sum = 0.0f;
					for (int e = 0; e < 4; e++)
					{
						sum += _elements[x + e * 4] * matrix._elements[e + y * 4];
					}
					data[x + y * 4] = sum;
				}
			}
			memcpy(_elements, data, sizeof(float) * 16);
			return *this;
		}

		Matrix4 operator*(const Matrix4& left, const Matrix4& right)
		{
			return left.Multiply(right);
		}
		Matrix4& operator*=(Matrix4& left, const Matrix4& right)
		{
			return left.MultiplyEquals(right);
		}

		void Matrix4::Log()
		{
			LOG("Matrix4:\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f", _elements[0], _elements[4], _elements[8], _elements[12], _elements[1], _elements[5], _elements[9], _elements[13], _elements[2], _elements[6], _elements[10], _elements[14], _elements[3], _elements[7], _elements[11], _elements[15]);
		}
	}
}