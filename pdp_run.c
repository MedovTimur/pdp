#include "pdp.h"
#include "stdio.h"
#include "stdlib.h"

#define NO_PARAM 0
#define HAS_SS 1
#define HAS_DD 2

void do_mov()
void do_add()
void do_nothing()
struct mr get_mr(word w)

typedef struct {
	word mask;
	word opcode;
	char * name;
	void (*do_func)(void);
	char params;
} Command;


Command cmd[] = {
	{0777777, 0000000, "HALT",	do_halt,	NO_PARAM},
	{0170000, 0060000, "ADD",	do_add,		HAS_SS|HAS_DD}, 
	{0770000, 0010000, "MOV",	do_mov,		HAS_SS|HAS_DD},

struct mr {
	word adr; // адрес аргумента
	word val; // значение аргумента
	word space; // адрес mem[] или reg[]
}ss,dd;

struct mr get_mr(word w) {
	struct mr res;
	int r = w & 7;            // номер регистра 
	int mode = (w >> 3) & 7; // номер моды
	switch (mode) {
		case 0:   // R3
			res.adr = r;
			res.val = reg[r];
			trace ("R%o", r);
			break;
		case 1:   // (R3)
			res.adr = reg[r];
			res.val = w_read(res.adr) ;
			trace ("(R%o)", r);
			break;
		case 2:   // (R3)+
			res.adr = reg[r];
			res.val = w_read(res.adr);
			reg[r] += 2; 
			if (r == 7)
				trace("#%o ", res.val);
			else 
				trace(" (R%o)+ ", r);
			break;	
		default:
			fprintf(stderr,
				"Mode %o NOT IMPLEMENTED YET!\n", mode);
			exit(1); 			

	}
}

void run() {
	pc = 01000 ;
	while (1) {
		word w = w_read(pc);
		trace("%06o %06o: ", pc, w); // 6 цифр; o - восьмиричная система;
									  // 0 в начале для красоты; 
		pc += 2;

		if (w == 0) {
			trace("halt\n");
			do_halt();
		}
		else if ((w & 0170000) == 0010000) {     //01SSDD (0 001 s s d d) & (1 111 000 000 000 000)        
												 // 0170000 = 0xF000
			trace("mov\n");
			do_mov();
			ss = get_mr(w>>6);
			dd = get_mr(w);


		} 
	}

}

void do_mov(){
	w_write(dd.adr, ss.val);
}
void do_add(){
	w_write (dd.adr, ss.val + dd.val);
}
void do_movb() {
	b_write (dd.adr, (byte)ss.val);
}

void do_halt(){
	trace("\nR0 = %06o R2 = %06o R4 = %06o sp = %06o\
	\nR1 = %06o R3 = %06o R5 = %06o pc = %06o\n"  ,\
	reg[0], reg[2], reg[4], reg[6], reg[1], reg[3], reg[5], reg[7]);
	exit (0);
}





























