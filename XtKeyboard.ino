#include <Keyboard.h>


#define NUMBITS	10
#define CLOCK	1		/* PD1 */
#define DATA	0		/* PD0 */
#define DEBUG0	3		/* PD3 */
#define DEBUG1	2		/* PD2 */
#define MAXWAIT	2000

char m[255];


void
setupMap()
{
	m[1] = KEY_ESC;
	m[2] = '1';
	m[3] = '2';
	m[4] = '3';
	m[5] = '4';
	m[6] = '5';
	m[7] = '6';
	m[8] = '7';
	m[9] = '8';
	m[10] = '9';
	m[11] = '0';
	m[12] = '-';
	m[13] = '=';
	m[14] = KEY_BACKSPACE;
	m[15] = KEY_TAB;
	m[16] = 'q';
	m[17] = 'w';
	m[18] = 'e';
	m[19] = 'r';
	m[20] = 't';
	m[21] = 'y';
	m[22] = 'u';
	m[23] = 'i';
	m[24] = 'o';
	m[25] = 'p';
	m[26] = '[';
	m[27] = ']';
	m[28] = KEY_RETURN;
	m[29] = KEY_LEFT_CTRL;
	m[30] = 'a';
	m[31] = 's';
	m[32] = 'd';
	m[33] = 'f';
	m[34] = 'g';
	m[35] = 'h';
	m[36] = 'j';
	m[37] = 'k';
	m[38] = 'l';
	m[39] = ';';
	m[40] = '\'';
	m[41] = '`';
	m[42] = KEY_LEFT_SHIFT;
	m[43] = '\\';
	m[44] = 'z';
	m[45] = 'x';
	m[46] = 'c';
	m[47] = 'v';
	m[48] = 'b';
	m[49] = 'n';
	m[50] = 'm';
	m[51] = ',';
	m[52] = '.';
	m[53] = '/';
	m[54] = KEY_RIGHT_SHIFT;
	m[55] = '\335';
	m[56] = KEY_LEFT_ALT;
	m[57] = ' ';
	m[58] = KEY_CAPS_LOCK;
	m[59] = KEY_F1;
	m[60] = KEY_F2;
	m[61] = KEY_F3;
	m[62] = KEY_F4;
	m[63] = KEY_F5;
	m[64] = KEY_F6;
	m[65] = KEY_F7;
	m[66] = KEY_F8;
	m[67] = KEY_F9;
	m[68] = KEY_F10;
	const byte KEY_NUM_LOCK = 219;
	m[69] = KEY_NUM_LOCK;
	const byte KEY_SCROLL_LOCK = 207;
	m[70] = KEY_SCROLL_LOCK;
	m[71] = '\347';
	m[72] = '\350';
	m[73] = '\351';
	m[74] = '\336';
	m[75] = '\344';
	m[76] = '\345';
	m[77] = '\346';
	m[78] = '\337';
	m[79] = '\341';
	m[80] = '\342';
	m[81] = '\343';
	m[82] = '\352';
	m[83] = '\353';
	m[101] = KEY_F11;
	m[102] = KEY_F12;
	m[107] = KEY_INSERT;
	m[108] = KEY_DELETE;
	m[109] = KEY_UP_ARROW;
	m[110] = KEY_DOWN_ARROW;
	m[111] = KEY_LEFT_ARROW;
	m[112] = KEY_RIGHT_ARROW;
}

void
process(byte data)
{
	byte k;

	if (data >= 128) {
		k = m[data - 128];
		Keyboard.release(k);
	} else {
		k = m[data];
		Keyboard.press(k);
	}
	Serial.print(data);
	Serial.print(',');
}

void
setup()
{
	setupMap();
	PORTD = (1 << CLOCK) | (1 << DATA);	/* pull-ups there */
	DDRD = (1 << DEBUG0) | (1 << DEBUG1);	/* DEBUG? as outputs, with 0
						 * states */
	Serial.begin(9600);
}

void
loop()
{
	enum {
		WCL, CL, WCH
	};
	static byte data = 0;
	static int n = 0;
	static int s = WCH;
	static int t = 0;

	for (;;) {
		PORTD ^= (1 << DEBUG0);

		switch (s) {
		case WCL:	/* Waiting for Clock to go Low */
			if ((PIND & (1 << CLOCK)) == 0)
				s = CL;
			else if (n > 0 && ++t > MAXWAIT) {
				/* We are most probably out of sync */
				data = 0;
				n = 0;
			}
			break;
		case CL:	/* Clock just went Low */
			data = data >> 1;
			if (PIND & (1 << DATA)) {
				bitSet(data, 7);
				PORTD |= (1 << DEBUG1);
			} else {
				bitClear(data, 7);
				PORTD &= ~(1 << DEBUG1);
			}
			n++;
			t = 0;
			if (n == NUMBITS) {
				PORTD &= ~(1 << CLOCK);
				DDRD |= (1 << CLOCK);	/* as zero output */
				process(data);
				DDRD &= ~(1 << CLOCK);	/* as input */
				PORTD |= (1 << CLOCK);	/* with pull-up */
				data = 0;
				n = 0;
			}
			s = WCH;
			break;
		case WCH:	/* Waiting for Clock to go High */
			if (PIND & (1 << CLOCK))
				s = WCL;
			break;
		}
	}
}
