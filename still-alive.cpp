#include "io.h"
#include "still-alive.h"

#include <string.h>

#define TEXT_SIZE_X 35
#define TEXT_SIZE_Y 23

#define PICTURE_SIZE_X 41
#define PICTURE_SIZE_Y 20

void PutChar(char ch, size_t x, size_t y)
{
    MoveCursorTo(x, y);

    char str[2] = {ch, '\0'};
    WriteString(str);
    RefreshScreen();
}

void PutText(const char* text, size_t x, size_t y, int number_of_chars = -1)
{
    size_t len = strlen(text);
    if (number_of_chars < 0)
        number_of_chars = len;

    for (int i = 0; i < number_of_chars; i++) {
        if (i < len)
            PutChar(text[i], x+i, y);
        else
            PutChar(' ', x+i, y);
    }
}

void TypeText(const char* text, size_t x, size_t y, size_t before_delay, size_t inter_delay,
        size_t after_delay, char cursorCharacter = ' ')
{
    if (cursorCharacter != ' ')
        PutChar(cursorCharacter, x, y);
    SleepFor(before_delay);

    MoveCursorTo(x, y);

    size_t c = 0;
    while (text[c] != '\0'){
        if (cursorCharacter != ' ')
            PutChar(cursorCharacter, x + c + 1, y);
        PutChar(text[c], x + c, y);

        if (text[c] != ' ')
            SleepFor(inter_delay);

        c++;
    }

    SleepFor(after_delay);

    if (cursorCharacter != ' ')
        PutChar(' ', x + c, y);
}

void ClearRectangle(size_t x, size_t y, size_t sx, size_t sy)
{
    for (size_t i = y; i < y + sy; i++){
        for (size_t j = x; j < x + sx; j++)
            PutChar(' ', j, i);
    }
}

///////////////////////////////////////////////////////////
void DrawPicture(const char* image, int X, int Y)
{
    size_t screenSizeX = GetScreenSizeX();
    size_t screenSizeY = GetScreenSizeY();

    if (X == -1)
        X = screenSizeX - PICTURE_SIZE_X;

    if (Y == -1)
        Y = screenSizeY - PICTURE_SIZE_Y;

    if (strcmp(image, "aperture") == 0) {
        PutText("              .,-:;//;:=,               ", X, Y + 0, PICTURE_SIZE_X);
        PutText("          . :H@@@MM@M#H/.,+%;,          ", X, Y + 1, PICTURE_SIZE_X);
        PutText("       ,/X+ +M@@M@MM%=,-%HMMM@X/,       ", X, Y + 2, PICTURE_SIZE_X);
        PutText("     -+@MM; $M@@MH+-,;XMMMM@MMMM@+-     ", X, Y + 3, PICTURE_SIZE_X);
        PutText("    ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.   ", X, Y + 4, PICTURE_SIZE_X);
        PutText("  ,>MM@@MH ,@%=            .---=-=:=,.  ", X, Y + 5, PICTURE_SIZE_X);
        PutText("  =@#@@@MX .,              -%HX$$%%%+;  ", X, Y + 6, PICTURE_SIZE_X);
        PutText(" =-./@M@M$                  .;@MMMM@MM: ", X, Y + 7, PICTURE_SIZE_X);
        PutText(" X@/.-$MM/                    .+MM@@@M$ ", X, Y + 8, PICTURE_SIZE_X);
        PutText(",@M@H: :@:                    . =X#@@@@-", X, Y + 9, PICTURE_SIZE_X);
        PutText(",@@@MMX, .                    /H- ;@M@M=", X, Y + 10, PICTURE_SIZE_X);
        PutText(".H@@@@M@+,                    %MM+..%#$.", X, Y + 11, PICTURE_SIZE_X);
        PutText(" /MMMM@MMH/.                  XM@MH, =; ", X, Y + 12, PICTURE_SIZE_X);
        PutText("  /%+%$XHH@$=              , .H@@@@MX,  ", X, Y + 13, PICTURE_SIZE_X);
        PutText("   .=--------.           -%H.,@@@@@MX,  ", X, Y + 14, PICTURE_SIZE_X);
        PutText("   .%MM@@@HHHXX$$$%+- .:$MMX =M@@MM%.   ", X, Y + 15, PICTURE_SIZE_X);
        PutText("     =XMMM@MM@MM#H;,-+HMM@M+ /MMMX=     ", X, Y + 16, PICTURE_SIZE_X);
        PutText("       =%@M@M#@$-.=$@MM@@@M: %M<=       ", X, Y + 17, PICTURE_SIZE_X);
        PutText("         ,:+$+-,/H#MMMMMMM@= =,         ", X, Y + 18, PICTURE_SIZE_X);
        PutText("               =++%%%%+/:-              ", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "explosion") == 0) {
        PutText("            .+                          ", X, Y + 0, PICTURE_SIZE_X);
        PutText("             /M.                        ", X, Y + 1, PICTURE_SIZE_X);
        PutText("              H#@:              ,.      ", X, Y + 2, PICTURE_SIZE_X);
        PutText("              -###H-          -@/       ", X, Y + 3, PICTURE_SIZE_X);
        PutText("               %####$.  -;  .%#X        ", X, Y + 4, PICTURE_SIZE_X);
        PutText("                M#####+;#H :M#M.        ", X, Y + 5, PICTURE_SIZE_X);
        PutText("..          .+/;%#########X###-         ", X, Y + 6, PICTURE_SIZE_X);
        PutText(" -/%H%+;-,    +##############/          ", X, Y + 7, PICTURE_SIZE_X);
        PutText("    .:$M###MH$%+############X  ,--=;-   ", X, Y + 8, PICTURE_SIZE_X);
        PutText("        -/H#####################H+=.    ", X, Y + 9, PICTURE_SIZE_X);
        PutText("            +#################X.        ", X, Y + 10, PICTURE_SIZE_X);
        PutText("         =%M####################H;.     ", X, Y + 11, PICTURE_SIZE_X);
        PutText("            /@###############+;;/%%;,   ", X, Y + 12, PICTURE_SIZE_X);
        PutText("         -%###################$.        ", X, Y + 13, PICTURE_SIZE_X);
        PutText("       ;H######################M=       ", X, Y + 14, PICTURE_SIZE_X);
        PutText("    ,%#####MH$%:+#####M###-/@####%      ", X, Y + 15, PICTURE_SIZE_X);
        PutText("  :$H%+;=-      -####X.,M#   -+M##@-    ", X, Y + 16, PICTURE_SIZE_X);
        PutText(" .              .###.    :      =$##+   ", X, Y + 17, PICTURE_SIZE_X);
        PutText("                .#H,               :XH, ", X, Y + 18, PICTURE_SIZE_X);
        PutText("                 +                   .:-", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "atom") == 0) {
        PutText("                 =/;;/-                 ", X, Y + 0, PICTURE_SIZE_X);
        PutText("                +:    //                ", X, Y + 1, PICTURE_SIZE_X);
        PutText("               /,      /:               ", X, Y + 2, PICTURE_SIZE_X);
        PutText("              -X        H.              ", X, Y + 3, PICTURE_SIZE_X);
        PutText(".//;;::;:-,   X=        :+   .-;:=;:;%;.", X, Y + 4, PICTURE_SIZE_X);
        PutText("M-       ,=;;;#:,      ,:#;;:=,       ,@", X, Y + 5, PICTURE_SIZE_X);
        PutText(":%           :%.=/++++/=.$=           %=", X, Y + 6, PICTURE_SIZE_X);
        PutText(" ,%;         %/:+/:,,/++:+/         ;+. ", X, Y + 7, PICTURE_SIZE_X);
        PutText("   ,+/.    ,;@+,        ,%H;,    ,/+,   ", X, Y + 8, PICTURE_SIZE_X);
        PutText("      ;+;;/= @.  .H##X   -X :///+;      ", X, Y + 9, PICTURE_SIZE_X);
        PutText("      ;+=;;;.@,  .XM@$.  =X.//;=%/.     ", X, Y + 10, PICTURE_SIZE_X);
        PutText("   ,;:      :@%=        =$H:     .+%-   ", X, Y + 11, PICTURE_SIZE_X);
        PutText(" ,%=         %;-///==///-//         =%, ", X, Y + 12, PICTURE_SIZE_X);
        PutText(";+           :%-;;;;;;;;-X-           +:", X, Y + 13, PICTURE_SIZE_X);
        PutText("@-      .-;;;;M-        =M/;;;-.      -X", X, Y + 14, PICTURE_SIZE_X);
        PutText(" :;;::;;-.    %-        :+    ,-;;-;:== ", X, Y + 15, PICTURE_SIZE_X);
        PutText("              /X        H..             ", X, Y + 16, PICTURE_SIZE_X);
        PutText("               ./      %=               ", X, Y + 17, PICTURE_SIZE_X);
        PutText("                //    +;                ", X, Y + 18, PICTURE_SIZE_X);
        PutText("                 ,////,                 ", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "radioactive") == 0) {
        PutText("             =+$HM####@H%;,             ", X, Y + 0, PICTURE_SIZE_X);
        PutText("          /H###############M$           ", X, Y + 1, PICTURE_SIZE_X);
        PutText("          ,:################+           ", X, Y + 2, PICTURE_SIZE_X);
        PutText("           .H##############+            ", X, Y + 3, PICTURE_SIZE_X);
        PutText("             X############/             ", X, Y + 4, PICTURE_SIZE_X);
        PutText("              $##########/              ", X, Y + 5, PICTURE_SIZE_X);
        PutText("               %########/               ", X, Y + 6, PICTURE_SIZE_X);
        PutText("                /X/;;+X/                ", X, Y + 7, PICTURE_SIZE_X);
        PutText("                                        ", X, Y + 8, PICTURE_SIZE_X);
        PutText("                 -XHHX-                 ", X, Y + 9, PICTURE_SIZE_X);
        PutText("                ,######,                ", X, Y + 10, PICTURE_SIZE_X);
        PutText("#############X  .M####M.  X#############", X, Y + 11, PICTURE_SIZE_X);
        PutText("##############-   -//-.  -##############", X, Y + 12, PICTURE_SIZE_X);
        PutText("X##############%.      .+##############X", X, Y + 13, PICTURE_SIZE_X);
        PutText("-##############X        X##############-", X, Y + 14, PICTURE_SIZE_X);
        PutText(" %############%          %############% ", X, Y + 15, PICTURE_SIZE_X);
        PutText("  %##########,            ;##########%  ", X, Y + 16, PICTURE_SIZE_X);
        PutText("   ;#######M=              =M#######.   ", X, Y + 17, PICTURE_SIZE_X);
        PutText("    .+M###@,                ,@###M+.    ", X, Y + 18, PICTURE_SIZE_X);
        PutText("       :XH.                  .HX:       ", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "fire") == 0) {
        PutText("                     -$-                ", X, Y + 0, PICTURE_SIZE_X);
        PutText("                    .H##H.              ", X, Y + 1, PICTURE_SIZE_X);
        PutText("                   +######+             ", X, Y + 2, PICTURE_SIZE_X);
        PutText("                .+#########H.           ", X, Y + 3, PICTURE_SIZE_X);
        PutText("              -$############@.          ", X, Y + 4, PICTURE_SIZE_X);
        PutText("            =H###############@  -X:     ", X, Y + 5, PICTURE_SIZE_X);
        PutText("          .$##################:  @#@-   ", X, Y + 6, PICTURE_SIZE_X);
        PutText("     .;  .M###################;  H###:  ", X, Y + 7, PICTURE_SIZE_X);
        PutText("   ;@#:  @###################@  ,#####: ", X, Y + 8, PICTURE_SIZE_X);
        PutText(" -M###.  M#################@.  .######H ", X, Y + 9, PICTURE_SIZE_X);
        PutText(" M####-  +###############$   =@#######X ", X, Y + 10, PICTURE_SIZE_X);
        PutText(" H####$   -M###########+   :#########M. ", X, Y + 11, PICTURE_SIZE_X);
        PutText("  /####X-   =########%   :M########@/.  ", X, Y + 12, PICTURE_SIZE_X);
        PutText("    .;%H@X;   .$###X   :##MM@%+;:-      ", X, Y + 13, PICTURE_SIZE_X);
        PutText("                 ..                     ", X, Y + 14, PICTURE_SIZE_X);
        PutText("  -/;:-,.              ,,-==+M########H ", X, Y + 15, PICTURE_SIZE_X);
        PutText(" -##################@HX%%+%%$%%%+:,,    ", X, Y + 16, PICTURE_SIZE_X);
        PutText("    .-/H%%%+%%$H@###############M@+=:/+:", X, Y + 17, PICTURE_SIZE_X);
        PutText("/XHX%:#####MH%=    ,---:;;;;/%%XHM,:###$", X, Y + 18, PICTURE_SIZE_X);
        PutText("$@#MX %+;-                           .  ", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "ok") == 0) {
        PutText("                                      :X-", X, Y + 0, PICTURE_SIZE_X);
        PutText("                                   :X### ", X, Y + 1, PICTURE_SIZE_X);
        PutText("                                 ;@####@ ", X, Y + 2, PICTURE_SIZE_X);
        PutText("                               ;M######X ", X, Y + 3, PICTURE_SIZE_X);
        PutText("                             -@########$ ", X, Y + 4, PICTURE_SIZE_X);
        PutText("                           .$##########@ ", X, Y + 5, PICTURE_SIZE_X);
        PutText("                          =M############-", X, Y + 6, PICTURE_SIZE_X);
        PutText("                         +##############$", X, Y + 7, PICTURE_SIZE_X);
        PutText("                       .H############$=. ", X, Y + 8, PICTURE_SIZE_X);
        PutText("          ,/:         ,M##########M,.    ", X, Y + 9, PICTURE_SIZE_X);
        PutText("       -+@###;       =##########M;       ", X, Y + 10, PICTURE_SIZE_X);
        PutText("    =%M#######;     :#########M/         ", X, Y + 11, PICTURE_SIZE_X);
        PutText("-$M############;   :#########/           ", X, Y + 12, PICTURE_SIZE_X);
        PutText(" ,;X############; =########$.            ", X, Y + 13, PICTURE_SIZE_X);
        PutText("     ;H##########+#######M=              ", X, Y + 14, PICTURE_SIZE_X);
        PutText("       .+###############+                ", X, Y + 15, PICTURE_SIZE_X);
        PutText("          /M##########@-                 ", X, Y + 16, PICTURE_SIZE_X);
        PutText("            ;M#######%                   ", X, Y + 17, PICTURE_SIZE_X);
        PutText("              +#####;                    ", X, Y + 18, PICTURE_SIZE_X);
        PutText("               ,$M-                      ", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "blackmesa") == 0) {
        PutText("           .-,+$XHHHHHHX$+;-.           ", X, Y + 0, PICTURE_SIZE_X);
        PutText("        ,;X@@X%/.=----=:/%X@@X/,        ", X, Y + 1, PICTURE_SIZE_X);
        PutText("      =$@@%=.              .=+H@X:      ", X, Y + 2, PICTURE_SIZE_X);
        PutText("    -XMX:                      =XMX=    ", X, Y + 3, PICTURE_SIZE_X);
        PutText("   /@@:                          =H@+   ", X, Y + 4, PICTURE_SIZE_X);
        PutText("  %@X,                            .$@$  ", X, Y + 5, PICTURE_SIZE_X);
        PutText(" +@X.                               $@% ", X, Y + 6, PICTURE_SIZE_X);
        PutText("-@@.                                .@@=", X, Y + 7, PICTURE_SIZE_X);
        PutText("%@%                                  +@$", X, Y + 8, PICTURE_SIZE_X);
        PutText("H@:                                  :@H", X, Y + 9, PICTURE_SIZE_X);
        PutText("H@:         :HHHHHHHHHHHHHHHHHHX,    =@H", X, Y + 10, PICTURE_SIZE_X);
        PutText("%@%         ;@M@@@@@@@@@@@@@@@@@H-   +@$", X, Y + 11, PICTURE_SIZE_X);
        PutText("=@@,        :@@@@@@@@@@@@@@@@@@@@@= .@@:", X, Y + 12, PICTURE_SIZE_X);
        PutText(" +@X        :@@@@@@@@@@@@@@@M@@@@@@:%@% ", X, Y + 13, PICTURE_SIZE_X);
        PutText("  $@$,      ;@@@@@@@@@@@@@@@@@M@@@@@@$. ", X, Y + 14, PICTURE_SIZE_X);
        PutText("   +@@HHHHHHH@@@@@@@@@@@@@@@@@@@@@@@+   ", X, Y + 15, PICTURE_SIZE_X);
        PutText("    =X@@@@@@@@@@@@@@@@@@@@@@@@@@@@X=    ", X, Y + 16, PICTURE_SIZE_X);
        PutText("      :$@@@@@@@@@@@@@@@@@@@M@@@@$:      ", X, Y + 17, PICTURE_SIZE_X);
        PutText("        ,;$@@@@@@@@@@@@@@@@@@X/-        ", X, Y + 18, PICTURE_SIZE_X);
        PutText("           .-;+$XXHHHHHX$+;-.           ", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "heart_broken") == 0) {
        PutText("                          .,---.        ", X, Y + 0, PICTURE_SIZE_X);
        PutText("                        ,/XM#MMMX;,     ", X, Y + 1, PICTURE_SIZE_X);
        PutText("                      -%##########M%,   ", X, Y + 2, PICTURE_SIZE_X);
        PutText("                     -@######%  $###@=  ", X, Y + 3, PICTURE_SIZE_X);
        PutText("      .,--,         -H#######$   $###M: ", X, Y + 4, PICTURE_SIZE_X);
        PutText("   ,;$M###MMX;     .;##########$;HM###X=", X, Y + 5, PICTURE_SIZE_X);
        PutText(" ,/@##########H=      ;################+", X, Y + 6, PICTURE_SIZE_X);
        PutText("-+#############M/,      %##############+", X, Y + 7, PICTURE_SIZE_X);
        PutText("%M###############=      /##############:", X, Y + 8, PICTURE_SIZE_X);
        PutText("H################      .M#############;.", X, Y + 9, PICTURE_SIZE_X);
        PutText("@###############M      ,@###########M:. ", X, Y + 10, PICTURE_SIZE_X);
        PutText("X################,      -$=X#######@:   ", X, Y + 11, PICTURE_SIZE_X);
        PutText("/###################%-     +######$-    ", X, Y + 12, PICTURE_SIZE_X);
        PutText(".;##################X     .X#####+,     ", X, Y + 13, PICTURE_SIZE_X);
        PutText(" .;H################/     -X####+.      ", X, Y + 14, PICTURE_SIZE_X);
        PutText("   ,;X##############,       .MM/        ", X, Y + 15, PICTURE_SIZE_X);
        PutText("      ,:+$H@M#######M#$-    .$$=        ", X, Y + 16, PICTURE_SIZE_X);
        PutText("           .,-=;+$@###X:    ;/=.        ", X, Y + 17, PICTURE_SIZE_X);
        PutText("                  .,/X$;   .;:,         ", X, Y + 18, PICTURE_SIZE_X);
        PutText("                      .,    ..          ", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "heart") == 0) {
        PutText("                     .,---.       ", X, Y + 0, PICTURE_SIZE_X);
        PutText("                   ,/XM#MMMX;,    ", X, Y + 1, PICTURE_SIZE_X);
        PutText("                 -%##########M%,  ", X, Y + 2, PICTURE_SIZE_X);
        PutText("                -@#############@= ", X, Y + 3, PICTURE_SIZE_X);
        PutText("      .,--,    -H###############M:", X, Y + 4, PICTURE_SIZE_X);
        PutText("   ,;$M###MMX;.;#################X", X, Y + 5, PICTURE_SIZE_X);
        PutText(" ,/@##########H=;################+", X, Y + 6, PICTURE_SIZE_X);
        PutText("-+###############################+", X, Y + 7, PICTURE_SIZE_X);
        PutText("%M###############################:", X, Y + 8, PICTURE_SIZE_X);
        PutText("H###############################;.", X, Y + 9, PICTURE_SIZE_X);
        PutText("@#############################M:. ", X, Y + 10, PICTURE_SIZE_X);
        PutText("X############################@:   ", X, Y + 11, PICTURE_SIZE_X);
        PutText("/############################$-   ", X, Y + 12, PICTURE_SIZE_X);
        PutText(".;##########################+,    ", X, Y + 13, PICTURE_SIZE_X);
        PutText(" .;H#######################+.     ", X, Y + 14, PICTURE_SIZE_X);
        PutText("   ,;X##################H/        ", X, Y + 15, PICTURE_SIZE_X);
        PutText("      ,:+$H@M##########$=         ", X, Y + 16, PICTURE_SIZE_X);
        PutText("           .,-=;+$@###X:;         ", X, Y + 17, PICTURE_SIZE_X);
        PutText("                  .,/X$;          ", X, Y + 18, PICTURE_SIZE_X);
        PutText("                     .,           ", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "cake") == 0) {
        PutText("            ,:/+/-                      ", X, Y + 0, PICTURE_SIZE_X);
        PutText("            /M/              .,-=.//.-  ", X, Y + 1, PICTURE_SIZE_X);
        PutText("       .:/= ;MH/,    ,=/+%$XH@MM#@:     ", X, Y + 2, PICTURE_SIZE_X);
        PutText("      -$##@+$###@H@MMM#######H:.    -/H#", X, Y + 3, PICTURE_SIZE_X);
        PutText(" .,H@H@ X######@ -H#####@+-     -+H###@X", X, Y + 4, PICTURE_SIZE_X);
        PutText("  .,@##H;      +XM##M/,     =%@###@X;-  ", X, Y + 5, PICTURE_SIZE_X);
        PutText("X%-  :M##########$.    .:%M###@%:       ", X, Y + 6, PICTURE_SIZE_X);
        PutText("M##H,   +H@@@$/-.  ,;$M###@%           -", X, Y + 7, PICTURE_SIZE_X);
        PutText("M####M=,,---,.-%%H####M$:          ,+@##", X, Y + 8, PICTURE_SIZE_X);
        PutText("@##################@/.         :%H##@$- ", X, Y + 9, PICTURE_SIZE_X);
        PutText("M###############H,         ;HM##M$=     ", X, Y + 10, PICTURE_SIZE_X);
        PutText("#################.    .=$M##M$=         ", X, Y + 11, PICTURE_SIZE_X);
        PutText("################H..;XM##M$=          .:+", X, Y + 12, PICTURE_SIZE_X);
        PutText("M###################@%=           =+@MH%", X, Y + 13, PICTURE_SIZE_X);
        PutText("@################M/.          =+H#X%=   ", X, Y + 14, PICTURE_SIZE_X);
        PutText("=+M##############M,       ,/X#X+;,      ", X, Y + 15, PICTURE_SIZE_X);
        PutText("  .;XM##########H=    ,/X#H+:,          ", X, Y + 16, PICTURE_SIZE_X);
        PutText("     .=+HM######M+/+HM@+=.              ", X, Y + 17, PICTURE_SIZE_X);
        PutText("         ,:/%XM####H/.                  ", X, Y + 18, PICTURE_SIZE_X);
        PutText("              ,.:=-.                    ", X, Y + 19, PICTURE_SIZE_X);
    } else if (strcmp(image, "glados") == 0) {
        PutText("       #+ @      # #              M#@   ", X, Y + 0, PICTURE_SIZE_X);
        PutText(" .    .X  X.%##@,# #   +@#######X. @#%  ", X, Y + 1, PICTURE_SIZE_X);
        PutText("   ,==.   ,######M+  -#####%M####M-    #", X, Y + 2, PICTURE_SIZE_X);
        PutText("  :H##M%:=##+ .M##M,;#####/+#######% ,M#", X, Y + 3, PICTURE_SIZE_X);
        PutText(" .M########=  =@#@.=#####M=M#######=  X#", X, Y + 4, PICTURE_SIZE_X);
        PutText(" :@@MMM##M.  -##M.,#######M#######. =  M", X, Y + 5, PICTURE_SIZE_X);
        PutText("             @##..###:.    .H####. @@ X,", X, Y + 6, PICTURE_SIZE_X);
        PutText("   ############: ###./####;  /##= @#. M ", X, Y + 7, PICTURE_SIZE_X);
        PutText("           ,M## ,##,@#M;/M#M  @# X#% X# ", X, Y + 8, PICTURE_SIZE_X);
        PutText(" %=   ######M## ##.M#:   ./#M ,M #M ,#$ ", X, Y + 9, PICTURE_SIZE_X);
        PutText("##/         $## #+;#: #### ;#/ M M- @# :", X, Y + 10, PICTURE_SIZE_X);
        PutText("#+ #M@MM###M-;M #:$#-##$H# .#X @ + $#. #", X, Y + 11, PICTURE_SIZE_X);
        PutText("      ######/.: #%=# M#:MM./#.-#  @#: H#", X, Y + 12, PICTURE_SIZE_X);
        PutText("+,.=   @###: /@ %#,@  ##@X #,-#@.##% .@#", X, Y + 13, PICTURE_SIZE_X);
        PutText("#####+,/##/ @##  @#,+       /#M      X, ", X, Y + 14, PICTURE_SIZE_X);
        PutText("   ,###M#@ M###H .#M-     ,##M  ;@@; ###", X, Y + 15, PICTURE_SIZE_X);
        PutText("   .M#M##H ;####X ,@#######M/ -M###$  -H", X, Y + 16, PICTURE_SIZE_X);
        PutText("    .M###%  X####H  .@@MM@;  .@#M@      ", X, Y + 17, PICTURE_SIZE_X);
        PutText("      H#M    /@####/      ,++.  / ==-,  ", X, Y + 18, PICTURE_SIZE_X);
        PutText("               ,=/:. .+X@MMH@#H  #####$=", X, Y + 19, PICTURE_SIZE_X);
    }
}

size_t currentCursorYPosition = 1;

void WriteLine(const char* text, size_t after_delay, size_t inter_delay)
{
    TypeText(text, 1, currentCursorYPosition, 0, inter_delay, after_delay, ' ');
    currentCursorYPosition++;
}

void ClearArea()
{
    ClearRectangle(1, 1, TEXT_SIZE_X, TEXT_SIZE_Y);
    currentCursorYPosition = 1;
    PutChar(' ', 1, currentCursorYPosition);
}

/* void* RadioLoopFunc(void*) */
/* { */
/*     while (true) */
/*     { */
/*         PlayMusic("./portal-radio.wav"); */
/*         SleepFor(22000); */
/*     } */
/*     return NULL; */
/* } */

/* void PlayRadioLoop() */
/* { */
/*     pthread_t tid; */
/*     pthread_create(&tid, NULL, &RadioLoopFunc, NULL); */
/* } */

int Song()
{
    size_t screenSizeX = GetScreenSizeX();
    size_t screenSizeY = GetScreenSizeY();

    if ((screenSizeX < 6 + 2*PICTURE_SIZE_X) || 
            (screenSizeY < 5 + PICTURE_SIZE_Y))
        return 1;

    InitColorPair(1, COLOR_YELLOW, COLOR_BLACK);
    InitColorPair(2, COLOR_MAGENTA, COLOR_BLACK);
    InitColorPair(3, COLOR_WHITE, COLOR_BLACK);
    SetClearColorPair(1);
    SetWriteColorPair(1);

    ClearScreen();
    //HideCursor();

    for (int y = 0; y < screenSizeY; y++) {
        for (int x = 0; x < screenSizeX; x++) {
            if ((y == 0) || ((y == screenSizeY - PICTURE_SIZE_Y - 1) &&
                        (x > screenSizeX - PICTURE_SIZE_X - 1)) ||
                    ((y == screenSizeY - 1) && (x <= screenSizeX - PICTURE_SIZE_X - 1)))
                PutChar('-', x, y);
            else if ((x == 0) || (x == screenSizeX - PICTURE_SIZE_X - 1) ||
                    ((x == screenSizeX - 1) && (y < screenSizeY - PICTURE_SIZE_Y)))
                PutChar('|', x, y);
        }
    }

    PutText("THANK YOU FOR PARTICIPATING", screenSizeX - PICTURE_SIZE_X,
            (screenSizeY - PICTURE_SIZE_Y) / 2 - 1);
    PutText("IN THIS", screenSizeX - PICTURE_SIZE_X, (screenSizeY - PICTURE_SIZE_Y) / 2);
    PutText("ENRICHMENT CENTER ACTIVITY!!", screenSizeX - PICTURE_SIZE_X,
            (screenSizeY - PICTURE_SIZE_Y) / 2 + 1);

    ClearArea();
    WriteLine("Forms FORM-29827281-12:", 0);
    WriteLine("Test Assesment Report", 0);
    WriteLine("", 100);
    WriteLine("", 1100);

    PlayMusic("./still-alive.wav");
    SleepFor(400);

    WriteLine("This was a triumph.", 2000);
    WriteLine("I'm making a note here:", 200);
    WriteLine("HUGE SUCCESS.", 1500, 120);
    WriteLine("It's hard to overstate", 0);
    WriteLine("my satisfaction.", 3700);
    DrawPicture("aperture");
    WriteLine("Aperture Science,", 2200);
    WriteLine("We do what we must", 0);
    WriteLine("because we can.", 2700);
    WriteLine("For the good of all of us.", 1600, 90);
    DrawPicture("radioactive");
    WriteLine("Except the ones who are dead.", 420, 70);
    WriteLine("", 100);
    WriteLine("But there's no sense crying", 0, 90);
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
    WriteLine("", 100);
    WriteLine("Dear <<Subject Name Here>>,", 0);
    WriteLine("", 100);
    WriteLine("I'm not even angry.", 2800);
    WriteLine("I'm being so sincere right now.", 2000);
    DrawPicture("heart_broken");
    WriteLine("Even though you broke my heart.", 1100);
    WriteLine("And killed me.", 2200);
    DrawPicture("explosion");
    WriteLine("And tore me to pieces.", 2100);
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
    WriteLine("So I'm GLaD I got burned.", 0, 90);
    DrawPicture("atom");
    WriteLine("Think of all the things we learned", 0, 90);
    DrawPicture("aperture");
    WriteLine("for the people who are", 0, 90);
    WriteLine("still alive.", 1500, 90);
    ClearArea();

    WriteLine("FORMS Form-55551-6:", 0, 70);
    WriteLine("Personnel File Addendum Addendum:", 400, 70);
    WriteLine("", 100);
    WriteLine("One last thing:", 200);
    WriteLine("", 100);
    WriteLine("Go ahead and leave me.", 2100);
    WriteLine("I think I prefer to stay inside.", 2200);
    WriteLine("Maybe you'll find someone else", 1200);
    WriteLine("to help you.", 2300);
    DrawPicture("blackmesa");
    WriteLine("Maybe Black Mesa...", 2000, 120);
    WriteLine("THAT WAS A JOKE. *Fake Laugh* FAT CHANCE.", 2000, 130);
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

    WriteLine("", 100);
    WriteLine("", 100);
    WriteLine("PS: And believe me I am", 0, 90);
    WriteLine("still alive.", 600, 90);
    WriteLine("PPS: I'm doing Science and I'm", 0, 90);
    WriteLine("still alive.", 600, 90);
    WriteLine("PPPS: I feel FANTASTIC and I'm", 0, 90);
    WriteLine("still alive.", 100, 90);
    WriteLine("", 100);
    WriteLine("FINAL THOUGHT:", 0, 90);
    WriteLine("While you're dying I'll be", 0, 90);
    WriteLine("still alive.", 0, 90);
    WriteLine("FINAL THOUGHT PS:", 0, 60);
    WriteLine("And when you're dead I will be", 0, 80);
    WriteLine("still alive.", 300);
    WriteLine("", 100);
    WriteLine("", 100);
    WriteLine("STILL ALIVE", 5000, 120);
    ClearArea();

    PlayMusic("./portal-radio.wav");
    ClearScreen();

    SetWriteColorPair(1);
    DrawPicture("cake", screenSizeX - PICTURE_SIZE_X - 3, screenSizeY - PICTURE_SIZE_Y - 3);

    SetWriteColorPair(2);
    DrawPicture("heart", 3, 3);

    SetWriteColorPair(3);
    PutText("Portal is awesome! =)", (screenSizeX - 21) / 2, 0);
    PutText("Terminal remake by Ivan Podmazov", screenSizeX - 33, screenSizeY - 1);

    MoveCursorTo(screenSizeX - 1, screenSizeY - 1);
    
    SleepFor(22000);

    return 0;
}

