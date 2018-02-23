#include "output.h"

int screenSizeX = 80;
int screenSizeY = 40;

const int pictureSizeX = 41;
const int pictureSizeY = 20;

void PutChar(char ch, int x, int y)
{
  MoveCursorTo(x, y);

  char str[2] = {ch, '\0'};
  WriteString(str);
  RefreshScreen();
}

void PutText(const char* text, int x, int y, int size_x = -1)
{
  int len = strlen(text);
  if (size_x < 0)
    size_x = len;

  for (int i = 0; i < size_x; i++) {
    if (i < len)
      PutChar(text[i], x+i, y);
    else
      PutChar(' ', x+i, y);
  }
}

int CurPos = 0;
void WriteLine(const char* text, int wait = 0, int speed = 100)
{
  int c = 0;

  if(text[0] == '\0')
    SleepFor(100);

  int x = 1;
  while(text[c] != '\0'){
    PutChar('_', x + 1, CurPos + 1);
    PutChar(text[c], x, CurPos + 1);

    if(text[c] != ' ')
      SleepFor(speed);

    c++;
    x++;
  }
  PutChar(' ', x, CurPos + 1);

  CurPos++;
  PutChar('_', 1, CurPos + 1);

  SleepFor(wait);
}

void ClearArea()
{
  for(int y = 1; y < screenSizeY - 1; y++){
    for(int x = 1; x < screenSizeX - pictureSizeX - 1; x++)
      PutChar(' ', x, y);
  }

  CurPos = 0;

  PutChar('_', 1, CurPos + 1);
}

void DrawPicture(const char* image, int X = -1, int Y = -1)
{
  if(X == -1)
    X = screenSizeX - pictureSizeX;

  if(Y == -1)
    Y = screenSizeY - pictureSizeY;

  if(strcmp(image, "aperture") == 0){
    PutText("              .,-:;//;:=,               ", X, Y + 0, pictureSizeX);
    PutText("          . :H@@@MM@M#H/.,+%;,          ", X, Y + 1, pictureSizeX);
    PutText("       ,/X+ +M@@M@MM%=,-%HMMM@X/,       ", X, Y + 2, pictureSizeX);
    PutText("     -+@MM; $M@@MH+-,;XMMMM@MMMM@+-     ", X, Y + 3, pictureSizeX);
    PutText("    ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.   ", X, Y + 4, pictureSizeX);
    PutText("  ,>MM@@MH ,@%=            .---=-=:=,.  ", X, Y + 5, pictureSizeX);
    PutText("  =@#@@@MX .,              -%HX$$%%%+;  ", X, Y + 6, pictureSizeX);
    PutText(" =-./@M@M$                  .;@MMMM@MM: ", X, Y + 7, pictureSizeX);
    PutText(" X@/.-$MM/                    .+MM@@@M$ ", X, Y + 8, pictureSizeX);
    PutText(",@M@H: :@:                    . =X#@@@@-", X, Y + 9, pictureSizeX);
    PutText(",@@@MMX, .                    /H- ;@M@M=", X, Y + 10, pictureSizeX);
    PutText(".H@@@@M@+,                    %MM+..%#$.", X, Y + 11, pictureSizeX);
    PutText(" /MMMM@MMH/.                  XM@MH, =; ", X, Y + 12, pictureSizeX);
    PutText("  /%+%$XHH@$=              , .H@@@@MX,  ", X, Y + 13, pictureSizeX);
    PutText("   .=--------.           -%H.,@@@@@MX,  ", X, Y + 14, pictureSizeX);
    PutText("   .%MM@@@HHHXX$$$%+- .:$MMX =M@@MM%.   ", X, Y + 15, pictureSizeX);
    PutText("     =XMMM@MM@MM#H;,-+HMM@M+ /MMMX=     ", X, Y + 16, pictureSizeX);
    PutText("       =%@M@M#@$-.=$@MM@@@M: %M<=       ", X, Y + 17, pictureSizeX);
    PutText("         ,:+$+-,/H#MMMMMMM@= =,         ", X, Y + 18, pictureSizeX);
    PutText("               =++%%%%+/:-              ", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "explosion") == 0){
    PutText("            .+                          ", X, Y + 0, pictureSizeX);
    PutText("             /M.                        ", X, Y + 1, pictureSizeX);
    PutText("              H#@:              ,.      ", X, Y + 2, pictureSizeX);
    PutText("              -###H-          -@/       ", X, Y + 3, pictureSizeX);
    PutText("               %####$.  -;  .%#X        ", X, Y + 4, pictureSizeX);
    PutText("                M#####+;#H :M#M.        ", X, Y + 5, pictureSizeX);
    PutText("..          .+/;%#########X###-         ", X, Y + 6, pictureSizeX);
    PutText(" -/%H%+;-,    +##############/          ", X, Y + 7, pictureSizeX);
    PutText("    .:$M###MH$%+############X  ,--=;-   ", X, Y + 8, pictureSizeX);
    PutText("        -/H#####################H+=.    ", X, Y + 9, pictureSizeX);
    PutText("            +#################X.        ", X, Y + 10, pictureSizeX);
    PutText("         =%M####################H;.     ", X, Y + 11, pictureSizeX);
    PutText("            /@###############+;;/%%;,   ", X, Y + 12, pictureSizeX);
    PutText("         -%###################$.        ", X, Y + 13, pictureSizeX);
    PutText("       ;H######################M=       ", X, Y + 14, pictureSizeX);
    PutText("    ,%#####MH$%:+#####M###-/@####%      ", X, Y + 15, pictureSizeX);
    PutText("  :$H%+;=-      -####X.,M#   -+M##@-    ", X, Y + 16, pictureSizeX);
    PutText(" .              .###.    :      =$##+   ", X, Y + 17, pictureSizeX);
    PutText("                .#H,               :XH, ", X, Y + 18, pictureSizeX);
    PutText("                 +                   .:-", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "atom") == 0){
    PutText("                 =/;;/-                 ", X, Y + 0, pictureSizeX);
    PutText("                +:    //                ", X, Y + 1, pictureSizeX);
    PutText("               /,      /:               ", X, Y + 2, pictureSizeX);
    PutText("              -X        H.              ", X, Y + 3, pictureSizeX);
    PutText(".//;;::;:-,   X=        :+   .-;:=;:;%;.", X, Y + 4, pictureSizeX);
    PutText("M-       ,=;;;#:,      ,:#;;:=,       ,@", X, Y + 5, pictureSizeX);
    PutText(":%           :%.=/++++/=.$=           %=", X, Y + 6, pictureSizeX);
    PutText(" ,%;         %/:+/:,,/++:+/         ;+. ", X, Y + 7, pictureSizeX);
    PutText("   ,+/.    ,;@+,        ,%H;,    ,/+,   ", X, Y + 8, pictureSizeX);
    PutText("      ;+;;/= @.  .H##X   -X :///+;      ", X, Y + 9, pictureSizeX);
    PutText("      ;+=;;;.@,  .XM@$.  =X.//;=%/.     ", X, Y + 10, pictureSizeX);
    PutText("   ,;:      :@%=        =$H:     .+%-   ", X, Y + 11, pictureSizeX);
    PutText(" ,%=         %;-///==///-//         =%, ", X, Y + 12, pictureSizeX);
    PutText(";+           :%-;;;;;;;;-X-           +:", X, Y + 13, pictureSizeX);
    PutText("@-      .-;;;;M-        =M/;;;-.      -X", X, Y + 14, pictureSizeX);
    PutText(" :;;::;;-.    %-        :+    ,-;;-;:== ", X, Y + 15, pictureSizeX);
    PutText("              /X        H..             ", X, Y + 16, pictureSizeX);
    PutText("               ./      %=               ", X, Y + 17, pictureSizeX);
    PutText("                //    +;                ", X, Y + 18, pictureSizeX);
    PutText("                 ,////,                 ", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "radioactive") == 0){
    PutText("             =+$HM####@H%;,             ", X, Y + 0, pictureSizeX);
    PutText("          /H###############M$           ", X, Y + 1, pictureSizeX);
    PutText("          ,:################+           ", X, Y + 2, pictureSizeX);
    PutText("           .H##############+            ", X, Y + 3, pictureSizeX);
    PutText("             X############/             ", X, Y + 4, pictureSizeX);
    PutText("              $##########/              ", X, Y + 5, pictureSizeX);
    PutText("               %########/               ", X, Y + 6, pictureSizeX);
    PutText("                /X/;;+X/                ", X, Y + 7, pictureSizeX);
    PutText("                                        ", X, Y + 8, pictureSizeX);
    PutText("                 -XHHX-                 ", X, Y + 9, pictureSizeX);
    PutText("                ,######,                ", X, Y + 10, pictureSizeX);
    PutText("#############X  .M####M.  X#############", X, Y + 11, pictureSizeX);
    PutText("##############-   -//-.  -##############", X, Y + 12, pictureSizeX);
    PutText("X##############%.      .+##############X", X, Y + 13, pictureSizeX);
    PutText("-##############X        X##############-", X, Y + 14, pictureSizeX);
    PutText(" %############%          %############% ", X, Y + 15, pictureSizeX);
    PutText("  %##########,            ;##########%  ", X, Y + 16, pictureSizeX);
    PutText("   ;#######M=              =M#######.   ", X, Y + 17, pictureSizeX);
    PutText("    .+M###@,                ,@###M+.    ", X, Y + 18, pictureSizeX);
    PutText("       :XH.                  .HX:       ", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "fire") == 0){
    PutText("                     -$-                ", X, Y + 0, pictureSizeX);
    PutText("                    .H##H.              ", X, Y + 1, pictureSizeX);
    PutText("                   +######+             ", X, Y + 2, pictureSizeX);
    PutText("                .+#########H.           ", X, Y + 3, pictureSizeX);
    PutText("              -$############@.          ", X, Y + 4, pictureSizeX);
    PutText("            =H###############@  -X:     ", X, Y + 5, pictureSizeX);
    PutText("          .$##################:  @#@-   ", X, Y + 6, pictureSizeX);
    PutText("     .;  .M###################;  H###:  ", X, Y + 7, pictureSizeX);
    PutText("   ;@#:  @###################@  ,#####: ", X, Y + 8, pictureSizeX);
    PutText(" -M###.  M#################@.  .######H ", X, Y + 9, pictureSizeX);
    PutText(" M####-  +###############$   =@#######X ", X, Y + 10, pictureSizeX);
    PutText(" H####$   -M###########+   :#########M. ", X, Y + 11, pictureSizeX);
    PutText("  /####X-   =########%   :M########@/.  ", X, Y + 12, pictureSizeX);
    PutText("    .;%H@X;   .$###X   :##MM@%+;:-      ", X, Y + 13, pictureSizeX);
    PutText("                 ..                     ", X, Y + 14, pictureSizeX);
    PutText("  -/;:-,.              ,,-==+M########H ", X, Y + 15, pictureSizeX);
    PutText(" -##################@HX%%+%%$%%%+:,,    ", X, Y + 16, pictureSizeX);
    PutText("    .-/H%%%+%%$H@###############M@+=:/+:", X, Y + 17, pictureSizeX);
    PutText("/XHX%:#####MH%=    ,---:;;;;/%%XHM,:###$", X, Y + 18, pictureSizeX);
    PutText("$@#MX %+;-                           .  ", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "ok") == 0){
    PutText("                                      :X-", X, Y + 0, pictureSizeX);
    PutText("                                   :X### ", X, Y + 1, pictureSizeX);
    PutText("                                 ;@####@ ", X, Y + 2, pictureSizeX);
    PutText("                               ;M######X ", X, Y + 3, pictureSizeX);
    PutText("                             -@########$ ", X, Y + 4, pictureSizeX);
    PutText("                           .$##########@ ", X, Y + 5, pictureSizeX);
    PutText("                          =M############-", X, Y + 6, pictureSizeX);
    PutText("                         +##############$", X, Y + 7, pictureSizeX);
    PutText("                       .H############$=. ", X, Y + 8, pictureSizeX);
    PutText("          ,/:         ,M##########M,.    ", X, Y + 9, pictureSizeX);
    PutText("       -+@###;       =##########M;       ", X, Y + 10, pictureSizeX);
    PutText("    =%M#######;     :#########M/         ", X, Y + 11, pictureSizeX);
    PutText("-$M############;   :#########/           ", X, Y + 12, pictureSizeX);
    PutText(" ,;X############; =########$.            ", X, Y + 13, pictureSizeX);
    PutText("     ;H##########+#######M=              ", X, Y + 14, pictureSizeX);
    PutText("       .+###############+                ", X, Y + 15, pictureSizeX);
    PutText("          /M##########@-                 ", X, Y + 16, pictureSizeX);
    PutText("            ;M#######%                   ", X, Y + 17, pictureSizeX);
    PutText("              +#####;                    ", X, Y + 18, pictureSizeX);
    PutText("               ,$M-                      ", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "blackmesa") == 0){
    PutText("           .-,+$XHHHHHHX$+;-.           ", X, Y + 0, pictureSizeX);
    PutText("        ,;X@@X%/.=----=:/%X@@X/,        ", X, Y + 1, pictureSizeX);
    PutText("      =$@@%=.              .=+H@X:      ", X, Y + 2, pictureSizeX);
    PutText("    -XMX:                      =XMX=    ", X, Y + 3, pictureSizeX);
    PutText("   /@@:                          =H@+   ", X, Y + 4, pictureSizeX);
    PutText("  %@X,                            .$@$  ", X, Y + 5, pictureSizeX);
    PutText(" +@X.                               $@% ", X, Y + 6, pictureSizeX);
    PutText("-@@.                                .@@=", X, Y + 7, pictureSizeX);
    PutText("%@%                                  +@$", X, Y + 8, pictureSizeX);
    PutText("H@:                                  :@H", X, Y + 9, pictureSizeX);
    PutText("H@:         :HHHHHHHHHHHHHHHHHHX,    =@H", X, Y + 10, pictureSizeX);
    PutText("%@%         ;@M@@@@@@@@@@@@@@@@@H-   +@$", X, Y + 11, pictureSizeX);
    PutText("=@@,        :@@@@@@@@@@@@@@@@@@@@@= .@@:", X, Y + 12, pictureSizeX);
    PutText(" +@X        :@@@@@@@@@@@@@@@M@@@@@@:%@% ", X, Y + 13, pictureSizeX);
    PutText("  $@$,      ;@@@@@@@@@@@@@@@@@M@@@@@@$. ", X, Y + 14, pictureSizeX);
    PutText("   +@@HHHHHHH@@@@@@@@@@@@@@@@@@@@@@@+   ", X, Y + 15, pictureSizeX);
    PutText("    =X@@@@@@@@@@@@@@@@@@@@@@@@@@@@X=    ", X, Y + 16, pictureSizeX);
    PutText("      :$@@@@@@@@@@@@@@@@@@@M@@@@$:      ", X, Y + 17, pictureSizeX);
    PutText("        ,;$@@@@@@@@@@@@@@@@@@X/-        ", X, Y + 18, pictureSizeX);
    PutText("           .-;+$XXHHHHHX$+;-.           ", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "heart_broken") == 0){
    PutText("                          .,---.        ", X, Y + 0, pictureSizeX);
    PutText("                        ,/XM#MMMX;,     ", X, Y + 1, pictureSizeX);
    PutText("                      -%##########M%,   ", X, Y + 2, pictureSizeX);
    PutText("                     -@######%  $###@=  ", X, Y + 3, pictureSizeX);
    PutText("      .,--,         -H#######$   $###M: ", X, Y + 4, pictureSizeX);
    PutText("   ,;$M###MMX;     .;##########$;HM###X=", X, Y + 5, pictureSizeX);
    PutText(" ,/@##########H=      ;################+", X, Y + 6, pictureSizeX);
    PutText("-+#############M/,      %##############+", X, Y + 7, pictureSizeX);
    PutText("%M###############=      /##############:", X, Y + 8, pictureSizeX);
    PutText("H################      .M#############;.", X, Y + 9, pictureSizeX);
    PutText("@###############M      ,@###########M:. ", X, Y + 10, pictureSizeX);
    PutText("X################,      -$=X#######@:   ", X, Y + 11, pictureSizeX);
    PutText("/###################%-     +######$-    ", X, Y + 12, pictureSizeX);
    PutText(".;##################X     .X#####+,     ", X, Y + 13, pictureSizeX);
    PutText(" .;H################/     -X####+.      ", X, Y + 14, pictureSizeX);
    PutText("   ,;X##############,       .MM/        ", X, Y + 15, pictureSizeX);
    PutText("      ,:+$H@M#######M#$-    .$$=        ", X, Y + 16, pictureSizeX);
    PutText("           .,-=;+$@###X:    ;/=.        ", X, Y + 17, pictureSizeX);
    PutText("                  .,/X$;   .;:,         ", X, Y + 18, pictureSizeX);
    PutText("                      .,    ..          ", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "heart") == 0){
    PutText("                     .,---.       ", X, Y + 0, pictureSizeX);
    PutText("                   ,/XM#MMMX;,    ", X, Y + 1, pictureSizeX);
    PutText("                 -%##########M%,  ", X, Y + 2, pictureSizeX);
    PutText("                -@#############@= ", X, Y + 3, pictureSizeX);
    PutText("      .,--,    -H###############M:", X, Y + 4, pictureSizeX);
    PutText("   ,;$M###MMX;.;#################X", X, Y + 5, pictureSizeX);
    PutText(" ,/@##########H=;################+", X, Y + 6, pictureSizeX);
    PutText("-+###############################+", X, Y + 7, pictureSizeX);
    PutText("%M###############################:", X, Y + 8, pictureSizeX);
    PutText("H###############################;.", X, Y + 9, pictureSizeX);
    PutText("@#############################M:. ", X, Y + 10, pictureSizeX);
    PutText("X############################@:   ", X, Y + 11, pictureSizeX);
    PutText("/############################$-   ", X, Y + 12, pictureSizeX);
    PutText(".;##########################+,    ", X, Y + 13, pictureSizeX);
    PutText(" .;H#######################+.     ", X, Y + 14, pictureSizeX);
    PutText("   ,;X##################H/        ", X, Y + 15, pictureSizeX);
    PutText("      ,:+$H@M##########$=         ", X, Y + 16, pictureSizeX);
    PutText("           .,-=;+$@###X:;         ", X, Y + 17, pictureSizeX);
    PutText("                  .,/X$;          ", X, Y + 18, pictureSizeX);
    PutText("                     .,           ", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "cake") == 0){
    PutText("            ,:/+/-                      ", X, Y + 0, pictureSizeX);
    PutText("            /M/              .,-=.//.-  ", X, Y + 1, pictureSizeX);
    PutText("       .:/= ;MH/,    ,=/+%$XH@MM#@:     ", X, Y + 2, pictureSizeX);
    PutText("      -$##@+$###@H@MMM#######H:.    -/H#", X, Y + 3, pictureSizeX);
    PutText(" .,H@H@ X######@ -H#####@+-     -+H###@X", X, Y + 4, pictureSizeX);
    PutText("  .,@##H;      +XM##M/,     =%@###@X;-  ", X, Y + 5, pictureSizeX);
    PutText("X%-  :M##########$.    .:%M###@%:       ", X, Y + 6, pictureSizeX);
    PutText("M##H,   +H@@@$/-.  ,;$M###@%           -", X, Y + 7, pictureSizeX);
    PutText("M####M=,,---,.-%%H####M$:          ,+@##", X, Y + 8, pictureSizeX);
    PutText("@##################@/.         :%H##@$- ", X, Y + 9, pictureSizeX);
    PutText("M###############H,         ;HM##M$=     ", X, Y + 10, pictureSizeX);
    PutText("#################.    .=$M##M$=         ", X, Y + 11, pictureSizeX);
    PutText("################H..;XM##M$=          .:+", X, Y + 12, pictureSizeX);
    PutText("M###################@%=           =+@MH%", X, Y + 13, pictureSizeX);
    PutText("@################M/.          =+H#X%=   ", X, Y + 14, pictureSizeX);
    PutText("=+M##############M,       ,/X#X+;,      ", X, Y + 15, pictureSizeX);
    PutText("  .;XM##########H=    ,/X#H+:,          ", X, Y + 16, pictureSizeX);
    PutText("     .=+HM######M+/+HM@+=.              ", X, Y + 17, pictureSizeX);
    PutText("         ,:/%XM####H/.                  ", X, Y + 18, pictureSizeX);
    PutText("              ,.:=-.                    ", X, Y + 19, pictureSizeX);
  }else if(strcmp(image, "glados") == 0){
    PutText("       #+ @      # #              M#@   ", X, Y + 0, pictureSizeX);
    PutText(" .    .X  X.%##@,# #   +@#######X. @#%  ", X, Y + 1, pictureSizeX);
    PutText("   ,==.   ,######M+  -#####%M####M-    #", X, Y + 2, pictureSizeX);
    PutText("  :H##M%:=##+ .M##M,;#####/+#######% ,M#", X, Y + 3, pictureSizeX);
    PutText(" .M########=  =@#@.=#####M=M#######=  X#", X, Y + 4, pictureSizeX);
    PutText(" :@@MMM##M.  -##M.,#######M#######. =  M", X, Y + 5, pictureSizeX);
    PutText("             @##..###:.    .H####. @@ X,", X, Y + 6, pictureSizeX);
    PutText("   ############: ###./####;  /##= @#. M ", X, Y + 7, pictureSizeX);
    PutText("           ,M## ,##,@#M;/M#M  @# X#% X# ", X, Y + 8, pictureSizeX);
    PutText(" %=   ######M## ##.M#:   ./#M ,M #M ,#$ ", X, Y + 9, pictureSizeX);
    PutText("##/         $## #+;#: #### ;#/ M M- @# :", X, Y + 10, pictureSizeX);
    PutText("#+ #M@MM###M-;M #:$#-##$H# .#X @ + $#. #", X, Y + 11, pictureSizeX);
    PutText("      ######/.: #%=# M#:MM./#.-#  @#: H#", X, Y + 12, pictureSizeX);
    PutText("+,.=   @###: /@ %#,@  ##@X #,-#@.##% .@#", X, Y + 13, pictureSizeX);
    PutText("#####+,/##/ @##  @#,+       /#M      X, ", X, Y + 14, pictureSizeX);
    PutText("   ,###M#@ M###H .#M-     ,##M  ;@@; ###", X, Y + 15, pictureSizeX);
    PutText("   .M#M##H ;####X ,@#######M/ -M###$  -H", X, Y + 16, pictureSizeX);
    PutText("    .M###%  X####H  .@@MM@;  .@#M@      ", X, Y + 17, pictureSizeX);
    PutText("      H#M    /@####/      ,++.  / ==-,  ", X, Y + 18, pictureSizeX);
    PutText("               ,=/:. .+X@MMH@#H  #####$=", X, Y + 19, pictureSizeX);
  }
}
