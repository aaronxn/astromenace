/************************************************************************************

	AstroMenace (Hardcore 3D space shooter with spaceship upgrade possibilities)
	Copyright (c) 2006-2018 Mikhail Kurinnoi, Viewizard


	AstroMenace is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	AstroMenace is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with AstroMenace. If not, see <http://www.gnu.org/licenses/>.


	Web Site: http://www.viewizard.com/
	Project: https://github.com/viewizard/astromenace
	E-mail: viewizard@viewizard.com

*************************************************************************************/

#include "math.h"

namespace {

constexpr double Cos[360] =
		  {1.000000,0.999848,0.999391,0.998630,0.997564,0.996195,0.994522,
		   0.992546,0.990268,0.987688,0.984808,0.981627,0.978148,0.974370,
		   0.970296,0.965926,0.961262,0.956305,0.951057,0.945519,0.939693,
		   0.933580,0.927184,0.920505,0.913545,0.906308,0.898794,0.891007,
		   0.882948,0.874620,0.866025,0.857167,0.848048,0.838671,0.829038,
		   0.819152,0.809017,0.798636,0.788011,0.777146,0.766044,0.754710,
		   0.743145,0.731354,0.719340,0.707107,0.694658,0.681998,0.669131,
		   0.656059,0.642788,0.629320,0.615661,0.601815,0.587785,0.573576,
		   0.559193,0.544639,0.529919,0.515038,0.500000,0.484810,0.469472,
		   0.453990,0.438371,0.422618,0.406737,0.390731,0.374607,0.358368,
		   0.342020,0.325568,0.309017,0.292372,0.275637,0.258819,0.241922,
		   0.224951,0.207912,0.190809,0.173648,0.156434,0.139173,0.121869,
		   0.104528,0.087156,0.069756,0.052336,0.034899,0.017452,0.000000,
		   -0.017452,-0.034899,-0.052336,-0.069756,-0.087156,-0.104528,
		   -0.121869,-0.139173,-0.156434,-0.173648,-0.190809,-0.207912,
		   -0.224951,-0.241922,-0.258819,-0.275637,-0.292372,-0.309017,
		   -0.325568,-0.342020,-0.358368,-0.374607,-0.390731,-0.406737,
		   -0.422618,-0.438371,-0.453990,-0.469472,-0.484810,-0.500000,
		   -0.515038,-0.529919,-0.544639,-0.559193,-0.573576,-0.587785,
		   -0.601815,-0.615661,-0.629320,-0.642788,-0.656059,-0.669131,
		   -0.681998,-0.694658,-0.707107,-0.719340,-0.731354,-0.743145,
		   -0.754710,-0.766044,-0.777146,-0.788011,-0.798636,-0.809017,
		   -0.819152,-0.829038,-0.838671,-0.848048,-0.857167,-0.866025,
		   -0.874620,-0.882948,-0.891007,-0.898794,-0.906308,-0.913545,
		   -0.920505,-0.927184,-0.933580,-0.939693,-0.945519,-0.951057,
		   -0.956305,-0.961262,-0.965926,-0.970296,-0.974370,-0.978148,
		   -0.981627,-0.984808,-0.987688,-0.990268,-0.992546,-0.994522,
		   -0.996195,-0.997564,-0.998630,-0.999391,-0.999848,-1.000000,
		   -0.999848,-0.999391,-0.998630,-0.997564,-0.996195,-0.994522,
		   -0.992546,-0.990268,-0.987688,-0.984808,-0.981627,-0.978148,
		   -0.974370,-0.970296,-0.965926,-0.961262,-0.956305,-0.951057,
		   -0.945519,-0.939693,-0.933580,-0.927184,-0.920505,-0.913545,
		   -0.906308,-0.898794,-0.891007,-0.882948,-0.874620,-0.866025,
		   -0.857167,-0.848048,-0.838671,-0.829038,-0.819152,-0.809017,
		   -0.798636,-0.788011,-0.777146,-0.766044,-0.754710,-0.743145,
		   -0.731354,-0.719340,-0.707107,-0.694658,-0.681998,-0.669131,
		   -0.656059,-0.642788,-0.629320,-0.615661,-0.601815,-0.587785,
		   -0.573576,-0.559193,-0.544639,-0.529919,-0.515038,-0.500000,
		   -0.484810,-0.469472,-0.453990,-0.438371,-0.422618,-0.406737,
		   -0.390731,-0.374607,-0.358368,-0.342020,-0.325568,-0.309017,
		   -0.292372,-0.275637,-0.258819,-0.241922,-0.224951,-0.207912,
		   -0.190809,-0.173648,-0.156434,-0.139173,-0.121869,-0.104528,
		   -0.087156,-0.069756,-0.052336,-0.034899,-0.017452,-0.000000,
		   0.017452,0.034899,0.052336,0.069756,0.087156,0.104528,0.121869,
		   0.139173,0.156434,0.173648,0.190809,0.207912,0.224951,0.241922,
		   0.258819,0.275637,0.292372,0.309017,0.325568,0.342020,
		   0.358368,0.374607,0.390731,0.406737,0.422618,0.438371,0.453990,
		   0.469472,0.484810,0.500000,0.515038,0.529919,0.544639,0.559193,
		   0.573576,0.587785,0.601815,0.615661,0.629320,0.642788,0.656059,
		   0.669131,0.681998,0.694658,0.707107,0.719340,0.731354,0.743145,
		   0.754710,0.766044,0.777146,0.788011,0.798636,0.809017,0.819152,
		   0.829038,0.838671,0.848048,0.857167,0.866025,0.874620,0.882948,
		   0.891007,0.898794,0.906308,0.913545,0.920505,0.927184,0.933580,
		   0.939693,0.945519,0.951057,0.956305,0.961262,0.965926,0.970296,
		   0.974370,0.978148,0.981627,0.984808,0.987688,0.990268,0.992546,
		   0.994522,0.996195,0.997564,0.998630,0.999391,0.999848
		  };

constexpr double Sin[360] =
		  {0.000000,0.017452,0.034899,0.052336,0.069756,0.087156,
		   0.104528,0.121869,0.139173,0.156434,0.173648,0.190809,0.207912,
		   0.224951,0.241922,0.258819,0.275637,0.292372,0.309017,0.325568,
		   0.342020,0.358368,0.374607,0.390731,0.406737,0.422618,0.438371,
		   0.453990,0.469472,0.484810,0.500000,0.515038,0.529919,0.544639,
		   0.559193,0.573576,0.587785,0.601815,0.615661,0.629320,0.642788,
		   0.656059,0.669131,0.681998,0.694658,0.707107,0.719340,0.731354,
		   0.743145,0.754710,0.766044,0.777146,0.788011,0.798636,0.809017,
		   0.819152,0.829038,0.838671,0.848048,0.857167,0.866025,0.874620,
		   0.882948,0.891007,0.898794,0.906308,0.913545,0.920505,0.927184,
		   0.933580,0.939693,0.945519,0.951057,0.956305,0.961262,0.965926,
		   0.970296,0.974370,0.978148,0.981627,0.984808,0.987688,0.990268,
		   0.992546,0.994522,0.996195,0.997564,0.998630,0.999391,0.999848,
		   1.000000,0.999848,0.999391,0.998630,0.997564,0.996195,0.994522,
		   0.992546,0.990268,0.987688,0.984808,0.981627,0.978148,0.974370,
		   0.970296,0.965926,0.961262,0.956305,0.951057,0.945519,0.939693,
		   0.933580,0.927184,0.920505,0.913545,0.906308,0.898794,0.891007,
		   0.882948,0.874620,0.866025,0.857167,0.848048,0.838671,0.829038,
		   0.819152,0.809017,0.798636,0.788011,0.777146,0.766044,0.754710,
		   0.743145,0.731354,0.719340,0.707107,0.694658,0.681998,0.669131,
		   0.656059,0.642788,0.629320,0.615661,0.601815,0.587785,0.573576,
		   0.559193,0.544639,0.529919,0.515038,0.500000,0.484810,0.469472,
		   0.453990,0.438371,0.422618,0.406737,0.390731,0.374607,0.358368,
		   0.342020,0.325568,0.309017,0.292372,0.275637,0.258819,0.241922,
		   0.224951,0.207912,0.190809,0.173648,0.156434,0.139173,0.121869,
		   0.104528,0.087156,0.069756,0.052336,0.034899,0.017452,0.000000,
		   -0.017452,-0.034899,-0.052336,-0.069756,-0.087156,-0.104528,
		   -0.121869,-0.139173,-0.156434,-0.173648,-0.190809,-0.207912,
		   -0.224951,-0.241922,-0.258819,-0.275637,-0.292372,-0.309017,
		   -0.325568,-0.342020,-0.358368,-0.374607,-0.390731,-0.406737,
		   -0.422618,-0.438371,-0.453990,-0.469472,-0.484810,-0.500000,
		   -0.515038,-0.529919,-0.544639,-0.559193,-0.573576,-0.587785,
		   -0.601815,-0.615661,-0.629320,-0.642788,-0.656059,-0.669131,
		   -0.681998,-0.694658,-0.707107,-0.719340,-0.731354,-0.743145,
		   -0.754710,-0.766044,-0.777146,-0.788011,-0.798636,-0.809017,
		   -0.819152,-0.829038,-0.838671,-0.848048,-0.857167,-0.866025,
		   -0.874620,-0.882948,-0.891007,-0.898794,-0.906308,-0.913545,
		   -0.920505,-0.927184,-0.933580,-0.939693,-0.945519,-0.951057,
		   -0.956305,-0.961262,-0.965926,-0.970296,-0.974370,-0.978148,
		   -0.981627,-0.984808,-0.987688,-0.990268,-0.992546,-0.994522,
		   -0.996195,-0.997564,-0.998630,-0.999391,-0.999848,-1.000000,
		   -0.999848,-0.999391,-0.998630,-0.997564,-0.996195,-0.994522,
		   -0.992546,-0.990268,-0.987688,-0.984808,-0.981627,-0.978148,
		   -0.974370,-0.970296,-0.965926,-0.961262,-0.956305,-0.951057,
		   -0.945519,-0.939693,-0.933580,-0.927184,-0.920505,-0.913545,
		   -0.906308,-0.898794,-0.891007,-0.882948,-0.874620,-0.866025,
		   -0.857167,-0.848048,-0.838671,-0.829038,-0.819152,-0.809017,
		   -0.798636,-0.788011,-0.777146,-0.766044,-0.754710,-0.743145,
		   -0.731354,-0.719340,-0.707107,-0.694658,-0.681998,-0.669131,
		   -0.656059,-0.642788,-0.629320,-0.615661,-0.601815,-0.587785,
		   -0.573576,-0.559193,-0.544639,-0.529919,-0.515038,-0.500000,
		   -0.484810,-0.469472,-0.453990,-0.438371,-0.422618,-0.406737,
		   -0.390731,-0.374607,-0.358368,-0.342020,-0.325568,-0.309017,
		   -0.292372,-0.275637,-0.258819,-0.241922,-0.224951,-0.207912,
		   -0.190809,-0.173648,-0.156434,-0.139173,-0.121869,-0.104528,
		   -0.087156,-0.069756,-0.052336,-0.034899,-0.017452
		  };

// utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
template<class Facet>
struct deletable_facet : Facet
{
    template<class ...Args>
    deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
    ~deletable_facet() {}
};
// utf8 <=> utf32 converter
std::wstring_convert<deletable_facet<std::codecvt<char32_t, char, std::mbstate_t>>, char32_t> ConvUTF8_UTF32;

} // unnamed namespace

/*
 * Fast cosine function.
 */
double vw_dcos(int Angle)
{
	return Cos[Angle];
}

/*
 * Fast sine function.
 */
double vw_dsin(int Angle)
{
	return Sin[Angle];
}

/*
 * Convert utf8 to utf32 code. Return pointer on next utf8 character in string.
 */
const char *vw_UTF8toUTF32(const char *utf8, unsigned *utf32)
{
	if (!utf8 || !utf32)
		return nullptr;

	unsigned char *u_utf8 = (unsigned char*)utf8;
	unsigned char b = *u_utf8++;

	if (!(b & 0x80)) {
		*utf32 = b;
		return utf8 + 1;
	}

	unsigned len = 0;
	while (b & 0x80) {
		b <<= 1;
		++len;
	}

	unsigned c = b;
	unsigned shift = 6 - len;

	while (--len) {
		c <<= shift;
		c |= (*u_utf8++) & 0x3f;
		shift = 6;
	}

	*utf32 = c;
	return (char*)u_utf8;
}

/*
 * Convert utf8 to utf32 code.
 */
std::u32string vw_UTF8toUTF32(const char *utf8)
{
	return ConvUTF8_UTF32.from_bytes(utf8);
}

/*
 * Convert utf32 to utf8 code.
 */
std::string vw_UTF32toUTF8(const std::u32string &utf32)
{
	return ConvUTF8_UTF32.to_bytes(utf32);
}

/*
 * Convert utf8 to utf32 code with variadic arguments
 */
std::u32string vw_UTF8VAtoUTF32(const char *utf8, ...)
{
	va_list ap;
	va_start(ap, utf8);
	std::vector<char> buffer(1 + std::vsnprintf(nullptr, 0, utf8, ap));
	va_end(ap);
	// get data into buffer
	va_start(ap, utf8);
	std::vsnprintf(buffer.data(), buffer.size(), utf8, ap);
	va_end(ap);
	return ConvUTF8_UTF32.from_bytes(buffer.data());
}

/*
 * Fast root (without sqrtf)
 */
#ifdef WIN32
static float __fastcall InvSqrt(const float x)
#else
static float InvSqrt(const float x)
#endif
{
	union {
		float f;
		int i;
	} t;
	float y;

	t.f = x;
	t.i = 0x5f3759df - (t.i >> 1);
	y = t.f;
	y = y * (1.5F - (0.5F * x * y * y));

	return y;
}

/*
 * Fast sqrtf function.
 */
float vw_sqrtf(float x)
{
	return x*InvSqrt(x);
}

/*
 * sVECTOR3D Vector
 */
float sVECTOR3D::Length()
{
	return vw_sqrtf(x * x + y * y + z * z);
}

void sVECTOR3D::Normalize()
{
	float L_squared, one_over_L;
	L_squared = (x * x) + (y * y) + (z * z);
	one_over_L = InvSqrt(L_squared);
	x = x * one_over_L;
	y = y * one_over_L;
	z = z * one_over_L;
}

void sVECTOR3D::NormalizeHi()
{
	float Length;
	Length = sqrtf((x * x) + (y * y) + (z * z));
	if (Length < 0.001f)
		return;
	x = x / Length;
	y = y / Length;
	z = z / Length;
}

void sVECTOR3D::Multiply(const sVECTOR3D &A)
{
	float tV[3]{y * A.z - z * A.y,
		    z * A.x - x * A.z,
		    x * A.y - y * A.x};
	x = tV[0];
	y = tV[1];
	z = tV[2];
}

/*
 * Calculates the plane equation given three points.
 */
void vw_GetPlaneABCD(float &A, float &B, float &C, float &D,
		     const sVECTOR3D &Point1, const sVECTOR3D &Point2, const sVECTOR3D &Point3)
{
	A = (Point2.y - Point1.y) * (Point3.z - Point1.z) - (Point2.z - Point1.z) * (Point3.y - Point1.y);
	B = (Point2.z - Point1.z) * (Point3.x - Point1.x) - (Point2.x - Point1.x) * (Point3.z - Point1.z);
	C = (Point2.x - Point1.x) * (Point3.y - Point1.y) - (Point2.y - Point1.y) * (Point3.x - Point1.x);
	D = - Point1.x * (Point2.y - Point1.y) * (Point3.z - Point1.z)
	    - Point1.y * (Point2.z - Point1.z) * (Point3.x - Point1.x)
	    - Point1.z * (Point2.x - Point1.x) * (Point3.y - Point1.y)
	    + Point1.z * (Point2.y - Point1.y) * (Point3.x - Point1.x)
	    + Point1.y * (Point2.x - Point1.x) * (Point3.z - Point1.z)
	    + Point1.x * (Point2.z - Point1.z) * (Point3.y - Point1.y);
}

/*
 * Calculate point rotation.
 */
void vw_RotatePoint(sVECTOR3D &Point, const sVECTOR3D &Angle)
{
	constexpr float DegToRadFactor = 0.0174532925f; // conversion factor to convert degrees to radians
	float tmpX = Point.x;
	float tmpY = Point.y;
	float tmpZ = Point.z;

	// X
	if (Angle.x != 0) {
		float a = -Angle.x * DegToRadFactor;
		float c = cosf(a);
		float s = sinf(a);
		Point.y = tmpY * c + tmpZ * s;
		Point.z = -tmpY * s + tmpZ * c;
		tmpX = Point.x;
		tmpY = Point.y;
		tmpZ = Point.z;
	}
	// Y
	if (Angle.y != 0) {
		float a = Angle.y * DegToRadFactor;
		float c = cosf(a);
		float s = sinf(a);
		Point.x = tmpX*c + tmpZ * s;
		Point.z = -tmpX*s + tmpZ * c;
		tmpX = Point.x;
		tmpY = Point.y;
	}
	// Z
	if (Angle.z != 0) {
		float a = -Angle.z * DegToRadFactor;
		float c = cosf(a);
		float s = sinf(a);
		Point.x = tmpX * c + tmpY * s;
		Point.y = -tmpX * s + tmpY * c;
	}
}

/*
 * Calculate point inverse rotation.
 */
void vw_RotatePointInv(sVECTOR3D &Point, const sVECTOR3D &Angle)
{
	constexpr float DegToRadFactor = 0.0174532925f; // conversion factor to convert degrees to radians
	float tmpX = Point.x;
	float tmpY = Point.y;
	float tmpZ = Point.z;

	// Z
	if (Angle.z != 0) {
		float a = -Angle.z * DegToRadFactor;
		float c = cosf(a);
		float s = sinf(a);
		Point.x = tmpX * c + tmpY * s;
		Point.y = -tmpX * s + tmpY * c;
		tmpX = Point.x;
		tmpY = Point.y;
		tmpZ = Point.z;
	}
	// Y
	if (Angle.y != 0) {
		float a = Angle.y * DegToRadFactor;
		float c = cosf(a);
		float s = sinf(a);
		Point.x = tmpX * c + tmpZ * s;
		Point.z = -tmpX * s + tmpZ * c;
		tmpY = Point.y;
		tmpZ = Point.z;
	}
	// X
	if (Angle.x != 0) {
		float a = -Angle.x * DegToRadFactor;
		float c = cosf(a);
		float s = sinf(a);
		Point.y = tmpY * c + tmpZ * s;
		Point.z = -tmpY * s + tmpZ * c;
	}
}
