/* ����Z�[���X�}�����������Ǐ��T���@�ɂ��ߎ�
�A���S���Y���i2-OPT�����OR-OPT�ߖT) */

#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#define N 600         /* �n�_���̍ő�T�C�Y */
#define ZERO 0.0001   /* ���e�덷�� */
#define SEED 14       /* �����̎� */ 
struct point          /* �\����p�̒�` */
{
 float x;
 float y;
};
int n;                /* �O���ϐ��F�_�̌� */
struct point p[N];    /* p[i].x��p[i].y�͓_i�̍��W */

/* �֐��̐錾 */
float initial(int *x);
float local(int *init, float lginit, int *lopt);
float length(int i, int j, int *x);
void printtour(int *x, float length);
int rand_from(int min, int max);

main()
/* TSP�Ǐ��T���@�̃e�X�g�v���O���� */
{
 int i, init[N], lopt[N];
 float initlg, bestlg;
 FILE *file;
 file=fopen("locdata1", "r");   /* ���̓f�[�^�̓Ǎ� */
 fscanf(file, "%d", &n);
 for(i=0; i<n; i++)
 {
  fscanf(file, "%f", &p[i].x);
  fscanf(file, "%f", &p[i].y);
 }
 printf("n = %d\n", n);
 printf("point i       x[i]       y[i] \n");
 for(i=0; i<n; i++) 
  printf("%3d        %f,  %f\n", i, p[i].x, p[i].y);
 srand(SEED);
 initlg=initial(init);          /* ��������H�̐��� */
 printf("initial tour\n");
 printtour(init, initlg);
 bestlg=local(init, initlg, lopt);  /* �Ǐ��T�� */
 printf("local optimal tour\n");    /* ���ʂ̏o�� */
 printtour(lopt, bestlg);
}

float initial(int *x)
/* �n�_��n�̏�������H��x�ɍ쐬�A���̒������o�͂���� */
{
 int i, j, r, test[N];   /* test[i]�͒n�_i�̗��p�t���O */
 float lg;
 for(i=0; i<n; i++) test[i]=0;
 j=0;
 for(i=0; i<n; i++)
 {
  r=rand_from(0, n-i-1);  /* �n�_j����r�ڂ̋󂫒n�_��x[i]�Ƃ��� */
  while(test[j]==1) j=(j+1)%n;
  while(r>0)
  {
   r=r-1; j=(j+1)%n;
   while(test[j]==1) j=(j+1)%n;
  }
  test[j]=1; x[i]=j;
 } 
 lg=0.0;
 for(i=0; i<n; i++) lg=lg+length(i, i+1, x);  /* ����H�̒��� */
 return(lg);
}

float local(int *init, float initlg, int *lopt)
/* ����Hinit���������i���̒���initlg�j�Ƃ��āA2-OPT�����OR-OPT�ߖT��
���Ǐ��T����K�p���A�Ǐ��œK��lopt�̏���H�����o�� */
{
 int i, i0, j, k, h, temp, tm[3];
 float lg, lgtemp, lg0, lg1;
 for(i=0; i<n; i++) lopt[i]=init[i];
 lg=initlg;
 i0=0;
 RESTART:                             /* �ߖT�T���̊J�n */
 for(i=i0; i<i0+n; i++)               /* 2-OPT�ߖT�̒T�� */
 {
  for(j=i+2; j<i+n-1; j++)            /* lgtemp�͒����̑��� */
  {
   lgtemp=length(i, j, lopt)+length(i+1, j+1, lopt)
          -length(i, i+1, lopt)-length(j, j+1, lopt);
   if(lgtemp<-ZERO)                   /* ���ǉ��̔��� */
   {
    lg=lg+lgtemp;
    for(k=0; k<(j-i)/2; k++)          /* ���ǉ��̍\�� */
    {
     temp=lopt[(i+1+k)%n]; lopt[(i+1+k)%n]=lopt[(j-k)%n]; 
     lopt[(j-k)%n]=temp;
    }
    printf("improved solution by 2-OPT\n");
    goto IMPROVED;
   }
  } 
 } 
 for(i=i0; i<i0+n; i++)             /* Or-OPT�ߖT�̒T�� */ 
 {
  for(k=i+1; k<=i+3; k++)
  {
   for(j=k+1; j<i+n-1; j++)         /* lgtemp�͒����̑��� */
   {
    lg0=length(i, i+1, lopt)+length(j, j+1,lopt)+length(k, k+1, lopt);
    lg1=length(i, k+1, lopt)+length(j, k, lopt)+length(i+1, j+1,lopt);
    lgtemp=lg1-lg0;
    if(lgtemp<-ZERO)                /* ���ǉ��̔��� */
    {
     lg=lg+lgtemp;                  /* ���ǉ��̍\�� */
     for(h=i+1; h<=k; h++) tm[h-i-1]=lopt[h%n];
     for(h=k+1; h<=j; h++) lopt[(h-k+i)%n]=lopt[h%n];
     for(h=0; h<k-i; h++) lopt[(j-k+i+1+h)%n]=tm[k-i-1-h];
     printf("improved solution by Or-OPT\n");
     goto IMPROVED;
    }
    if(k==i+1) continue;
    lg1=length(i, k+1, lopt)+length(j, i+1, lopt) /* �t�����̑}�� */
        +length(k, j+1, lopt);
    lgtemp=lg1-lg0;
    if(lgtemp<-ZERO)                /* ���ǉ��̔��� */
    {
     lg=lg+lgtemp;                  /* ���ǉ��̍\�� */
     for(h=i+1; h<=k; h++) tm[h-i-1]=lopt[h%n];
     for(h=k+1; h<=j; h++) lopt[(h-k+i)%n]=lopt[h%n];
     for(h=0; h<k-i; h++) lopt[(j-h)%n]=tm[k-i-1-h];
     printf("improved solution by Or-OPT\n");
     goto IMPROVED;
    }
   }
  }
 }
 return(lg);                     /* �Ǐ��T���I�� */
 IMPROVED:                       /* �b����̍X�V�F���̋ߖT�T���� */
 printtour(lopt, lg); i0=(i+1)%n; 
 goto RESTART;
}

float length(int i, int j, int *x)
/* p[x[i%n]]��p[x[j%n]]�̃��[�N���b�h���� */
{
 return(sqrt(pow(p[x[i%n]].x-p[x[j%n]].x, 2)
    +pow(p[x[i%n]].y-p[x[j%n]].y, 2)));
}


void printtour(int *x, float lg)
/* ����Hx�Ƃ��̒���lg���o�� */
{
 int i;
 printf("tour = (");
 for(i=0; i<n; i++) printf("%d ", x[i]); printf(")\n");
 printf("length = %f\n", lg);
}

int rand_from(int min, int max)
/* ���[min, max]���烉���_���ɐ�����I�� */
/* rand��0����RAND_MAX�܂ł̗������𐶐�����W���֐� */
{
 return((int)(((double)rand()/((unsigned)RAND_MAX+1))
   *(max-min+1))+min);
}

