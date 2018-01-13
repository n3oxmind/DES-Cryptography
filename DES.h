#ifndef DES_H_
#define DES_H_
#include <array>

class DES {
private:
	 std::string plainText;
	 std::string cipherText;			// Final permutation ouput; 64-bits of cipher text
	 std::string leftBlk32;				// Left Block of the plainText
	 std::string rightBlk32;			// Right Block of the plainText
	 std::string leftKey28;
	 std::string rightKey28;
	 std::string key64;					// input key 64-bits
	 std::string key56;					// Parity bit drop permutation output
	 std::string key48;					// Round key that will be used for encryption
	 std::string xorOut48;
	 std::string sBoxOut32;				// 32-bit data Block from S-Boxes operation
	 std::string initialPermuOut64;		// Initial permutation to the plain text
	 std::string expansionPermuOut48;	// expansion permutation output; 48-bits
	 std::string funOut;				// Straight permutation ouput; 32-bits

public:
	 // getters and setters functions
	void setPlainText (const std::string &msg) {plainText = msg;};
	std::string getPlainText() const { return plainText;};
	std::string getCipherText() const { return cipherText;};
	std::string getKey48() const { return key48;};
	std::string getKey56() const { return key56; };
	std::string getKey64() const { return key64;};
	std::string getLeftKey28() const { return leftKey28; };
	std::string getRightKey28() const { return rightKey28; };
	std::string getXorOut48() const { return xorOut48; };
	std::string getSBoxOut() const { return sBoxOut32; };
	std::string getFunOut() const { return funOut; };
	std::string getInitialPermuOut() const { return initialPermuOut64; };
	std::string getExpansionPermuOut() const { return expansionPermuOut48; };
	std::string getLeftBlk32() const { return leftBlk32; };
	std::string getRightBlk32() const { return rightBlk32; };


	void xor48 (const std::string &strA64, const std::string &strB64);
	void xor32 (const std::string &lBlk32, const std::string &rBlk32);
	void split (const std::string &inBlk64);
	void initialPermu (const std::string &inBlk64);
	void finalPermu (const std::string &inBlk64);
	void expansionPermu (const std::string &inBlk32);
	void straightPermu (const std::string &inBlk32);
	void parityDropPermu (const std::string &inKey64);
	void compressionPermu(const std::string &inKey56);
	void shiftLeft(std::string &str28, int n);
	void keygen (const std::string &key64, int round);
	void sBox (const std::string &inBlk48);
	void desFun (const std::string &inBlk32, const std::string &key48);
	void cipher(const std::string &plainText, const std::string &key64);
	void decipher(const std::string &cipherText, const std::string &key48);
	void showCipher();

	//Functions provide conversion between number systems
	const static std::string bin2hex (const std::string &binStr);
	const static std::string hex2bin (const std::string &hexStr);
	const static int bin2dec (const std::string &binStr);

public:
	// DES Table
	const static std::array<int, 64> initialPermutationTable;
	const static std::array<int, 64> finalPermutationTable;
	const static std::array<int, 48> expansionPermutationTalbe;
	const static std::array<int, 56> parityBitDropTable;
	const static std::array<int, 48> keyCompressionTable;
	const static std::array<int, 32> straightPermutationTable;
	// DES S-Boxes
	const static int s1[4][16];
	const static int s2[4][16];
	const static int s3[4][16];
	const static int s4[4][16];
	const static int s5[4][16];
	const static int s6[4][16];
	const static int s7[4][16];
	const static int s8[4][16];
	DES(std::string PlainText, std::string inKey64);
	virtual ~DES();
};

#endif /* DES_H_ */
