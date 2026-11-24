#include "tabulas.h"

//
// 😃 - veretiys, veiksmei, lai kods strada 🙏🙏🙏
// 
const uint8_t referenceMATRICA[4][4]{
     { 0x02,0x03,0x01,0x01 },
     { 0x01,0x02,0x03,0x01 },
     { 0x01,0x01,0x02,0x03 },
     { 0x03,0x01,0x01,0x02 } };

void g_func(uint8_t w3[4], int* ptr_round) {
    uint8_t g_temp[4] = { w3[1], w3[2], w3[3], w3[0] }; // nobidits pa kreisi

    // ( RotWord X1)

    for (int i = 0; i < 4; i++)
    {
        g_temp[i] = sbox[g_temp[i]];
    }
    // ( SubWord Y1)

    g_temp[0] ^= RCON[*ptr_round]; //XOR ar r1

    for (int i = 0; i < 4; i++)
    {
        w3[i] = g_temp[i];
    }
}

void keyExpansion(uint8_t key[4][4], uint8_t w[44][4]) {

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            w[i][j] = key[j][i];
        }
    } // parkopet matricu

    for (int i = 4; i < 44; i++)
    {
        uint8_t temp[4] = { w[i - 1][0], w[i - 1][1], w[i - 1][2], w[i - 1][3] };

        if (i % 4 == 0)
        {
            int round = i / 4;
            g_func(temp, &round);
        } // w3 ieliek gfunc

        for (int j = 0; j < 4; j++)
        {
            w[i][j] = w[i - 4][j] ^ temp[j]; // XOR operacija
        }
    }

            //   26:12 AES III

}

void printState(uint8_t matrica[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++)
            std::cout << std::hex << std::setw(2) << std::setfill('0')
            << (int)matrica[r][c] << ' ';
        std::cout << '\n';
    }
}

void subBytes(uint8_t matrica[4][4]){ //strada ka vaig, paldies veretiy 😃

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++) {
            matrica[r][c] = sbox[matrica[r][c]];
        }
    }
}

void shiftRows(uint8_t matrica[4][4]) {
    
    uint8_t tempMatrica[4][4];

    std::memcpy(tempMatrica, matrica, 16);

    for (int r = 1; r < 4;r++){
    for (int c = 0; c < 4; c++)
        {
        matrica[r][c] = tempMatrica[r][(c+r)%4]; // ideja - atlikums ir nobide, paldies veretiy 😃, kods strada
    }
 }


}

void mixColumns(uint8_t matrica[4][4]) {

    uint8_t rezultats[4][4];
  

    for (int r = 0;r < 4;r++)
    {
        for (int c = 0;c < 4;c++)
        {
            rezultats[r][c] =
                galoisFieldReizinasana(referenceMATRICA[r][0], matrica[0][c]) ^ // pec reizinasanas darit XOR darbibas
                galoisFieldReizinasana(referenceMATRICA[r][1], matrica[1][c]) ^ // b1 = (b1 * 2) XOR (b2*3) XOR (b3*1) XOR (b4*1)
                galoisFieldReizinasana(referenceMATRICA[r][2], matrica[2][c]) ^ //
                galoisFieldReizinasana(referenceMATRICA[r][3], matrica[3][c]);  //


        }
    }

    /*for (int r = 0; r < 4; r++)  {   
        for (int c = 0; c < 4; c++){
            matrica[r][c] = rezultats[r][c];
        }
    }*/
    std::memcpy(matrica, rezultats, 16);

}

void addRoundKey(uint8_t matrica[4][4], uint8_t w[44][4],int round) {

        // matrica[0][0] XOR w[0][0]
        for (int j = 0; j < 4; j++)
        {
            matrica[0][j] ^= w[j + (round * 4)][0];
            matrica[1][j] ^= w[j + (round * 4)][1];
            matrica[2][j] ^= w[j + (round * 4)][2];
            matrica[3][j] ^= w[j + (round * 4)][3];
        }
}

    //inputToMatrix(matrica);
     
    /*keyExpansion(masterKey, w);
    
    addRoundKey(matrica, w, 0);
    printState(matrica);    std::cout << '\n';
    

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
    printState(matrica);

*/
