# DES (Data Encryption Standard)
This code designed to help researchers and students to study and analyze the structure of DES. plainText and a cypherKey must be a 64-bit long.
All encryption rounds will be printed out on each run. The code structure is based on "Cryptography and Network Security" by Behrouz A. Forouzan

### EXAMPLE
```sh
plainText = "123456ABCD132536";
key = "AABB09182736CCDD";
will generate
round    left       right     Round Key
   1    18CA18AD    5A78E394  194CD072DE8C
   2    5A78E394    4A1210F6  4568581ABCCE
   3    4A1210F6    B8089591  06EDA4ACF5B5
   4    B8089591    236779C2  DA2D032B6EE3
   5    236779C2    A15A4B87  69A629FEC913
   6    A15A4B87    2E8F9C65  C1948E87475E
   7    2E8F9C65    A9FC20A3  708AD2DDB3C0
   8    A9FC20A3    308BEE97  34F822F0C66D
   9    308BEE97    10AF9D37  84BB4473DCCC
  10    10AF9D37    6CA6CB20  02765708B5BF
  11    6CA6CB20    FF3C485F  6D5560AF7CA5
  12    FF3C485F    22A5963B  C2C1E96A4BF3
  13    22A5963B    387CCDAA  99C31397C91F
  14    387CCDAA    BD2DD2AB  251B8BC717D0
  15    BD2DD2AB    CF26B472  3330C5D9A36D
  16    19BA9212    CF26B472  181C5D75C66D
Plain  Text: 123456ABCD132536
Key        : AABB09182736CCDD
Cipher Text: C0B7A8D05F3A829C
Plain  Text: 0001001000110100010101101010101111001101000100110010010100110110
Key        : 1010101010111011000010010001100000100111001101101100110011011101
Cipher Text: 1100000010110111101010001101000001011111001110101000001010011100
```
NOTE: This code has no command line options,so every time you need to change the plainText and/or cipherKey change
them from inside the Main.cpp file and recompile the code with g++. as bellow
`g++ Main.cpp DES.cpp DES.h Colors.h -o des`. 
To run it type
`./des`
