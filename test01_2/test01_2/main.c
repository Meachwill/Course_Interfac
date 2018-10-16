/*
 * test01_2.c
 *
 * Created: 2018/8/17 下午 01:52:13
 * Author : lenovo
 */ 
#define  F_CPU 11059200
#include <avr/io.h>
#include "ASA_Lib.h"

void interation(float *a,float *b);
int j=0;



int main(void)
{
	ASA_M128_set();
    /* Replace with your application code */
    while (1) 
    {
		float u=0,y=0;
		int i;
			printf("u[0]=\n");
			scanf("%f",&u);
			interation(&u,&y);
			printf("%f\n",y);
		
    }
}

void interation(float *a,float *b){
	static float A[2][2];
	static float B[2][1];
	static float C[1][2];
	static float D;
	static float x[2][1];
	static float x1[2][1],u1[2][1],xy,uy;
	int i,k;
	for(i=0;i<2;i++){    //初始化
		u1[i][1]=0;
	}
	if(j==0){
		for(i=0;i<2;i++){    //初始化
			x1[i][1]=0;
			u1[i][1]=0;
		}
		for(i=0;i<2;i++){      //輸入x
			printf("x[%d]=\n",i);
			scanf("%f",&x[i][0]);
		}
		for(i=0;i<2;i++){      //輸入A
			for(k=0;k<2;k++){
				printf("A[%d][%d]=\n",i,k);
				scanf("%f",&A[i][k]);
			}
		}
		for(i=0;i<2;i++){   //輸入B
			for(k=0;k<1;k++){
				printf("B[%d][%d]=\n",i,k);
				scanf("%f",&B[i][k]);
			}
		}
		for(i=0;i<1;i++){   //輸入C
			for(k=0;k<2;k++){
				printf("C[%d][%d]=\n",i,k);
				scanf("%f",&C[i][k]);
			}
			printf("D[0]=\n");//輸入D
			scanf("%f",&D);
			j+=1;
		}
	}
	else{
		x[1][0]=x1[1][0];
	}

	
	
	
	/*------------------------------------------------------------------*/
	for(i=0;i<2;i++){                 //A*X
		for(k=0;k<2;k++){
			x1[i][0]=x1[i][0]+A[i][k]*x[k][0];
		}
	}
	for(i=0;i<2;i++){                //B*u
		for(k=0;k<1;k++){
			u1[i][0]=u1[i][0]+B[i][k]*(*a);
		}
	}
	for(i=0;i<2;i++){                //x(t+1)
		x1[i][0]=x1[i][0]+u1[i][0];
	}
	for(i=0;i<2;i++){               //C*X
		for(k=0;k<1;k++){
			xy=xy+C[k][i]*x1[i][k];
		}
	}
    uy=uy+D*(*a);//D*u          
	*b=xy+uy;//y(t+1)
}

