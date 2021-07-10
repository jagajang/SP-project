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


typedef struct _smbtable {
	char name[30];
	int addr;
	struct _smbtable* next;
} smbtable;

typedef smbtable* sptr;


typedef struct _asmline {
	int addr;
	char obco[20];
	struct _asmline* next;
} asmline;

typedef asmline* lptr;


typedef struct _relocate {
	int addr;
	int leng;
	struct _relocate* next;
} relocate;

typedef relocate* rptr;


typedef struct _exsym {
	char name[10];
	int addr;
} exsym;


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

// order assemble
int function_assemble();
// part 1 symbol table
void free_symbol_table(); // reset symbol table before assemble
int make_symbol_table(char*); // make new symbol table
int scan_asm(FILE*, char*, int*, int); // scan opcode and others by 1 line
int insert_symbol(char*, int); // insert symbol to table

// part 2 list file
int make_lst();
void set_asm_line(); // make head pointer of asmline
void reset_asm_line(); // delete data of asmline
void insert_asm_line(int, char*); // insert data of asmline
void free_asm_line(); // free all asmline
int symbol_address(char*); // get symbol name and return symbol address
int reg(char*); // get register name and return register number
int find_address(char*, int*); // get name(or number) and return its address

// part 3 object file
int make_obj();
void print_obli(FILE*);
void print_relo(FILE*);
void set_relocate();
void insert_relocate(int, int);
void free_relocate();

// order type
int function_type();

// order symbol
int function_symbol();
void set_symbol_top();
void free_symbol_top();

// order progaddr
int function_progaddr();
void set_progaddr();

// order loader
int function_loader();
void loader_part1(char[3][20], int);
void loader_part2(char[3][20], int);
int find_addr(char[20]);
void print_estab(int);
void add_modi(int, int);

// order bp
int function_bp();
void set_bp();

// order run
int function_run();
void exec_opcode(int);
int find_format(int, int);
void format1(int);
void format2(int, int);
void format34(int, int);

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
optr* opcode; //pointer for reading hash table

// symbol table
sptr top;

// asm linked list
lptr lhead, lnow;
char asmname[10];
int asmst, asmen;

// relocation list
rptr rhead, rtail;

// program address
int progaddr;

// external symbol table
exsym estab[3][100];
int progst[3], proglen[3], esnum[3];
int execst[3];

// breakpoint
char bp[65536][16];
int bpnum;

// run
int regis[10];
