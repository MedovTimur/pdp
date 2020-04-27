
#include <stdio.h>
#include <stdlib.h>
#include "headers.h"


void trace(const char *  format, ...){
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

struct mr get_mr(word w) {
	struct mr res;
	res.space = MEM;
	int r = w & 7;            // номер регистра 
	int mode = (w >> 3) & 7; // номер моды
	switch (mode) {
		case 0:   // R3
			res.adr = r;
			res.val = reg[r];
			res.space = REG;
			trace ("R%o ", r);
			break;
		case 1:   // (R3)
			res.adr = reg[r];
			res.val = w_read(res.adr) ;
			trace ("(R%o) ", r);
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
	return res;
}

void run() {
	pc = 01000 ;
	while (1) {
		word w = w_read(pc);
		trace("%06o %06o: ", pc, w); // 6 цифр; o - восьмиричная система;
									  // 0 в начале для красоты; 
		pc += 2;

		for(int i = 0; i < 3; i++)
			if ((w & cmd[i].mask) == cmd[i].opcode){
				trace("%s ", cmd[i].name);
				if((cmd[i].params & 1) == 1 ){   // has_ss = 00000001 & 1 = 1
					ss = get_mr(w>>6);
				}
				if((cmd[i].params & 2) == 2){   // has_dd = 00000010 & 2 = 2
					dd = get_mr(w);
				}

				trace("\n");
				cmd[i].do_func();
			}	

	}

}

void do_mov(){
	if (dd.space == REG)	
		reg[dd.adr] = ss.val;
	if (dd.space == MEM)
		w_write(dd.adr, ss.val);
	if (dd.adr == odata)
		printf ("  %c", mem[odata]);
}

void do_add(){
	if (dd.space == REG)
		reg[dd.adr] = ss.val + dd.val;
	if (dd.space == MEM)
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





























