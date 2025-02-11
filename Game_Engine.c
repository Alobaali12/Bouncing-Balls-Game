/*
Author: Ali Mohamed
Date: 06/05/2022
Game Title: Bouncing balls
The code written below simulates a bouncing ball Game, where the user is given options to read the instructions, start or quit the game
and based on the users input the program will display the relevant screen, if the user decided to proceed with the game a registration
process will be displayed for the user to sign in then start the game, if the the user decided to read the instructions the file "Bouncing_Ball.txt"
will be read by the program to display the instructions, then if everything is all setup the program will execute the game engine where
the user gets to track a bouncing ball and not let it enter the private area otherwise lives will be lost. The program follows a algorithm
that keeps track of the ball and bat in the 2D plane with some boundaries. The score and lives will be displayed on the screen. The program
also follows a algorithm that makes the game more complex the more you proceed, and this was done by increasing the frame rates which
makes the illusion of the ball bouncing faster, moreover if the score exceeded a certain limit the game becomes more complex and another
ball will be added.
*/

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // sleep functions in ms
#include <conio.h> // hold console handle to be able to draw
#include <string.h> // drawing strings to the screen
#include <time.h> // delay time library
#define Score_com 70 //complexity VARIABLE
#define DELAY 6
//Player Info Structure
struct player{
char name[50];
int score;
};

// Game Parameters Structure
struct Game_Parameters{

// Coordinate reference frame values
    int SCREEN_WIDTH; // Maximum game horizontal width (x-axis)
    int SCREEN_HEIGHT; // Maximum game Vertical height (y-axis)
// Bat/Bar details
    int bat_y; // y-coordinate
    int bat_x; // x-coordinate
    int BAT_WIDTH; // maximum horizontal bat movement (x-axis)
    int BAT_SPEED; // bat speed columns per frame
    int ball_x;  // ball x-axis Coordinate
    int ball_y;  // ball y-axis Coordinate
    int lives;  // lives (3)
    int game_state; // game state Playing/Game Over/Staring
    int GAME_STATE_OVER;  // End game
    int GAME_STATE_PLAYING; // playing state variable
    int GOAL_POINTS; // game points incremental value
    int delta_x;  // ball speed on the x-axis
    int delta_y;  // ball speed on the y-axis
    int score;  // Score value
    // Previous Coordinate value to keep track of the update
    int prev_bat_x; // Previous bat x-axis Coordinate value
    int prev_bat_y; // Previous bat y-axis Coordinate value
    int prev_ball_x; // Previous ball x-axis Coordinate value
    int prev_ball_y; // Previous ball y-axis Coordinate value
    char ball_string[2]; // ball shape
    char bat_string[20]; // bat/bar shape
    char game_over_string[30]; // end game string message
    HANDLE _output_handle; // output handle
};

void print_at_xy(int x, int y, char val[],struct Game_Parameters *GP); // printing at a specified x and y coordinates
void display_score(struct Game_Parameters *GP);// displaying the score to the screen
void init(struct Game_Parameters *GP); // initializing all game parameters
int zero_lives(struct Game_Parameters *GP); // zero lives when game is over
void set_game_state_over(struct Game_Parameters *GP); // setting game over state
void update_ball(struct Game_Parameters *GP); // updating ball position
char get_input(); // getting user input and not waiting!.
void update_player(struct Game_Parameters *GP,char ch); // updating players position
int ball_collides_with_bat(struct Game_Parameters *GP); // ball colliding with bat function
void increment_score(struct Game_Parameters *GP); // incrementing score points
int ball_is_out_bounds(struct Game_Parameters *GP); // checking ball bounds must be within screen width
void decrement_lives(struct Game_Parameters *GP); // decrementing the scores
void draw(struct Game_Parameters *GP,struct Game_Parameters Gp, int i); // drawing on the screen
void clean_up(); // End game display
int ball_collides_with_wall(struct Game_Parameters *GP);// if ball collides with wall ball reverse direction
int ball_collides_with_ceiling(struct Game_Parameters *GP);// if ball collides with ceiling reverse direction
void capture_previous_pos(struct Game_Parameters *GP); // capturing previous ball and bat positions
void clear_screen(struct Game_Parameters *GP);// clearing screen, basically printing empty strings on specific xy coordinate from to create the illusion of clearing screen
void reset_ball(struct Game_Parameters *GP); // resting ball position
void display_message(const char *message,struct Game_Parameters *GP); // displaying messages to the screen
void hidecursor(struct Game_Parameters *GP, int tf); // hiding output handle cursor
void FILEread(char Fr[30]); // reading files
void GameEngine(struct Game_Parameters GP[]); // Main game engine algorithm
void P_detials(struct player *Player); // player detials
void wall(struct Game_Parameters *GP); // wall
void delay(int number_of_seconds); // delay function




// main function
void main()

{
    int One=1,s,p=1,sum=0; // initializing integer variavles
    struct Game_Parameters GP[2];// struct array
    struct player Pls; // player info
    char a,l;

    printf("WELCOME TO BOUNCING BALL !!\n\n");
    FILEread("Bouncing_Ball.txt"); // Title

    FILEread("Menu.txt");
    printf("\n");
    do{
        fflush(stdin);
        scanf("%d", &s);
    }while(s!=1 && s!=2 && s!=3);

    if( s==1) // main loop start
    {
            clear_screen(&GP[1]);
            printf("Registration:\n\n"); // registration
            P_detials(&Pls);

        do{
            system("@cls||clear");
            GameEngine(GP);
            system("@cls||clear");
            hidecursor(&GP[1],0);
             do
             {
                 fflush(stdin);
                 clean_up();
                 scanf("%c",&l);
             }while(l!='N' && l!='Y');
         }while(l=='Y');


    }
    else if(s==2) // registration loop
    {
        FILEread("Instructions.txt");
        printf("\n\nRegistration:\n\n");
        P_detials(&Pls);
        fflush(stdin);
         do{
            system("@cls||clear");
            GameEngine(GP);
            system("@cls||clear");
            hidecursor(&GP[1],0);
             do
             {
               fflush(stdin);
               clean_up();
               scanf("%c",&l);
             }while(l!='N' && l!='Y');
        }while(l=='Y');
    }

        printf("Thanks for your attention");


}
void GameEngine(struct Game_Parameters GP[])
{
    int i=1, One=1;
    char c;
    fflush(stdin);
    printf("READY !!? (press anything)\n");
    scanf("%c",&c);
    fflush(stdin);
    init(&GP[1]);
    init(&GP[2]);
    printf("\ninit");
    system("@cls||clear");

    //1000/30
    //game loop
    // one frame

    while(One==1){

        clear_screen(&GP[1]);
        clear_screen(&GP[2]);
        //wall(&GP[1]);

        // if lives are zero then game over
        if(zero_lives(&GP[1])){
            set_game_state_over(&GP[1]);
            print_at_xy((GP[1]).SCREEN_WIDTH/2 - 10, (GP[1]).SCREEN_HEIGHT/2, (GP[1]).game_over_string,&GP[1]);
            display_message("Enter 'q' to quit...",&GP[1]);
        }

        // get user input, but not wait!
        char ch = get_input();

        //if game is over then clear screen and quit
        if(GP[1].game_state == GP[1].GAME_STATE_OVER && ch == 'q'){
                One=2;
                system("@cls||clear"); // system clearing screen
            }

        // Game loop if game state is playing
        if(GP[1].game_state == GP[1].GAME_STATE_PLAYING){
            //wall(&GP[1]);
            clear_screen(&GP[1]); // clearing the screen but only were bat and ball is basically drawing empty strings, therefore not similar to system clear, clearing previous positions
            capture_previous_pos(&GP[1]); // capturing previous position
            update_ball(&GP[1]); // updating ball position
            update_player(&GP[1],ch); // updating bat position

            // if the ball collides with the wall
            if(ball_collides_with_wall(&GP[1])){
                GP[1].delta_x *= -1; // reverse direction , multiplying it by -1
            }


            // if ball collides with ceiling
            if(ball_collides_with_ceiling(&GP[1])){
                GP[1].delta_y *= -1; // reverse direction
            }

            // if ball collides with bat
            if(ball_collides_with_bat(&GP[1])){

                display_message("OH YEAH!!!",&GP[1]); // display this message

                if(GP[1].delta_y > 0){ // if the ball is moving down hence delta_y > 0
                    GP[1].delta_y *= -1; // then reverse its direction
                }

                increment_score(&GP[1]); //increment score
            }
            // if the ball is out of bounds
            if(ball_is_out_bounds(&GP[1])){
                decrement_lives(&GP[1]); // decrement the lives
                reset_ball(&GP[1]); // reset ball
                display_message("OOPSIE!!",&GP[1]); // displaying this message
            }

            // Frame rate update
            if(GP[1].score>=Score_com)
            {
                clear_screen(&GP[2]);
                capture_previous_pos(&GP[2]); // capturing previous position
                update_ball(&GP[2]); // updating ball position
                           // if the balSl collides with the wall
               if(ball_collides_with_wall(&GP[2])){
                  GP[2].delta_x *= -1; // reverse direction , multiplying it by -1
                }


            // if ball collides with ceiling
               if(ball_collides_with_ceiling(&GP[2])){
                  GP[2].delta_y *= -1; // reverse direction
                }

            // if ball collides with bat


            // solving the collision ball 2 here!!!!!!!!!!!!!!
                if(GP[1].bat_y == GP[2].ball_y || (GP[1].bat_y == GP[2].ball_y-1)){ //if bat in the y and the ball in the y are in the same vertical plain or minus -1 "ball is 1 above bat"

                    if(GP[1].bat_x <= GP[2].ball_x && (GP[1].bat_x + GP[1].BAT_WIDTH) >= GP[2].ball_x){ // checking the horizontal plain and making sure the the ball is touching the bat

                        display_message("OH YEAH!!!",&GP[2]); // display this message

                           if(GP[2].delta_y > 0){ // if the ball is moving down hence delta_y > 0
                               GP[2].delta_y *= -1; // then reverse its direction
                             }
                       }
                    increment_score(&GP[1]); //increment score
                }

            // if the ball is out of bounds
               if(ball_is_out_bounds(&GP[2])){
                    decrement_lives(&GP[1]); // decrement the lives
                    reset_ball(&GP[2]); // reset ball
                    display_message("OOPSIE!!",&GP[2]); // displaying this message
            }

            // drawing to the screen
            draw(&GP[1],GP[1],i);
            draw(&GP[2],GP[2],i+1);
                Sleep(30);
            }
            else{
                draw(&GP[1],GP[1],i);
                Sleep(100-GP[1].score+2);
            }

        }
    }
printf("while loop failed");
}

void hidecursor(struct Game_Parameters *GP, int tf) // hide the output handle
{
    if (tf==1)
    {
       (*GP)._output_handle = GetStdHandle(STD_OUTPUT_HANDLE); //getting standard output handle
       CONSOLE_CURSOR_INFO info; //parameters
       info.dwSize = 100;
       info.bVisible = FALSE;
       SetConsoleCursorInfo((*GP)._output_handle, &info);
    }
    else{
           (*GP)._output_handle = GetStdHandle(STD_OUTPUT_HANDLE); //getting standard output handle
           CONSOLE_CURSOR_INFO info; //parameters
           info.dwSize = 100;
           info.bVisible = TRUE;
           SetConsoleCursorInfo((*GP)._output_handle, &info);
    }

}

// initializing game parameter values
void init(struct Game_Parameters *GP){
    (*GP).score = 0;
    (*GP).lives = 3;
    (*GP).GOAL_POINTS = 10;
    (*GP).GAME_STATE_OVER = 1;
    (*GP).GAME_STATE_PLAYING = 2;
    (*GP).SCREEN_WIDTH = 40;
    (*GP).SCREEN_HEIGHT = 20;
    (*GP).bat_y = (*GP).SCREEN_HEIGHT;
    (*GP).BAT_WIDTH = 8;
    (*GP).BAT_SPEED = 2;
   (*GP).bat_x = ((*GP).SCREEN_WIDTH/2) - ((*GP).BAT_WIDTH/2);
    (*GP).delta_x = 1;
   (*GP).delta_y = -1;
    (*GP).ball_x = ((*GP).SCREEN_WIDTH/2);
    (*GP).ball_y = (*GP).SCREEN_HEIGHT;
    (*GP).game_state = (*GP).GAME_STATE_PLAYING;
    strcpy((*GP).bat_string, "========");
    strcpy((*GP).ball_string, "O");
    strcpy((*GP).game_over_string, "GAME OVER");
    hidecursor(&(*GP),1);

    (*GP).prev_bat_x = (*GP).bat_x;
    (*GP).prev_bat_y = (*GP).bat_y;
    (*GP).prev_ball_x = (*GP).ball_x;
    (*GP).prev_ball_y = (*GP).ball_y;
}
// reset ball back to it position
void reset_ball(struct Game_Parameters *GP){
    int i=1;
    (*GP).ball_x = (*GP).SCREEN_WIDTH/2;
    (*GP).ball_y = (*GP).SCREEN_HEIGHT;
    (*GP).delta_x = 1;
    (*GP).delta_y = -1;
}

// very basic function to check the lives
int zero_lives(struct Game_Parameters *GP){
    if((*GP).lives == 0){
        return 1; // if lives are zero return 1
    }
    return 0; // otherwise return zero
}
// setting the game state to over
void set_game_state_over(struct Game_Parameters *GP){
    (*GP).game_state = (*GP).GAME_STATE_OVER;
}
// updating ball
void update_ball(struct Game_Parameters *GP){
    (*GP).ball_x += (*GP).delta_x; // basic addition
    (*GP).ball_y += (*GP).delta_y;
}

// SO basically this function is designed to keep the game going and not wait for an input
char get_input(){
    char ch = 0;     // just settling it to zero first

    if(kbhit()){ // check if a key is pressed down kbhit() function is from the conio library
        ch = getch(); // if so then get that input
    }

    return ch; // return input
}
// updating player
void update_player(struct Game_Parameters *GP,char ch){
    if(ch == 'a' || ch == 'A') // moving left
    {
        (*GP).bat_x -= (*GP).BAT_SPEED;
    }
    else if(ch == 'd' || ch =='D'){ // moving right
        (*GP).bat_x += (*GP).BAT_SPEED;
    }

    if((*GP).bat_x < 0){ //checking to avoid moving out of bounds
        (*GP).bat_x = 0;
    }
    // checking to avoid bat going of the screen to the right
    if((*GP).bat_x + (*GP).BAT_WIDTH > (*GP).SCREEN_WIDTH){ // checking right edge of the bat against the screen width
        (*GP).bat_x = (*GP).SCREEN_WIDTH - (*GP).BAT_WIDTH; // if so just limiting it within that screen width range
    }
}

// collision detection (if the ball is touching the bat)
int ball_collides_with_bat(struct Game_Parameters *GP){
    if((*GP).bat_y == (*GP).ball_y || ((*GP).bat_y == (*GP).ball_y-1)){ //if bat in the y and the ball in the y are in the same vertical plain or minus -1 "ball is 1 above bat"
        if((*GP).bat_x <= (*GP).ball_x && ((*GP).bat_x + (*GP).BAT_WIDTH) >= (*GP).ball_x){ // checking the horizontal plain and making sure the the ball is touching the bat
            return 1; // return 1
        }
    }

    return 0; // otherwise return zero
}

// incrementing the score
void increment_score(struct Game_Parameters *GP){
    (*GP).score += (*GP).GOAL_POINTS; // very simple just adding score points
}

// checking if the ball is out of bounds
int ball_is_out_bounds(struct Game_Parameters *GP){
    if((*GP).ball_y > (*GP).SCREEN_HEIGHT + 3){ // Well the full screen could have been used in this game however I have decided to limit my screen height
        return 1; // if true return 1
    }

    return 0; // otherwise return 0
}
// decrement lives
void decrement_lives(struct Game_Parameters *GP){
    (*GP).lives--;
}
// drawing of printing to the screen
void draw(struct Game_Parameters *GP,struct Game_Parameters Gp,int i){
    if (i==1)
    {
            print_at_xy((*GP).ball_x, (*GP).ball_y, (*GP).ball_string,&Gp); // taking the x and y coordinates of the ball and printing the ball string on there!.
            print_at_xy((*GP).bat_x, (*GP).bat_y, (*GP).bat_string, &Gp); // similar method taking the x and y coordinates of the bat and printing the bat string on there!.
            display_score(&Gp); // displaying the score at the zero position of the screen
    }
    else{
            print_at_xy((*GP).ball_x, (*GP).ball_y, (*GP).ball_string, &Gp);
       // display_score(&Gp);
    }
}
// key function to actually draw to the screen, where I pass the x and y coordinates to draw a string
void print_at_xy(int x, int y, char val[],struct Game_Parameters *GP)
{
  COORD coord; // struct coordinate system from the conio library
  coord.X = x; // setting x-coordinate on the screen
  coord.Y = y; // setting x-coordinate on the screen
  SetConsoleCursorPosition((*GP)._output_handle, coord); //Set the Console cursor position
  printf("%s", val);
  fflush(stdout);
}
// displaying the score
void display_score(struct Game_Parameters *GP){
    char buffer[50] = {0};
    sprintf(buffer, "SCORE: %4d LIVES: %d", (*GP).score, (*GP).lives); // similar to printf but just storing the whole statement on to the buffer
    print_at_xy(0, 0, buffer, &(*GP)); // then passing the buffer to the print_at_xy function to display the score on top of the screen
}

// if the ball collided with the wall then pass this condition
int ball_collides_with_wall(struct Game_Parameters *GP){
    return (*GP).ball_x <= 0 || ((*GP).ball_x-1) >= (*GP).SCREEN_WIDTH;
}
// if the ball collided with the ceiling then pass this condition
int ball_collides_with_ceiling(struct Game_Parameters *GP){
    return (*GP).ball_y <= 0;
}
// just capturing the previous position
void capture_previous_pos(struct Game_Parameters *GP){
    (*GP).prev_ball_x = (*GP).ball_x;
    (*GP).prev_ball_y = (*GP).ball_y;
    (*GP).prev_bat_x = (*GP).bat_x;
    (*GP).prev_bat_y = (*GP).bat_y;
}
// clearing the screen
void clear_screen(struct Game_Parameters *GP){
    char buffer[] = "                      "; // well actually I'm printing this empty to my specified coordinates and that makes the illusion that I'm clearing the screen, not same as system clearing!.
    print_at_xy((*GP).prev_ball_x, (*GP).prev_ball_y, buffer, &(*GP)); // printing the empty buffer at the specified x and y coordinates
    print_at_xy((*GP).prev_bat_x, (*GP).prev_bat_y, buffer, &(*GP));
    print_at_xy(0, 0, buffer, &(*GP));// clearing score board
}
// display message which as to receive an argument of a string
void display_message(const char *message, struct Game_Parameters *GP){
    char buffer[100] = {0}; // empty buffer
    strcpy(buffer, message); // copying message to that buffer
    print_at_xy((*GP).SCREEN_WIDTH/2 - strlen(message)/2,(*GP).SCREEN_HEIGHT/2 - 1, buffer, &(*GP)); // printing it to the screen
}
// just printing this at the end
void clean_up(){
    printf("Thanks for playing would you like another try? (Y/N)\n");
}
// reading files
void FILEread(char Fr[30]){

FILE *READ;
char a;
READ= fopen(Fr,"r");
if (READ==NULL)
{
    printf("FILE doesn't Exist\n");
}
else{
    while(fscanf(READ,"%c",&a)!=EOF)
            {
                printf("%c",a);
            }
}
}

void P_detials(struct player *Player) // player details
{
    char name[50];
    printf("Enter Player name: ");
    fflush(stdin);
    gets(name);
    strcpy((*Player).name,name);
    printf("\nHello %s, bare in mind the game complexity\nincreases the further you proceed\n  SO !!! WATCH OUT !!!", (*Player).name);
    delay(DELAY);
    fflush(stdin);

}
void wall(struct Game_Parameters *GP) // wall
{
    char test[]="||||";
    for (int i=0; i<(*GP).SCREEN_HEIGHT ;i++)
    {
        print_at_xy((*GP).SCREEN_WIDTH,i,test,&(*GP));

    }
}
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}


