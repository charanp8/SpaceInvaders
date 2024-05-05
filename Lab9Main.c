// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Your name
// Last Modified: 12/31/2023

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
//#include <math.h>
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void) { // set phase lock loop (PLL)
    // Clock_Init40MHz(); // run this line for 40MHz
    Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M = 1;

uint32_t Random32(void) {
    M = 1664525 * M + 1013904223;
    return M;
}

uint32_t Random(uint32_t n) {
    return (Random32() >> 16) % n;
}

// Define a global instance of the Player struct
struct Player {
    unsigned long x; // x coordinate
    unsigned long y; // y coordinate
    uint32_t oldx; //old x
    uint32_t oldy; //old y
    //uint32_t sema; //ready flag
};
struct Enemy{
    uint32_t x; // x coordinate
    uint32_t y; // y coordinate
    uint32_t rx;
    uint32_t speed;
    uint8_t active; //0 means inactive, 1 means active
    uint32_t missile; //number of missile that deleted it
};

//struct Enemy TArray[25];
struct Enemy Screen[25];
//struct Enemy Attack[10];
typedef struct EnemyMissile{
    uint32_t x; // x coordinate
    uint32_t y; // y coordinate
    uint8_t alive; // 0 means inactive, 1 means active
    uint32_t Spriteshoot; //which sprite is shooting the missile
    uint32_t missile; //missile it collided with
}EnemyMissile;
typedef struct TinyMissle {
    uint32_t x; // x coordinate
    uint32_t y; // y coordinate
    uint8_t alive; //0 mean inactive, 1 means active
    uint32_t enemy; //number of enemy it attacked
    //uint32_t sema; //ready flag
}TinyMissle;
TinyMissle* head = NULL;
TinyMissle* back = NULL;


TinyMissle missiles[25];
EnemyMissile attack[25];
void MisArrayInit(void)
{
    for(int i = 0; i < 25; i++)
    {
        missiles[i].x = 0;
        missiles[i].y = 0;
        missiles[i].alive = 0;
    }
}
uint32_t totalmis = 0;
uint32_t score = 0;
struct Player PlayerShip = {10, 159};
uint32_t shootsound = 0;
uint32_t MisArrayAdd(void)
{
    //Sound_Shoot();
    for(uint32_t i = 0; i < 25; i++){
        if(missiles[i].alive == 0)
        {
            missiles[i].x = PlayerShip.x;
            missiles[i].y = 159;
            missiles[i].alive = 1;
            Sound_Shoot();
            return i;
        }
    }
}
void MisArrayRem(int i)
{
    missiles[i].alive = 0;
}
void EnArrayInit(void)
{
    for(int i = 0; i < 25; i++)
    {
        attack[i].x = 0;
        attack[i].y = 0;
        attack[i].alive = 0;
        attack[i].missile = 25;
    }
}

uint32_t totalen = 0;
uint32_t EnArrayAdd(uint32_t index)
{
    //for(uint32_t i = 0; i < 25; i++){
        if(attack[0].alive == 0)
        {
            attack[0].x = Screen[index].x;
            attack[0].y = Screen[index].y + 10;
            attack[0].alive = 1;
            //totalen++;
            //return i;
        }

}
void EnArrayRem(int i)
{
    attack[i].alive = 0;
}

//front = NULL;
//back = NULL;
//TinyMissle Queue[];
// Define the PlayerShip coordinates
 // Initial position of PlayerShip

struct TinyMissle missile = {10, 159};
 //Move PlayerShip based on ADC input
uint32_t time = 0;
uint32_t value = 0;
uint32_t data = 0;

void Player_Move(uint32_t variable) {

    PlayerShip.x = (variable) >> 5;
}

void Collision_Out(void)
{
    for(int i = 0; i < totalmis; i++)
    {
        for(int j = 0; j < 25; j++)
        {
        if(missiles[i].enemy == j)
        {
            ST7735_DrawBitmap(missiles[i].x, missiles[i].y, erase, 5, 16);
            ST7735_DrawBitmap(Screen[j].x, Screen[j].y, erase, 16, 16);


            //j = 0;
            j = 25;
            //score++;
        }


    }
}
}
void Missile_Collision(uint32_t index)
{
    if(((missiles[index].x - 3) <= attack[0].x) && ((missiles[index].x + 6) >= attack[0].x))
    {
        if(((missiles[index].y + 16) >= attack[0].x) && ((missiles[index].y) <= attack[0].x))
        {
            missiles[index].alive = 0;
            attack[0].alive = 0;
            attack[0].missile = index;

            //ST7735_DrawBitmap(missiles[index].x, missiles[index].y, erase, 3, 16);
            //ST7735_DrawBitmap(attack[0].x, attack[0].y, erase, 3, 16);
        }
    }
}
void MissileCol_Out()
{
    for(int i = 0; i < totalmis; i++)
    {
        if(attack[0].missile == i)
        {
            ST7735_DrawBitmap(missiles[i].x, missiles[i].y, erase, 3, 16);
            ST7735_DrawBitmap(attack[0].x, attack[0].y, erase, 3, 16);
        }
    }
}
void Check_Collision(uint32_t index) {
    //Missile_Collision(index);
    if(missiles[index].alive == 0)
        return;
    for(int i = 0; i < 25; i++) {
        //uint32_t tempx = missiles[index].x;
        //uint32_t tempy = missiles[index].y;
        if (((Screen[i].x - 8) <= missiles[index].x) && (Screen[i].active == 1) && ((Screen[i].x + 8) >= missiles[index].x) && (missiles[index].alive == 1)) {
            if ((Screen[i].y == missiles[index].y)) {
                missiles[index].alive = 0;
                Screen[i].active = 0;
                missiles[index].enemy = i;
                Sound_Explosion();
                // Increment the score when an enemy is destroyed
                //score++;



                //i = 0;
                return;
            }

        }
    }
}
void MissileMove_Out()
{
    for(int i = 0; i < totalmis; i++)
        {
            if(missiles[i].alive == 1)
            {
                ST7735_FillRect(missiles[i].x + 7, missiles[i].y - 1, 2, 8, ST7735_BLACK);
                ST7735_DrawBitmap(missiles[i].x + 7, missiles[i].y, missile1, 2,8);
            }
            if(missiles[i].y == 0)
            {
                ST7735_DrawBitmap(missiles[i].x + 7, missiles[i].y, erase, 2, 4);
            }

        }

}
void Missile_Move(void)
{
    //for(int j = 140; j >= 0; j--)
    //{
      //if(totalmis == 0)
         // return;
      for(uint32_t i = 0; i < totalmis; i++)
      {
      if(missiles[i].alive == 1)
      {
      //ST7735_FillRect(missiles[i].x + 7, j + 1, 3, 16, ST7735_BLACK);
      //ST7735_DrawBitmap(missiles[i].x + 7, j, missile1, 3,16);
      missiles[i].y -= 1;
      //Sound_Shoot();
      Check_Collision(i);
      if(missiles[i].y == 0)
      {
          MisArrayRem(i);
      }
      }


      //Clock_Delay1ms(1);
      //ST7735_FillRect(missiles[i].x + 7, 0, 3, 2, ST7735_BLACK);
      }

    //}
    //missile.x = (variable) >> 9; old code

}
void EnemyShoot_Out()
{
    //for(int i = 0; i < totalen; i++)
    //{
        if(attack[0].alive == 1)
        {
            ST7735_DrawBitmap(attack[0].x + 8, attack[0].y - 1, erase, 3, 8);
            ST7735_DrawBitmap(attack[0].x + 8, attack[0].y, missile1, 3, 8);
            //if(attack[0].y == 160)
            //{

            //}
            //ST7735_FillRect(attack[0].x + 8, attack[0].y, 3, 16, ST7735_BLACK);
        }

    //}
}
void Enemy_Shoot(uint32_t index)
{
    if(Screen[index].active == 0)
        return;
    EnArrayAdd(index);
    attack[0].Spriteshoot = index;
      //uint32_t timer = 10;
      //double inc = .0000000001;
      //uint32_t increment = (uint32_t)inc;
      //for(int j = Screen[index].y + 10; j <= 160; j += increment){

      if(attack[0].alive == 1)
       {

         attack[0].y += 1;
         //Check_Collision(i);
         if(attack[0].y == 160)
         {
             EnArrayRem(0);
             //totalen--;
         }
         //uint32_t k = 10000;
         //while(k > 0)
         //{
             //k--;
         //}
         //Clock_Delay1ms(10);
      }
      //ST7735_FillRect(attack[0].x + 8, 144, 3, 16, ST7735_BLACK);
    //}

}

//void Check_Collision(void)
//{

//}
// games  engine runs at 30Hz
uint32_t sema = 0;
uint32_t last0 = 0;
uint32_t now0 = 0;

void TIMG12_IRQHandler(void) {
    uint32_t pos, msg;
    uint32_t samp = 0;
    //uint32_t sema = 0;
    //uint32_t sema = 0;
    if ((TIMG12->CPU_INT.IIDX) == 1) { // this will acknowledge
        GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
        GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
        // game engine goes here
        // 1) sample slide pot
        //samp = ADCin();
        data = ADCin();
        Player_Move(data);
        // 2) read input switches
        static uint32_t mis = 0;
        uint32_t now0 = GPIOA->DIN31_0 & (1<<12);
        if((now0 != 0) && (last0 == 0)){
            uint32_t i = MisArrayAdd();
            totalmis++;

            //Sound_Shoot();
            //Missile_Move();
              //Missile_Move(data);
              //for(int j = 155; j > 0; j--) {
                   //ST7735_FillRect(missiles[i].x + 7, j + 1, 3, 16, ST7735_BLACK);
                   //ST7735_DrawBitmap(missiles[i].x + 7, j, missile1, 3,16);
                   //missiles[i].y = j;
                   //Check_Collision();
                   //Clock_Delay1ms(1);
               //}
             // ST7735_FillRect(PlayerShip.x + 7, 0, 3, 2, ST7735_BLACK);
             }
        Missile_Move();


        Enemy_Shoot(Random(5) + 20);
        last0 = now0;




        // 3) move sprites
        // 4) start sounds
        // 5) set semaphore
        // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
        sema = 1;
        GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void) {
    return (0x80 | ((GPIOB->DOUT31_0 >> 26) & 0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
    for(Language_t myL=English; myL<= French; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_InitR(INITR_REDTAB);

  ST7735_SetCursor(3, 1);
  ST7735_OutString("Choose Language:");
  ST7735_SetCursor(3, 5);
  ST7735_OutString("English (UP)");
  ST7735_SetCursor(3, 8);
  ST7735_OutString("Spanish (DOWN)");


    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  /*
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }
  */
}

// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs
      while(Switch_In() == 0){
      }
      LED_On(17);
  }
}
// use main4 to test sound outputs
int main4(void){
uint32_t now;
uint32_t last = 0;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    while((last == 0)&&(now == 0)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
#include <stdint.h>

// Define your functions and global variables here

 int main(void) { // final main
    uint32_t last0 = 0;
    uint8_t gameOver = 0; // Flag to indicate if the game is over
    uint8_t gamePaused = 0; // Flag to indicate if the game is paused
    __disable_irq();
    PLL_Init(); // set bus speed
    LaunchPad_Init();
    ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
    ST7735_FillScreen(ST7735_BLACK);
    ADCinit();     //PB18 = ADC1 channel 5, slidepot
    Switch_Init(); // initialize switches
    LED_Init();    // initialize LED
    Sound_Init();  // initialize sound
    TExaS_Init(0, 0, &TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
    TimerG12_IntArm(80000000 / 30, 2);
    // initialize all data structures
    __enable_irq();
    ST7735_InitR(INITR_REDTAB);
    ST7735_SetCursor(3, 1);
    ST7735_OutString("Choose Language:");
    ST7735_SetCursor(3, 5);
    ST7735_OutString("English (UP)");
    ST7735_SetCursor(3, 8);
    ST7735_OutString("Spanish (DOWN)");
    uint32_t currentswitch = 0;
    uint32_t language = 0;
    uint32_t xpos = 10;
    uint32_t ypos = 15;
    for(int i = 0; i < 5; i++)
    {
        Screen[i].x = xpos;
        xpos += 24;
        Screen[i].y = ypos;
        Screen[i].active = 1;
    }
    xpos = 10;
    ypos += 15;
    for(int i = 5; i < 10; i++)
    {
        Screen[i].x = xpos;
        xpos += 24;
        Screen[i].y = ypos;
        Screen[i].active = 1;
    }
    xpos = 10;
    ypos += 15;
    for(int i = 10; i < 15; i++)
    {
        Screen[i].x = xpos;
        xpos += 24;
        Screen[i].y = ypos;
        Screen[i].active = 1;
    }
    xpos = 10;
    ypos += 15;
    for(int i = 15; i < 20; i++)
    {
        Screen[i].x = xpos;
        xpos += 24;
        Screen[i].y = ypos;
        Screen[i].active = 1;
    }
    xpos = 10;
    ypos += 15;
    for(int i = 20; i < 25; i++)
    {
         Screen[i].x = xpos;
         xpos += 24;
         Screen[i].y = ypos;
         Screen[i].active = 1;
    }
    //Sound_Shoot();
    //uint32_t sema = 0;
    while (1) {
        currentswitch = Switch_In();

        if ((currentswitch & (1 << 28)) != 0) { // Check if the up button is pressed
            // English language selected
            // Display enemies and start game loop
            language = 0;
            ST7735_FillScreen(ST7735_BLACK);
            ST7735_SetCursor(8, 7);
            // Draw enemies
            for(int i = 0; i < 20; i++)
            {
               ST7735_DrawBitmap(Screen[i].x, Screen[i].y, SmallEnemy10pointA, 16, 10);
            }
            for(int i = 20; i < 25; i++)
            {
                ST7735_DrawBitmap(Screen[i].x, Screen[i].y, SmallEnemy30pointA, 16, 10);
            }
            //ST7735_DrawBitmap(53, 151, Bunker0, 18, 5);
            // Start game loop
            MisArrayInit();
            EnArrayInit();
            while (1) {
                // Toggle game state if left switch is pressed
                if ((Switch_In() & (1 << 24)) != 0) {
                    gamePaused = !gamePaused; // Toggle game state
                    while ((Switch_In() & (1 << 24)) != 0); // Wait until left switch is released
                }

                if (gamePaused) {
                    // Display pause message or handle pause logic here
                    continue; // Skip the rest of the loop and start from the beginning
                }

                // Game is not paused, continue normal game logic
                MissileMove_Out();


                Collision_Out();

                EnemyShoot_Out();
                //MissileCol_Out();
                while(sema == 0){}
                //if()
                // Update player's position
                ST7735_FillRect(0, 154, 128, 6, ST7735_BLACK); // Erase previous position
                ST7735_DrawBitmap(PlayerShip.x, PlayerShip.y, PlayerShip0, 14, 6); // Draw player's ship at updated position
                //MissileMove_Out();
                //EnemyShoot_Out();
                // Update enemy positions
                if(((attack[0].y) == PlayerShip.y) && ((attack[0].x - 8) <= PlayerShip.x) && ((attack[0].x + 16) >= PlayerShip.x))
                {
                      Sound_Killed();
                      ST7735_FillScreen(ST7735_BLACK);
                      ST7735_SetCursor(3, 5);
                      ST7735_OutString("Game Over!");


                      ST7735_SetCursor(3, 7);
                      ST7735_OutString("Score: ");
                      ST7735_OutUDec(score);


                      gameOver = 1;
                      break;
                }
                uint32_t totaldead = 0;
               for(int i = 0; i < 25; i++)
               {
                   if(Screen[i].active == 0)
                         totaldead++;


               }

               score = totaldead;
              if(totaldead == 25)
              {
                   ST7735_FillScreen(ST7735_BLACK);
                   ST7735_SetCursor(3, 5);
                   if (language == 1) {
                        ST7735_OutString("¡Ganaste!");
                        ST7735_SetCursor(3, 7);
                        ST7735_OutString("Puntuación: ");
                     //break;
                   }
              else {
                          ST7735_OutString("You Win!");
                          ST7735_SetCursor(3, 7);
                          ST7735_OutString("Score: ");
                   }
                          ST7735_OutUDec(score);
                          break;
                                        //gameOver = 1;
               }
                //Enemy_Shoot(Random(5)+20);
                //Enemy_Shoot(20);
                // Inside the main game loop where enemies move down the screen
                for (int i = 0; i < 25; i++) {
                    if (Screen[i].active == 1) {
                        Screen[i].y += 1; // Move enemy down the screen
                        if (Screen[i].y >= 159) {
                            // Enemy reached bottom of the screen
                            // Game over
                            Sound_Killed();
                            gameOver = 1;
                            break; // Exit the game loop
                        }
                        // Check for collision with player's ship
                        if (((Screen[i].y + 10) >= PlayerShip.y) && ((Screen[i].x - 8) <= PlayerShip.x) && ((Screen[i].x + 8) >= PlayerShip.x)) {
                            // Player's ship collided with an enemy
                            // Game over
                            Sound_Killed();
                            gameOver = 1;
                            break; // Exit the game loop
                        }
                        // Redraw enemy
                        if (i < 20) {
                            ST7735_DrawBitmap(Screen[i].x, Screen[i].y, SmallEnemy10pointA, 16, 10);
                        } else {
                            ST7735_DrawBitmap(Screen[i].x, Screen[i].y, SmallEnemy30pointA, 16, 10);
                        }
                    }
                }

                // After the game loop
                if (gameOver) {

                            ST7735_FillScreen(ST7735_BLACK); // Clear the screen
                            ST7735_SetCursor(3, 5);
                            if (language == 1) {
                                ST7735_OutString("¡Juego Terminado!");
                                ST7735_SetCursor(3, 7);
                                ST7735_OutString("Puntuación: ");
                            } else {
                                ST7735_OutString("Game Over!");
                                ST7735_SetCursor(3, 7);
                                ST7735_OutString("Score: ");
                            }
                            //Sound_Killed();
                            ST7735_OutUDec(score);
                    // Game over logic
                    // Display game-over message, score, etc.
                    break; // Exit the main loop
                }


                // Delay to control the speed of movement
                Clock_Delay1ms(50); // Adjust this value to change the speed of movement
                sema = 0;
            }
        } else if ((currentswitch & (1 << 31)) != 0) { // Check if the down button is pressed
            // Spanish language selected
            // Display enemies and start game loop
            language = 1;
            ST7735_FillScreen(ST7735_BLACK);
            ST7735_SetCursor(8, 7);
            // Draw enemies
            for(int i = 0; i < 20; i++)
            {
               ST7735_DrawBitmap(Screen[i].x, Screen[i].y, SmallEnemy10pointA, 16, 10);
            }
            for(int i = 20; i < 25; i++)
            {
                ST7735_DrawBitmap(Screen[i].x, Screen[i].y, SmallEnemy30pointA, 16, 10);
            }
            //ST7735_DrawBitmap(53, 151, Bunker0, 18, 5);
            // Start game loop
            MisArrayInit();
            EnArrayInit();
            while (1) {
                // Toggle game state if left switch is pressed
                if ((Switch_In() & (1 << 24)) != 0) {
                    gamePaused = !gamePaused; // Toggle game state
                    while ((Switch_In() & (1 << 24)) != 0); // Wait until left switch is released
                }

                if (gamePaused) {
                    // Display pause message or handle pause logic here
                    continue; // Skip the rest of the loop and start from the beginning
                }

                // Game is not paused, continue normal game logic
                MissileMove_Out();
                Collision_Out();
                //Sound_Shoot();
                EnemyShoot_Out();

                //MissileCol_Out();
                while(sema == 0){}

                // Update player's position
                ST7735_FillRect(0, 154, 128, 6, ST7735_BLACK); // Erase previous position
                ST7735_DrawBitmap(PlayerShip.x, PlayerShip.y, PlayerShip0, 14, 6); // Draw player's ship at updated position

                //MissileMove_Out();
                //EnemyShoot_Out();
                // Update enemy positions
                if(((attack[0].y) == PlayerShip.y) && ((attack[0].x - 8) <= PlayerShip.x) && ((attack[0].x + 16) >= PlayerShip.x))
                {
                      ST7735_FillScreen(ST7735_BLACK);
                      ST7735_SetCursor(3, 5);
                      ST7735_OutString("¡Juego Terminado!");


                      ST7735_SetCursor(3, 7);
                      ST7735_OutString("Puntuación: ");
                      ST7735_OutUDec(score);
                      Sound_Killed();
                      gameOver = 1;
                      break;
                }


                //Enemy_Shoot(Random(5)+20);
                //Enemy_Shoot(20);
                // Inside the main game loop where enemies move down the screen
                for (int i = 0; i < 25; i++) {
                    if (Screen[i].active == 1) {
                        Screen[i].y += 1; // Move enemy down the screen
                        if (Screen[i].y >= 159) {
                            // Enemy reached bottom of the screen
                            // Game over
                            Sound_Killed();
                            gameOver = 1;
                            break; // Exit the game loop
                        }
                        // Check for collision with player's ship
                        if (((Screen[i].y + 10) >= PlayerShip.y) && ((Screen[i].x - 8) <= PlayerShip.x) && ((Screen[i].x + 8) >= PlayerShip.x)) {
                            // Player's ship collided with an enemy
                            // Game over
                            Sound_Killed();
                            gameOver = 1;
                            break; // Exit the game loop
                        }
                        // Redraw enemy
                        if (i < 20) {
                            ST7735_DrawBitmap(Screen[i].x, Screen[i].y, SmallEnemy10pointA, 16, 10);
                        } else {
                            ST7735_DrawBitmap(Screen[i].x, Screen[i].y, SmallEnemy30pointA, 16, 10);
                        }
                    }
                }
                uint32_t totaldead = 0;
                for(int i = 0; i < 25; i++)
                {
                    if(Screen[i].active == 0)
                            totaldead++;

                }

               score = totaldead;
              if(totaldead == 25)
              {
                 ST7735_FillScreen(ST7735_BLACK);
                 ST7735_SetCursor(3, 5);
                 if (language == 1) {
                   ST7735_OutString("¡Ganaste!");
                   ST7735_SetCursor(3, 7);
                   ST7735_OutString("Puntuación: ");
                   ST7735_OutUDec(score);
                   break;
                   }
                 else {
                        ST7735_OutString("You Win!");
                        ST7735_SetCursor(3, 7);
                        ST7735_OutString("Score: ");
                      }
                        ST7735_OutUDec(score);
                        break;
                        //gameOver = 1;
                      }
                // After the game loop
                if (gameOver) {

                            ST7735_FillScreen(ST7735_BLACK); // Clear the screen
                            ST7735_SetCursor(3, 5);
                            if (language == 1) {
                                ST7735_OutString("¡Juego Terminado!");
                                ST7735_SetCursor(3, 7);
                                ST7735_OutString("Puntuación: ");
                            } else {
                                ST7735_OutString("Game Over!");
                                ST7735_SetCursor(3, 7);
                                ST7735_OutString("Score: ");
                            }
                            //Sound_Killed();
                            ST7735_OutUDec(score);
                    // Game over logic
                    // Display game-over message, score, etc.
                    break; // Exit the main loop
                }


                // Delay to control the speed of movement
                Clock_Delay1ms(50); // Adjust this value to change the speed of movement
                sema = 0;
            }
        }

        // Handle case when neither up nor down button is pressed
        // You can add any specific handling for this case if needed
    }
}
