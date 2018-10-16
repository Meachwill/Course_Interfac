#define F_CPU 11059200
#include <avr/io.h>
#include "./lib/ASA_Lib_test.h"
#include <string.h>
//#include "./lib/ASA_Lib_HMI.h"

#define HMI_TYPE_I8   0
#define HMI_TYPE_I16  1
#define HMI_TYPE_I32  2`
#define HMI_TYPE_I64  3
#define HMI_TYPE_UI8  4
#define HMI_TYPE_UI16 5
#define HMI_TYPE_UI32 6
#define HMI_TYPE_UI64 7
#define HMI_TYPE_F32  8
#define HMI_TYPE_F64  9

float matrix(float *a,float *b,float *c,float *d,int32_t *u,float *x_t);

typedef struct St {
	float A[2][2];
	float B[2][1];
	float C[1][2];
  float D[1];
  int32_t U[1];
} st_t;

int main() {
  ASA_M128_set();
  char s[20];
  float test1_y[100];
  int i=0;
  //float x_test[2][100];
  st_t data = {
    {{0,1}, {-2,2}},
    {{0},{1}},
    {{-2,4}},
    {1},
    {0}
  };
	float x_test[2][100];
	float testu[100];


  uint16_t bytes = sizeof(st_t);
  uint16_t bytes_ans=100*sizeof(float);
  uint16_t bytes_ans2=200*sizeof(float);
  printf("M128 Start -------------!\n");
  printf("Plz say something to M128\n");
  scanf("%s", s);
	M128_HMI_Form_put("f32x4,f32x2,f32x2,f32x1,i32x1\0",bytes, &data);
	M128_HMI_get(bytes_ans, &testu);

while(i<100){
  //M128_HMI_Form_put("f32x4,f32x2,f32x2,f32x1,i32x1\0",bytes, &data);
  test1_y[i]=matrix(&data.A[0][0],&data.B[0][0],&data.C[0][0],&data.D[0],&testu[i],&x_test[0][0]);
	//printf("%f\n",testu[i]);
  i++;
}
  M128_HMI_Form_put("f32x100\0", bytes_ans, &test1_y);
  //M128_HMI_Form_put("f32x200\0", bytes_ans2, &x_test);
  return 0;
}

  float matrix(float *a,float *b,float *c,float *d,int32_t *u,float *x_t){
  static float x[2][1]={{0},{0}};
	float u1[2][1],x2,u2,ans=0,x1[2][1]={{0},{0}};
	static int z=0;
  u1[0][0]=0;
  u1[1][0]=0;
  x2=0;
  u2=0;
	printf("%f\n%f\n",x[0][0],x[1][0]);
  x1[0][0]=(*(a+0))*x[0][0]+(*(a+1))*x[1][0];
  x1[1][0]=(*(a+2))*x[0][0]+(*(a+3))*x[1][0];
	//printf("%f\n%f\n%f\n%f\n",(*(a+0)),(*(a+1)),(*(a+2)),(*(a+3)));
	//printf("%f\n%f\n",x1[0][0],x1[1][0]);
  for(int i=0;i<2;i++){
    u1[i][0]=(*(b+i))*(*u);
  }

  x1[0][0]=x1[0][0]+u1[0][0];
  x1[1][0]=x1[1][0]+u1[1][0];

  x2=(*(c+0))*x[0][0]+(*(c+1))*x[1][0];
  u2=(*d)*(*u);
  x[0][0]=x1[0][0];
  x[1][0]=x1[1][0];
	(*(x_t+z))=x[0][0];
	(*(x_t+z+100))=x[1][0];
	z++;
ans=x2+u2;
  return(ans);

}
