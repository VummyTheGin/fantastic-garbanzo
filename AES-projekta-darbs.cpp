#include <iostream>
#include <cstdint>
#include <iomanip>
#include <string>
#include <cstring>
#include "decrypt.h"
#include "cypher.h"

//
// 😃 - veretiys, veiksmei, lai kods strada 🙏🙏🙏
// 
void inputToMatrix(uint8_t matrica[4][4]) {
    std::string input;
    std::cout << "Ievadat maximums 16bit tekstu" << std::endl;
    std::getline(std::cin, input);

    if (input.length() > 16) {
        std::cout << "Kļūda: max 16 biti.\n,tiek šifrēta kodā piejamā matrica.";
        return;
    }

    for (int i = 0; i < 16; i++) {
        if (i < (int)input.length())
            matrica[i % 4][i / 4] = input[i]; // %atlikums 0 1 2 3/ 0 1 2 3  | / 0000 1111 2222 3333
        else
            matrica[i % 4][i / 4] = 16 - input.length();  // padding
    }
}

std::string matrixToString(const uint8_t matrica[4][4]) {
    std::string text;
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            uint8_t byteVal = matrica[r][c];
            text.push_back(byteVal);
        }
    }
    std::cout << "Matrica kā teksts: " << text << std::endl;
    return text;
}

int main()
{
    uint8_t w[44][4] = {}; // inicalizet visus baitus
    int izvele = 1;
    uint8_t masterKey[4][4] = { // Key = 2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c
    { 0x2b, 0x28, 0xab, 0x09 },
    { 0x7e, 0xae, 0xf7, 0xcf },
    { 0x15, 0xd2, 0x15, 0x4f },
    { 0x16, 0xa6, 0x88, 0x3c }
    };  

    uint8_t matrica[4][4] = {
        { 0x32, 0x88, 0x31, 0xe0 },
        { 0x43, 0x5a, 0x31, 0x37 },
        { 0xf6, 0x30, 0x98, 0x07 },
        { 0xa8, 0x8d, 0xa2, 0x34 }
    };
    inputToMatrix(matrica);
    std::cout << "Matrica parasta: \n";
    printState(matrica);
    keyExpansion(masterKey, w);
    addRoundKey(matrica, w, 0);
    do{
    switch (izvele)
    {
    case 1:
    

        for (int round = 1; round <= 9; round++) {
            subBytes(matrica);
            shiftRows(matrica);
            mixColumns(matrica);
         addRoundKey(matrica, w, round);  // 1-9 raunds
        }


        subBytes(matrica);                   // pedeja raunda nav mixColumns
        shiftRows(matrica);
        addRoundKey(matrica, w, 10);
        std::cout << '\n';
        std::cout << "Matrica sifreta: \n";
        printState(matrica);
        izvele = 2;
        break;
    case 2:
            addRoundKey(matrica, w, 10);
        for(int invRound = 9; invRound >= 1; invRound--){
                invShiftRows(matrica);
                invSubBytes(matrica);
                addRoundKey(matrica, w, invRound);
                invMixColumns(matrica);
        }
                invShiftRows(matrica); 
                invSubBytes(matrica); 
                addRoundKey(matrica, w, 0);
                std::cout << "Matrica atsifreta: \n";
                printState(matrica);
                izvele = 10000;
        break;
    default:
        std::cout << "nothing";
        break;
    }}while((izvele == 1 ) || ( izvele ==2 ));

    matrixToString(matrica);
	 return 0;
}