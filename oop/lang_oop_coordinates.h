/*
 * Author        : Diego Pacheco
 * DataStructure : Language-C
 * Problem       : OOP
 * Complexity    : -
 * Source        : -
 */
 typedef struct Coordinate {
   int x;
   int y;
   // Methods
   void (*setx)(struct Coordinate *this,int x);
   void (*sety)(struct Coordinate *this,int y);
   void (*print)(struct Coordinate *this);
 } Coordinate;

Coordinate *newCoordinate(void);
void destroyCoordinate(Coordinate *this);
