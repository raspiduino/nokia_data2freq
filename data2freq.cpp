#include "data2freq.h"

#define STEP 1

char log_buf[100];

char path_a[100];
VMWCHAR path_w[100];

VMINT counter = 0;
char p[3];

void transmit_separator(VMINT result)
{
	if (counter % 2 == 0)
	{
		transmit_3bit(p[counter / 2]);
	}
	
	else
	{
		// Play separator
		sprintf(path_a, "e:\\freq_wav\\10.wav");
		vm_ascii_to_ucs2(path_w, 100, path_a);

		counter++;
		sprintf(log_buf, "vm_audio_play_file_ex(%s) -> %d\n", path_a, vm_audio_play_file_ex(path_w, 0, VM_DEVICE_SPEAKER, transmit_separator));
		console_str_in(log_buf);
	}
}

void transmit_3bit(char b)
{
	// Play audio
	if (counter <= 4) {
		// Freq for each group of 3 bits will be b + SEPARATOR
		// So if SEPARATOR = 1, 0b000 will be 2Hz, 0b001 will be 3Hz,..
		// Generate the according wav path for that freq
		sprintf(path_a, "e:\\freq_wav\\%d.wav", (int)b + STEP);
		vm_ascii_to_ucs2(path_w, 100, path_a);

		counter++;
		sprintf(log_buf, "vm_audio_play_file_ex(%s) -> %d\n", path_a, vm_audio_play_file_ex(path_w, 0, VM_DEVICE_SPEAKER, transmit_separator));
		console_str_in(log_buf);
	}
	else {
		return;
	}
}

void transmit_char(char c)
{
	// Set volume to max
	vm_set_volume(5);

	// Extract char into 3 parts.
	// Part 1 and 2 contains 3 bits, part 3 contain 2 bits
	char x = (c >> 5) & 0x07; // Right shift 5 bits and mask with 0x07
	char y = (c >> 2) & 0x07; // Right shift 2 bits and mask with 0x07
	char z = c & 0x03; // Mask with 0x03

	p[0] = x;
	p[1] = y;
	p[2] = z;
	counter = 0;
	transmit_3bit(x);
}
