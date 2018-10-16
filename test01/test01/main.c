/*
 * test01.c
 *
 * Created: 2018/8/16 下午 02:24:19
 * Author : lenovo
 */ 

#define F_CPU 11059200
#include <avr/io.h>
#include "ASA_Lib.h"
#include <string.h>
#include "ASA_Lib_HMI.h"

#define HMI_TYPE_I8   0
#define HMI_TYPE_I16  1
#define HMI_TYPE_I32  2
#define HMI_TYPE_I64  3
#define HMI_TYPE_UI8  4
#define HMI_TYPE_UI16 5
#define HMI_TYPE_UI32 6
#define HMI_TYPE_UI64 7
#define HMI_TYPE_F32  8
#define HMI_TYPE_F64  9

typedef struct St {
	uint8_t ui8[10];
	int32_t i32[10];
	float   f32[5];
} st_t;

int main() {
	ASA_M128_set();
	char s[20];

	st_t data = {
		{1, 2, 3,  4,  5,  6,   7,   8,   9,   10},
		{2, 4, 8, 16, 32, 64, 128, 256, 512, 1024},
		{1.0, 2.5, 4.0, 5.5, 6.0}
	};
	uint16_t bytes = sizeof(st_t);

	printf("M128 Start -------------!\n");
	printf("Plz say something to M128\n");
	scanf("%s", s);

	M128_HMI_Form_put("ui8x10,i32x10,f32x5\0", bytes, &data);

	M128_HMI_Form_get("ui8x10,i32x10,f32x5\0", bytes, &data);
	data.ui8[0] += 1;
	data.i32[0] += 1;
	data.f32[0] += 1.0;

	M128_HMI_Form_put("ui8x10,i32x10,f32x5\0", bytes, &data);

	return 0;
}
