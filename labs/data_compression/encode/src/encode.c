#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#define MAX_SYMBOLS 255
#define MAX_SYMBOLS 255
#define MAX_LEN    20 

struct tnode
{
    struct  tnode* left; /*used when in tree*/
    struct  tnode*right; /*used when in tree*/  
    struct  tnode*parent;/*used when in tree*/
    struct  tnode* next; /*used when in list*/ 
    float   freq;
    int     isleaf;
    char     symbol;
};


/*global variables*/
char code[MAX_SYMBOLS][MAX_LEN];
struct tnode* root=NULL; /*tree of symbols*/
struct tnode* qhead=NULL; /*list of current symbols*/
struct cnode* chead=NULL;/*list of code*/

/*
    @function   talloc
    @desc       allocates new node 
*/
struct tnode* talloc(int symbol,float freq)
{
    struct tnode* p=(struct tnode*)malloc(sizeof(struct tnode));
    if(p!=NULL)
    {
        p->left=p->right=p->parent=p->next=NULL;
        p->symbol=symbol;
        p->freq=freq;
		p->isleaf=1;
    }
    return p;
}

/*
    @function display_tnode_list
    @desc     displays the list of tnodes during code construction
*/
void pq_display(struct tnode* head)
{
    struct tnode* p=NULL;
    printf("list:");
    for(p=head;p!=NULL;p=p->next)
    {
        printf("(%c,%f) ",p->symbol,p->freq);
    }
    printf("\n");
}

/*
 *  parse_file function.
 *
 *  desc::
 *    This function takes a filepointer as input, and an float pointer.
 *    The function will generate frequency array information for each symbol.
 */

void parse_file ( FILE *infp, float *freqp ) {
  unsigned long uniq_cnt = 0;
  unsigned long scnt[MAX_SYMBOLS];
  char c;

  //Initialize array to 0
  memset(scnt,0,sizeof(scnt));

  //Keep track of occurance per symbol.
  //Keep track of totals
  while ( (c = fgetc(infp)) != EOF ) {
    scnt[c] += 1;
    uniq_cnt++;
    printf("Found %c, %lu times\n",c,scnt[c-'a']);
  }

  for(int i=0; i<MAX_SYMBOLS; i++)
    *(freqp+i) = ((float) scnt[i])/uniq_cnt;
}

/*
    @function pq_insert
    @desc     inserts an element into the priority queue
    NOTE:     makes use of global variable qhead
*/
void pq_insert(struct tnode* p)
{
    struct tnode* curr=NULL;
    struct tnode* prev=NULL;
   printf("inserting:%c,%f\n",p->symbol,p->freq);
   if(qhead==NULL) {
     //First element added.
     qhead=p;
   } else {
    for(curr=qhead; curr!=NULL; curr=curr->next) {
      if( curr->freq > p->freq ) {
        //We've gone too far, insert it at previous position.
        //Prevous could be NULL,
        //  If !NULL
        //    Insert p between previous and current.
        //  else
        //    Insert p at root, followed by curr.
        if(prev) {
          prev->next = p;
          p->next=curr;
          return;
        } else {
          qhead = p;
          p->next = curr;
          return;
        }
      }
      prev = curr;
    }
    //Searched all elements, and no insertion was done,
    //insert at end
    prev->next = p;
   }
}

/*
    @function pq_pop
    @desc     removes the first element
    NOTE:     makes use of global variable qhead
*/

struct tnode* pq_pop()
{
    struct tnode* p=NULL;
    if(qhead) {
      p = qhead;
      qhead = qhead->next;
	    printf("popped:%c,%f\n",p->symbol,p->freq);
    }
    return p;
}

/*
	@function build_code
	@desc     generates the string codes given the tree
	NOTE: makes use of the global variable root
*/
void generate_code(struct tnode* root,int depth)
{
	char symbol;
	int len; /*length of code*/
  struct tnode *cur = NULL;
  struct tnode *prev = root;
	if(root->isleaf)
	{
		symbol=root->symbol;
		len   =depth;
    if(depth > MAX_LEN) {
      printf("Too deep. Limit is %d",MAX_LEN);
      exit(1);
    }
		/*start backwards*/
		code[symbol][len]=0;
    //Search backwards, and stop when parent == NULL
    //  When parrent == NULL, then we're at the top of the tree.
    for(cur=root->parent; cur != NULL; cur=cur->parent) {
      len--;
      if(prev == cur->right)
        code[symbol][len] = '1';
      else if ( prev == cur->left)
        code[symbol][len] = '0';
      else
        exit(1);
      //Keep track of previous
      prev=cur;
    }

		printf("built code:%c,%s\n",symbol,code[symbol]);
	}
	else
	{
		generate_code(root->left,depth+1);
		generate_code(root->right,depth+1);
	}
	
}

/*
	@func	dump_code
	@desc	output code file
*/
void dump_code(FILE* fp)
{
	int i=0;
	for(i=0;i<MAX_SYMBOLS;i++)
	{
		if(code[i][0]) /*non empty*/
			fprintf(fp,"<tag>%c</tag>,%s\n",i,code[i]);
	}
}

void encode_fp(FILE *fin, FILE *fout) {
  char c;
  while ( (c = fgetc(fin)) != EOF ) {
    fprintf(fout,"%s",code[c]);
  }
}

/*
	@func	encode
	@desc	outputs compressed stream
*/
void encode(char* str,FILE* fout)
{
	while(*str)
	{
		fprintf(fout,"%s",code[*str]);
		str++;
	}
}

/*
 * @function cleanall
 */
void cleanall(struct tnode *root) {
  if(root->isleaf) {
    free(root);
  } else {
    cleanall(root->right);
    cleanall(root->left);

    //All children are not clean, clean parent
    free(root);
  }
}

/*
    @function main
*/

void usage(void) {
  printf("Usage for encoder::\
      encode -i FILENAME -o FILENAME\
      -i FILENAME, input filename for txt file to compress\
      -o FILENAME, output filename for txt file to compress\
      ");
}

int main(int argc, char *argv[])
{
    /*test pq*/
    struct tnode* p=NULL;
    struct tnode* lc,*rc;
    float freq[MAX_SYMBOLS];
    int i=0;
    int opt;
    int ucnt=0;
	  const char *CODE_FILE = "code.txt";
	  const char *IN_FILE;
	  const char *OUT_FILE;
	  FILE *fin=NULL;
    FILE* fout=NULL;
	  /*zero out code*/
	  memset(code,0,sizeof(code));
	  memset(freq,0,sizeof(freq));


    //Parse commandline args to get input file from commandline.
    while( ( opt = (getopt(argc,argv,"i:o:h")) ) != -1 ) {
      switch(opt) {
        case 'h':
          usage();
          break;
        case 'i':
          IN_FILE = optarg;
          printf("Filename:: %s\n",IN_FILE);
          break;
        case 'o':
          OUT_FILE = optarg;
          printf("Compressed file:: %s\n",OUT_FILE);
          break;
      }
    }

    if(!IN_FILE) {
      printf("Error! Input file must be provided by user.\n");
      usage();
      exit(1);
    }

    /* fin = fopen(IN_FILE,'r'); */
    /* fout = fopen(OUT_FILE,'r'); */


  fin = fopen(IN_FILE,"r");

  //populate freq
  parse_file(fin,freq);

  fclose(fin);

	  qhead=NULL;
    /*initialize with freq*/
    for(i=0;i<MAX_SYMBOLS;i++)
    {
      if(freq[i] != 0) {
        pq_insert(talloc(i,freq[i]));
        ucnt++;
      }
    }
    /*build tree*/
    for(i=0;i<ucnt-1;i++)
    {
        lc=pq_pop();
        rc=pq_pop();
        /*create parent*/
        p=talloc(0,lc->freq+rc->freq);
        /*set parent link*/
        lc->parent=rc->parent=p;
        /*set child link*/
        p->right=rc; p->left=lc;
		    /*make it non-leaf*/
	      p->isleaf=0;
        /*add the new node to the queue*/
        pq_insert(p);
    }
    /*get root*/
    root=pq_pop();

    if(root==NULL) {
      printf("Error! tree could not be created\n");
      exit(1);
    }


	generate_code(root,0);
	/*output code*/
	puts("code:");
	fout=fopen(CODE_FILE,"w");
	dump_code(stdout);
	dump_code(fout);
	fclose(fout);

	/*encode a sample string*/
	fout=fopen(OUT_FILE,"w");
  fin=fopen(IN_FILE,"r");
  encode_fp(fin,fout);
	fclose(fout);
	getchar();
  cleanall(root);
  return 0;
}
