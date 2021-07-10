/* structures */

typedef struct _history {
    int num;
    int order_num;
    char rest[100];
    struct _history* next;
} history;

typedef history* hptr;


typedef struct _optable {
    int num;
    char name[10];
    char data[5];
    struct _optable* next;
} optable;

typedef optable* optr;


/* functions */

// inputs
int scan_order(char*); // get order from keyboard
int order_link(char*); // from input, excute order function
int order_to_num(char*); // change order to number
void dump_rest(); // scan left words

// order help
int function_help();

// order dir
int function_dir();

// order quit
int function_quit();

// order history
int function_history();
void set_history(); // make head pointer of history
void insert_history(int, char*); // insert data of order to history
void print_order(int); // from order number, print order in word
void free_history(); // free all history structures

// order dump
int function_dump();
void print_memory(int, int); // from start & end, print memory in range
void print_memory_by_line(int, int, int); // print memory by line

// order edit
int function_edit();

// order fill
int function_fill();

// order reset
int function_reset();

// order opcode
int function_opcode();
void set_hash_table(); // make hash table
int word_to_hash(char*); // change opcode to hash
void free_hash_table(); // free hash table

// order opcodelist
int function_opcodelist();


/* global variables */

// input
int rest_exist; // 1 if rest inputs exist

// history
hptr head, tail; // pointer for checking history

// memory
unsigned char memory[65536][16]; // where memory are saved
const int memory_end = 1048576; // value of 65536 * 16

// dump function
int last_dump = 0; // the last end range of dump order

// opcode table
optr* opcode; // pointer for reading hash table
