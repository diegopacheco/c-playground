#include <stdio.h>
#include <stdlib.h>
#include "lang_oop_coordinates.h"

/*
 * Author        : Diego Pacheco
 * DataStructure : Language-C
 * Problem       : OOP
 * Complexity    : -
 * Source        : -
 */
static void coordinate_setx(Coordinate *this, int x){
  if (this!=NULL){
    this->x=x;
  }
}

static void coordinate_sety(Coordinate *this, int y){
  if (this!=NULL){
    this->y=y;
  }
}

static void coordinate_print(Coordinate *this){
  if (this!=NULL){
    printf("Coordinate[x: %d, y: %d]\n", this->x,this->y);
  }else{
    printf("Coordinate is NULL\n");
  }
}

Coordinate *newCoordinate(void){
  Coordinate *c = malloc(sizeof(Coordinate));
  if(c!=0){
    c->setx = &coordinate_setx;
    c->sety = &coordinate_sety;
    c->print = &coordinate_print;
    c->x = 0;
    c->y = 0;
  }
  return c;
}

void destroyCoordinate(Coordinate *this){
  if(this!=NULL){
    free(this);
  }
}
