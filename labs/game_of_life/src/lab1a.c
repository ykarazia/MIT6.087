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
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
/* number of generations to evolve the world */
#define NUM_GENERATIONS 50
/* hard-coded world size */
#define WORLDWIDTH 39
#define WORLDHEIGHT 20
/* character representations of cell states */
#define CHAR_ALIVE '*'
#define CHAR_DEAD ' '

const char kPathSeparator =
#ifdef _WIN32
                            '\\';
#else
                            '/';
#endif

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

  typedef enum initType { HC, FLE } init_t;
  //typedef enum initType init_t;

void usage(void) {
  printf("\
      Usage: gameoflife [-h] [-o FILENAME] -f FILENAME\
        -h : Print this usage information, and exit.\
        -f FILENAME : Provide the filename by which you would like to initialize the game.\
        -o FILENAME : Provide a save file to which the final world state will be saved.\
      ");
}

int main(int argc, char *argv[])
{
  int n;
  int height,width;
  int opt;
  char *fni;
  char *fno;
  char *fdo;
  char *default_fdo = ".savefiles";

  init_t init = HC;
  height=get_world_height();
  width=get_world_width();


  //process commandline arguments to select input file
  while( ( opt = (getopt(argc,argv,"i:o:d:h")) ) != -1 ) {
    switch(opt) {
      case 'h':
        usage();
        break;
      case 'i':
        init = FLE;
        fni = optarg;
        printf("Filename:: %s\n",fni);
        break;
      case 'o':
        fno = optarg;
        printf("Save File:: %s\n",fno);
        break;
      case 'd':
        fdo = optarg;
        printf("Save Directory:: %s\n",fdo);
        break;
    }
  }

  switch(init) {
    case HC : 
      initialize_world();
      break;
    case FLE:
      initialize_world_from_file(fni);
  }

  printf("Initialized World\n");
  output_world();

  for (n = 0; n < NUM_GENERATIONS; n++)
    next_generation();

  printf("Final World\n");
  output_world();

  if(fno) {
    DIR *d;

    if(!fdo)
      fdo = default_fdo;

    d = opendir(fdo);

    if(!d) 
      mkdir(fdo, 0777);
    

    chdir(fdo);
    save_world_to_file(fno);
    chdir("../");
  }

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
