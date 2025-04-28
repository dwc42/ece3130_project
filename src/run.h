#ifndef __MAIN_h
#define __MAIN_h
int run(void);
struct NoteProperties
{
	uint16_t frequency;
	char octaves;
	char note;
};
extern struct NoteProperties newFrequencies[3][16];
extern uint8_t presetIndex;
extern uint8_t syncPlayback;
uint32_t checkMemoryUsage();
#endif /*__MAIN_h*/