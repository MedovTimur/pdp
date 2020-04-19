#include <stdio.h>
#include <assert.h>

typedef unsigned char byte; //8 bit
typedef unsigned short int word; // 16 bit
typedef word Adress; 

#define MEMSIZE (64 * 1024)
#define pc reg[7]

word mem[MEMSIZE];
word reg[8]; //регистры R0 .. R7

void b_write(Adress adr, byte b); // пишем байт b по адресу adr
byte b_read (Adress adr); // читаем байт по адресу adr;
void w_write(Adress adr, word w); // пишем слово w по адресу adr
word w_read (Adress adr); // читаем слово по адресу adr


void test_mem() {

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

int main () {
	test_mem();
	return 0;
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




