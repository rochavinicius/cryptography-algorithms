#include "ARIA.h"
/*
* This is an implementation of the cipher algorithm ARIA
*
* Implementation References:
* - https://datatracker.ietf.org/doc/html/rfc5794#section-2.4.3
* - https://www.oryx-embedded.com/doc/aria_8c_source.html
*/

// constants
unsigned __int32 CK1[4] = { 0x517cc1b7, 0x27220a94, 0xfe13abe8, 0xfa9a6ee0 };
unsigned __int32 CK2[4] = { 0x6db14acc, 0x9e21c820, 0xff28b1d5, 0xef5de2b0 };
unsigned __int32 CK3[4] = { 0xdb92371d, 0x2126e970, 0x03249775, 0x04e8c90e };

// registers
unsigned __int32 KR[4] = { 0, 0, 0, 0 };

unsigned __int32 W0[4];
unsigned __int32 W1[4];
unsigned __int32 W2[4];
unsigned __int32 W3[4];

// encryption keys and decryptions keys
unsigned __int32 ek1[4];
unsigned __int32 ek2[4];
unsigned __int32 ek3[4];
unsigned __int32 ek4[4];
unsigned __int32 ek5[4];
unsigned __int32 ek6[4];
unsigned __int32 ek7[4];
unsigned __int32 ek8[4];
unsigned __int32 ek9[4];
unsigned __int32 ek10[4];
unsigned __int32 ek11[4];
unsigned __int32 ek12[4];
unsigned __int32 ek13[4];

unsigned __int32 dk1[4];
unsigned __int32 dk2[4];
unsigned __int32 dk3[4];
unsigned __int32 dk4[4];
unsigned __int32 dk5[4];
unsigned __int32 dk6[4];
unsigned __int32 dk7[4];
unsigned __int32 dk8[4];
unsigned __int32 dk9[4];
unsigned __int32 dk10[4];
unsigned __int32 dk11[4];
unsigned __int32 dk12[4];
unsigned __int32 dk13[4];

// S-Boxes
const unsigned __int8 SB1[256] = {
								0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
								0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
								0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
								0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
								0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
								0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
								0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
								0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
								0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
								0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
								0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
								0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
								0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
								0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
								0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
								0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const unsigned __int8 SB2[256] = {
								0xe2, 0x4e, 0x54, 0xfc, 0x94, 0xc2, 0x4a, 0xcc, 0x62, 0x0d, 0x6a, 0x46, 0x3c, 0x4d, 0x8b, 0xd1,
								0x5e, 0xfa, 0x64, 0xcb, 0xb4, 0x97, 0xbe, 0x2b, 0xbc, 0x77, 0x2e, 0x03, 0xd3, 0x19, 0x59, 0xc1,
								0x1d, 0x06, 0x41, 0x6b, 0x55, 0xf0, 0x99, 0x69, 0xea, 0x9c, 0x18, 0xae, 0x63, 0xdf, 0xe7, 0xbb,
								0x00, 0x73, 0x66, 0xfb, 0x96, 0x4c, 0x85, 0xe4, 0x3a, 0x09, 0x45, 0xaa, 0x0f, 0xee, 0x10, 0xeb,
								0x2d, 0x7f, 0xf4, 0x29, 0xac, 0xcf, 0xad, 0x91, 0x8d, 0x78, 0xc8, 0x95, 0xf9, 0x2f, 0xce, 0xcd,
								0x08, 0x7a, 0x88, 0x38, 0x5c, 0x83, 0x2a, 0x28, 0x47, 0xdb, 0xb8, 0xc7, 0x93, 0xa4, 0x12, 0x53,
								0xff, 0x87, 0x0e, 0x31, 0x36, 0x21, 0x58, 0x48, 0x01, 0x8e, 0x37, 0x74, 0x32, 0xca, 0xe9, 0xb1,
								0xb7, 0xab, 0x0c, 0xd7, 0xc4, 0x56, 0x42, 0x26, 0x07, 0x98, 0x60, 0xd9, 0xb6, 0xb9, 0x11, 0x40,
								0xec, 0x20, 0x8c, 0xbd, 0xa0, 0xc9, 0x84, 0x04, 0x49, 0x23, 0xf1, 0x4f, 0x50, 0x1f, 0x13, 0xdc,
								0xd8, 0xc0, 0x9e, 0x57, 0xe3, 0xc3, 0x7b, 0x65, 0x3b, 0x02, 0x8f, 0x3e, 0xe8, 0x25, 0x92, 0xe5,
								0x15, 0xdd, 0xfd, 0x17, 0xa9, 0xbf, 0xd4, 0x9a, 0x7e, 0xc5, 0x39, 0x67, 0xfe, 0x76, 0x9d, 0x43,
								0xa7, 0xe1, 0xd0, 0xf5, 0x68, 0xf2, 0x1b, 0x34, 0x70, 0x05, 0xa3, 0x8a, 0xd5, 0x79, 0x86, 0xa8,
								0x30, 0xc6, 0x51, 0x4b, 0x1e, 0xa6, 0x27, 0xf6, 0x35, 0xd2, 0x6e, 0x24, 0x16, 0x82, 0x5f, 0xda,
								0xe6, 0x75, 0xa2, 0xef, 0x2c, 0xb2, 0x1c, 0x9f, 0x5d, 0x6f, 0x80, 0x0a, 0x72, 0x44, 0x9b, 0x6c,
								0x90, 0x0b, 0x5b, 0x33, 0x7d, 0x5a, 0x52, 0xf3, 0x61, 0xa1, 0xf7, 0xb0, 0xd6, 0x3f, 0x7c, 0x6d,
								0xed, 0x14, 0xe0, 0xa5, 0x3d, 0x22, 0xb3, 0xf8, 0x89, 0xde, 0x71, 0x1a, 0xaf, 0xba, 0xb5, 0x81
};

const unsigned __int8 SB3[256] = {
								0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
								0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
								0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
								0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
								0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
								0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
								0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
								0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
								0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
								0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
								0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
								0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
								0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
								0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
								0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
								0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

const unsigned __int8 SB4[256] = {
								0x30, 0x68, 0x99, 0x1b, 0x87, 0xb9, 0x21, 0x78, 0x50, 0x39, 0xdb, 0xe1, 0x72, 0x09, 0x62, 0x3c,
								0x3e, 0x7e, 0x5e, 0x8e, 0xf1, 0xa0, 0xcc, 0xa3, 0x2a, 0x1d, 0xfb, 0xb6, 0xd6, 0x20, 0xc4, 0x8d,
								0x81, 0x65, 0xf5, 0x89, 0xcb, 0x9d, 0x77, 0xc6, 0x57, 0x43, 0x56, 0x17, 0xd4, 0x40, 0x1a, 0x4d,
								0xc0, 0x63, 0x6c, 0xe3, 0xb7, 0xc8, 0x64, 0x6a, 0x53, 0xaa, 0x38, 0x98, 0x0c, 0xf4, 0x9b, 0xed,
								0x7f, 0x22, 0x76, 0xaf, 0xdd, 0x3a, 0x0b, 0x58, 0x67, 0x88, 0x06, 0xc3, 0x35, 0x0d, 0x01, 0x8b,
								0x8c, 0xc2, 0xe6, 0x5f, 0x02, 0x24, 0x75, 0x93, 0x66, 0x1e, 0xe5, 0xe2, 0x54, 0xd8, 0x10, 0xce,
								0x7a, 0xe8, 0x08, 0x2c, 0x12, 0x97, 0x32, 0xab, 0xb4, 0x27, 0x0a, 0x23, 0xdf, 0xef, 0xca, 0xd9,
								0xb8, 0xfa, 0xdc, 0x31, 0x6b, 0xd1, 0xad, 0x19, 0x49, 0xbd, 0x51, 0x96, 0xee, 0xe4, 0xa8, 0x41,
								0xda, 0xff, 0xcd, 0x55, 0x86, 0x36, 0xbe, 0x61, 0x52, 0xf8, 0xbb, 0x0e, 0x82, 0x48, 0x69, 0x9a,
								0xe0, 0x47, 0x9e, 0x5c, 0x04, 0x4b, 0x34, 0x15, 0x79, 0x26, 0xa7, 0xde, 0x29, 0xae, 0x92, 0xd7,
								0x84, 0xe9, 0xd2, 0xba, 0x5d, 0xf3, 0xc5, 0xb0, 0xbf, 0xa4, 0x3b, 0x71, 0x44, 0x46, 0x2b, 0xfc,
								0xeb, 0x6f, 0xd5, 0xf6, 0x14, 0xfe, 0x7c, 0x70, 0x5a, 0x7d, 0xfd, 0x2f, 0x18, 0x83, 0x16, 0xa5,
								0x91, 0x1f, 0x05, 0x95, 0x74, 0xa9, 0xc1, 0x5b, 0x4a, 0x85, 0x6d, 0x13, 0x07, 0x4f, 0x4e, 0x45,
								0xb2, 0x0f, 0xc9, 0x1c, 0xa6, 0xbc, 0xec, 0x73, 0x90, 0x7b, 0xcf, 0x59, 0x8f, 0xa1, 0xf9, 0x2d,
								0xf2, 0xb1, 0x00, 0x94, 0x37, 0x9f, 0xd0, 0x2e, 0x9c, 0x6e, 0x28, 0x3f, 0x80, 0xf0, 0x3d, 0xd3,
								0x25, 0x8a, 0xb5, 0xe7, 0x42, 0xb3, 0xc7, 0xea, 0xf7, 0x4c, 0x11, 0x33, 0x03, 0xa2, 0xac, 0x60
};

void XOR_128(unsigned __int32* y, unsigned __int32* x)
{
	y[0] ^= x[0];
	y[1] ^= x[1];
	y[2] ^= x[2];
	y[3] ^= x[3];
}

void MOV_128(unsigned __int32* y, unsigned __int32* x)
{
	y[0] = x[0];
	y[1] = x[1];
	y[2] = x[2];
	y[3] = x[3];
}

// Rotate Left circular shift 128 bits
void ROL_128(unsigned __int32* y, unsigned __int32* x, unsigned __int32 n)
{
	y[0] = (x[0] << n) | (x[1] >> (32 - n));
	y[1] = (x[1] << n) | (x[2] >> (32 - n));
	y[2] = (x[2] << n) | (x[3] >> (32 - n));
	y[3] = (x[3] << n) | (x[0] >> (32 - n));
}

// Rotate Right circular shift 128 bits
void ROR_128(unsigned __int32* y, unsigned __int32* x, unsigned __int32 n)
{
	y[3] = (x[3] >> n) | (x[2] << (32 - n));
	y[2] = (x[2] >> n) | (x[1] << (32 - n));
	y[1] = (x[1] >> n) | (x[0] << (32 - n));
	y[0] = (x[0] >> n) | (x[3] << (32 - n));
}

void SL1(unsigned __int32* input, unsigned __int32* output)
{
	/*
		y0 = SB1(x0),  y1 = SB2(x1),  y2 = SB3(x2),  y3 = SB4(x3),
		y4 = SB1(x4),  y5 = SB2(x5),  y6 = SB3(x6),  y7 = SB4(x7),
		y8 = SB1(x8),  y9 = SB2(x9),  y10= SB3(x10), y11= SB4(x11),
		y12= SB1(x12), y13= SB2(x13), y14= SB3(x14), y15= SB4(x15).
	*/

	output[0] = SB1[(unsigned __int8)(input[0] >> 24)] << 24
		| SB2[(unsigned __int8)(input[0] >> 16)] << 16
		| SB3[(unsigned __int8)(input[0] >> 8)] << 8
		| SB4[(unsigned __int8)(input[0] >> 0)];

	output[1] = SB1[(unsigned __int8)(input[1] >> 24)] << 24
		| SB2[(unsigned __int8)(input[1] >> 16)] << 16
		| SB3[(unsigned __int8)(input[1] >> 8)] << 8
		| SB4[(unsigned __int8)(input[1] >> 0)];

	output[2] = SB1[(unsigned __int8)(input[2] >> 24)] << 24
		| SB2[(unsigned __int8)(input[2] >> 16)] << 16
		| SB3[(unsigned __int8)(input[2] >> 8)] << 8
		| SB4[(unsigned __int8)(input[2] >> 0)];

	output[3] = SB1[(unsigned __int8)(input[3] >> 24)] << 24
		| SB2[(unsigned __int8)(input[3] >> 16)] << 16
		| SB3[(unsigned __int8)(input[3] >> 8)] << 8
		| SB4[(unsigned __int8)(input[3] >> 0)];
}

void SL2(unsigned __int32* input, unsigned __int32* output)
{
	/*
		y0 = SB3(x0),  y1 = SB4(x1),  y2 = SB1(x2),  y3 = SB2(x3),
		y4 = SB3(x4),  y5 = SB4(x5),  y6 = SB1(x6),  y7 = SB2(x7),
		y8 = SB3(x8),  y9 = SB4(x9),  y10= SB1(x10), y11= SB2(x11),
		y12= SB3(x12), y13= SB4(x13), y14= SB1(x14), y15= SB2(x15).
	*/

	output[0] = SB3[(unsigned __int8)(input[0] >> 24)] << 24
		| SB4[(unsigned __int8)(input[0] >> 16)] << 16
		| SB1[(unsigned __int8)(input[0] >> 8)] << 8
		| SB2[(unsigned __int8)(input[0] >> 0)];

	output[1] = SB3[(unsigned __int8)(input[1] >> 24)] << 24
		| SB4[(unsigned __int8)(input[1] >> 16)] << 16
		| SB1[(unsigned __int8)(input[1] >> 8)] << 8
		| SB2[(unsigned __int8)(input[1] >> 0)];

	output[2] = SB3[(unsigned __int8)(input[2] >> 24)] << 24
		| SB4[(unsigned __int8)(input[2] >> 16)] << 16
		| SB1[(unsigned __int8)(input[2] >> 8)] << 8
		| SB2[(unsigned __int8)(input[2] >> 0)];

	output[3] = SB3[(unsigned __int8)(input[3] >> 24)] << 24
		| SB4[(unsigned __int8)(input[3] >> 16)] << 16
		| SB1[(unsigned __int8)(input[3] >> 8)] << 8
		| SB2[(unsigned __int8)(input[3] >> 0)];
}

void A(unsigned __int32* input, unsigned __int32* output)
{
	/*
		y0  = x3 ^ x4 ^ x6 ^ x8  ^ x9  ^ x13 ^ x14,
		y1  = x2 ^ x5 ^ x7 ^ x8  ^ x9  ^ x12 ^ x15,
		y2  = x1 ^ x4 ^ x6 ^ x10 ^ x11 ^ x12 ^ x15,
		y3  = x0 ^ x5 ^ x7 ^ x10 ^ x11 ^ x13 ^ x14,
		y4  = x0 ^ x2 ^ x5 ^ x8  ^ x11 ^ x14 ^ x15,
		y5  = x1 ^ x3 ^ x4 ^ x9  ^ x10 ^ x14 ^ x15,
		y6  = x0 ^ x2 ^ x7 ^ x9  ^ x10 ^ x12 ^ x13,
		y7  = x1 ^ x3 ^ x6 ^ x8  ^ x11 ^ x12 ^ x13,
		y8  = x0 ^ x1 ^ x4 ^ x7  ^ x10 ^ x13 ^ x15,
		y9  = x0 ^ x1 ^ x5 ^ x6  ^ x11 ^ x12 ^ x14,
		y10 = x2 ^ x3 ^ x5 ^ x6  ^ x8  ^ x13 ^ x15,
		y11 = x2 ^ x3 ^ x4 ^ x7  ^ x9  ^ x12 ^ x14,
		y12 = x1 ^ x2 ^ x6 ^ x7  ^ x9  ^ x11 ^ x12,
		y13 = x0 ^ x3 ^ x6 ^ x7  ^ x8  ^ x10 ^ x13,
		y14 = x0 ^ x3 ^ x4 ^ x5  ^ x9  ^ x11 ^ x14,
		y15 = x1 ^ x2 ^ x4 ^ x5  ^ x8  ^ x10 ^ x15.
	*/

	unsigned __int8 x0 = input[0] >> 24;
	unsigned __int8 x1 = input[0] >> 16;
	unsigned __int8 x2 = input[0] >> 8;
	unsigned __int8 x3 = input[0];
	unsigned __int8 x4 = input[1] >> 24;
	unsigned __int8 x5 = input[1] >> 16;
	unsigned __int8 x6 = input[1] >> 8;
	unsigned __int8 x7 = input[1];
	unsigned __int8 x8 = input[2] >> 24;
	unsigned __int8 x9 = input[2] >> 16;
	unsigned __int8 x10 = input[2] >> 8;
	unsigned __int8 x11 = input[2];
	unsigned __int8 x12 = input[3] >> 24;
	unsigned __int8 x13 = input[3] >> 16;
	unsigned __int8 x14 = input[3] >> 8;
	unsigned __int8 x15 = input[3];

	unsigned __int8 y0 = x3 ^ x4 ^ x6 ^ x8 ^ x9 ^ x13 ^ x14;
	unsigned __int8 y1 = x2 ^ x5 ^ x7 ^ x8 ^ x9 ^ x12 ^ x15;
	unsigned __int8 y2 = x1 ^ x4 ^ x6 ^ x10 ^ x11 ^ x12 ^ x15;
	unsigned __int8 y3 = x0 ^ x5 ^ x7 ^ x10 ^ x11 ^ x13 ^ x14;
	unsigned __int8 y4 = x0 ^ x2 ^ x5 ^ x8 ^ x11 ^ x14 ^ x15;
	unsigned __int8 y5 = x1 ^ x3 ^ x4 ^ x9 ^ x10 ^ x14 ^ x15;
	unsigned __int8 y6 = x0 ^ x2 ^ x7 ^ x9 ^ x10 ^ x12 ^ x13;
	unsigned __int8 y7 = x1 ^ x3 ^ x6 ^ x8 ^ x11 ^ x12 ^ x13;
	unsigned __int8 y8 = x0 ^ x1 ^ x4 ^ x7 ^ x10 ^ x13 ^ x15;
	unsigned __int8 y9 = x0 ^ x1 ^ x5 ^ x6 ^ x11 ^ x12 ^ x14;
	unsigned __int8 y10 = x2 ^ x3 ^ x5 ^ x6 ^ x8 ^ x13 ^ x15;
	unsigned __int8 y11 = x2 ^ x3 ^ x4 ^ x7 ^ x9 ^ x12 ^ x14;
	unsigned __int8 y12 = x1 ^ x2 ^ x6 ^ x7 ^ x9 ^ x11 ^ x12;
	unsigned __int8 y13 = x0 ^ x3 ^ x6 ^ x7 ^ x8 ^ x10 ^ x13;
	unsigned __int8 y14 = x0 ^ x3 ^ x4 ^ x5 ^ x9 ^ x11 ^ x14;
	unsigned __int8 y15 = x1 ^ x2 ^ x4 ^ x5 ^ x8 ^ x10 ^ x15;

	output[0] = y0 << 24 | y1 << 16 | y2 << 8 | y3;
	output[1] = y4 << 24 | y5 << 16 | y6 << 8 | y7;
	output[2] = y8 << 24 | y9 << 16 | y10 << 8 | y11;
	output[3] = y12 << 24 | y13 << 16 | y14 << 8 | y15;
}

void FO(unsigned __int32* D, unsigned __int32* RK, unsigned __int32* output)
{
	// A(SL1(D ^ RK))
	unsigned __int32 y[4];
	MOV_128(y, D);

	// D xor RK
	XOR_128(y, RK);

	// substitution layer 1 (odd)
	SL1(y, y);

	// diffusion layer
	A(y, output);
}

void FE(unsigned __int32* D, unsigned __int32* RK, unsigned __int32* output)
{
	// A(SL2(D ^ RK))
	unsigned __int32 y[4];
	MOV_128(y, D);

	// D xor RK
	XOR_128(y, RK);

	// substitution layer 2 (even)
	SL2(y, y);

	// diffusion layer
	A(y, output);
}

void generateEncryptionKeys(void)
{
	/*
		ek1  = W0 ^(W1 >>> 19),
		ek2  = W1 ^(W2 >>> 19),
		ek3  = W2 ^(W3 >>> 19),
		ek4  = (W0 >>> 19) ^ W3,
		ek5  = W0 ^ (W1 >>> 31),
		ek6  = W1 ^ (W2 >>> 31),
		ek7  = W2 ^ (W3 >>> 31),
		ek8  = (W0 >>> 31) ^ W3,
		ek9  = W0 ^ (W1 <<< 61),
		ek10 = W1 ^ (W2 <<< 61),
		ek11 = W2 ^ (W3 <<< 61),
		ek12 = (W0 <<< 61) ^ W3,
		ek13 = W0 ^ (W1 <<< 31)
	*/
	unsigned __int32 temp[4];

	ROR_128(ek1, W1, 19);
	XOR_128(ek1, W0);
	ROR_128(ek2, W2, 19);
	XOR_128(ek2, W1);
	ROR_128(ek3, W3, 19);
	XOR_128(ek3, W2);
	ROR_128(ek4, W0, 19);
	XOR_128(ek4, W3);
	ROR_128(ek5, W1, 31);
	XOR_128(ek5, W0);
	ROR_128(ek6, W2, 31);
	XOR_128(ek6, W1);
	ROR_128(ek7, W3, 31);
	XOR_128(ek7, W2);
	ROR_128(ek8, W0, 31);
	XOR_128(ek8, W3);

	ROL_128(ek9, W1, 31);
	ROL_128(temp, ek9, 30);
	MOV_128(ek9, temp);
	XOR_128(ek9, W0);
	ROL_128(ek10, W2, 31);
	ROL_128(temp, ek10, 30);
	MOV_128(ek10, temp);
	XOR_128(ek10, W1);
	ROL_128(ek11, W3, 31);
	ROL_128(temp, ek11, 30);
	MOV_128(ek11, temp);
	XOR_128(ek11, W2);
	ROL_128(ek12, W0, 31);
	ROL_128(temp, ek12, 30);
	MOV_128(ek12, temp);
	XOR_128(ek12, W3);
	ROL_128(ek13, W1, 31);
	XOR_128(ek13, W0);
}

void generateDecryptionKeys(void)
{
	/*
		n = 12

		dk1 = ek{n+1},
		dk2 = A(ek{n}),
		dk3 = A(ek{n-1}),
		...,
		dk{n}= A(ek2),
		dk{n+1}= ek1.
	*/
	MOV_128(dk1, ek13);
	A(ek12, dk2);
	A(ek11, dk3);
	A(ek10, dk4);
	A(ek9, dk5);
	A(ek8, dk6);
	A(ek7, dk7);
	A(ek6, dk8);
	A(ek5, dk9);
	A(ek4, dk10);
	A(ek3, dk11);
	A(ek2, dk12);
	MOV_128(dk13, ek1);
}

void ARIA_encrypt(unsigned __int32* block, unsigned __int32* key, unsigned __int32* P)
{
	// Init registers
	MOV_128(W0, key);

	FO(W0, CK1, W1);
	XOR_128(W1, KR);

	FE(W1, CK2, W2);
	XOR_128(W2, W0);

	FO(W2, CK3, W3);
	XOR_128(W3, W1);

	generateEncryptionKeys();

	// encryption rounds
	FO(block, ek1, P);
	FE(P, ek2, P);
	FO(P, ek3, P);
	FE(P, ek4, P);
	FO(P, ek5, P);
	FE(P, ek6, P);
	FO(P, ek7, P);
	FE(P, ek8, P);
	FO(P, ek9, P);
	FE(P, ek10, P);
	FO(P, ek11, P);

	// C = SL2(P11 ^ ek12) ^ ek13;
	XOR_128(P, ek12);

	SL2(P, P);

	XOR_128(P, ek13);
}

void ARIA_decrypt(unsigned __int32* block, unsigned __int32* key, unsigned __int32* P)
{
	// Init registers
	MOV_128(W0, key);

	FO(W0, CK1, W1);
	XOR_128(W1, KR);

	FE(W1, CK2, W2);
	XOR_128(W2, W0);

	FO(W2, CK3, W2);
	XOR_128(W3, W1);

	generateDecryptionKeys();

	FO(block, dk1, P);
	FE(P, dk2, P);
	FO(P, dk3, P);
	FE(P, dk4, P);
	FO(P, dk5, P);
	FE(P, dk6, P);
	FO(P, dk7, P);
	FE(P, dk8, P);
	FO(P, dk9, P);
	FE(P, dk10, P);
	FO(P, dk11, P);

	// C = SL2(P11 ^ dk12) ^ dk13;
	XOR_128(P, dk12);

	SL2(P, P);

	XOR_128(P, dk13);
}

void ARIA_main(void)
{
	unsigned __int32 key[4];
	unsigned __int32 text[4];
	unsigned __int32 cipherText[4];
	unsigned __int32 expectedCipherText[4];
	unsigned __int32 decryptedText[4];

	// key 000102030405060708090a0b0c0d0e0f
	key[0] = 0x00010203;
	key[1] = 0x04050607;
	key[2] = 0x08090a0b;
	key[3] = 0x0c0d0e0f;

	// text 00112233445566778899aabbccddeeff
	text[0] = 0x00112233;
	text[1] = 0x44556677;
	text[2] = 0x8899aabb;
	text[3] = 0xccddeeff;

	// expected encryption text d718fbd6ab644c739da95f3be6451778
	expectedCipherText[0] = 0xd718fbd6;
	expectedCipherText[1] = 0xab644c73;
	expectedCipherText[2] = 0x9da95f3b;
	expectedCipherText[3] = 0xe6451778;

	ARIA_encrypt(text, key, cipherText);
	ARIA_decrypt(cipherText, key, decryptedText);

	printf("key: ");
	for (int i = 0; i < 4; i++)
	{
		printf("%08x ", key[i]);
	}
	printf("\n");

	printf("expected encrypted text: ");
	for (int i = 0; i < 4; i++)
	{
		printf("%08x ", cipherText[i]);
	}
	printf("\n");

	printf("expected encrypted text: ");
	for (int i = 0; i < 4; i++)
	{
		printf("%08x ", expectedCipherText[i]);
	}
	printf("\n");

	printf("decrypted text: ");
	for (int i = 0; i < 4; i++)
	{
		printf("%08x ", decryptedText[i]);
	}
	printf("\n");

	printf("text: ");
	for (int i = 0; i < 4; i++)
	{
		printf("%08x ", text[i]);
	}
	printf("\n");
}