// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

uint32_t Index;
uint32_t period;
uint32_t priority;
uint32_t sound;

void SysTick_IntArm(uint32_t period, uint32_t priority){
  // write this
    SysTick->CTRL = 0;
    SysTick->LOAD = period - 1;
    SysTick->VAL = 0;
    SCB->SHP[1] = SCB->SHP[1] & (~0xC0000000) |priority<<30;
    SysTick->CTRL = 0x0007;
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){
// write this
    DAC5_Init();
    SysTick_IntArm(7273, 1);
    SysTick->CTRL = 0;


}

volatile uint32_t SoundNumber;
const uint8_t *SoundSelect;

void SysTick_Handler(void){ // called at 11 kHz

        if(SoundNumber){
            DAC5_Out(*SoundSelect);
            SoundSelect++;
            SoundNumber--;
        }
        else{
            SysTick->CTRL = 0;
        }
    }



//******* Sound_Start ************
// This function does not output to the DAC.
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count){
// write this
    SoundSelect = pt;
    SoundNumber = count;
    //SysTick->LOAD = 9090 - 1;
    //SysTick->VAL = 0;
    SysTick->CTRL = 0x07;

}
void Sound_Shoot(void){
    uint8_t  *shootpt;
    shootpt = &shoot[0];
    Sound_Start(shootpt, 4080);

}
void Sound_Killed(void){
// write this
    uint8_t  *killpt;
        killpt = &invaderkilled[0];
        Sound_Start(killpt, 4080);

}
void Sound_Explosion(void){
// write this
  //uint8_t
    uint8_t  *explodept;
    explodept = &explosion[0];
    Sound_Start(explodept, 2000);
}

void Sound_Fastinvader1(void){

}
void Sound_Fastinvader2(void){

}
void Sound_Fastinvader3(void){

}
void Sound_Fastinvader4(void){

}
void Sound_Highpitch(void){

}
