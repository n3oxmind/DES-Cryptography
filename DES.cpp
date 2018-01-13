#include <iostream>
#include <sstream>
#include <bitset>
#include <iomanip>
#include <algorithm>
#include "DES.h"
#include "Colors.h"
#include <cmath>

DES::DES(std::string P, std::string K) {
	plainText = hex2bin(P);
	key64 = hex2bin(K);
}
DES::~DES() {}

void DES::xor48 (const std::string &inBlk48, const std::string &inKey48) {
	std::bitset<48> out48, inA(inBlk48), inB(inKey48);
	out48 = inA ^ inB;
	xorOut48 = out48.to_string();
}
void DES::xor32 (const std::string &lBlk32, const std::string &rBlk32) {
	std::bitset<32> out32, inA(lBlk32), inB(rBlk32);
		out32 = inA ^ inB;
		leftBlk32 = out32.to_string();
}
void DES::initialPermu(const std::string &inBlk64) {
	std::bitset<64> outBlk64;			// initial permutation output
	int index;
	for (auto it = inBlk64.begin(); it < inBlk64.end(); it++) {
		if (*it == '1') {
			index = std::distance(initialPermutationTable.begin(),
							 std::find (initialPermutationTable.begin(),
									 initialPermutationTable.end(),
									 std::distance ( inBlk64.begin(), it) + 1));
			outBlk64.set( 63 - index,1);		// this to set the MSB to right
		}

	}
	initialPermuOut64 =  outBlk64.to_string();
}
void DES::finalPermu (const std::string &inBlk64) {
		std::bitset<64> outBlk64;			// initial permutation output
		int index;
		for (auto it = inBlk64.begin(); it < inBlk64.end(); it++) {
			if (*it == '1') {
				index = std::distance(finalPermutationTable.begin(),
								std::find (finalPermutationTable.begin(),
										finalPermutationTable.end(),
										std::distance ( inBlk64.begin(), it) + 1));
				outBlk64.set( 63 - index,1);
			}

		}
		cipherText =  outBlk64.to_string();

}
void DES::expansionPermu (const std::string &inBlk32) {
	if (inBlk32.length() !=32) {
		std::cout <<"ERROR!: ExpansionPermu, Invalid inBlk size ";
		exit(0);
	}
	std::string outBlk48;
	for (int i = 0; i < 48; i++)
		outBlk48 += inBlk32[expansionPermutationTalbe.at(i)- 1];
	expansionPermuOut48 = outBlk48;
}
void DES::shiftLeft (std::string &str28, int n) {
	if (n > (int)str28.length()){
		std::cout << "ERROR!, shiftleft out of range" << std::endl;
		exit(0);
	}
	else
		std::rotate (str28.begin(), str28.begin()+n, str28.end());
}
void DES::keygen (const std::string &key64, int round) {
	if (round == 1) {
	parityDropPermu(key64);
	leftKey28 = key56.substr(0, 28);
	rightKey28 = key56.substr(28, 28);
	}

	if (round == 1 || round == 2 || round == 9 || round == 16) {
		shiftLeft(leftKey28, 1);
		shiftLeft(rightKey28, 1);
	}
	else {
		shiftLeft(leftKey28, 2);
		shiftLeft(rightKey28, 2);
	}
	key56 = leftKey28 + rightKey28;
	compressionPermu(key56);
}
void DES::sBox (const std::string &inBlk48) {
	sBoxOut32 = ("");			// clear the old container
	std::bitset<4> sbox4{};
	int row{}, col{};
	for (int i = 0; i < 48; i += 6) {
		row = bin2dec(inBlk48.substr(i, 1) + inBlk48.substr(i + 5, 1));		// s-box row
		col = bin2dec(inBlk48.substr(i + 1, 4));								//s-box column
		// selection of s-box
		switch(i) {
				case 0:
						sbox4 = s1[row][col];
						break;
				case 6:
						sbox4 = s2[row][col];
						break;
				case 12:
						sbox4 = s3[row][col];
						break;
				case 18:
						sbox4 = s4[row][col];
						break;
				case 24:
						sbox4 = s5[row][col];
						break;
				case 30:
						sbox4 = s6[row][col];
						break;
				case 36:
						sbox4 = s7[row][col];
						break;
				case 42:
						sbox4 = s8[row][col];
						break;
				default: {
						std::cout << "ERROR!, sBoxSelector invalid arqument" << std::endl;
						exit(0);
				}
			}
		sBoxOut32 += sbox4.to_string();
	}
}
void DES::straightPermu (const std::string &inBlk32) {
	std::bitset<32> outBlk32;			// initial permutation output
		int index;
		for (auto it = inBlk32.begin(); it < inBlk32.end(); it++) {
			if (*it == '1') {
				index = std::distance(straightPermutationTable.begin(),
								 std::find (straightPermutationTable.begin(),
										 straightPermutationTable.end(),
										 std::distance ( inBlk32.begin(), it) + 1));
				outBlk32.set( 31 - index,1);		// this to set the MSB to right
			}

		}
		funOut =  outBlk32.to_string();		// OR straightPermuOut32

}
void DES::parityDropPermu (const std::string &inKey64) {
	std::bitset<56> outKey56;
	int index, pos;
	for (auto it = inKey64.begin(); it < inKey64.end(); it++) {
		pos = std::distance ( inKey64.begin(), it) + 1;// std::cout <<pos << std::endl;
		if ( *it == '1' && pos != 8 && pos != 16 &&  pos != 24 &&  pos != 32 &&  pos != 40 &&  pos != 48 &&  pos != 56 &&  pos != 64 ) {
			index = std::distance(parityBitDropTable.begin(),
							 std::find (parityBitDropTable.begin(),
									 parityBitDropTable.end(), pos));
			outKey56.set(55 - index,1);
		}
	}
	 key56 = outKey56.to_string();
}
void DES::compressionPermu (const std::string &inKey56) {
	std::bitset<48> outKey48;
		int index, pos;
		for (auto it = inKey56.begin(); it < inKey56.end(); it++) {
			pos = std::distance ( inKey56.begin(), it) + 1;
			if (*it == '1' && pos != 9 && pos != 18 && pos != 22 && pos != 25 && pos !=35 && pos != 38 && pos != 43 && pos != 54) {
				index = std::distance(keyCompressionTable.begin(),
								 std::find (keyCompressionTable.begin(),
										 keyCompressionTable.end(), pos));
				outKey48.set(47 - index,1);
			}
		}
		key48 =  outKey48.to_string();
}
void DES::desFun (const std::string &inBlk32, const std::string &key48) {
	expansionPermu(inBlk32);
	xor48(expansionPermuOut48, key48);
	sBox(xorOut48);
	straightPermu(sBoxOut32);
}
void DES::split (const std::string &inBlk64) {
	leftBlk32 = inBlk64.substr(0, 32);
	rightBlk32 = inBlk64.substr(32, 32);
}
const std::string DES::bin2hex (const std::string &binStr)  {
	std::string hexStr{};
	std::string b4;				// to hold 4-bit binary number
	for (int i = 0; i < (int)binStr.length(); i += 4){
		b4 = binStr.substr(i, 4);
		switch(bin2dec(b4)) {
		case 0:
			hexStr.append("0");
			break;
		case 1:
			hexStr.append("1");
			break;
		case 2:
			hexStr.append("2");
			break;
		case 3:
			hexStr.append("3");
			break;
		case 4:
			hexStr.append("4");
			break;
		case 5:
			hexStr.append("5");
			break;
		case 6:
			hexStr.append("6");
			break;
		case 7:
			hexStr.append("7");
			break;
		case 8:
			hexStr.append("8");
			break;
		case 9:
			hexStr.append("9");
			break;
		case 10:
			hexStr.append("A");
			break;
		case 11:
			hexStr.append("B");
			break;
		case 12:
			hexStr.append("C");
			break;
		case 13:
			hexStr.append("D");
			break;
		case 14:
			hexStr.append("E");
			break;
		case 15:
			hexStr.append("F");
			break;
		default:{
			std::cout << "ERROR!, bin2hex binary string must be multiple of 4";
			exit(0);
		}
		}
	}
	/*std::string hexStr;
	std::stringstream ss;
	std::bitset<64> b(binStr);
	ss << std::setfill('0') << std::setw(12) << std::hex << b.to_ulong();
	hexStr = ss.str();
	std::transform (hexStr.begin(), hexStr.end(), hexStr.begin(), ::toupper);*/
	return hexStr;
}
const std::string DES::hex2bin (const std::string &hexStr) {
	std::string binStr{};
	for (auto c = hexStr.begin(); c < hexStr.end(); ++c) {
		switch(*c) {
		case '0':
			binStr.append("0000");
			break;
		case '1':
			binStr.append("0001" );
			break;
		case '2':
			binStr.append("0010");
			break;
		case '3':
			binStr.append("0011");
			break;
		case '4':
			binStr.append("0100");
			break;
		case '5':
			binStr.append("0101");
			break;
		case '6':
			binStr.append("0110");
			break;
		case '7':
			binStr.append("0111");
			break;
		case '8':
			binStr.append("1000");
			break;
		case '9':
			binStr.append("1001");
			break;
		case 'A':
			binStr.append("1010");
			break;
		case 'B':
			binStr.append("1011");
			break;
		case 'C':
			binStr.append("1100");
			break;
		case 'D':
			binStr.append("1101");
			break;
		case 'E':
			binStr.append("1110");
			break;
		case 'F':
			binStr.append("1111");
			break;
		default:
			std::cout << "ERROR!, Invalid input, can't convert to binary" << std::endl;
		}
	}
	return binStr;
}
const int DES::bin2dec (const std::string &binStr) {
	int decNum {};			// initialize decNum to zero c++11
	for (auto it = binStr.rbegin(); it < binStr.rend(); it++)
		if (*it == '1')
		decNum += pow(2, std::distance(binStr.rbegin(), it));
	return decNum;
}
void DES::cipher (const std::string &plainText, const std::string &key64) {
	initialPermu(plainText);				// InitialPermutation
	split(initialPermuOut64);				// divide the 64-bit inBlk into two Blks of 32-bit each
								// Generate 16 round key  as an array
	for (int i = 0; i < 16; i++) {			// for loop for 16 round
		keygen(key64, i+1);
		desFun(rightBlk32, key48);		//output funOut 32-bit blk
		xor32(funOut, leftBlk32);			// output xorOut32 = leftblok32
		if (i != 15)
			leftBlk32.swap(rightBlk32);			// Swap the 32-bit data blocks
		showCipher();
		}
	finalPermu(leftBlk32 + rightBlk32);
}
void DES::decipher (const std::string &cipherText, const std::string &key48){
	// do not forget in order to be able to decipher
	// generate the 16 keys and store it in array
	// redesign the key function as an array
	// to do later
}
void DES::showCipher() {
	static int round= 0;
	std::cout<<	 BOLDBLUE<<std::setw(4) <<++round<<RESET<<std::setw(12) << bin2hex(leftBlk32) <<
			     std::setw(12) << bin2hex(rightBlk32) <<
				 std::setw(14) << bin2hex(key48)<<std::endl;
}

// DES tables declarations
// Initial Permutation Table
const std::array<int, 64> DES::initialPermutationTable
	{
		58, 50, 42, 34, 26, 18, 10,  2,
		60, 52, 44, 36, 28, 20, 12,  4,
		62, 54, 46, 38, 30, 22, 14,  6,
		64, 56, 48, 40, 32, 24, 16,  8,
		57, 49, 41, 33, 25, 17,  9,  1,
		59, 51, 43, 35, 27, 19, 11,  3,
		61, 53, 45, 37, 29, 21, 13,  5,
		63, 55, 47, 39, 31, 23, 15,  7
	};
// Final Permutation Table
const std::array<int, 64> DES::finalPermutationTable
	{
		40,  8, 48, 16, 56, 24, 64, 32,
		39,  7, 47, 15, 55, 23, 63, 31,
		38,  6, 46, 14, 54, 22, 62, 30,
		37,  5, 45, 13, 53, 21, 61, 29,
		36,  4, 44, 12, 52, 20, 60, 28,
		35,  3, 43, 11, 51, 19, 59, 27,
		34,  2, 42, 10, 50, 18, 58, 26,
		33,  1, 41,  9, 49, 17, 57, 25
	};
//Expansion Permutation Table
const std::array<int, 48> DES::expansionPermutationTalbe
	{
	   32,  1,  2,  3,  4,  5,
	    4,  5,  6,  7,  8,  9,
	    8,  9, 10, 11, 12, 13,
	   12, 13, 14, 15, 16, 17,
	   16, 17, 18, 19, 20, 21,
	   20, 21, 22, 23, 24, 25,
	   24, 25, 26, 27, 28, 29,
	   28, 29, 30, 31, 32, 01
	};
//Parity-bit Drop Table
const std::array<int, 56> DES::parityBitDropTable
	{
		57, 49, 41, 33, 25, 17,  9,  1,
		58, 50, 42, 34, 26, 18, 10,  2,
		59, 51, 43, 35, 27, 19, 11,  3,
		60, 52, 44, 36, 63, 55, 47, 39,
		31, 23, 15,  7, 62, 54, 46, 38,
		30, 22, 14,  6, 61, 53, 45, 37,
		29, 21, 13,  5, 28, 20, 12,  4
	};
//The compression premutation (P-box) changes the 56 bits to 48 bits, which are used as a key for a round
const std::array<int, 48> DES::keyCompressionTable
	{
		14, 17, 11, 24,  1,  5,  3, 28,
		15,  6, 21, 10, 23, 19, 12,  4,
		26,  8, 16,  7, 27, 20, 13,  2,
		41, 52, 31, 37, 47, 55, 30, 40,
		51, 45, 33, 48, 44, 49, 39, 56,
		34, 53, 46, 42, 50, 36, 29, 32
	};
// Straight Permutation Table
const std::array<int, 32> DES::straightPermutationTable
	{
		16,  7, 20, 21, 29, 12, 28, 17,
		 1, 15, 23, 26, 05, 18, 31, 10,
		 2,  8, 24, 14, 32, 27,  3,  9,
		19, 13, 30,  6, 22, 11,  4, 25
	};
// Substitution Boxes (S-Boxes)
const int DES::s1[4][16] =
	{ /* S1 */
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	};
const int DES::s2[4][16] =
	{ /* S2 */
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
	};

const int DES::s3[4][16] =
	{ /* S3 */
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
	};

const int DES::s4[4][16] =
	{ /* S4 */
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
	};
const int DES::s5[4][16] =
	{ /* S5 */
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
	};
const int DES::s6[4][16] =
	{ /* S6 */
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
	};
const int DES::s7[4][16] =
	{ /* S7 */
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
	};
const int DES::s8[4][16] =
	{ /* S8 */
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	};

