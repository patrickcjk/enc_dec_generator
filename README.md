# enc_dec_generator
Generates an header file containing macros for pointer encryption and decryption

# Example
```c++
#pragma once
#include "stdafx.h"

#define ENC_32_1(x) (__ROR4__(x + (0x4244BA8D72587921 ^ 0x78521D49416D4E6E), 0x6702EA0B))
#define DEC_32_1(x) (__ROL4__(x, 0x6702EA0B) - (0x4244BA8D72587921 ^ 0x78521D49416D4E6E))

#define ENC_32_2(x) (__ROR4__(x + (0x4373EB4B764CC49A ^ 0x68747F216BE9FEFC), 0xEC206A6))
#define DEC_32_2(x) (__ROL4__(x, 0xEC206A6) - (0x4373EB4B764CC49A ^ 0x68747F216BE9FEFC))

#define ENC_64_1(x) (__ROR8__(x + (0x6731999B58E8BDD8 ^ 0x48A05698665A29DB), 0x592E847E))
#define DEC_64_1(x) (__ROL8__(x, 0x592E847E) - (0x6731999B58E8BDD8 ^ 0x48A05698665A29DB))

#define ENC_64_2(x) (__ROL8__(x + (0x6ED32EDB69DC1113 ^ 0x7D3FCC764A955A22), 0x4525452D))
#define DEC_64_2(x) (__ROR8__(x, 0x4525452D) - (0x6ED32EDB69DC1113 ^ 0x7D3FCC764A955A22))

#define ENC_64_3(x) (__ROR8__(x + (0x6085EA3C4F8A5D1B ^ 0x56789CC15C8CDC4D), 0xAE1CC5E1))
#define DEC_64_3(x) (__ROL8__(x, 0xAE1CC5E1) - (0x6085EA3C4F8A5D1B ^ 0x56789CC15C8CDC4D))

```
