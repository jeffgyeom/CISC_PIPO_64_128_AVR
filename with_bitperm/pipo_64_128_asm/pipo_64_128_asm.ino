#pragma GCC optimize("O3")
#include <string.h>
#include <stdint.h>

//test vector
static const u8 tv_K[16] = {
  0x6D, 0xC4, 0x16, 0xDD, 0x77, 0x94, 0x28, 0xD2, 
  0x7E, 0x1D, 0x20, 0xAD, 0x2E, 0x15, 0x22, 0x97
};
static const u8 tv_P[8] = {
  0x09, 0x85, 0x52, 0xF6, 0x1E, 0x27, 0x00, 0x26
};
static const u8 tv_C[8] = {
  0x6B, 0x6B, 0x29, 0x81, 0xAD, 0x5D, 0x03, 0x27
};

#define NUM_BLOCKS  10000
typedef uint8_t u8;

extern "C"
{
  extern void pipo_64_128_enc_asm(u8 X[8], u8 K[16]);
  extern void pipo_64_128_dec_asm(u8 X[8], u8 K[16]);
}

void setup() {
  Serial.begin(9600);
}


void loop() {
  u8 t[8], k[16];
  int i;
  unsigned long enc_time, dec_time;
  
  memcpy(t, tv_P, sizeof(tv_P));
  memcpy(k, tv_K, sizeof(tv_K));
  pipo_64_128_enc_asm(t, k);
  if (memcmp(t, tv_C, sizeof(tv_C)) != 0)
  {   
      Serial.print("There is an Error in the Encryption\n");
      return;
  }
  memcpy(t, tv_C, sizeof(tv_C));
  memcpy(k, tv_K, sizeof(tv_K));
  pipo_64_128_dec_asm(t, k);
  if (memcmp(t, tv_P, sizeof(tv_P)) != 0)
  {   
      Serial.print("There is an Error in the Decryption\n");
      return;
  }
  Serial.print("Test Vector Pass!\n");

  memcpy(t, tv_P, sizeof(tv_P));
  memcpy(k, tv_K, sizeof(tv_K));
  enc_time = micros();
  for(i=0; i<NUM_BLOCKS; i++)
    pipo_64_128_enc_asm(t, k);
  enc_time = micros() - enc_time;
  Serial.print("Enc : ");
  Serial.println(enc_time);

  memcpy(t, tv_C, sizeof(tv_C));
  memcpy(k, tv_K, sizeof(tv_K));
  dec_time = micros();
  for(i=0; i<NUM_BLOCKS; i++)
    pipo_64_128_dec_asm(t, k);
  dec_time = micros() - dec_time;
  Serial.print("Dec : ");
  Serial.println(dec_time);

}
