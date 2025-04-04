#ifndef __BUZZER_H
#define __BUZZER_H

void Init_buzzer();
void SetFrequency(double freq);
void AddFrequency(double freq);
void RemoveFrequency(double freq);
void CheckFrequency();
#endif