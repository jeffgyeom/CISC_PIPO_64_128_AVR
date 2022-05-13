#pragma GCC optimize("O3")
#include <stdint.h>
typedef uint8_t u8;
#define sbox(s7, s6, s5, s4, s3, s2, s1, s0)      \
  s5 ^= s7 & s6;                                  \
  s4 ^= s3 & s5;                                  \
  s7 ^= s4;                                       \
  s6 ^= s3;                                       \
  s3 ^= s4 | s5;                                  \
  s5 ^= s7;                                       \
  s4 ^= s5 & s6;                                  \
                                                  \
  s2 ^= s1 & s0;                                  \
  s0 ^= s2 | s1;                                  \
  s1 ^= s2 | s0;                                  \
  s2 = ~s2;                                       \
                                                  \
  s7 ^= s1;                                       \
  s3 ^= s2;                                       \
  s4 ^= s0;                                       \
                                                  \
  c0 = s7;                                        \
  c1 = s3;                                        \
  c2 = s4;                                        \
                                                  \
  s6 ^= c0 & s5;                                  \
  c0 ^= s6;                                       \
  s6 ^= c2 | c1;                                  \
  c1 ^= s5;                                       \
  s5 ^= s6 | c2;                                  \
  c2 ^= c1 & c0;                                  \
                                                  \
  s2 ^= c0;                                       \
  c0 = s1 ^ c2;                                   \
  s1 = s0 ^ c1;                                   \
                                                  \
  s0 = s7;                                        \
  s7 = c0;                                        \
  c1 = s3;                                        \
  s3 = s6;                                        \
  s6 = c1;                                        \
  c2 = s4;                                        \
  s4 = s5;                                        \
  s5 = c2;                                        \


#define inv_sbox(s7, s6, s5, s4, s3, s2, s1, s0)  \
  c0 = s7;                                        \
  s7 = s0;                                        \
  s0 = s1;                                        \
  s1 = c0;                                        \
  c0 = s7;                                        \
  c1 = s6;                                        \
  c2 = s5;                                        \
                                                  \
  s4 ^= s3 | c2;                                  \
  s3 ^= c2 | c1;                                  \
  c1 ^= s4;                                       \
  c0 ^= s3;                                       \
  c2 ^= c1 & c0;                                  \
  s3 ^= s4 & s7;                                  \
                                                  \
  s0 ^= c1;                                       \
  s1 ^= c2;                                       \
  s2 ^= c0;                                       \
                                                  \
  c0 = s3;                                        \
  s3 = s6;                                        \
  s6 = c0;                                        \
  c0 = s5;                                        \
  s5 = s4;                                        \
  s4 = c0;                                        \
                                                  \
  s7 ^= s1;                                       \
  s3 ^= s2;                                       \
  s4 ^= s0;                                       \
                                                  \
  s4 ^= s5 & s6;                                  \
  s5 ^= s7;                                       \
  s3 ^= s4 | s5;                                  \
  s6 ^= s3;                                       \
  s7 ^= s4;                                       \
  s4 ^= s3 & s5;                                  \
  s5 ^= s7 & s6;                                  \
                                                  \
  s2 = ~s2;                                       \
  s1 ^= s2 | s0;                                  \
  s0 ^= s2 | s1;                                  \
  s2 ^= s1 & s0;                                  \


#define pbox(s7, s6, s5, s4, s3, s2, s1, s0)      \
  s1 = ((s1 << 7)) | ((s1 >> 1));                 \
  s2 = ((s2 << 4)) | ((s2 >> 4));                 \
  s3 = ((s3 << 3)) | ((s3 >> 5));                 \
  s4 = ((s4 << 6)) | ((s4 >> 2));                 \
  s5 = ((s5 << 5)) | ((s5 >> 3));                 \
  s6 = ((s6 << 1)) | ((s6 >> 7));                 \
  s7 = ((s7 << 2)) | ((s7 >> 6));                 \


#define inv_pbox(s7, s6, s5, s4, s3, s2, s1, s0)  \
  s1 = ((s1 << 1)) | ((s1 >> 7));                 \
  s2 = ((s2 << 4)) | ((s2 >> 4));                 \
  s3 = ((s3 << 5)) | ((s3 >> 3));                 \
  s4 = ((s4 << 2)) | ((s4 >> 6));                 \
  s5 = ((s5 << 3)) | ((s5 >> 5));                 \
  s6 = ((s6 << 7)) | ((s6 >> 1));                 \
  s7 = ((s7 << 6)) | ((s7 >> 2));                 \


#define akey_e(s7, s6, s5, s4, s3, s2, s1, s0, e) \
  s0 ^= K[15] ^ e;                                \
  s1 ^= K[14];                                    \
  s2 ^= K[13];                                    \
  s3 ^= K[12];                                    \
  s4 ^= K[11];                                    \
  s5 ^= K[10];                                    \
  s6 ^= K[9];                                     \
  s7 ^= K[8];                                     \


#define akey_o(s7, s6, s5, s4, s3, s2, s1, s0, o) \
  s0 ^= K[7] ^ o;                                 \
  s1 ^= K[6];                                     \
  s2 ^= K[5];                                     \
  s3 ^= K[4];                                     \
  s4 ^= K[3];                                     \
  s5 ^= K[2];                                     \
  s6 ^= K[1];                                     \
  s7 ^= K[0];                                     \

void pipo_64_128_enc(u8 X[8], u8 K[16])
{
  u8 r0 = X[7];
  u8 r1 = X[6];
  u8 r2 = X[5];
  u8 r3 = X[4];
  u8 r4 = X[3];
  u8 r5 = X[2];
  u8 r6 = X[1];
  u8 r7 = X[0];

  u8 c0, c1, c2; // temporary variables to copy the 3-bit value of S-box

  //Whitening
  akey_e(r7, r6, r5, r4, r3, r2, r1, r0, 0x00);

  //1Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x01);

  //2Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_e(r7, r6, r5, r4, r3, r2, r1, r0, 0x02);

  //3Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x03);

  //4Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_e(r7, r6, r5, r4, r3, r2, r1, r0, 0x04);

  //5Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x05);

  //6Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_e(r7, r6, r5, r4, r3, r2, r1, r0, 0x06);

  //7Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x07);

  //8Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_e(r7, r6, r5, r4, r3, r2, r1, r0, 0x08);

  //9Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x09);

  //10Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_e(r7, r6, r5, r4, r3, r2, r1, r0, 0x0A);

  //11Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x0B);

  //12Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_e(r7, r6, r5, r4, r3, r2, r1, r0, 0x0C);

  //13Round
  sbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  pbox  (r7, r6, r5, r4, r3, r2, r1, r0);
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x0D);

  X[7] = r0;
  X[6] = r1;
  X[5] = r2;
  X[4] = r3;
  X[3] = r4;
  X[2] = r5;
  X[1] = r6;
  X[0] = r7;
}

void pipo_64_128_dec(u8 X[8], u8 K[16])
{
  u8 r0 = X[7];
  u8 r1 = X[6];
  u8 r2 = X[5];
  u8 r3 = X[4];
  u8 r4 = X[3];
  u8 r5 = X[2];
  u8 r6 = X[1];
  u8 r7 = X[0];

  u8 c0, c1, c2; // temporary variables to copy the 3-bit value of S-box

  //13Round
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x0D);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //12Round
  akey_e  (r7, r6, r5, r4, r3, r2, r1, r0, 0x0C);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //11Round
  akey_o  (r7, r6, r5, r4, r3, r2, r1, r0, 0x0B);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //10Round
  akey_e  (r7, r6, r5, r4, r3, r2, r1, r0, 0x0A);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //9Round
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x09);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //8Round
  akey_e  (r7, r6, r5, r4, r3, r2, r1, r0, 0x08);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //7Round
  akey_o  (r7, r6, r5, r4, r3, r2, r1, r0, 0x07);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //6Round
  akey_e  (r7, r6, r5, r4, r3, r2, r1, r0, 0x06);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //5Round
  akey_o(r7, r6, r5, r4, r3, r2, r1, r0, 0x05);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //4Round
  akey_e  (r7, r6, r5, r4, r3, r2, r1, r0, 0x04);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //3Round
  akey_o  (r7, r6, r5, r4, r3, r2, r1, r0, 0x03);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //2Round
  akey_e  (r7, r6, r5, r4, r3, r2, r1, r0, 0x02);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //1Round
  akey_o  (r7, r6, r5, r4, r3, r2, r1, r0, 0x01);
  inv_pbox(r7, r6, r5, r4, r3, r2, r1, r0);
  inv_sbox(r7, r6, r5, r4, r3, r2, r1, r0);

  //Whitening
  akey_e(r7, r6, r5, r4, r3, r2, r1, r0, 0x00);



  X[7] = r0;
  X[6] = r1;
  X[5] = r2;
  X[4] = r3;
  X[3] = r4;
  X[2] = r5;
  X[1] = r6;
  X[0] = r7;
}
