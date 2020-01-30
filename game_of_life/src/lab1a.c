/*
 * lab1a.c
 *
 *  Created on:
 *      Author:
 */

/* include helper functions for game */
#include "lifegame.h"

/* add whatever other includes here */
#include <stdio.h>
#include <stdlib.h>

/* number of generations to evolve the world */
#define NUM_GENERATIONS 50
/* hard-coded world size */
#define WORLDWIDTH 39
#define WORLDHEIGHT 20
/* character representations of cell states */
#define CHAR_ALIVE '*'
#define CHAR_DEAD ' '

/* functions to implement */

/* this function should set the state of all
   the cells in the next generation and call
   finalize_evolution() to update the current
   state of the world to the next generation */
void next_generation(void);

/* this function should return the state of the cell
   at (x,y) in the next generation, according to the
   rules of Conway's Game of Life (see handout) */
int get_next_state(int x, int y);

/* this function should calculate the number of alive
   neighbors of the cell at (x,y) */
int num_neighbors(int x, int y);

int main(void)
{
  int n;
  int height,width;

  height=get_world_height();
  width=get_world_width();


  /* TODO: initialize the world */
  initialize_world();

  for (n = 0; n < NUM_GENERATIONS; n++)
    next_generation();

  /* TODO: output final world state */
  output_world();

  return 0;
}

void next_generation(void) {
  int height,width;
  height=get_world_height();
  width=get_world_width();

  for(int x=0; x<width; x++)
    for(int y=0; y<height; y++)
      set_cell_state(x,y,get_next_state(x,y));

  finalize_evolution(); /* called at end to finalize */
}

int get_next_state(int x, int y) {
  int cur_status = get_cell_state(x,y);
  int neighbours = num_neighbors(x,y);
  int res = DEAD;

  if(cur_status == ALIVE) {
    if( neighbours == 3 || neighbours == 2 )
      res = ALIVE;
    else
      res = DEAD;
  } else if ( cur_status == DEAD ) {
    if( neighbours == 3 )
      res = ALIVE;
    else
      res = DEAD;
  }
  return res;
}

int num_neighbors(int x, int y) {
  int count=0;

  //Left+Right
  count+=get_cell_state(x+1,y);
  count+=get_cell_state(x-1,y);
  //Above
  count+=get_cell_state(x-1,y+1);
  count+=get_cell_state(x,y+1);
  count+=get_cell_state(x+1,y+1);
  //Below
  count+=get_cell_state(x-1,y-1);
  count+=get_cell_state(x,y-1);
  count+=get_cell_state(x+1,y-1);

  return count;

}
