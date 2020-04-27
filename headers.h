#ifndef PDPH
#define PDPH

#include <stdio.h>
#include <stdarg.h>

#define MEMSIZE (64 * 1024)
#define pc reg[7]
#define NO_PARAM 0
#define HAS_SS 1
#define HAS_DD 2
#define REG 1456
#define MEM 2345
#define odata 0177566

typedef unsigned char byte; //8 bit
typedef unsigned short int word; // 16 bit
typedef word Adress; 

void b_write(Adress adr, byte b); // пишем байт b по адресу adr
byte b_read (Adress adr); // читаем байт по адресу adr;
void w_write(Adress adr, word w); // пишем слово w по адресу adr
word w_read (Adress adr); // читаем слово по адресу adr
void run();

struct mr {
	word adr; // адрес аргумента
	word val; // значение аргумента
	word space; // адрес mem[] или reg[]
}ss,dd;

struct mr ss, dd;

void do_mov();
void do_halt();
void do_add();
void do_nothing();
struct mr get_mr(word w);

word mem[MEMSIZE];
word reg[8]; //регистры R0 .. R7

typedef struct {
	word mask;
	word opcode;
	char * name;
	void (*do_func)(void);
	char params;
} Command;
extern const Command cmd[];
/*Command cmd[] = {
	{0170000, 0000000, "HALT",	do_halt,	NO_PARAM},
	{0170000, 0060000, "ADD",	do_add,		HAS_SS|HAS_DD}, 
	{0170000, 0010000, "MOV",	do_mov,		HAS_SS|HAS_DD}};
*/

void trace(const char *  format, ...);

#endif

















