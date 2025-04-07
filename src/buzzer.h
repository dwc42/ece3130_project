#ifndef __BUZZER_H
#define __BUZZER_H
/**
 * @brief Initializes the buzzer
 */
void Init_buzzer();
/**
 * @brief sets the frequency of the buzzer plays
 * @param freq the frequency to be set in Hz
 * @note This function will set the frequency of the buzzer to the desired value
 */
void SetFrequency(double freq);
/**
 * @brief Adds a frequency to the list of frequencies to be played
 * @param freq the frequency to be added in Hz
 * @note This function will add the frequency to the list of frequencies to be played
 */
void AddFrequency(double freq);
/**
 * @brief Removes a frequency from the list of frequencies to be played
 * @param freq the frequency to be removed in Hz
 * @note This function will remove the frequency from the list of frequencies to be played
 */
void RemoveFrequency(double freq);
/**
 * @brief Checks the frequency list and sets the frequency of the buzzer to the next frequency in the list
 */
void CheckFrequency();
#endif