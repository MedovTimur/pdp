#include <stdio.h>
#include <assert.h>
#include "headers.h"





/*void test_mem() {

	byte b0 = 0x0a; // пишем байт , читаем байт
	byte b1 = 0x0b;
	word w = 0x0b0a;

	//ТЕСТ№1
	w_write (0, w);//записываем слово
	word wres = w_read(0);//читаем слово 
	printf("%04hx = %04hx\n",w, wres ); // hhx - half half hex (4) 
	//%02hhx - ширина 2 с ведущими нулями
	assert(w == wres); // чтобы в мае гулять на природе и шашлыки жарить
						// ну или ботать матан 
	
	//ТЕСТ№2 читаем слово пишем два байта

	Adress a = 4;
	w_write(a, w);
	word b0res = b_read(a);
	word b1res = b_read(a+1);
	printf("ww/br \t %04hx = %02hhx%02hhx\n", wres, b1res, b0res );
	assert (w == wres);


	//ТЕСТ№3 проверка функции b_write

	word w0 = 0x0b0a;
	word w1 = 0x0c0c;
	byte b = 0x0c;
	w_write(4, w0);

	b_write(4,b);
	b_write(5,b);
	word wres1 = w_read(4);//читаем слово  
	printf("wo/bb \t %04hx = %02hhx%02hhx\n", wres1, b, b);
	assert(w1 == wres1);
}
*/
void load_file(const char * file_name);

int main (int argc, char * argv[]) {
	//printf("%d, %s\n", argc, argv[0] );
	b_write(ostat, 0200);
	load_file(argv[1]);
	run();
	//test_mem();
	return 0;
}

const Command cmd[] = {
	{0177777, 0000000, "HALT",	do_halt,	NO_PARAM},
	{0170000, 0060000, "ADD",	do_add,		HAS_SS|HAS_DD}, 
	{0170000, 0010000, "MOV",	do_mov,		HAS_SS|HAS_DD},
	{0177000, 0077000, "SOB",	do_sob,		HAS_NN},
	{0170000, 0110000, "MOVb",	do_movb,	HAS_SS|HAS_DD}, 
	{0177400, 0000400, "BR",	do_br,		HAS_XX},
	{0177400, 0001400, "BEQ",	do_beq,		HAS_XX},
	{0177700, 0005700, "TST",	do_tst,		HAS_DD},
	{0177700, 0105700, "TSTb",	do_tstb,	HAS_DD}, 
	{0177400, 0100000, "BPL",	do_bpl,		HAS_XX},
	{0177400, 0100400, "BMI",	do_bmi,		HAS_XX},
	{0177400, 0001000, "BNE",	do_bne,		HAS_XX},
	{0177700, 0005000, "CLR",	do_clr,		HAS_DD},
	{0177700, 0000100, "JMP",	do_jmp,		HAS_DD},
	{0170000, 0020000, "CMP",	do_cmp,		HAS_SS|HAS_DD},
	{0170000, 0120000, "CMPB",	do_cmpb,	HAS_SS|HAS_DD},
	{0000000, 0000000, "UNKNOWN", do_unknown,   NO_PARAM}
};

void load_file(const char * file_name){
	FILE * f_inp = fopen(file_name, "rb");
	assert(f_inp);
	unsigned int start = 0, i = 0, n = 0 ,w = 0;

	while(2 == fscanf(f_inp,"%x%x",&start, &n)){
		for (i = 0; i < n; i++){
		fscanf (f_inp, "%x", &w);
		 b_write (start+i, (byte)w);
		}
	}
	fclose (f_inp);
}

void b_write(Adress adr, byte b) {
	word x;
	if (adr % 2 == 0) {
		x = mem[adr];
		x = x>>8;
		mem[adr] = (x<<8) | (word)b;
	}
	else {
		x = mem[adr-1];
		mem[adr-1] = (byte)x | ((word)b<<8);
	}


}

byte b_read (Adress adr) {
	byte b;
	if (adr % 2 == 0)
		b = (byte)mem[adr];
	else {
		word w = mem[adr-1];
		b = (byte)(w >> 8);
	}
	return b;
}

word w_read (Adress adr) {
	assert(adr % 2 == 0);
	return mem [adr];
}

void w_write (Adress adr, word w) {
	assert(adr % 2 == 0);
	mem[adr] = w;
}


