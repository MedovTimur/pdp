
#include <stdio.h>
#include <stdlib.h>
#include "headers.h"
#include <assert.h>


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
	int e; // для 6 и 7 мод
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
			
			if(it_is_byte)
        		res.val = b_read(res.adr);
        	else
        		res.val = w_read(res.adr);
			reg[r] += 2; 
			if (it_is_byte == 1)
			 reg[r] -= 1;
			if (r == 7)
				trace("#%o ", res.val);
			else 
				trace(" (R%o)+ ", r);
			break;
		case 3: // @(R3)+
			res.adr = w_read(reg[r]);
			res.val = w_read(res.adr);
			reg[r] += 2;
			if (r == 7)
				trace( "@#%o ", res.adr);
			else 
				trace( "@(R%d)+", r);
			break;
		case 4: // -(R)
            if (it_is_byte == 1) // байтовая операция?
			 reg[r] -= 1; 
            else
            	reg[r] -= 2; // слово

        	res.adr = reg[r];
        	res.val = w_read(res.adr);
        	trace( "-(R%d)", r);
        	break;
        case 5: // @-(R) аналог 3-ей моды
        	reg[r] -=2;
        	res.adr = w_read(reg[r]);
        	res.val = w_read(res.adr);
			trace( "@-(R%d)", r);
			break;
		case 6: // содержимое регистра складывается с числом, записанным после команды, и полученная сумма используется в качестве адрес операнда
			e = w_read(pc);
			pc +=2;
			res.adr = reg[r] + e;
			//res.adr = reg[r] + w_read(pc) +2;
			//pc += 2;
			res.val = w_read(res.adr);
			if (r !=7 )
			   trace ("(R%d)", r);
			else
			   trace(" %06o", res.adr);
			break;
		case 7:
			e = w_read(pc);
			pc += 2;
			res.adr = w_read(reg[r] + e);
			res.val = w_read(res.adr);
			if (r != 7)
			   trace ("@%o(R%d)", e, r);
			else
			   trace("@%06o", res.adr);
			break;
		default:
			fprintf(stderr,
				"Mode %o NOT IMPLEMENTED YET!\n", mode);
			exit(1); 			

	}
	return res;
}

void run() {
	int num_cmd_command = 17;
	pc = 01000 ;
	while (1) {
		word w = w_read(pc);
		trace("%06o %06o: ", pc, w); // 6 цифр; o - восьмиричная система;
									  // 0 в начале для красоты; 
		pc += 2;

		for(int i = 0; i < num_cmd_command; i++)
			if ((w & cmd[i].mask) == cmd[i].opcode){	
				trace("%s ", cmd[i].name);
				it_is_byte = (w>>15)&1; // проверяем байтовая ли операция
				if((cmd[i].params & 1) == 1 ){   // has_ss = 00000001 & 1 = 1
					ss = get_mr(w>>6);
				}
				if((cmd[i].params & 2) == 2){   // has_dd = 00000010 & 2 = 2
					dd = get_mr(w);
				}
				if((cmd[i].params & 4) == 4){    //// has_nn = 00000100 & 4 = 4
					nn = w & 0000077;
					Rnn = (w>>6)&7; //077(R)nn 
				}
				if ((cmd[i].params & 8) == 8)
					xx = w & 0000377;   ///8 последних битов
			 

				trace("\n");
				cmd[i].do_func();
				break;
			}	
		it_is_byte = 0;

	}

}

void set_flags(word w){
	Z = 0;
	if (it_is_byte)
		N = (w>>7)&1;
	else
		N = (w>>15)&1;
	if (w==0)
		Z = 1;
	//printf (" N = %d, Z = %d\n", N, Z);
}

void set_C (word w){
	C = (w>>15)&1;
}

void do_sob() {
	reg[Rnn] = reg[Rnn]-1;
	if (reg[Rnn] != 0) {
		pc = pc - 2*nn;
	}
	
}

void do_mov(){
	if (dd.space == REG)	
		reg[dd.adr] = ss.val;
	if (dd.space == MEM)
		w_write(dd.adr, ss.val);
	if (dd.adr == odata)
		trace ("  %c", mem[odata]);
	set_flags(ss.val);
	set_C(ss.val);
	assert (pc != 0177776);
    
}

void do_add () {
	int res_for_flag = ss.val + dd.val;
		if (dd.adr <= 999) // или 7
			reg[dd.adr] = ss.val + dd.val;
		else
		w_write(dd.adr, ss.val + dd.val);

	set_flags(res_for_flag);
	set_C (res_for_flag);

}

void do_movb() {
	if (dd.space == REG)
		reg[dd.adr] = (word)((byte)ss.val);
	if (dd.space == MEM)
		b_write (dd.adr, (byte)ss.val);
	if (dd.adr == odata)
		trace ("%c\n", mem[odata]);
	
	set_flags(ss.val);
	set_C(ss.val);
}


void do_br() { // branch
	trace("xx = %d \n", xx);
	pc = pc + 2*xx;
	trace("%06o \n", pc) ; 
}

void do_beq() { // branch if Equal
	if (Z==1) 
		do_br ();
}

void do_bne() { // branch if not Equal
	if (Z==0) 
		do_br ();
}

void do_bmi() { // branch if minus
	if (N == 1)
		do_br ();
}

void do_bpl() { // branch if plus 
	if (N == 0) 
		do_br();
}
void do_tst() {
	set_flags(dd.val);
	C = 0;
	
}
void do_tstb() {
	//printf("0o%o\n", dd.val);
	set_flags(dd.val);
	C = 0;
}
	

void do_jmp() {
	pc = dd.val;
}
void do_cmp() {
	set_flags(ss.val-dd.val);
	set_C(ss.val-dd.val);
}
void do_cmpb() {
	set_flags(ss.val-dd.val);
	set_C(ss.val-dd.val);
}

void do_clr() {
	w_write(dd.adr, 0);
	N = 0;
	Z = 1;
	C = 0;
}


void do_unknown() {
	printf("UNKNOWN\n");
	exit(0);
}

void do_halt(){
	trace("\nR0 = %06o R2 = %06o R4 = %06o sp = %06o\
	\nR1 = %06o R3 = %06o R5 = %06o pc = %06o\n"  ,\
	reg[0], reg[2], reg[4], reg[6], reg[1], reg[3], reg[5], reg[7]);
	exit (0);
}





























