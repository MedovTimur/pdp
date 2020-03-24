#include <stdio.h>
#include <assert.h>

typedef unsigned char byte; //8 bit
typedef unsigned short int word; // 16 bit
typedef word Adress; 

#define MEMSIZE (64 * 1024)

byte mem[MEMSIZE];

void b_write(Adress adr, byte b); // пишем байт b по адресу adr
byte b_read (Adress adr); // читаем байт по адресу adr;
void w_write(Adress adr, word w); // пишем слово w по адресу adr
word w_read (Adress adr); // читаем слово по адресу adr


void test_mem() {
	byte b0 = 0x0a; // пишем байт , читаем байт
	b_write (2, b0);
	byte bres = b_read(2);
	printf("%02hhx = %02hhx\n",b0, bres ); // hhx - half half hex (4) 
	//%02hhx - ширина 2 с ведущими нулями
	assert(b0 == bres); // чтобы в мае гулять на природе и шашлыки жарить
						// ну или ботать матан 

	// пишем 2 байта читаем слово
	byte b1 = 0x0b;
	word w = 0x0b0a;
	Adress a = 4;
	b_write(a, b0);
	b_write(a+1, b1);
	word wres = w_read(a);
	printf("ww/br \t %04hx = %02hhx%02hhx\n", wres, b1, b0 );
	assert(w == wres);
}
int main () {
	test_mem();
	return 0;
}

void b_write(Adress adr, byte b) {
	mem[adr] = b;
}



word w_read (Adress a) {
	word w = ((word)mem[a+1]) << 8;
	//printf("w = %x\n", w );
	w = w | mem[a];

	return w;
}

