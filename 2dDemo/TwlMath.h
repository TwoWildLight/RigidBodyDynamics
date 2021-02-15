#pragma once
#include <DirectXMath.h>
#include <utility>
#include <vector>

constexpr float PI_F = 3.1415926535F;

namespace math
{
	using Int3 = DirectX::XMINT3;
	using Int4 = DirectX::XMINT4;
	using Float2 = DirectX::XMFLOAT2;
	using Float3 = DirectX::XMFLOAT3;

	class Matrix;
	class Vector final
	{
	public:
		Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
		Vector(const Vector& v);
		Vector(const DirectX::XMFLOAT3& f3);
		Vector(const DirectX::XMVECTOR& v);
		Vector operator -() const;
		Vector& operator =(const Vector& v);
		Vector& operator =(const DirectX::XMFLOAT3& f3);
		Vector& operator =(const DirectX::XMVECTOR& v);
		Vector operator +(const Vector& v) const;
		Vector operator +(const DirectX::XMFLOAT3& f3) const;
		Vector operator +(const DirectX::XMVECTOR& v) const;
		Vector& operator +=(const Vector& v);
		Vector& operator +=(const DirectX::XMFLOAT3& f3);
		Vector& operator +=(const DirectX::XMVECTOR& v);
		Vector operator -(const Vector& v) const;
		Vector operator -(const DirectX::XMFLOAT3& f3) const;
		Vector operator -(const DirectX::XMVECTOR& v) const;
		Vector& operator -=(const Vector& v);
		Vector& operator -=(const DirectX::XMFLOAT3& f3);
		Vector& operator -=(const DirectX::XMVECTOR& v);
		Vector operator *(float fScalar) const;
		Vector& operator *=(float fScalar);
		Vector operator /(float fScalar) const;
		Vector& operator /=(float fScalar);
		Vector operator *(const Matrix& m) const;
		Vector operator *(const DirectX::XMMATRIX& m) const;
		Vector operator *=(const Matrix& m);
		Vector operator *=(const DirectX::XMMATRIX& m);
		float& operator[](size_t uIndex);
		float operator[](size_t uIndex) const;
	public:
		float Length() const;
		float LengthSq() const;
		float Dot(const Vector& v) const;
		float Dot(const DirectX::XMFLOAT3& f3) const;
		float Dot(const DirectX::XMVECTOR& v) const;
		Vector Cross(const Vector& v) const;
		Vector Cross(const DirectX::XMFLOAT3& f3) const;
		Vector Cross(const DirectX::XMVECTOR& v) const;
		Vector& Normalize();
		static Vector Normalize(const Vector& v);
		static Vector Normalize(const DirectX::XMFLOAT3& f3);
		static Vector Normalize(const DirectX::XMVECTOR& v);
		Vector& Normalize2d();
		static Vector Normalize2d(const Vector& v);
		static Vector Normalize2d(const DirectX::XMFLOAT3& f3);
		static Vector Normalize2d(const DirectX::XMVECTOR& v);
		static Vector Slerp(const Vector& q1, const Vector& q2, float fAlpha);
	public:
		union
		{
			DirectX::XMVECTOR v;
			struct
			{
				float x, y, z, w;
			};
		};
	};

	class Matrix final
	{
	public:
		Matrix();
		Matrix(const DirectX::XMMATRIX& m);
		Matrix& operator =(const Matrix& m);
		Matrix& operator =(const DirectX::XMMATRIX& m);
		Matrix operator *(const Matrix& m) const;
		Matrix operator *(const DirectX::XMMATRIX& m) const;
		Matrix& operator *=(const Matrix& m);
		Matrix& operator *=(const DirectX::XMMATRIX& m);
	public:
		static Matrix Identity();
		Matrix& Transpose();
		static Matrix Transpose(const Matrix& m);
		static Matrix Transpose(const DirectX::XMMATRIX& m);
		static Matrix Scaling(float x, float y, float z);
		static Matrix Scaling(const Vector& v);
		static Matrix Rotation(float x, float y, float z);
		static Matrix Rotation(const Vector& v);
		static Matrix RotationAxis(const Vector& v, float fAng);
		static Matrix RotationQuaternion(const Vector& quaternion);
		static Matrix Translation(float x, float y, float z);
		static Matrix Translation(const Vector& v);
		static Matrix LookAt(const Vector& vPos, const Vector& vDir, const Vector& vUp);
		static Matrix PerspectiveFov(float fHFov, float fAR, float fN, float fF);
	public:
		DirectX::XMMATRIX m;
	};

	struct Mesh
	{
		std::vector<Vector> vertices;
		std::vector<uint32_t> indices;
	};

	template <typename T>
	T Squared(const T& value)
	{
		return value * value;
	}

	float WrapAngle(float fValue);
}