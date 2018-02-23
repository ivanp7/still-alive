// Portal 1 Ending Credits remake
// Written by Ivan Podmazov, 2011
// Remade for Linux in 2016

// GLaDOS 3.11 - [Still Alive]

#include "draw.h"

void Song(const char* path);
void MakeBSOD(const char* causer);

int main(int argc, char* argv[])
{
  InitializeScreen();

  screenSizeX = GetScreenSizeX();
  screenSizeY = GetScreenSizeY();

  if ((screenSizeX < 80) || (screenSizeY < 25)) {
    ReleaseScreen();
    return 1;
  }

  InitColorPair(1, COLOR_YELLOW, COLOR_BLACK);
  SetClearColorPair(1);
  SetWriteColorPair(1);
  ClearScreen();

  HideCursor();

  char ch;
  for (int y = 0; y < screenSizeY; y++) {
    for (int x = 0; x < screenSizeX; x++) {
      if((y == 0) || ((y == screenSizeY - pictureSizeY - 1) &&
                      (x > screenSizeX - pictureSizeX - 1)) ||
         ((y == screenSizeY - 1) && (x <= screenSizeX - pictureSizeX - 1)))
        PutChar('-', x, y);
      else if((x == 0) || (x == screenSizeX - pictureSizeX - 1) ||
              ((x == screenSizeX - 1) && (y < screenSizeY - pictureSizeY)))
        PutChar('|', x, y);
    }
  }

  PutText("THANK YOU FOR PARTICIPATING", screenSizeX - pictureSizeX,
          (screenSizeY - pictureSizeY) / 2 - 1);
  PutText("IN THIS", screenSizeX - pictureSizeX, (screenSizeY - pictureSizeY) / 2);
  PutText("ENRICHMENT CENTER ACTIVITY!!", screenSizeX - pictureSizeX,
          (screenSizeY - pictureSizeY) / 2 + 1);

  char path[256];
  strcpy(path, argv[0]);
  strcat(path, ".wav");

  Song(path);

  MakeBSOD("GlaDOS");

  InitColorPair(1, COLOR_WHITE, COLOR_BLACK);
  SetClearColorPair(1);
  ClearScreen();
  RefreshScreen();

  ReleaseScreen();
}

void Song(const char* path)
{
  WriteLine("Forms FORM-29827281-12:");
  WriteLine("Test Assesment Report");
  WriteLine("");
  WriteLine("", 1000);

  PlayMusic(path);
  SleepFor(400);

  WriteLine("This was a triumph.", 2000);
  WriteLine("I'm making a note here:", 200);
  WriteLine("HUGE SUCCESS.", 1500, 120);
  WriteLine("It's hard to overstate");
  WriteLine("my satisfaction.", 3700);
  DrawPicture("aperture");
  WriteLine("Aperture Science", 2200);
  WriteLine("We do what we must");
  WriteLine("because we can.", 2700);
  WriteLine("For the good of all of us.", 1600, 90);
  DrawPicture("radioactive");
  WriteLine("Except the ones who are dead.", 420, 70);
  WriteLine("");
  WriteLine("But there's no sence crying", 0, 90);
  WriteLine("over every mistake.", 0, 90);
  WriteLine("You just keep on trying", 0, 90);
  WriteLine("till you run out of cake.", 200, 90);
  DrawPicture("atom");
  WriteLine("And the Science gets done.", 0, 90);
  WriteLine("And you make a neat gun.", 0, 90);
  DrawPicture("aperture");
  WriteLine("For the people who are", 0, 90);
  WriteLine("still alive.", 1500, 90);
  ClearArea();

  WriteLine("FORMS Form-55551-5:", 0, 70);
  WriteLine("Personnel File Addendum:", 100, 70);
  WriteLine("");
  WriteLine("Dear <<Subject Name Here>>,");
  WriteLine("");
  WriteLine("I'm not even angry.", 2800);
  WriteLine("I'm being so sincere right now.", 2000);
  DrawPicture("heart_broken");
  WriteLine("Even though you broke my heart.", 1100);
  WriteLine("And killed me.", 2200);
  DrawPicture("explosion");
  WriteLine("And tore me into pieces.", 2100);
  DrawPicture("fire");
  WriteLine("And threw every piece into a fire.", 2200);
  WriteLine("As they burned it hurt because", 500);
  DrawPicture("ok");
  WriteLine("I was so happy for you!", 1000);
  WriteLine("Now these points of data", 0, 90);
  WriteLine("make a beautiful line.", 0, 90);
  WriteLine("And we're out of beta.", 0, 90);
  WriteLine("We're releasing on time.", 200, 90);
  DrawPicture("explosion");
  WriteLine("So I'm GLaD. I got burned.", 0, 90);
  DrawPicture("atom");
  WriteLine("Think of all the things we learned", 0, 90);
  DrawPicture("aperture");
  WriteLine("for the people who are", 0, 90);
  WriteLine("still alive.", 1500, 90);
  ClearArea();

  WriteLine("FORMS Form-55551-6:", 0, 70);
  WriteLine("Personnel File Addendum Addendum:", 400, 70);
  WriteLine("");
  WriteLine("One last thing:", 200);
  WriteLine("");
  WriteLine("Go ahead and leave me.", 2100);
  WriteLine("I think I prefer to stay inside.", 2200);
  WriteLine("Maybe you'll find someone else", 1200);
  WriteLine("to help you.", 2300);
  DrawPicture("blackmesa");
  WriteLine("Maybe Black Mesa...", 2000, 120);
  WriteLine("THAT WAS A JOKE. FAT CHANCE.", 2000, 130);
  DrawPicture("cake");
  WriteLine("Anyway, this cake is great.", 1000);
  WriteLine("It's so delicious and moist.", 100, 90);
  DrawPicture("glados");
  WriteLine("Look at me still talking", 0, 90);
  DrawPicture("radioactive");
  WriteLine("when there's Science to do.", 0, 90);
  DrawPicture("aperture");
  WriteLine("When I look out there,", 0, 90);
  WriteLine("it makes me GLaD I'm not you.", 0, 90);
  DrawPicture("atom");
  WriteLine("I've experiments to run.", 0, 90);
  DrawPicture("explosion");
  WriteLine("There is research to be done.", 0, 90);
  DrawPicture("aperture");
  WriteLine("On the people who are", 0, 90);
  WriteLine("still alive.", 1200, 90);
  ClearArea();

  WriteLine("");
  WriteLine("");
  WriteLine("PS: And believe me I am", 0, 90);
  WriteLine("still alive.", 600, 90);
  WriteLine("PPS: I'm doing Science and I'm", 0, 90);
  WriteLine("still alive.", 600, 90);
  WriteLine("PPPS: I feel FANTASTIC and I'm", 0, 90);
  WriteLine("still alive.", 100, 90);
  WriteLine("");
  WriteLine("FINAL THOUGHT:", 0, 90);
  WriteLine("While you're dying I'll be", 0, 90);
  WriteLine("still alive.", 0, 90);
  WriteLine("FINAL THOUGHT PS:", 0, 60);
  WriteLine("And when you're dead I will be", 0, 80);
  WriteLine("still alive.", 300);
  WriteLine("");
  WriteLine("");
  WriteLine("STILL ALIVE", 5000, 120);
  ClearArea();

  /* // from Windows version

     PlaySound("song_loop", 0, SND_RESOURCE | SND_ASYNC | SND_NODEFAULT | SND_PURGE | SND_LOOP);

     for(int y = 0; y < screenSizeY; y++){
     for(int x = 0; x < screenSizeX; x++)
     PutChar(' ', x, y);
     }

     Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
     DrawPicture("cake", screenSizeX - pictureSizeX - 3, screenSizeY - pictureSizeY - 3);

     Color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
     DrawPicture("heart", 3, 3, false);

     Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
     PutText("Portal is awesome! =)", (screenSizeX - 21) / 2, 0);
     PutText("Ivan Podmazov", screenSizeX - 14, screenSizeY - 1);

     for(;;){
     PutChar('_', screenSizeX - 1, screenSizeY - 1);
     Sleep(500);
     PutChar(' ', screenSizeX - 1, screenSizeY - 1);
     Sleep(500);
     }
  */
}

#include <string>

void MakeBSOD(const char* causer)
{
  using std::string;

  string errorCauser = causer;

  errorCauser = " " + errorCauser + " ";

  int errorCauserShift = (screenSizeX - errorCauser.length()) / 2;

  //******************************************//

  srand(time(NULL));
  char A[] = "0123456789ABCDEF";

  string errorRandomNumbers[5];

  for(int i = 0; i < 2; i++)
    errorRandomNumbers[0] += A[rand() % 16];

  for(int i = 0; i < 2; i++)
    errorRandomNumbers[1] += A[rand() % 16];
  errorRandomNumbers[1] = "00" + errorRandomNumbers[1];

  for(int i = 0; i < 8; i++)
    errorRandomNumbers[2] += A[rand() % 16];

  for(int i = 0; i < 1; i++)
    errorRandomNumbers[3] += A[rand() % 10];
  errorRandomNumbers[3] = "0" + errorRandomNumbers[3];

  for(int i = 0; i < 8; i++)
    errorRandomNumbers[4] += A[rand() % 16];

  string reason[2];
  reason[0] = "A fatal exception " + errorRandomNumbers[0] + " has occured at " +
    errorRandomNumbers[1] + ":" + errorRandomNumbers[2] + " in VXD VMM(" +
    errorRandomNumbers[3] + ") +";
  reason[1] = errorRandomNumbers[4] + ". The current application will be terminated.";

  int reasonShift = (screenSizeX - reason[0].length()) / 2;

  //******************************************//

  string message[3];
  message[0] = "* Press any key to terminate current application.";
  message[1] = "* Press CTRL+ALT+DEL again to restart your computer. You will";
  message[2] = "  lose any unsaved information in all applications.";

  //******************************************//

  string prompt = "Press any key to continue";

  int promptShift = (screenSizeX - prompt.length()) / 2;

  //******************************************//

  InitColorPair(1, COLOR_WHITE, COLOR_BLUE);
  InitColorPair(2, COLOR_BLUE, COLOR_WHITE);
  SetClearColorPair(1);
  ClearScreen();

  int posX, posY;

  posX = errorCauserShift;
  posY = (screenSizeY - 10) / 2;
  MoveCursorTo(posX, posY);

  SetWriteColorPair(2);
  WriteString(errorCauser.c_str());

  posX = reasonShift;
  posY += 2;
  MoveCursorTo(posX, posY);
  SetWriteColorPair(1);
  WriteString(reason[0].c_str());

  posY++;
  MoveCursorTo(posX, posY);
  WriteString(reason[1].c_str());

  posY += 2;
  MoveCursorTo(posX, posY);
  WriteString(message[0].c_str());

  posY++;
  MoveCursorTo(posX, posY);
  WriteString(message[1].c_str());

  posY++;
  MoveCursorTo(posX, posY);
  WriteString(message[2].c_str());

  posY += 2;
  posX = promptShift;
  MoveCursorTo(posX, posY);
  WriteString(prompt.c_str());

  posX += prompt.length() + 1;
  MoveCursorTo(posX, posY);

  ShowCursor();

  RefreshScreen();
  getch();
}
