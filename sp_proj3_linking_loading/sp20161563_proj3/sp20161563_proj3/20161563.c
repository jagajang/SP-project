#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "20161563.h"

/* inputs */

// scanf order from input
int scan_order(char* order)
{
    char c;
    rest_exist = 0;

    // scanf order until ' ' or '\n' comes.
    for(int i = 0; ; i++)
    {
        if(i >= 15) // if input is too long, skip the input
        {
            scanf("%*[^\n]"); // get input until '\n'
            scanf("%*c"); // get '\n'

            printf("Please input valid order\n");
            return 1;
        }

        // check if rest exists & set rest_exist variable
        scanf("%c", &c);
        if(c == ' ' || c == '\n')
        {
            if(c == ' ')
                rest_exist = 1;

            order[i] = '\0';
            break;
        }
        order[i] = c;
    }

    return 0;
}

// get order and excute that function
int order_link(char* order)
{
    // get the number of order(1 to 10) + new orders in proj2 (11~13)
    int order_num = order_to_num(order);

    int error_check = 0;

    if (order_num == 1)      error_check = function_help();
    else if (order_num == 2) error_check = function_dir();
    else if (order_num == 3) error_check = function_quit();
    else if (order_num == 4) error_check = function_history();
    else if (order_num == 5) error_check = function_dump();
    else if (order_num == 6) error_check = function_edit();
    else if (order_num == 7) error_check = function_fill();
    else if (order_num == 8) error_check = function_reset();
    else if (order_num == 9) error_check = function_opcode();
    else if (order_num == 10)error_check = function_opcodelist();
	// proj2
	else if (order_num == 11)error_check = function_assemble();
	else if (order_num == 12)error_check = function_type();
	else if (order_num == 13)error_check = function_symbol();
	// proj3
	else if (order_num == 14)error_check = function_progaddr();
	else if (order_num == 15)error_check = function_loader();
	else if (order_num == 16)error_check = function_bp();
	else if (order_num == 17)error_check = function_run();

	// case of error message
	else if (rest_exist == 1)
	{
		dump_rest();
		return -1;
	}
	else
	{
		printf("input valid order\n");
		return -1;
	}
	
    return error_check;
    /*
        error_check == -1 -> real error
        error_check == 0 -> no error
        error_check == 3 -> quit order
    */
}

// change valid order to number 1 to 10 + (11~13: proj 2). in error case, return 0
int order_to_num(char* s)
{
    // change valid order to number 1 to 10
    if(strcmp(s, "h") == 0 || strcmp(s, "help")   == 0) return 1;
    if(strcmp(s, "d") == 0 || strcmp(s, "dir")    == 0) return 2;
    if(strcmp(s, "q") == 0 || strcmp(s, "quit")   == 0) return 3;
    if(strcmp(s, "hi")== 0 || strcmp(s, "history")== 0) return 4;
    if(strcmp(s, "du")== 0 || strcmp(s, "dump")   == 0) return 5;
    if(strcmp(s, "e") == 0 || strcmp(s, "edit")   == 0) return 6;
    if(strcmp(s, "f") == 0 || strcmp(s, "fill")   == 0) return 7;
    if(strcmp(s, "reset")     == 0) return 8;
    if(strcmp(s, "opcode")    == 0) return 9;
    if(strcmp(s, "opcodelist")== 0) return 10;
	// proj 2
	if(strcmp(s, "assemble")  == 0) return 11;
	if(strcmp(s, "type")      == 0) return 12;
	if(strcmp(s, "symbol")    == 0) return 13;
	// proj 3
	if(strcmp(s, "progaddr")  == 0) return 14;
	if(strcmp(s, "loader")    == 0) return 15;
	if(strcmp(s, "bp")        == 0) return 16;
	if(strcmp(s, "run")       == 0) return 17;

    // if unvalid order, return 0
    return 0;
}

// get rest inputs and print error message
void dump_rest()
{
    scanf("%*[^\n]");
    scanf("%*c");

    printf("Please input valid order\n");

    return;
}


/* help(1) : print name of orders */
int function_help()
{
    // check additional word -> error
    if(rest_exist == 1)
    {
        dump_rest();
        return -1;
    }

    // make history record
    insert_history(1, "");

    // print all orders
    printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\n");
    printf("e[dit] adress, value\nf[ill] start, end, value\nreset\n");
    printf("opcode mnemonic\nopcodelist\n");
	// project 2
	printf("assemble filename\ntype filename\nsymbol\n");
	// project 3
	printf("progaddr\nloader\nbp\nrun\n");

	return 0;
}


/* dir(2) : print files in current directory */
int function_dir()
{
    // check additional word -> error
    if(rest_exist == 1)
    {
        dump_rest();
        return -1;
    }

    // make history record
    insert_history(2, "");

    // read files from current directory
    DIR* now_dir = opendir("./");

    if(now_dir != NULL)
    {
        struct dirent* inp;
        struct stat buf;

        for(int i = 0; (inp = readdir(now_dir)) != NULL; i++)
        {
            lstat(inp->d_name, &buf);

            if(i % 4 == 0 && i != 0)
                printf("\n");
            printf("%25s", inp->d_name);

            // print additional symbol(*, /) by the type of file
            if(S_ISDIR(buf.st_mode))
                printf("/");
            else if(S_ISREG(buf.st_mode))
                printf("*");
        }
        printf("\n");

        closedir(now_dir);
    }
    else // if failed to read directory, send error message
    {
        printf("DIRECTORY READ ERROR!\n");
        return -1;
    }

    return 0;
}


/* quit(3) : quit the program */
int function_quit()
{
    // check additional word -> error
    if(rest_exist == 1)
    {
        dump_rest();
        return -1;
    }

    // make history record
    insert_history(3, "");

    return 3;
}


/* history(4) : read the history linked list from head to the end */
int function_history()
{
    // check additional word -> error
    if(rest_exist == 1)
    {
        dump_rest();
        return -1;
    }

    // make history record
    insert_history(4, "");

    // check if history exists
    if(head->next == NULL)
    {
        printf("NO DATA IN HISTORY!\n");
        return -1;
    }

    // read history linked list from head to end & print all data
    hptr reader = head->next;
    while(1)
    {
        printf("%-5d", reader->num);
        print_order(reader->order_num);
        printf("%s\n", reader->rest);

        if(reader->next == NULL)
            break;
        reader = reader->next;
    }

    return 0;
}

// malloc head & tail pointer of history
void set_history()
{
    head = (hptr)malloc(sizeof(history));

    head->num = 0;
    head->next = NULL;

    tail = head;

    return;
}

// insert history in linked list
void insert_history(int order_num, char* rest)
{
    tail->next = (hptr)malloc(sizeof(history));

    tail->next->num = tail->num + 1;
    tail->next->order_num = order_num;
    strcpy(tail->next->rest, rest);
    tail->next->next = NULL;

    tail = tail->next;

    return;
}

// get order number and print in sentense (only used for history)
void print_order(int order_num)
{
    if (order_num == 1)      printf("h[elp]     ");
    else if (order_num == 2) printf("d[ir]      ");
    else if (order_num == 3) printf("q[uit]     ");
    else if (order_num == 4) printf("hi[story]  ");
    else if (order_num == 5) printf("du[mp]     ");
    else if (order_num == 6) printf("e[dit]     ");
    else if (order_num == 7) printf("f[ill]     ");
    else if (order_num == 8) printf("reset      ");
    else if (order_num == 9) printf("opcode     ");
    else if (order_num == 10)printf("opcodelist ");
	// project 2
	else if (order_num == 11)printf("assemble   ");
	else if (order_num == 12)printf("type       ");
	else if (order_num == 13)printf("symbol     ");
	// project 3
	else if (order_num == 14)printf("progaddr   ");
	else if (order_num == 15)printf("loader     ");
	else if (order_num == 16)printf("bp         ");
	else if (order_num == 17)printf("run        ");

    return;
}

// free history linked list
void free_history()
{
    hptr next;

    while(1)
    {
        next = head->next;
        free(head);

        if(next == NULL)
            break;

        head = next;
    }

    return;
}


/* dump(5) : get range(start, end) and print data in memory */
int function_dump()
{
    int st, en;
    int var_num = 0;

    // if any words behind, scan them int hexadecimal
    if(rest_exist == 1)
    {
        scanf("%x", &st);

        // check if start is in range
        if(st >= memory_end || st < 0)
        {
            printf("out of range\n");

            scanf("%*[^\n]");
            scanf("%*c");

            return -1;
        }

        var_num = 1;

        char c;
        scanf("%c", &c);

        if(c == '\n') // one number (start)
        {
            en = st + 160;
            if(en > memory_end)
                en = memory_end;

            last_dump = en;
            if(last_dump == memory_end)
                last_dump = 0;    
        }
        else if(c == ',') // two numbers (start, end)
        {
            scanf("%x", &en);
            en++;
            
            var_num = 2;

            // scanf others left
            scanf("%*[^\n]");
            scanf("%*c");

            // check if start < end + 1
            if(st >= en)
            {
                printf("end must be same or bigger than start\n");
                return -1;
            }

            // check if end is in range
            if(en > memory_end)
            {
                printf("out of range\n");
                return -1;
            }

            last_dump = en;
            if(last_dump == memory_end)
                last_dump = 0;
        }
        else // input error
        {
            dump_rest();
            return -1;
        }
    }
    else // no letter behind
    {
        st = last_dump;

        en = st + 160;
        if(en > memory_end)
            en = memory_end;

        last_dump = en;
        if(last_dump == memory_end)
            last_dump = 0;
    }

    // print memory in range
    print_memory(st, en);

    // make history record
    if(var_num == 0)
        insert_history(5, "");
    else
    {
        char rest_to_word[30];

        if(var_num == 1)
            sprintf(rest_to_word, "%X", st);
        else
            sprintf(rest_to_word, "%X, %X", st, en - 1);

        insert_history(5, rest_to_word);
    }

    return 0;
}

// check range and call print function by line
void print_memory(int st, int en)
{
    for(int i = 0; i < 65536; i++)
        print_memory_by_line(i, st, en);

    return;
}

// check range and print data in one given line
void print_memory_by_line(int line, int st, int en)
{
    int line_st = line * 16;
    int line_en = line * 16 + 16;

    // check if the line is out of range
    if(en <= line_st || line_en <= st)
        return;

    // print first address
    printf("%05X ", line_st);

    // print data of line in hexadecimal
    for(int i = line_st; i < line_en; i++)
    {
        unsigned char mem = memory[line][i - line_st];

        if(st <= i && i < en)
            printf("%02X ", mem);
        else
            printf("   ");
    }

    printf("; ");

    // print data of line in character
    for(int i = line_st; i < line_en; i++)
    {
        unsigned char mem = memory[line][i - line_st];

        if(st <= i && i < en)
            if(mem >= 32 && mem <= 126)
                printf("%c", mem);
            else
                printf(".");
        else
            printf(".");
    }
    printf("\n");

    return;
}


/* edit(6) : change a given address of memory to a given value */
int function_edit()
{
    // check additional word -> if not, error
    if(rest_exist == 0)
    {
        printf("input valid order\n");
        return -1;
    }

    int pos, val;
    scanf("%x, %x", &pos, &val);

    // scan other words behind
    scanf("%*[^\n]");
    scanf("%*c");

    // check if address is in range
    if (pos < 0 || pos >= memory_end)
    {
        printf("out of range\n");
        return -1;
    }

    // check if value is in data range
    if(val < 0 || val >= 256 )
    {
        printf("input value between 00 and FF (in hexadeximal)\n");
        return -1;
    }

    memory[pos/16][pos%16] = val;

    // make history record
    char rest_to_word[30];
    sprintf(rest_to_word, "%X, %X", pos, val);
    insert_history(6, rest_to_word);

    return 0;
}


/* fill(7) : set memory from start to end by given value */
int function_fill()
{
    // check additional word -> if not, error
    if(rest_exist == 0)
    {
        printf("input valid order\n");
        return -1;
    }

    int st, en, val;
    scanf("%x, %x, %x", &st, &en, &val);
    en++;

    scanf("%*[^\n]");
    scanf("%*c");

    // check inputs are in valid range
    if(st < 0 || en < 0 ||  st >= memory_end || en > memory_end)
    {
        printf("out of range\n");
        return -1;
    }
    if(st >= en)
    {
        printf("end must be same or bigger than start\n");
        return -1;
    }
    if(val < 0 || val >= 256)
    {
        printf("input value between 0 and FF (in hexadeximal)\n");
        return -1;
    }

    // change data of memory
    for(int i = st; i < en; i++)
        memory[i/16][i%16] = val;

    // make history record
    char rest_to_word[30];
    sprintf(rest_to_word, "%X, %X, %X", st, en - 1, val);
    insert_history(7, rest_to_word);

    return 0;
}


/* reset(8) : change every data in memory to 0 */
int function_reset()
{
    // check additional word -> error
    if(rest_exist == 1)
    {
        dump_rest();
        return -1;
    }

    // set all memory to 0
    for (int i = 0; i < 65536; i++)
        for (int j = 0; j < 16; j++)
            memory[i][j] = 0;

    // make history record
    insert_history(8, "");

    return 0;
}


/* opcode(9) : get mnemonic and print its number */
int function_opcode()
{
    // check additional word -> if not, error
    if(rest_exist == 0)
    {
        printf("input valid order\n");
        return -1;
    }

    char mnemonic[10];
    scanf("%s", mnemonic);

    // dump words left in input buffer
    scanf("%*[^\n]");
    scanf("%*c");

    // change mnemonic to hash
    int index = word_to_hash(mnemonic);
    optr now = opcode[index];

    // check mnemonic in hash table
    while(1)
    {
        if(strcmp(now->name, mnemonic) == 0)
        {
            printf("opcode is %X\n", now->num);
            break;
        }
    
        // last structure has num = -1 & next = NULL
        if(now->num == -1)
        {
            printf("there is no mnemonic\n");
            return -1;
        }

        now = now->next;
    }

    // make history record
    insert_history(9, mnemonic);

    return 0;
}

// make hash table
void set_hash_table()
{
    opcode = (optr*)malloc(20 * sizeof(optr));

    // make empty hash table
    for(int i = 0; i < 20; i++)
    {
        opcode[i] = (optr)malloc(sizeof(optable));
        opcode[i]->num = -1;
        opcode[i]->next = NULL;
    }

    // open "opcode.txt" file
    FILE* fp = fopen("opcode.txt", "r");

    if(fp == NULL)
    {
        printf("cannot read opcode file\n");
        return;
    }

    int num;
    char name[10], s[5];

    // scanf "opcode.txt" file and make hash_table
    while(fscanf(fp, "%x %s %s", &num, name, s) != EOF)
    {
        int index = word_to_hash(name);

        optr now = opcode[index];

        while(1)
        {
            if(now->num == -1)
            {
                now->num = num;
                strcpy(now->name, name);
                strcpy(now->data, s);
                now->next = (optr)malloc(sizeof(optable));

                now = now->next;
                now->num = -1;
                now->next = NULL;

                break;
            }
            else
                now = now->next;
        }
    }
    return;
}

// change mnemonic to hash number
int word_to_hash(char* name)
{
    int index = 0;

    for(int i = 0; i < strlen(name); i++)
        index += name[i] - 'A';

    return index % 20;
}

// free hash table
void free_hash_table()
{
    // free every row of table (0 to 19)
    for(int i = 0; i < 20; i++)
    {
        optr now, next;
        now = opcode[i];

        while(1)
        {
            next = now->next;
            free(now);

            if(next->num == -1)
            {
                free(next);
                break;
            }
            now = next;
        }
    }

    free(opcode);

    return;
}


/* opcodelist(10) : print all mnemonic data in hash tabke */
int function_opcodelist()
{
    // check additional word -> error
    if(rest_exist == 1)
    {
        dump_rest();
        return -1;
    }

    // print every data of hash table
    for(int i = 0; i < 20; i++)
    {
        printf("%d : ", i);
        optr now = opcode[i];

        while(1)
        {
            printf("[%s,%02X]", now->name, now->num);

            now = now->next;
            if(now->num == -1)
                break;
            
            printf(" -> ");
        }

        printf("\n");
    }

    // make history record
    insert_history(10, "");

    return 0;
}

/* assemblers */

/* assemble(11) : get assembly file and make list & object files */
int function_assemble()
{
	// check additional word -> if not, error.
	if(rest_exist == 0)
	{
		printf("input valid order\n");
		return -1;
	}

	// get name of file
	char filename[80];
	scanf("%s", filename);
	scanf("%*[^\n]");
	scanf("%*c");

	int flen = strlen(filename);

	// check if input is .asm file
	if(strcmp(filename + flen - 4, ".asm") != 0)
	{
		printf("input assembly file(end with *.asm)\n");
		return -1;
	}

	// check if filename is available
	FILE* fp = fopen(filename, "r");
	if(fp == NULL)
	{
		printf("cannot find file\n");
		return -1;
	}

	// erase previous table make new symbol table
	free_symbol_table();
	if(make_symbol_table(filename) == -1) // make symbol table with error check
	{
		// in error case, free symbol table and exit
		free_symbol_table();
		return -1;
	}

	// make list file
	if(make_lst(filename) == -1)
	{
		free_symbol_table();
		free_relocate();
		set_relocate();
		return -1;
	}

	// make object file
	make_obj(filename);

	insert_history(11, filename);
	
	return 0;
}

/* assemble part 1 : symbol table */

// free previous symbol table before making new symbol table
void free_symbol_table()
{
	for(int i = 0; i < 26; i++)
	{
		sptr now = (top+i)->next;

		while(now != NULL)
		{
			sptr temp = now->next;
			free(now);
			now = temp;
		}

		(top+i)->next = NULL;
	}

	return;
}

// read file and make new symbol table
int make_symbol_table(char* filename)
{
	FILE* fp = fopen(filename, "r");

	char fname[10] = {0, }, ifst[10];
	int faddr;

	// read first line
	fscanf(fp, "%s %s %X", fname, ifst, &faddr); // scan "name" "start" "addr"
	fscanf(fp, "%*c");

	if(strcmp(ifst, "START") != 0)
	{
		printf("this program has no START at first line\n");
		fclose(fp);
		return -1;
	}

	int cur_line = 10;

	while(1)
	{
		char vname[10] = {0, };
		if(fscanf(fp, "%c", &vname[0]) == EOF) // read line
		{
			// if program has no END
			printf("this program has no END\n");
			fclose(fp);
			return -1;
		}

		// begin with . : remark, faddr doesn't change
		if(vname[0] == '.')
		{
			char c;
			fscanf(fp, "%c", &c);

			if(c == '\n');
			else
				fscanf(fp, "%*[^\n]%*c");
		}
        else
		{
			if(vname[0] == ' ')
				vname[0] = 0;
			else
				if(fscanf(fp, "%s", vname + 1) == EOF)
				{
					// if program has no END
					printf("this program has no END\n");
					fclose(fp);
					return -1;
				}
			
			// scanf opcode and others, change address
			int ch = scan_asm(fp, vname, &faddr, cur_line);
			
			if(ch == 1)
				break;
			else if(ch == -1)
				// already sent error message in scan_asm
				return -1;
		}

		cur_line += 5;
	}

	fclose(fp);

	return 0;
}

// get opcode & operand in one line
int scan_asm(FILE* fp, char* vname, int* faddr, int cur_line)
{
	// get word and check opcode(type)/variable
	char vopc[10];
	fscanf(fp, "%s", vopc);
	
	if(strcmp(vname, "") != 0)
		if(insert_symbol(vname, *faddr) == -1)
		{
			printf("at line %d, unvalid symbol\n", cur_line + 5);
			return -1;
		}

	// case for variables
	if (strcmp(vopc, "BYTE") == 0)
	{
		char data[10];
		fscanf(fp, "%s", data);
		fscanf(fp, "%*c");

		int dsize = strlen(data) - 3;
		if (data[0] == 'C')
			*faddr += dsize;
		else if (data[0] == 'X')
			*faddr += dsize / 2;
	}
	else if (strcmp(vopc, "WORD") == 0)
	{
		char data[10];
		fscanf(fp, "%s", data);
		fscanf(fp, "%*c");

		*faddr += 3;
	}
	else if (strcmp(vopc, "RESB") == 0)
	{
		char data[10];
		fscanf(fp, "%s", data);
		fscanf(fp, "%*c");

		int size = 0;
		for(int i = 0; i < strlen(data); i++)
		{
			size *= 10;
			size += data[i] - '0';
		}

		*faddr += size;
	}
	else if (strcmp(vopc, "RESW") == 0)
	{
		char data[10];
		fscanf(fp, "%s", data);
		fscanf(fp, "%*c");

		int size = 0;
		for(int i = 0; i < strlen(data); i++)
		{
			size *= 10;
			size += data[i] - '0';
		}

		*faddr += 3 * size;
	}
	// special case : BASE (no adress increase)
	else if (strcmp(vopc, "BASE") == 0)
		fscanf(fp, "%*[^\n]%*c");

	// special case : RSUB (no operand behind)
	else if (strcmp(vopc, "RSUB") == 0)
	{
		fscanf(fp, "%*c");

		*faddr += 3;
	}
	// special case : END (last of program)
	else if (strcmp(vopc, "END") == 0)
	{
		return 1;
	}
	// case for opcode
	else
	{
		// check opcode start with + : type 4
		int type = 0;
		if (vopc[0] == '+')
		{
			type = 4;
			strcpy(vopc, vopc + 1);
		}

		// check the opcode is in hash table and its type
		int hashnum = word_to_hash(vopc);
		optr now = opcode[hashnum];
		while (1)
		{
			// name not found, error case
			if (now == NULL)
			{
				printf("at line %d, opcode error\n", cur_line);
				fclose(fp);
				return -1;
			}

			// name found, check it's type
			if (strcmp(now->name, vopc) == 0)
			{
				// type 1~3
				if (type == 0)
					type = now->data[0] - '0';

				// type 3 with + -> 4
				else if (now->data[0] == '3');

				// type 1 or 2 with + -> error
				else
				{
					printf("at line %d, format error\n", cur_line);
					fclose(fp);
					return -1;
				}

				*faddr += type;
				break;
			}

			now = now->next;
		}

		// dump rest data
		if(type == 1) // type 1 get '\n'
			fscanf(fp, "%*c");
		else // other types
			fscanf(fp, "%*[^\n]%*c");
	}

	return 0;
}

// insert variable name to symbol table
int insert_symbol(char* vname, int faddr)
{
	if(vname[0] < 'A' || vname[0] > 'Z')
		return -1;

	sptr now = top + vname[0] - 'A';
	sptr prev = now;

	while(1)
	{
		// vname is faster : insert vname between prev & now
		if(strcmp(vname, now->name) == -1)
		{
			sptr new = (sptr)malloc(sizeof(smbtable));
			strcpy(new->name, vname);
			new->addr = faddr;

			prev->next = new;
			new->next = now;

			break;
		}
		// 2 same name, error
		else if(strcmp(vname, now->name) == 0)
		{
			return -1;
		}
		// vname is slower & last : make vname come last
		else if(now->next == NULL)
		{
			sptr new = (sptr)malloc(sizeof(smbtable));
			strcpy(new->name, vname);
			new->addr = faddr;

			now->next = new;
			new->next = NULL;

			break;
		}
		// vname is slower & now last : can search more
		else
		{
			prev = now;
			now = now->next;
		}
	}

	return 0;
}

/* assemble part 2 : list file */

// make list file
int make_lst(char* filename)
{
	FILE* fp = fopen(filename, "r");

	char fname[10], fstart[10], saddr[10];
	fscanf(fp, "%s %s %s%*c", fname, fstart, saddr);
	
	strcpy(asmname, fname);
	
	reset_asm_line();

	// object code of line
	char obli[20] = {0, };

	// start address(str_hex to num)
	int faddr = 0;
	for(int i = 0; i < strlen(saddr); i++)
	{
		faddr *= 16;
		faddr += saddr[i] - '0';
	}
	asmst = faddr;

	// make name of list file
	char lstname[10];
	strcpy(lstname, filename);
	strcpy(lstname + strlen(lstname) - 3, "lst");

	FILE* fw = fopen(lstname, "w");

	fprintf(fw, "%-8s %-4s %8s %-15s %14s %s\n\n", "Line", "Loc", "", "Source statement", "", "Object Code");
	fprintf(fw, "%4d     %04X %4s %-10s %-10s %X\n", 5, faddr, "", fname, fstart, faddr);

	int base = -4096; // after BASE comes, saves its register

	// print by line
	for(int i = 10; ; i += 5)
	{
		fprintf(fw, "%4d     ", i);

		char vname[10] = {0, };
		fscanf(fp, "%[^\n' ']", vname);

		// line with remark
		if(vname[0] == '.')
		{
			char inp[100];
			fscanf(fp, "%c", &inp[0]);

			if(inp[0] == '\n')
				fprintf(fw, "%10s.", "");
			else
			{
				fscanf(fp, "%[^\n]%*c", inp);
				fprintf(fw, "%10s.%5s%s", "", "", inp);
			}
			fprintf(fw, "\n");
		}
		// line without remark (variable / opcode / base,end)
		else
		{
			// get word and check opcode(type)/variable
			char vopc[10];
			fscanf(fp, "%s", vopc);

			// variable
			if(strcmp(vopc, "BYTE") == 0)
			{
				char data[10];
				fscanf(fp, "%s%*c", data);

				fprintf(fw, "%04X      %-10s %-10s %-15s", faddr, vname, vopc, data);

				if(data[0] == 'C')
				{
					for(int i = 2; i < strlen(data) - 1; i++)
						sprintf(obli + 2*i - 4, "%02X", data[i]);
					fprintf(fw, "%s", obli);
					insert_asm_line(faddr, obli);

					faddr += strlen(data) - 3;
				}
				else if(data[0] == 'X')
				{
					for(int i = 2; i < strlen(data) - 1; i++)
						sprintf(obli + i - 2, "%c", data[i]);
					fprintf(fw, "%s", obli);
					insert_asm_line(faddr, obli);

					faddr += (strlen(data) - 3) / 2;
				}
			}
			else if(strcmp(vopc, "WORD") == 0)
			{
				char data[10];
				fscanf(fp, "%s%*c", data);

				fprintf(fw, "%04X      %-10s %-10s %-15s", faddr, vname, vopc, data);
			
				faddr += 3;

				int dec = 0;
				for(int i = 0; i < strlen(data); i++)
				{
					dec *= 10;
					dec += data[i] - '0';
				}

				sprintf(obli, "%06X", dec);
				fprintf(fw, "%s", obli);
				insert_asm_line(faddr, obli);
			}
			else if(strcmp(vopc, "RESB") == 0)
			{
				char data[10];
				fscanf(fp, "%s%*c", data);

				// str_dec to num
				int dec = 0;
				for(int i = 0; i < strlen(data); i++)
				{
					dec *= 10;
					dec += data[i] - '0';
				}

				fprintf(fw, "%04X      %-10s %-10s %d", faddr, vname, vopc, dec);

				faddr += dec;
			}
			else if(strcmp(vopc, "RESW") == 0)
			{
				char data[10];
				fscanf(fp, "%s%*c", data);
				
				// str_dec to num
				int dec = 0;
				for(int i = 0; i < strlen(data); i++)
				{
					dec *= 10;
					dec += data[i] - '0';
				}

				fprintf(fw, "%04X      %-10s %-10s %d", faddr, vname, vopc, dec);

				faddr += 3 * dec;
			}
			// special case : BASE
			else if(strcmp(vopc, "BASE") == 0)
			{
				char data[10];
				fscanf(fp, "%s%*c", data);
				fprintf(fw, "%20s %-10s %s", "", vopc, data);

				base = symbol_address(data);
			}
			// special case : RSUB (no operands behind)
			else if(strcmp(vopc, "RSUB") == 0)
			{
				fscanf(fp, "%*c");

				strcpy(obli, "4F0000");
				fprintf(fw, "%04X %6s %-8s %-8s %16s %s", faddr, "", vname, vopc, "", obli);
				insert_asm_line(faddr, obli);

				faddr += 3;
			}
			// special case : END (finish program)
			else if(strcmp(vopc, "END") == 0)
			{
				asmen = faddr;

				char data[10];
				fscanf(fp, "%s%*[^\n]", data);

				fprintf(fw, "%10s%-10s %-10s\n", "", vopc, data);
				fclose(fp);
				break;
			}
			// operand
			else
			{
				// check opcode start with '+'
				int type = 0;
				if(vopc[0] == '+')
				{	
					type = 4;
					strcpy(vopc, vopc + 1);
				}

				// check type of opcode
				int hashnum = word_to_hash(vopc);
				optr now = opcode[hashnum];

				while(1)
				{
					// name found, check type
					if(strcmp(now->name, vopc) == 0)
					{
						if(type == 0)
							type = now->data[0] - '0';

						break;
					}

					now = now->next;
				}

				fprintf(fw, "%04X      %-10s ", faddr, vname);

				// format 1
				if(type == 1)
				{
					fscanf(fp, "%*c");
					fprintf(fw, "%-10s ", now->name);
					
					sprintf(obli, "%02X", now->num);
					fprintf(fw, "%14s %s", "", obli);
					insert_asm_line(faddr, obli);
				}
				// format 2
				else if(type == 2)
				{
					char r1[5], r2[5] = {0, }, r3[10];
					fscanf(fp, "%s", r1);

					strcpy(r3, r1);

					// case with , : two registers
					if(r1[strlen(r1)-1] == ',')
					{
						r1[strlen(r1)-1] = '\0';
						fscanf(fp, "%s", r2);
						strcat(r3, " ");
						strcat(r3, r2);
					}
					
					fscanf(fp, "%*c");

					sprintf(obli, "%02X%1x%1X", now->num, reg(r1), reg(r2));
					fprintf(fw, "%-10s %-14s %s", now->name, r3, obli);
					insert_asm_line(faddr, obli);
				}
				// format 3 & 4
				else
				{
					char voper[10];
					fscanf(fp, "%s", voper);
					if(type == 3)
						fprintf(fw, "%-10s ", now->name);
					else
						fprintf(fw, "+%-9s ", now->name);

					int obco = now->num / 4 * 4; // make object code
					int xbpe = 0;

					// check x
					char x;
					fscanf(fp, "%c", &x);

					if(x == '\n')
						x = 0;
					else
					{
						fscanf(fp, "%*[^\n]%*c");
						voper[strlen(voper) - 1] = '\0';
						x = 1;
					}

					xbpe += 8 * x;

					int ta; // target address
					int ifnum = 0;

					// immediate addressing(get address) ni = 01
					if(voper[0] == '#')
					{
						obco += 1;
						ta = find_address(voper + 1, &ifnum);
					}
					// indirect addressing(get and go to data) ni = 10
					else if(voper[0] == '@')
					{
						obco += 2;
						ta = find_address(voper + 1, &ifnum);
					}
					// normal addressing ni = 11
					else
					{
						obco += 3;
						ta = find_address(voper, &ifnum);
					}

					int pc = faddr + 3;

					// type 4 : bp = 00, e = 1
					if(type == 4)
					{
						xbpe += 1;

						// insert relocation for object file, not immediate(i=1)
						if(obco % 4 == 3)
							insert_relocate(faddr + 1, 5);
					}
					// case with number bp = 00
					if(ifnum);
					// using program counter
					else if(pc - 2048 <= ta && ta < pc)
					{
						ta -= pc;
						ta += 4096;
						xbpe += 2;
					}
					else if(pc <= ta && ta < pc + 2048)
					{
						ta -= pc;
						xbpe += 2;
					}
					// using base address
					else if(base <= ta && ta < base + 4096)
					{
						ta -= base;
						xbpe += 4;
					}
					// range error at format 3
					else if(type == 3 && ta >= 4096)
					{
						printf("at line %d : out of range\n", i);
						return -1;
					}
					// range error at format 4
					else if(type == 4 && ta >= 1<<20)
					{
						printf("at line %d : out of range\n", i);
						return -1;
					}

					if(x)
						strcat(voper, ", X");

					if(type == 4)	sprintf(obli, "%02X%1X%05X", obco, xbpe, ta);
					else			sprintf(obli, "%02X%1X%03X", obco, xbpe, ta);
					fprintf(fw, "%-14s %s", voper, obli);
					insert_asm_line(faddr, obli);
				}

				faddr += type;
			}

			fprintf(fw, "\n");
		}

	}

	if(base == -4096) // base undeclared : send warning message
		printf("WARNING : BASE undeclared\n");

	fclose(fw);
	return 0;
}

// malloc head pointer of asmline linked list
void set_asm_line()
{
	lhead = (lptr)malloc(sizeof(asmline));

	lhead->addr = 0;
	strcpy(lhead->obco, "");
	lhead->next = NULL;

	lnow = lhead;

	return;
}

// free asmline except for head
void reset_asm_line()
{
	lnow = lhead->next;

	while(lnow != NULL)
	{
		lptr lnext = lnow->next;
		free(lnow);
		lnow = lnext;
	}

	lhead->next = NULL;
	lnow = lhead;

	return;
}

// insert address & object code in asmline
void insert_asm_line(int addr, char* obco)
{
	if(strcmp(obco, "") == 0)
		return;

	lnow->next = (lptr)malloc(sizeof(asmline));
	lnow = lnow->next;

	lnow->addr = addr;
	strcpy(lnow->obco, obco);
	lnow->next = NULL;

	return;
}

// free all asm line linked list
void free_asm_line()
{
	reset_asm_line();
	free(lhead);

	return;
}

// get symbol name and find adress
int symbol_address(char* name)
{
	if(name[0] - 'A' < 0 || name[0] - 'A' > 25)
		return 0;

	sptr now = top + name[0] - 'A';

	while(now != NULL)
	{
		if(strcmp(now->name, name) == 0)
			return now->addr;
		
		now = now->next;
	}

	return 0;
}

// retur number of register 
int reg(char* name)
{
	if		(strcmp(name, "A")  == 0)	return 0;
	else if (strcmp(name, "X")  == 0)	return 1;
	else if (strcmp(name, "L")  == 0)	return 2;
	else if (strcmp(name, "PC") == 0)	return 8;
	else if (strcmp(name, "SW") == 0)	return 9;
	else if (strcmp(name, "B")  == 0)	return 3;
	else if (strcmp(name, "S")  == 0)	return 4;
	else if (strcmp(name, "T")  == 0)	return 5;
	else if (strcmp(name, "F")  == 0)	return 6;

	return 0;
}

// get name(or number) and retur irs address
int find_address(char* name, int* ifnum)
{
	int ret = 0;

	// symbol case
	if(ret = symbol_address(name))
		return ret;
	
	// other case : number
	else
	{
		for(int i = 0; i < strlen(name); i++)
		{
			ret *= 10;
			ret += name[i] - '0';
		}

		*ifnum = 1;

		return ret;
	}
}

/* assemble part 3 : object file */

// make object file
int make_obj(char* filename)
{
	// make name of list file
	char objname[10];
	strcpy(objname, filename);
	strcpy(objname + strlen(objname) - 3, "obj");

	FILE* fw = fopen(objname, "w");

	// header record
	fprintf(fw, "H%-6s%06X%06X\n", asmname, asmst, asmen);

	// text record
	print_obli(fw);

	// relocation record
	print_relo(fw);

	free_relocate();
	set_relocate();

	// end record
	fprintf(fw, "E%06X\n", asmst);

	fclose(fw);

	return 0;
}

void print_obli(FILE* fw)
{
	int line_buf = 30;

	lptr lread = lhead->next;
	if(lread == NULL)
		return;

	int saddr = 0, faddr = 0;

	while(lread != NULL)
	{
		char fr[10] = {0, };
		sprintf(fr,"T%06X", lread->addr);

		char bc[100] = {0, };

		saddr = lread->addr;
		faddr = saddr - 1;

		while(1)
		{
			faddr += strlen(lread->obco) / 2;

			// last of object code
			if(lread->next == NULL)
			{
				sprintf(fr + 7, "%02X", faddr - saddr + 1);
				strcat(bc, lread->obco);

				lread = lread->next;
				break;
			}
			// variable comes next
			else if(faddr + 1 != lread->next->addr)
			{
				sprintf(fr + 7, "%02X", faddr - saddr + 1);
				strcat(bc, lread->obco);

				lread = lread->next;
				break;
			}
			// reached buffer
			else if(faddr == saddr + line_buf)
			{
				sprintf(fr + 7, "%02X", faddr - saddr + 1);
				strcat(bc, lread->obco);

				lread = lread->next;
				break;
			}
			else if(faddr > saddr + line_buf)
			{
				faddr -= strlen(lread->obco) / 2;
				sprintf(fr + 7, "%02X", faddr - saddr + 1);
				break;
			}

			strcat(bc, lread->obco);
			lread = lread->next;
		}

		fprintf(fw, "%s%s\n", fr, bc);
	}

	return;
}

void print_relo(FILE* fw)
{
	rptr temp = rhead->next;

	while(temp != NULL)
	{
		fprintf(fw, "M%06X%02X\n", temp->addr, temp->leng);
		temp = temp->next;
	}
	return;
}

void set_relocate()
{
	rhead = (rptr)malloc(sizeof(relocate));
	
	rhead->addr = -1;
	rhead->leng = 0;
	rhead->next = NULL;

	rtail = rhead;

	return;
}

void insert_relocate(int addr, int leng)
{
	rtail->next = (rptr)malloc(sizeof(relocate));
	rtail = rtail->next;

	rtail->addr = addr;
	rtail->leng = leng;
	rtail->next = NULL;

	return;
}

void free_relocate()
{
	rptr now = rhead;

	while(now != NULL)
	{
		rptr new = now->next;
		free(now);
		now = new;
	}

	return;
}


/* type(12) : print contents of the file */
int function_type()
{
	// check additional word -> if not, error.
	if(rest_exist == 0)
	{
		printf("input valid order\n");
		return -1;
	}

	char filename[80];
	scanf("%s", filename);
	scanf("%*[^\n]");
	scanf("%*c");

	// open file
	FILE* fp = fopen(filename, "r");
	if(fp == NULL)
	{
		printf("cannot find file\n");
		return -1;
	}

	// check if directory
	struct stat buf;
	lstat(filename, &buf);
	if(S_ISDIR(buf.st_mode))
	{
		printf("given name is directory\n");
		fclose(fp);
		return -1;
	}

	insert_history(12, filename);

	while(1)
	{
		char c;
		if(fscanf(fp, "%c", &c) == EOF)
			break;

		printf("%c", c);
	}

	fclose(fp);

	return 0;
}


/* symbol(13) : print symbol table of last assembled file */
int function_symbol()
{
	if(rest_exist == 1)
	{
		dump_rest();
		return -1;
	}

	int ch = 0;
	for(int i = 0; i < 26; i++)
	{
		sptr now = (top+i)->next;

		while(now != NULL)
		{
			ch = 1;
			printf("%-8s%-8s%04X\n", "\0", now->name, now->addr);

			now = now->next;
		}
	}

	if(ch == 0)
	{
		printf("no assemble yet or last assemble was error!\n");
		return -1;
	}

	insert_history(13, "");

	return 0;
}

// make top of symbol table(linked list) at start of program
void set_symbol_top()
{
	top = (sptr)malloc(26*sizeof(smbtable));

	for(int i = 0; i < 26; i++)
	{
		strcpy((top+i)->name, "");
		(top+i)->addr = -1;
		(top+i)->next = NULL;
	}
	return;
}

// free top of symbol table(linked list) at end of program
void free_symbol_top()
{
	free_symbol_table();
	free(top);
	return;
}


/* progaddr(14) : set program load address */
int function_progaddr()
{
	if(rest_exist == 0)
	{
		printf("input valid order\n");
		return -1;
	}

	char pa[10];
	scanf("%[^\n]%*c", pa);

	int temp = 0;
	for(int i = 0; i < strlen(pa); i++)
	{
		temp *= 16;

		if('A' <= pa[i] && pa[i] <= 'F')
			temp += pa[i] - 'A' + 10;
		else if('0' <= pa[i] && pa[i] <= '9')
			temp += pa[i] - '0';
		else
		{
			printf("address number error!\n");
			return -1;
		}
	}

	progaddr = temp;

	insert_history(14, pa);

	return 0;
}

void set_progaddr()
{
	progaddr = 0;
	return;
}


/* loader(15) : link and load object files */
int function_loader()
{
	if(rest_exist == 0)
	{
		printf("input valid order\n");
		return -1;
	}

	int files = 0;
	char filename[3][20];
	FILE* fp;

	// read filenames (maximum 3)
	for(files = 0; files < 3; files++)
	{
		char check;
		scanf("%s", filename[files]);
		scanf("%c", &check);

		// check file exists
		fp = fopen(filename[files], "r");
		if(fp == NULL)
		{
			printf("file open error\n");
			return -1;
		}
		fclose(fp);

		if(check == '\n')
		{
			files++;
			break;
		}
	}

	loader_part1(filename, files);
	loader_part2(filename, files);
	print_estab(files);

	// set registers
	for(int i = 0; i < 10; i++)
		regis[i] = 0;
	regis[reg("PC")] = progaddr;

	for(int i = 0; i < files; i++)
		regis[reg("L")] += proglen[i];

	char tot[100] = {0, };
	for(int i = 0; i < files; i++)
	{
		strcat(tot, filename[i]);
		strcat(tot, " ");
	}
	insert_history(15, tot);

	return 0;
}

void loader_part1(char filename[3][20], int files)
{
	FILE* fp;

	int curad = progaddr;
	for(int i = 0; i < files; i++)
	{
		fp = fopen(filename[i], "r");

		progst[i] = curad;

		int index = 1;
		int init = 0;
		int len = 0;

		// read file
		while(1)
		{
			char st[300] = {0, };
			fgets(st, 300, fp);

			// check END
			if(st[0] == 'E')
				break;
			// header record
			else if(st[0] == 'H')
			{
				sscanf(st + 1, "%6s", estab[i][0].name);
				estab[i][0].addr = curad;
				
				sscanf(st + 7, "%6X%6X", &init, &len);
			}
			// define record
			else if(st[0] == 'D')
				for(int j = 1; j < strlen(st) - 1; j += 12)
				{
					sscanf(st + j, "%6s", estab[i][index].name);
					sscanf(st + j + 6, "%6X", &estab[i][index].addr);
					estab[i][index].addr += curad;

					index++;
				}
		}

		curad += len;
		esnum[i] = index;
		proglen[i] = len;

		fclose(fp);
	}

	return;
}

void loader_part2(char filename[3][20], int files)
{
	for(int fnum = 0; fnum < files; fnum++)
	{
		int csaddr = progst[fnum];
		FILE* fp = fopen(filename[fnum], "r");

		exsym reftab[100];
		strcpy(reftab[0].name, estab[fnum][0].name);
		reftab[0].addr = estab[fnum][0].addr;
		int refnum = 1;

		while(1)
		{
			char st[300];
			fgets(st, 300, fp);

			// reference record
			if(st[0] == 'R')
			{
				for(refnum = 1; 8*refnum - 5 < strlen(st) - 1; refnum++)
				{
					sscanf(st + 8*refnum - 5, "%6s", reftab[refnum].name);
					reftab[refnum].addr = find_addr(reftab[refnum].name);
				}
			}
			// text record
			else if(st[0] == 'T')
			{
				int naddr;
				sscanf(st + 1, "%6X", &naddr);
				naddr += csaddr;

				int linelen;
				sscanf(st + 7, "%2X", &linelen);

				for(int i = 0; i < linelen; i++)
				{
					int posy = (naddr + i) / 16;
					int posx = (naddr + i) % 16;

					int num;
					sscanf(st + 9 + 2*i, "%2X", &num);

					memory[posy][posx] = (unsigned char)num;
				}
			}
			// modification record
			else if(st[0] == 'M')
			{
				int modiaddr;
				sscanf(st + 1, "%6X", &modiaddr);
				modiaddr += progst[fnum];

				int modibit;
				sscanf(st + 7, "%2X", &modibit);

				char posneg;
				sscanf(st + 9, "%c", &posneg);

				int modiref;
				sscanf(st + 10, "%2X", &modiref);

				int adaddr = reftab[modiref-1].addr;
				if(posneg == '-')
					adaddr = (1<<24) - adaddr;

				add_modi(modiaddr, adaddr);
			}
			// end record
			else if(st[0] == 'E')
			{
				sscanf(st + 1, "%6X", &execst[files]);
				execst[files] += progst[files];
				break;
			}
		}
	}

	return;
}

int find_addr(char refs[20])
{
	for(int tnum = 0; tnum < 3; tnum++)
		for(int i = 0; i < esnum[tnum]; i++)
			if(strcmp(refs, estab[tnum][i].name) == 0)
				return estab[tnum][i].addr;
	
	printf("cannot find %s\n", refs);
	return 0;
}

void add_modi(int modiaddr, int adaddr)
{
	unsigned char uc[3];
	uc[0] = adaddr / (1<<16);
	uc[1] = (adaddr / (1<<8)) % (1<<8);
	uc[2] = adaddr % (1<<8);

	int carry = 0;
	for(int i = 2; i >= 0; i--)
	{
		int maddr = modiaddr + i;
		int posy = maddr / 16;
		int posx = maddr % 16;
		
		if(memory[posy][posx] + uc[i] + carry >= 256)
		{
			memory[posy][posx] += uc[i] + carry - 256;
			carry = 1;
		}
		else
		{
			memory[posy][posx] += uc[i] + carry;
			carry = 0;
		}
	}

	return;
}

void print_estab(int files)
{
	printf("control symbol address length\n");
	printf("section name\n");
	printf("--------------------------------\n");

	for(int i = 0; i < files; i++)
	{
		printf("%-6s  %6s  %04X   %04X\n", estab[i][0].name, "", estab[i][0].addr, proglen[i]);

		for(int x = 1; x < esnum[i]; x++)
			printf("%6s  %6s  %04X\n", "", estab[i][x].name, estab[i][x].addr);
	}
	printf("--------------------------------\n");
	
	int totlen = 0;
	for(int i = 0; i < files; i++)
		totlen += proglen[i];

	printf("          total length %04X\n", totlen);

	return;
}


/* bp(16) : set breakpoint */
int function_bp()
{
	// bp
	if(rest_exist == 0)
	{
		printf("\tbreakpoint\n");
		printf("\t----------\n");

		for(int y = 0; y < 65536; y++)
			for(int x = 0; x < 16; x++)
				if(bp[y][x]) printf("\t%X\n", y*16 + x);

		insert_history(16, "");
		return 0;
	}

	char order[20] = {0, };
	scanf("%[^\n]", order);
	scanf("%*c");

	// too long
	if(strlen(order) > 10)
	{
		printf("wrong address\n");
		return -1;
	}

	// bp clear
	if(strcmp(order, "clear") == 0)
	{
		printf("\t[ok] clear breakpoints\n");
		set_bp();

		insert_history(16, order);
		return 0;
	}

	// bp address
	int newbp = 0;
	for(int i = 0; i < 20; i++)
	{
		if(order[i] >= '0' && order[i] <= '9')
		{
			newbp *= 16;
			newbp += order[i] - '0';
		}
		else if(order[i] >= 'A' && order[i] <= 'F')
		{
			newbp *= 16;
			newbp += order[i] - 'A' + 10;
		}
		else if(order[i] >= 'a' && order [i] <= 'f')
		{
			newbp *= 16;
			newbp += order[i] - 'a' + 10;
		}
		else if(order[i] == '\0')
			break;
		else
		{
			printf("wrong address\n");
			return -1;
		}
	}

	if(newbp >= 0 && newbp < memory_end)
		bp[newbp/16][newbp%16] = 1;
	else
	{
		printf("address out of range\n");
		return -1;
	}

	printf("\t[ok] create breakpoint %X\n", newbp);
	bpnum++;

	insert_history(16, order);

	return 0;
}

void set_bp()
{
	for(int y = 0; y < 65536; y++)
		for(int x = 0; x < 16; x++)
			bp[y][x] = 0;

	bpnum = 0;
	return;
}


/* run(17) : run program */
int function_run()
{
	if(rest_exist == 1)
	{
		dump_rest();
		return -1;
	}

	int endaddr = progst[0] + proglen[0];

	while(1)
	{
		int curaddr = regis[reg("PC")];

		// execute opcode
		exec_opcode(curaddr);

		int nextaddr = regis[reg("PC")];

		// check breakpoint
		if(bp[nextaddr/16][nextaddr%16] == 1)
		{
			printf("A : %06X  X : %06X\n", regis[reg("A")], regis[reg("X")]);
			printf("L : %06X PC : %06X\n", regis[reg("L")], regis[reg("PC")]);
			printf("B : %06X  S : %06X\n", regis[reg("B")], regis[reg("S")]);
			printf("T : %06X\n", regis[reg("T")]);
			printf("            Stop at checkpoint[%X]\n", nextaddr);
			break;
		}
		// check program ends
		if(nextaddr >= endaddr)
		{
			printf("A : %06X  X : %06X\n", regis[reg("A")], regis[reg("X")]);
			printf("L : %06X PC : %06X\n", regis[reg("L")], regis[reg("PC")]);
			printf("B : %06X  S : %06X\n", regis[reg("B")], regis[reg("S")]);
			printf("T : %06X\n", regis[reg("T")]);
			printf("            End Program\n");
			break;
		}
	}

	insert_history(17, "");

	return 0;
}

void exec_opcode(int curaddr)
{
	int posy = curaddr / 16;
	int posx = curaddr % 16;

	int bit1 = memory[posy][posx];

	find_format(curaddr, bit1);
	
	return;
}

int find_format(int curaddr, int bit1)
{
	switch(bit1 / 16)
	{
		case 12:
		case 15:
			regis[reg("PC")] += 1;
			format1(bit1);
			break;
		case 9:
		case 10:
		case 11:
			regis[reg("PC")] += 2;
			format2(curaddr, bit1);
			break;
		default:
			regis[reg("PC")] += 3;
			format34(curaddr, bit1);
	}

	return 0;
}

void format1(int bit1)
{
	return;
}

void format2(int curaddr, int bit1)
{
	int bit2 = memory[(curaddr+1)/16][(curaddr+1)%16];

	int r1 = bit2 / 16;
	int r2 = bit2 % 16;

	if(bit1 == 0xA0) // COMPR
	{
		int regr1 = regis[r1];
		int datar1 = memory[regr1/16][regr1%16];

		int regr2 = regis[r2];
		int datar2 = memory[regr2/16][regr2%16];

		if(datar1 == datar2)
			regis[reg("SW")] = '=';
		else if(datar1 < datar2)
			regis[reg("SW")] = '<';
		else
			regis[reg("SW")] = '>';
	}
	if(bit1 == 0xB4) // CLEAR
	{
		regis[r1] = 0;
	}
	if(bit1 == 0xB8) // TIXR
	{
		regis[reg("X")]++;

		int regx = regis[reg("X")];
		int datax = memory[regx/16][regx%16];

		int regr = regis[r1];
		int datar = memory[regr/16][regr%16];

		if(datax == datar)
			regis[reg("SW")] = '=';
		else if(datax < datar)
			regis[reg("SW")] = '<';
		else
			regis[reg("SW")] = '>';
	}

	return;
}

void format34(int curaddr, int bit1)
{
	int op = (bit1 / 4) * 4;

	int nformat = 3;

	int bit2 = memory[(curaddr+1)/16][(curaddr+1)%16];
	int bit3 = memory[(curaddr+2)/16][(curaddr+2)%16];
	int bit4 = memory[(curaddr+3)/16][(curaddr+3)%16];

	// check e bit(format 4)
	if(bit2 & (1<<4))
	{
		nformat = 4;
		regis[reg("PC")]++;
	}

	int ta;

	if(nformat == 3)
	{
		ta = (bit2 % 16)<<8;
		ta += bit3;
	}
	if(nformat == 4)
	{
		ta = (bit2 % 16)<<16;
		ta += bit3<<8;
		ta += bit4;
	}

	if(bit2 & (1<<6)) // b bit
		ta += regis[reg("B")];

	if(bit2 & (1<<5)) // p bit
	{
		if(ta >= 2048)
			ta -= 4096;
		ta += regis[reg("PC")];
	}

	if(bit2 & (1<<7)) // x bit
		ta += regis[reg("X")];

	//check ni bit
	int tb = 0;

	// immediate addressing. ni = 01
	if(bit1 % 4 == 1)
		tb = ta;
	// simple addressing. ni = 11
	if(bit1 % 4 == 3)
	{
		tb = ta;

		int d1 = memory[ta/16][ta%16];
		int d2 = memory[(ta+1)/16][(ta+1)%16];
		int d3 = memory[(ta+2)/16][(ta+2)%16];
		ta = (d1<<16) + (d2<<8) + d3;
	}
	// indirect addressing. ni = 10
	if(bit1 % 4 == 2)
	{
		int d1 = memory[ta/16][ta%16];
		int d2 = memory[(ta+1)/16][(ta+1)%16];
		int d3 = memory[(ta+2)/16][(ta+2)%16];
		ta = (d1<<16) + (d2<<8) + d3;
		
		tb = ta;

		d1 = memory[ta/16][ta%16];
		d2 = memory[(ta+1)/16][(ta+1)%16];
		d3 = memory[(ta+2)/16][(ta+2)%16];
		ta = (d1<<16) + (d2<<8) + d3;
	}

	if(op == 0x00) // LDA
		regis[reg("A")] = ta;
	if(op == 0x0C) // STA
	{
		int data = regis[reg("A")];

		memory[tb/16][tb%16] = data / (1<<16);
		memory[(tb+1)/16][(tb+1)%16] = data / (1<<8) % (1<<8);
		memory[(tb+2)/16][(tb+2)%16] = data % (1<<8);
	}
	if(op == 0x10) // STX
	{
		int data = regis[reg("X")];

		memory[tb/16][ta%16] = data / (1<<16);
		memory[(tb+1)/16][(tb+1)%16] = data / (1<<8) % (1<<8);
		memory[(tb+2)/16][(tb+2)%16] = data % (1<<8);
	}
	if(op == 0x14) // STL
	{
		int data = regis[reg("L")];

		memory[tb/16][tb%16] = data / (1<<16);
		memory[(tb+1)/16][(tb+1)%16] = data / (1<<8) % (1<<8);
		memory[(tb+2)/16][(tb+2)%16] = data % (1<<8);
	}
	if(op == 0x28) // COMP
	{
		if(regis[reg("A")] == ta)
			regis[reg("SW")] = '=';
		if(regis[reg("A")] < ta)
			regis[reg("SW")] = '<';
		if(regis[reg("A")] > ta)
			regis[reg("SW")] = '>';
	}
	if(op == 0x30) // JEQ
	{
		if(regis[reg("SW")] == '=')
			regis[reg("PC")] = tb;
	}
	if(op == 0x38) // JLT
	{
		if(regis[reg("SW")] == '<')
			regis[reg("PC")] = tb;
	}
	if(op == 0x3C) // J
	{
		regis[reg("PC")] = tb;
	}
	if(op == 0x48) // JSUB
	{
		regis[reg("L")] = regis[reg("PC")];
		regis[reg("PC")] = tb;
	}
	if(op == 0x4C) // RSUB
	{
		regis[reg("PC")] = regis[reg("L")];
	}
	if(op == 0x50) // LDCH
	{
		regis[reg("A")] = ((regis[reg("A")]>>8)<<8) + ta / (1<<16);
	}
	if(op == 0x54) // STCH
	{
		memory[tb/16][tb%16] = regis[reg("A")] % (1<<8);
	}
	if(op == 0x68) // LDB
		regis[reg("B")] = ta;
	if(op == 0x74) // LDT
		regis[reg("T")] = ta;

	if(op == 0xD8) // RD
	{
		regis[reg("A")] = 0;
	}
	if(op == 0xDC) // WD
	{
		// do nothing
	}
	if(op == 0xE0) // TD
		regis[reg("SW")] = '<';

	return;
}


/* main : initialize history & hash table and recursively get order and excute it */
int main()
{
    // initialize history and hash table
    set_history();
    set_hash_table();
	set_symbol_top();
	set_asm_line();
	set_relocate();
	// initialize program load address and bp numbers
	set_progaddr();
	set_bp();

    while(1)
    {
        printf("sicsim> ");

        char order[15];

        if(scan_order(order)) // if wrong order, return 1.
        {
            printf("Please input valid order\n");
            continue;
        }

        // read order number & link to each order
        int error_check = order_link(order);

        if(error_check == 3) // case of quit order
            break;
    }

    // free history linked list and opcode hash table
    free_history();
    free_hash_table();
	free_symbol_top();
	free_asm_line();
	free_relocate();

    return 0;
}
