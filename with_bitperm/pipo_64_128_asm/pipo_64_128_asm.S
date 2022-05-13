#pragma GCC optimize("O3")
/*
 * pipo_64_128_asm.S
 * 
 */ 

.macro regBackup
  .irp i, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 28, 29
    push  \i
  .endr
.endm

.macro regRetrieve
  clr   r1
  .irp i, 29, 28, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 0
    pop   \i
  .endr
  ret
.endm

// r0  ~ r7  : s0 ~ s7
#define s0  r0
#define s1  r1
#define s2  r2
#define s3  r3
#define s4  r4
#define s5  r5
#define s6  r6
#define s7  r7
// r8  ~ r15 : k0 ~ k7
#define k0  r8
#define k1  r9
#define k2  r10
#define k3  r11
#define k4  r12
#define k5  r13
#define k6  r14
#define k7  r15
// r16 ~ r23 : k0 ~ k15
#define k8  r16
#define k9  r17
#define k10 r18
#define k11 r19
#define k12 r20
#define k13 r21
#define k14 r22
#define k15 r23
// r24 ~ r26 : c0 ~ c2
#define c0  r24
#define c1  r25
#define c2  r26
// r27 : t
#define t   r27
// r28 : c
#define c   r28

.macro sbox
  /* S5_1 */
  //s5 ^= s7 & s6
  mov   t,      s6
  and   t,      s7
  eor   s5,     t
  //s4 ^= s3 & s5;
  mov   t,      s5
  and   t,      s3
  eor   s4,     t
  //s7 ^= s4;
  eor   s7,     s4
  //s6 ^= s3;
  eor   s6,     s3
  //s3 ^= s4 | s5;
  mov   t,      s5
  or    t,      s4
  eor   s3,     t
  //s5 ^= s7;
  eor   s5,      s7
  //s4 ^= s5 & s6;
  mov   t,      s6
  and   t,      s5
  eor   s4,     t
  
  /* S3 */
  //s2 ^= s1 & s0;
  mov   t,      s0
  and   t,      s1
  eor   s2,     t
  //s0 ^= s2 | s1;
  mov   t,      s1
  or    t,      s2
  eor   s0,     t
  //s1 ^= s2 | s0; 
  mov   t,      s0
  or    t,      s2
  eor   s1,     t
  //s2 = ~s2;
  ser   t
  eor   s2,     t
  
  /* XOR */
  //s7 ^= s1;
  eor   s7,     s1
  //s3 ^= s2;
  eor   s3,     s2
  //s4 ^= s0;
  eor   s4,     s0

  /* COPY */
  //c0 = s7;
  mov   c0,     s7
  //c1 = s3;
  mov   c1,     s3
  //c0 = s4;
  mov   c2,     s4

  /* S5_2 */
  //s6 ^= c0 & s5;
  mov   t,      s5
  and   t,      c0
  eor   s6,     t
  //c0 ^= s6;
  eor   c0,     s6
  //s6 ^= c2 | c1;
  mov   t,      c1
  or    t,      c2
  eor   s6,     t  
  //c1 ^= s5;
  eor   c1,     s5
  //s5 ^= s6 | c2;
  mov   t,      c2
  or    t,      s6
  eor   s5,     t    
  //c2 ^= c1 & c0;
  mov   t,      c0
  and   t,      c1
  eor   c2,     t      

  /* XOR and Bit Permutation */
  //s2 ^= c0;
  eor   s2,     c0
  //c0 = s1 ^ c2;
  mov   t,      c2
  eor   t,      s1
  mov   c0,     t
  //s1 = s0 ^ c1;
  mov   t,      c1
  eor   t,      s0
  mov   s1,     t
  //s0 = s7; 
  mov   s0,     s7
  //s7 = c0;
  mov   s7,     c0
  //c1 = s3;
  mov   c1,     s3
  //s3 = s6;
  mov   s3,     s6
  //s6 = c1;
  mov   s6,     c1
  //c2 = s4;
  mov   c2,     s4
  //s4 = s5;
  mov   s4,     s5
  //s5 = c2;
  mov   s5,     c2
.endm

.macro inv_sbox
  /* bitperm (partial) */
  //c0 = s7;
  mov   c0,     s7
  //s7 = s0;
  mov   s7,     s0
  //s0 = s1;
  mov   s0,     s1
  //s1 = c0;
  mov   s1,     c0
  //c0 = s7;
  mov   c0,     s7
  //c1 = s6;
  mov   c1,     s6
  //c2 = s5;
  mov   c2,     s5

  /* S5_2_var */
  //s4 ^= s3 | c2;
  mov   t,      c2
  or    t,      s3
  eor   s4,     t
  //s3 ^= c2 | c1;
  mov   t,      c1
  or    t,      c2
  eor   s3,     t
  //c1 ^= s4;
  eor   c1,     s4
  //c0 ^= s3;
  eor   c0,     s3
  //c2 ^= c1 & c0;
  mov   t,      c0
  and   t,      c1
  eor   c2,     t
  //s3 ^= s4 & s7;
  mov   t,      s7
  and   t,      s4
  eor   s3,     t
  
  /* XOR */
  //s0 ^= c1;
  eor   s0,     c1
  //s1 ^= c2;
  eor   s1,     c2
  //s2 ^= c0;
  eor   s2,     c0
  
  /* bitperm (partial) */
  //c0 = s3;
  mov   c0,     s3
  //s3 = s6;
  mov   s3,     s6
  //s6 = c0;
  mov   s6,     c0
  //c0 = s5;
  mov   c0,     s5
  //s5 = s4;
  mov   s5,     s4
  //s4 = c0;
  mov   s4,     c0

  /* XOR */
  //s7 ^= s1;
  eor   s7,     s1
  //s3 ^= s2;
  eor   s3,     s2
  //s4 ^= s0;
  eor   s4,     s0



  /* inv S5_1 */
  //s4 ^= s5 & s6;
  mov   t,      s6
  and   t,      s5
  eor   s4,     t
  //s5 ^= s7;
  eor   s5,     s7
  //s3 ^= s4 | s5;
  mov   t,      s5
  or    t,      s4
  eor   s3,     t
  //s6 ^= s3;
  eor   s6,     s3
  //s7 ^= s4;
  eor   s7,     s4
  //s4 ^= s3 & s5;
  mov   t,      s5
  and   t,      s3
  eor   s4,     t
  //s5 ^= s7 & s6;
  mov   t,      s6
  and   t,      s7
  eor   s5,     t
  
  /* inv S3 */
  //s2 = ~s2;
  ser   t
  eor   s2,     t
  //s1 ^= s2 | s0;
  mov   t,      s0
  or    t,      s2
  eor   s1,     t
  //s0 ^= s2 | s1;
  mov   t,      s1
  or    t,      s2
  eor   s0,     t
  //s2 ^= s1 & s0;
  mov   t,      s0
  and   t,      s1
  eor   s2,     t
.endm


.macro pbox
  //zero
  eor   t,      t
  //s1 = ((s1 << 7)) | ((s1 >> 1));
  bst   s1,     0
  lsr   s1
  bld   s1,     7
  //s2 = ((s2 << 4)) | ((s2 >> 4));
  swap  s2
  //s3 = ((s3 << 3)) | ((s3 >> 5));
  swap  s3
  bst   s3,     0
  lsr   s3
  bld   s3,     7
  //s4 = ((s4 << 6)) | ((s4 >> 2));
  swap  s4
  lsl   s4
  adc   s4,     t
  lsl   s4
  adc   s4,     t
  //s5 = ((s5 << 5)) | ((s5 >> 3));
  swap  s5
  lsl   s5
  adc   s5,     t
  //s6 = ((s6 << 1)) | ((s6 >> 7));
  lsl   s6
  adc   s6,     t
  //s7 = ((s7 << 2)) | ((s7 >> 6));
  lsl   s7
  adc   s7,     t
  lsl   s7
  adc   s7,     t
.endm

.macro inv_pbox
  //zero
  eor   t,      t
  //s1 = ((s1 << 1)) | ((s1 >> 7));
  lsl   s1
  adc   s1,     t
  //s2 = ((s2 << 4)) | ((s2 >> 4));
  swap  s2
  //s3 = ((s3 << 5)) | ((s3 >> 3));
  swap  s3
  lsl   s3
  adc   s3,     t
  //s4 = ((s4 << 2)) | ((s4 >> 6));
  lsl   s4
  adc   s4,     t
  lsl   s4
  adc   s4,     t
  //s5 = ((s5 << 3)) | ((s5 >> 5));
  swap  s5
  bst   s5,     0
  lsr   s5
  bld   s5,     7
  //s6 = ((s6 << 7)) | ((s6 >> 1));
  bst   s6,     0
  lsr   s6
  bld   s6,     7
  //s7 = ((s7 << 6)) | ((s7 >> 2));
  swap  s7
  lsl   s7
  adc   s7,     t
  lsl   s7
  adc   s7,     t
.endm

.macro akey_e
  eor s7,     k15
  eor s6,     k14
  eor s5,     k13
  eor s4,     k12
  eor s3,     k11
  eor s2,     k10
  eor s1,     k9
  eor s0,     k8
  eor s0,     c
.endm

.macro akey_o
  eor s7,     k7
  eor s6,     k6
  eor s5,     k5
  eor s4,     k4
  eor s3,     k3
  eor s2,     k2
  eor s1,     k1
  eor s0,     k0
  eor s0,     c
.endm

.macro inc_rc
  inc c
.endm

.macro dec_rc
  dec c
.endm


// extern void pipo_64_128_enc_asm(u8 X[8], u8 K[16]);
// extern void pipo_64_128_dec_asm(u8 X[8], u8 K[16]);
// Input Args :
// X = r24, r25
// K = r22, r23
// Internal Register
// input variables
// r0  ~ r7  : s0 ~ s7  : 64-bit Block X[8]
// r8  ~ r15 : k0 ~ k7  : 128-bit Key  K[16]
// r16 ~ r23 : k8 ~ k15       
// local variables
// r24 ~ r26 : c0 ~ c2  : copied 3-bit value in sbox
// r27 : t              : temparary variable
// r28 : c              : Round Constants
// r29, r30             : Y-Register
.text
.global pipo_64_128_enc_asm

pipo_64_128_enc_asm:
  regBackup

  // push plaintext addr
  push  r24
  push  r25
  
  // Loading Plaintext
  movw  r28,  r24
  ld    s7,   Y
  ldd   s6,   Y+1
  ldd   s5,   Y+2
  ldd   s4,   Y+3
  ldd   s3,   Y+4
  ldd   s2,   Y+5
  ldd   s1,   Y+6
  ldd   s0,   Y+7

  // Loading Round Key
  movw  r28,  r22
  ld    k7,   Y
  ldd   k6,   Y+1
  ldd   k5,   Y+2
  ldd   k4,   Y+3
  ldd   k3,   Y+4
  ldd   k2,   Y+5
  ldd   k1,   Y+6
  ldd   k0,   Y+7

  ldd   k15,  Y+8
  ldd   k14,  Y+9
  ldd   k13,  Y+10
  ldd   k12,  Y+11
  ldd   k11,  Y+12
  ldd   k10,  Y+13
  ldd   k9,   Y+14
  ldd   k8,   Y+15
  // Init round idx
  eor   c,    c

  // (Whitening) Key Adds
  akey_e
  inc_rc

  //1Round
  sbox
  pbox
  akey_o
  inc_rc
  
  //2Round
  sbox
  pbox
  akey_e
  inc_rc
  
  //3Round
  sbox
  pbox
  akey_o
  inc_rc

  //4Round
  sbox
  pbox
  akey_e
  inc_rc

  //5Round
  sbox
  pbox
  akey_o
  inc_rc

  //6Round
  sbox
  pbox
  akey_e
  inc_rc

  //7Round
  sbox
  pbox
  akey_o
  inc_rc

  //8Round
  sbox
  pbox
  akey_e
  inc_rc

  //9Round
  sbox
  pbox
  akey_o
  inc_rc

  //10Round
  sbox
  pbox
  akey_e
  inc_rc
  
  //11Round
  sbox
  pbox
  akey_o
  inc_rc

  //12-Round
  sbox
  pbox
  akey_e
  inc_rc

  //13-Round
  sbox
  pbox
  akey_o
  
  // Store Out
  pop   r29
  pop   r28

  st    Y,    s7
  std   Y+1,  s6
  std   Y+2,  s5
  std   Y+3,  s4
  std   Y+4,  s3
  std   Y+5,  s2
  std   Y+6,  s1
  std   Y+7,  s0
  //Must clear before returning
  eor   r1,   r1
  regRetrieve



.text
.global pipo_64_128_dec_asm

pipo_64_128_dec_asm:
  regBackup

  // push plaintext addr
  push  r24
  push  r25
  
  // Loading Plaintext
  movw  r28,  r24
  ld    s7,   Y
  ldd   s6,   Y+1
  ldd   s5,   Y+2
  ldd   s4,   Y+3
  ldd   s3,   Y+4
  ldd   s2,   Y+5
  ldd   s1,   Y+6
  ldd   s0,   Y+7

  // Loading Round Key
  movw  r28,  r22
  ld    k7,   Y
  ldd   k6,   Y+1
  ldd   k5,   Y+2
  ldd   k4,   Y+3
  ldd   k3,   Y+4
  ldd   k2,   Y+5
  ldd   k1,   Y+6
  ldd   k0,   Y+7

  ldd   k15,  Y+8
  ldd   k14,  Y+9
  ldd   k13,  Y+10
  ldd   k12,  Y+11
  ldd   k11,  Y+12
  ldd   k10,  Y+13
  ldd   k9,   Y+14
  ldd   k8,   Y+15
  // Init round idx
  ldi   c,    0x0D

  //13Round
  akey_o
  inv_pbox
  inv_sbox
  dec_rc

  //12Round
  akey_e
  inv_pbox
  inv_sbox
  dec_rc
  
  //11Round
  akey_o
  inv_pbox
  inv_sbox
  dec_rc

  //10Round
  akey_e
  inv_pbox
  inv_sbox
  dec_rc

  //9Round
  akey_o
  inv_pbox
  inv_sbox
  dec_rc

  //8Round
  akey_e
  inv_pbox
  inv_sbox
  dec_rc
  
  //7Round
  akey_o
  inv_pbox
  inv_sbox
  dec_rc

  //6Round
  akey_e
  inv_pbox
  inv_sbox
  dec_rc
  
  //5Round
  akey_o
  inv_pbox
  inv_sbox
  dec_rc

  //4Round
  akey_e
  inv_pbox
  inv_sbox
  dec_rc
  
  //3Round
  akey_o
  inv_pbox
  inv_sbox
  dec_rc

  //2Round
  akey_e
  inv_pbox
  inv_sbox
  dec_rc
  
  //1Round
  akey_o
  inv_pbox
  inv_sbox
  dec_rc

  //Whitening Key
  akey_e

  // Store Out
  pop   r29
  pop   r28

  st    Y,    s7
  std   Y+1,  s6
  std   Y+2,  s5
  std   Y+3,  s4
  std   Y+4,  s3
  std   Y+5,  s2
  std   Y+6,  s1
  std   Y+7,  s0
  //Must clear before returning
  eor   r1,   r1
  regRetrieve
