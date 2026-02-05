#include "raylib.h"

#define COLS 12 //2 extra columns to the walls
#define ROWS 21 //1 extra wall to the floor
#define BOARD_X_AXIS 50
#define BOARD_Y_AXIS 70
#define SQUARE_SIZE 24

typedef enum CellState{
  EMPTY, MOVING_PIECE, PLACED_PIECE, CLEAN_LINE, BOARD_LIMIT
}CellState;

static CellState grid[COLS][ROWS];

typedef enum MainMenu{
  MAINSCREEN = 0, GAMEPLAY, SETTINGS
}MainMenu;

typedef enum ThemeOptions{
  PURPLE_THEME,RED_THEME, GREEN_THEME,BLUE_THEME, YELLOW_THEME, ORANGE_THEME, THEME_COUNT
}ThemeOptions;

typedef struct ThemeColors{
  Color background;
  Color text;
  Color highlight;
  const char *name;
}ThemeColors;

static const ThemeColors Themes[THEME_COUNT] = {
  [PURPLE_THEME]={PURPLE, DARKPURPLE, BLACK, "Purple"},
  [RED_THEME]={RED, MAROON, RAYWHITE, "Red"},
  [GREEN_THEME]={GREEN, DARKGREEN, LIME, "Green"},
  [BLUE_THEME]={BLUE, DARKBLUE, SKYBLUE, "Blue"},
  [YELLOW_THEME]={YELLOW, GOLD, (Color){111,118,17,255}, "Yellow"},
  [ORANGE_THEME]={ORANGE, (Color){230,76,20,255},(Color){168,63,24,255} , "Orange"},
};

static void GenerateGrid(){
  for(int x = 0; x < COLS; x++){
    for(int y = 0; y < ROWS; y++){
      if (x == 0 || x == COLS - 1 || y == ROWS - 1){
        grid[x][y] = BOARD_LIMIT;
      } else {
        grid[x][y] = EMPTY;
      }
    }
  }
}

static void GridGraphic(){
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLS; x++)
        {
            int xPos = BOARD_X_AXIS + x * SQUARE_SIZE;
            int yPos = BOARD_Y_AXIS + y * SQUARE_SIZE;

            switch (grid[x][y])
            {
                case EMPTY:
                    DrawRectangleLines(xPos, yPos, SQUARE_SIZE, SQUARE_SIZE, LIGHTGRAY);
                    break;

                case MOVING_PIECE:
                    DrawRectangle(xPos, yPos, SQUARE_SIZE, SQUARE_SIZE, DARKGRAY);
                    break;

                case PLACED_PIECE:
                    DrawRectangle(xPos, yPos, SQUARE_SIZE, SQUARE_SIZE, GRAY);
                    break;

                case CLEAN_LINE:
                    DrawRectangle(xPos, yPos, SQUARE_SIZE, SQUARE_SIZE, ORANGE);
                    break;

                case BOARD_LIMIT:
		  DrawRectangle(xPos, yPos, SQUARE_SIZE, SQUARE_SIZE, LIGHTGRAY);
                    break;
            }
        }
    }
}

static int frameCounter=0;

static void UpdateGameplay(){
    frameCounter++;
    if (frameCounter >= 60)//60fps
    {
        frameCounter = 0;
    }
}


int main() {
  const int screenWidth = 800;
  const int screenHeight = 600;
  const char *title = "TETRIS BETA V0.1 by edutavr";
  int fontSize = 45; 

  InitWindow(screenWidth, screenHeight, "TETRIS BETA V0.1");

  MainMenu currentScreen = MAINSCREEN;
  
  ThemeOptions currentTheme = PURPLE_THEME;
  
  SetTargetFPS(60);
  
  int textWidth = MeasureText(title, fontSize);
  int textWidth2 = MeasureText("Play Game", 40);
  int textWidth3 = MeasureText("Settings", 40);

  
  int centerTitle = (screenWidth - textWidth)/2;
  int centerPlay = (screenWidth - textWidth2)/2;
  int centerSettings = (screenWidth - textWidth3)/2;


  Rectangle playButton = { centerPlay, 240, textWidth2, 40 };
  Rectangle settingsButton = { centerSettings, 300, textWidth3, 40 };

  
  while (!WindowShouldClose()) {

    Color bgColor = Themes[currentTheme].background;
    Color textBase = Themes[currentTheme].text;
    Color highlight = Themes[currentTheme].highlight;

    Vector2 mousePoint = GetMousePosition();

    int textWidth5 = MeasureText("TELA DO JOGO",20);
    Rectangle backToTheBegginning={20,20,textWidth5,20};

    const char *themeLabel = TextFormat("Theme: %s", Themes[currentTheme].name);
    int textWidth4 = MeasureText(themeLabel, 20);
    Rectangle themeButton = {centerPlay+25, 550, textWidth4, 20};
      
    Color titleColor = textBase;
    Color playColor = textBase;
    Color settingsColor = textBase;
    Color themeColor = textBase;

    switch(currentScreen){
     case MAINSCREEN:{
       if (CheckCollisionPointRec(mousePoint, playButton)) {
  	 if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
	   GenerateGrid();
	   currentScreen = GAMEPLAY;
	 }
       }
       if (CheckCollisionPointRec(mousePoint, settingsButton)) {
	 if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
	   currentScreen = SETTINGS;
	 }
       }
       if (CheckCollisionPointRec(mousePoint, themeButton)) {
	 if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
	   currentTheme = (ThemeOptions)((currentTheme + 1) % THEME_COUNT);
	 }
       }
     }break;
     case GAMEPLAY:{
       if (CheckCollisionPointRec(mousePoint, backToTheBegginning)) {
	 if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
	   currentScreen = MAINSCREEN;
	 }
       }
     }break;
    }
    BeginDrawing();
    switch(currentScreen){
    //draw-menu-----------------------------------------------------------
    case(MAINSCREEN):      
      ClearBackground(bgColor);
      DrawText(title, centerTitle, 20, fontSize, titleColor);
      if (CheckCollisionPointRec(mousePoint, playButton)) {
	playColor = highlight;
      }
      DrawText("Play Game", centerPlay , 240, 40, playColor);
      if (CheckCollisionPointRec(mousePoint, settingsButton)) {
	settingsColor = highlight;
      }
      DrawText("Settings", centerSettings , 300, 40, settingsColor);
      DrawText(themeLabel, centerPlay + 25 , 550, 20, themeColor);
      break;
      
    case(GAMEPLAY):{
      UpdateGameplay();
      ClearBackground(BLACK);
      DrawText("TELA DO JOGO", 20, 20, 20, WHITE);
      GridGraphic();
      break;}

    case(SETTINGS):
      ClearBackground(bgColor);
      DrawText("SETTINGS", 20, 20, 40, DARKPURPLE);
      break;      
    }
    EndDrawing();
    //draw-menu----------------------------------------------------------- 
   
  }
  
  CloseWindow();
  return 0;
}
