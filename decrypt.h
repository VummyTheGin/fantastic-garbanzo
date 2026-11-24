#pragma once
#include "tabulas.h"

/*

	procedure INVCIPHER(in, Nr, w)
		2: state ← in . See Sec. 3.4
		3: state ← ADDROUNDKEY(state, w[4 ∗ Nr..4 ∗ Nr + 3]) . See Sec. 5.1.4
		4: for round from Nr − 1 downto 1 do
			5: state ← invShiftRows(state) . See Sec. 5.3.1			😃
			6: state ← invSubBytes(state) . See Sec. 5.3.2			😃
			7: state ← ADDROUNDKEY(state, w[4 ∗ round..4 ∗ round + 3]) 
			8: state ← invMixColumns(state) . See Sec. 5.3.3 😃
		9: end for
		10: state ← invShiftRows(state)
		11: state ← invSubBytes(state)
		12: state ← ADDROUNDKEY(state, w[0..3])
	13: return state


*/
const uint8_t invReferenceMATRICA[4][4] = {
        {0x0e, 0x0b, 0x0d, 0x09},
        {0x09, 0x0e, 0x0b, 0x0d},
        {0x0d, 0x09, 0x0e, 0x0b},
        {0x0b, 0x0d, 0x09, 0x0e}
    };

void invShiftRows(uint8_t matrica[4][4]) {

	uint8_t temp[4][4];
	std::memcpy(temp, matrica, 16);

	for (int r = 1; r < 4;r++) {
		for (int c = 0; c < 4; c++)
		{
			matrica[r][c] = temp[r][(c - r+4) % 4]; //bez +4 nestradaja, jo 0-1%4
		}
	}
};

void invSubBytes(uint8_t matrica[4][4]) {

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++) {
			matrica[r][c] = inv_sbox[matrica[r][c]];
		}
	}
}

void invMixColumns(uint8_t matrica[4][4]){

	uint8_t rezultats[4][4];
	std::memcpy(rezultats, matrica, 16);
    for (int r = 0;r < 4;r++)
    {
        for (int c = 0;c < 4;c++)
        {
            rezultats[r][c] =
                galoisFieldReizinasana(invReferenceMATRICA[r][0], matrica[0][c]) ^ // pec reizinasanas darit XOR darbibas
                galoisFieldReizinasana(invReferenceMATRICA[r][1], matrica[1][c]) ^ // b1 = (b1 * 2) XOR (b2*3) XOR (b3*1) XOR (b4*1)
                galoisFieldReizinasana(invReferenceMATRICA[r][2], matrica[2][c]) ^ //
                galoisFieldReizinasana(invReferenceMATRICA[r][3], matrica[3][c]);  //
        }
    }	
	std::memcpy(matrica, rezultats, 16);
}