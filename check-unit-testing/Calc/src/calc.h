/**
*
* Program: calc.h - Calculator Header
* Autor: Diego Pacheco
* Since: 02/MAY/2018
* Version: 1.0
*
**/

typedef struct Calc {
  long (*sum)(long a,long b);
  long (*sub)(long a,long b);
  long (*divi)(long a,long b);
  long (*mul)(long a,long b);
} Calc;

Calc *newCalc();
void destroyCalc(Calc *c);
