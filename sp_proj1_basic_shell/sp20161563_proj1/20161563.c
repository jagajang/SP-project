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
    // get the number of order(1 to 10)
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

    return error_check;
    /*
        error_check == -1 -> real error
        error_check == 0 -> no error
        error_check == 3 -> quit order
    */
}

// change valid order to number 1 to 10. in error case, return 0
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
            printf("\t%s", inp->d_name);

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
        dump_rest();
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
    if(val < 32 || val > 126 )
    {
        printf("input value between 20 and 7E (in hexadeximal)\n");
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
    if(val < 32 || val > 126)
    {
        printf("input value between 20 and 7E (in hexadeximal)\n");
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

/* main : initialize history & hash table and recursively get order and excute it */
int main()
{
    // initialize history and hash table
    set_history();
    set_hash_table();

    while(1)
    {
        printf("sicsim>");

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

    return 0;
}
