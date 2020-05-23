#ifndef PDPH
#define PDPH

#include <stdio.h>
#include <stdarg.h>

typedef unsigned char byte; //8 bit
typedef unsigned short int word; // 16 bit
typedef word Adress;

#define MEMSIZE (64 * 1024)
#define pc reg[7]
#define sp reg[6]
#define NO_PARAM 0
#define HAS_SS 1
#define HAS_DD 2
#define HAS_NN 4
#define HAS_XX 8
#define REG 4
#define MEM 5
#define odata 0177566
#define ostat 0177564
extern byte N, Z, V, C;
extern byte it_is_byte;
extern int nn;
extern char xx;
extern int Rnn;
extern int trc;
extern word mem[];
extern word reg[]; //регистры R0 .. R7

extern struct mr {
	word adr; // адрес аргумента
	word val; // значение аргумента
	word space; // адрес mem[] или reg[]
} ss, dd;

extern const struct Command{
	word mask;
	word opcode;
	char * name;
	void (*do_func)(void);
	char params;
}  cmd[];

void b_write(Adress adr, byte b); // пишем байт b по адресу adr
byte b_read (Adress adr); // читаем байт по адресу adr;
void w_write(Adress adr, word w); // пишем слово w по адресу adr
word w_read (Adress adr); // читаем слово по адресу adr
void run();

void set_flags(word w);
void set_C (word w);
void do_mov();
void do_movb();
void do_halt();
void do_add();
struct mr get_mr(word w);
void do_sob();
void do_br();
void do_beq();
void do_tst();
void do_bpl();
void do_bmi();
void do_bne();
void do_tstb();
void do_clr();
void do_cmp();
void do_cmpb();
void do_jmp();
void do_unknown();

void trace(const char *  format, ...);
void load_file(const char * file_name);

#endif

















