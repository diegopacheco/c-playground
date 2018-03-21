#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <stddef.h>
#include "lang_oop_coordinates.c"

/*
 * Author        : Diego Pacheco
 * DataStructure : Language-C
 * Problem       : OOP
 * Complexity    : -
 * Source        : -
 */
int main(){

  Coordinate *c1 = newCoordinate();
  c1->setx(c1, 1);
  c1->sety(c1, 2);

  Coordinate *c2 = newCoordinate();
  c2->setx(c2, 3);
  c2->sety(c2, 4);

  c1->print(c1);
  c2->print(c2);

  destroyCoordinate(c1);
  destroyCoordinate(c2);

  return 0;
}
