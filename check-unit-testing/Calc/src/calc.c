/**
*
* Program: calc.c - Calculator Implementation
* Autor: Diego Pacheco
* Since: 02/MAY/2018
* Version: 1.0
*
**/
#include <stdlib.h>
#include "calc.h"

static long sum(long a,long b) {
	 return a+b;
}

static long sub(long a,long b) {
	 return a-b;
}

static long divi(long a,long b) {
	 return a/b;
}

static long mul(long a,long b) {
	 return a*b;
}

Calc *newCalc() {
	Calc *c = malloc(sizeof(Calc));
  if (c==NULL){
    return NULL;
	}
  c->sum = &sum;
  c->sub = &sub;
  c->divi = &divi;
  c->mul = &mul;
  return c;
}

void destroyCalc(Calc* c) {
	 free(c);
}
