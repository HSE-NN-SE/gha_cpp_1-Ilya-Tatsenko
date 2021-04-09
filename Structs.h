#pragma once
#define amout_of_cities 703
typedef struct {
  int x;
  int y;
} city, Cities;


typedef struct {
  int path[amout_of_cities];
  int cost;
} Solution;
