#include "SnakeQue.h"
#define DIRECTION_UP 1
#define DIRECTION_RIGTH 2
#define DIRECTION_DOWN 3
#define DIRECTION_LEFT 4

#define TABLE_WIDTH	16
#define TABLE_HEIGTH 16

Chain c;
SnakeQue snake;
int snake_direction =  DIRECTION_RIGTH;
boolean prey_exist = false;
boolean game_over = false;
boolean speed_up = false;
int Point  = 0;

char sp = ' ';
char snk = '#';
char pry  = 'X';

char table[16][16] = { {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp},
  {sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp, sp}
};
void(* resetFunc) (void) = 0; //
void setup() {

  Serial.begin(500000);
  delay(1000);

  c.X = 1;
  c.Y = 12;
  snake.enqueue(&c);
  c.X = 2;
  c.Y = 13;
  snake.enqueue(&c);
  c.X = 3;
  c.Y = 14;
  snake.enqueue(&c);
  c.X = 4;
  c.Y = 15;
  snake.enqueue(&c);
  c.X = 5;
  c.Y = 16;
  snake.enqueue(&c);

}

void loop() {
  //snake.dump();
  Game();
}

void Game() {
  if(!speed_up){ 
    for (int i = 0; i < (Point / 10) + 1; i++)
      delay(100);
  }else{
     delay(100);
  }

  read_line();
  GeneratePrey();
  if (game_over) {
    print_gameover();
    while (!Serial.available());
    //restart jump 0
    resetFunc();  //call reset
  } else {
    clear_screen();
    Serial.println(Point);
    Move(snake_direction);
    print_scene();
  }
}

void clear_screen() {
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
}
void read_line() {
  if (Serial.available()) {
    char c = Serial.read();
    if ((c == '2' || c == 'w') && snake_direction != DIRECTION_DOWN)   snake_direction = DIRECTION_UP;
    if ((c == '6' || c == 'd') && snake_direction != DIRECTION_LEFT)   snake_direction = DIRECTION_RIGTH;
    if ((c == '8' || c == 's') && snake_direction != DIRECTION_UP)     snake_direction = DIRECTION_DOWN;
    if ((c == '4' || c == 'a') && snake_direction != DIRECTION_RIGTH)  snake_direction = DIRECTION_LEFT;
    if(c == 'p'){
      speed_up = !speed_up;
      }
    if(c == 'l'){speed_up = false;}
      
  }
  Serial.flush();

}

void CheckCollision(Chain c) {

  if (table[c.X][c.Y] == snk) {
    game_over = true;
  }

  if (table[c.X][c.Y] == pry) {
    prey_exist = false;
    Point += 1;
  }
  else {
    Chain *pC = snake.peek_ind(0);
    table[pC->X][pC->Y] = sp;
    snake.dequeue();
  }
}

void GeneratePrey()
{
  Chain prey;

  if (prey_exist == false)
  {
    
    prey.X = snake.peek()->X;
    prey.Y = snake.peek()->Y;
    if(snake_direction == DIRECTION_UP){      
      prey.Y+=3;      
    }else if(snake_direction == DIRECTION_DOWN){      
      prey.Y-=3;      
    }else if(snake_direction == DIRECTION_RIGTH){      
      prey.X+=3;      
    }else if(snake_direction == DIRECTION_LEFT){      
      prey.X-=3;      
    }

    FixLimit(&prey);

    if(table[prey.X][prey.Y] != snk){
      
    }else{      
      do {
        prey.X = random(16);
        prey.Y = random(16);
      } while (table[prey.X][prey.Y] == snk);
    }
    table[prey.X][prey.Y] = pry;
    prey_exist = true;
  }
}

void Move(int snake_direction) {
  Chain c;
  c.X = snake.peek()->X;
  c.Y = snake.peek()->Y;

  switch (snake_direction) {
    case DIRECTION_UP://up
      c.Y++;
      break;
    case DIRECTION_RIGTH://rigth
      c.X++;
      break;
    case DIRECTION_DOWN://buttom
      c.Y--;
      break;
    case DIRECTION_LEFT://left
      c.X--;
      break;
  }
  FixLimit(&c);
  CheckCollision(c);
  snake.enqueue(&c);
}

void FixLimit(Chain *c) {

  if ( c->X < 0 ) {
    c->X = TABLE_WIDTH - 1;
  }
  if ( c->X >= TABLE_WIDTH) {
    c->X = 0;
  }
  if ( c->Y < 0 ) {
    c->Y = TABLE_HEIGTH - 1;
  }
  if ( c->Y >= TABLE_HEIGTH) {
    c->Y = 0;
  }
}

void traverse_snake() {
  Chain *c;
  for (int i = 0; i < snake.count ; i++) {
    c =  snake.peek_ind(i);
    table[c->X][c->Y] = snk;
  }
}

void print_scene() {
  int i, j;
  traverse_snake();
  for ( i = 0; i < 32 ; i++) {
    Serial.print('_');
  }
  Serial.println(' ');

  for ( i = 15; i >= 0 ; i--) {
    for ( j = 0; j < 16 ; j++) {
      Serial.print(table[j][i]);
      Serial.print(' ');
    }
    Serial.println(' ');
  }

  for ( i = 0; i < 32 ; i++) {
    Serial.print('_');
  }
}

void print_gameover() {
  int i;
  for (i = 0; i < 16 ; i++) {
    Serial.print('_');
  }
  Serial.println(' ');

  Serial.println("####  GAME OVER ####");
  Serial.println("\tPOINT");
  Serial.print("\t");
  Serial.println(Point);
  Serial.print("Press key to RESTART!");

  for ( i = 0; i < 16 ; i++) {
    Serial.print('_');
  }
  Serial.println(' ');
}
