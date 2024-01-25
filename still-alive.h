// Portal 1 Ending Credits remake
// Written by Ivan Podmazov, 2011
// Remade for Linux in 2016
// Rewritten in 2018
// Completely rewritten again as station-app plugin in 2024

// GLaDOS 3.11 - [Still Alive]

#pragma once

// Text area size in characters
#define TEXT_SIZE_X (35 + 1) // minimum + extra
#define TEXT_SIZE_Y (23 + 5) // minimum + extra

// Picture area size in characters
#define PICTURE_SIZE_X 41
#define PICTURE_SIZE_Y 20

// Whole screen size in characters
#define SCREEN_SIZE_X (1 + TEXT_SIZE_X + 1 + PICTURE_SIZE_X + 1)
#define SCREEN_SIZE_Y (1 + TEXT_SIZE_Y + 1)

// Window parameters
#define WINDOW_SCALE 2 // window pixels per texture pixel

// Colors
#define TEXT_COLOR 0xFF0066AA
#define BG_COLOR0 0xFF001122 // main background color
#define BG_COLOR1 0xFF001929 // color of background flickering

// Background flickering parameters
#define BG_FLICKERING_MOD 3
#define BG_FLICKERING_PERIOD 10

// Cursor flickering parameters
#define CURSOR_FLICKERING_PERIOD 250

///////////////////////////////////////////////////////////////////////////////

enum song_event {
    EVENT_NONE = 0,
    EVENT_START_MUSIC,
    EVENT_CLEAR_SCREEN,
    EVENT_DRAW_PICTURE,

    EVENT_DRAW_APERTURE = EVENT_DRAW_PICTURE,
    EVENT_DRAW_EXPLOSION,
    EVENT_DRAW_ATOM,
    EVENT_DRAW_RADIOACTIVE,
    EVENT_DRAW_FIRE,
    EVENT_DRAW_CHECK,
    EVENT_DRAW_BLACKMESA,
    EVENT_DRAW_HEARTBROKEN,
    EVENT_DRAW_CAKE,
    EVENT_DRAW_GLADOS,

    EVENT_DRAW_HEART,
};

struct song_line {
    const char *text; // line text
    int start; // timestamp of line start
    int end;   // timestamp of line end
    enum song_event event; // event at line
    _Bool no_echo; // don't echo to terminal
};

///////////////////////////////////////////////////////////////////////////////

#define COMMENT0 "THANK YOU FOR PARTICIPATING"
#define COMMENT1 "IN THIS"
#define COMMENT2 "ENRICHMENT CENTER ACTIVITY!!"

static const struct song_line song[] = {
    {.text = "Forms FORM-29827281-12:\n",               .start = 0,         .end = 2500},
    {.text = "Test Assesment Report\n\n\n",             .start = 2500,      .end = 5000},

    {.text = "This was a triumph.\n",                   .start = 8000,      .end = 9500,    .event = EVENT_START_MUSIC},
    {.text = "I'm making a note here:\n",               .start = 12000,     .end = 14000},
    {.text = "HUGE SUCCESS.\n",                         .start = 14000,     .end = 15500},
    {.text = "It's hard to overstate\n",                .start = 17000,     .end = 19500},
    {.text = "my satisfaction.\n",                      .start = 19500,     .end = 22000},
    {.text = "Aperture Science\n",                      .start = 24000,     .end = 26000,   .event = EVENT_DRAW_APERTURE},
    {.text = "We do what we must\n",                    .start = 28000,     .end = 29900},
    {.text = "because we can.\n",                       .start = 29900,     .end = 31000},
    {.text = "For the good of all of us.\n",            .start = 33000,     .end = 36000},
    {.text = "Except the ones who are dead.\n",         .start = 36500,     .end = 38500,   .event = EVENT_DRAW_RADIOACTIVE},
    {.text = "\nBut there's no sense crying\n",         .start = 38500,     .end = 40900,   .event = EVENT_DRAW_APERTURE},
    {.text = "over every mistake.\n",                   .start = 40900,     .end = 42200},
    {.text = "You just keep on trying\n",               .start = 42500,     .end = 44500},
    {.text = "till you run out of cake.\n",             .start = 44500,     .end = 46000},
    {.text = "And the Science gets done.\n",            .start = 46500,     .end = 49000,   .event = EVENT_DRAW_ATOM},
    {.text = "And you make a neat gun.\n",              .start = 49000,     .end = 50500},
    {.text = "For the people who are\n",                .start = 50500,     .end = 52000,   .event = EVENT_DRAW_APERTURE},
    {.text = "still alive.\n",                          .start = 52000,     .end = 53000},

    {.text = "FORMS Form-55551-5:\n",                   .start = 55000,     .end = 55500,   .event = EVENT_CLEAR_SCREEN},
    {.text = "Personnel File Addendum:\n\n",            .start = 55500,     .end = 57000},
    {.text = "Dear <<Subject Name Here>>,\n\n",         .start = 57000,     .end = 59500},
    {.text = "I'm not even angry.\n",                   .start = 60000,     .end = 61800},
    {.text = "I'm being so sincere right now.\n",       .start = 64000,     .end = 67000},
    {.text = "Even though you",                         .start = 69000,     .end = 71000},
    {.text = " broke my heart.\n",                      .start = 71000,     .end = 72000,   .event = EVENT_DRAW_HEARTBROKEN},
    {.text = "And killed me.\n",                        .start = 72000,     .end = 74000},
    {.text = "And tore me into pieces.\n",              .start = 76000,     .end = 77000,   .event = EVENT_DRAW_EXPLOSION},
    {.text = "And threw every piece into",              .start = 80000,     .end = 82000},
    {.text = " a fire.\n",                              .start = 82000,     .end = 84000,   .event = EVENT_DRAW_FIRE},
    {.text = "As they burned it hurt because\n",        .start = 85000,     .end = 88000},
    {.text = "I was so happy for you!\n",               .start = 88000,     .end = 90000,   .event = EVENT_DRAW_CHECK},
    {.text = "Now these points of data\n",              .start = 90100,     .end = 92700},
    {.text = "make a beautiful line.\n",                .start = 92700,     .end = 94500},
    {.text = "And we're out of beta.\n",                .start = 94500,     .end = 96500},
    {.text = "We're releasing on time.\n",              .start = 96500,     .end = 98200},
    {.text = "So I'm GLaD. I got burned.\n",            .start = 98200,     .end = 100500,  .event = EVENT_DRAW_EXPLOSION},
    {.text = "Think of all the things we learned\n",    .start = 100500,    .end = 102700,  .event = EVENT_DRAW_ATOM},
    {.text = "for the people who are\n",                .start = 102700,    .end = 104000,  .event = EVENT_DRAW_APERTURE},
    {.text = "still alive.\n",                          .start = 104000,    .end = 106000},

    {.text = "FORMS Form-55551-6:\n",                   .start = 107000,    .end = 108000,  .event = EVENT_CLEAR_SCREEN},
    {.text = "Personnel File Addendum Addendum:\n\n",   .start = 108000,    .end = 109000},
    {.text = "One last thing:\n\n",                     .start = 110000,    .end = 112000},
    {.text = "Go ahead and leave me.\n",                .start = 112300,    .end = 114000},
    {.text = "I think I prefer to stay inside.\n",      .start = 116000,    .end = 119800},
    {.text = "Maybe you'll find someone else\n",        .start = 121000,    .end = 125000},
    {.text = "to help you.\n",                          .start = 125000,    .end = 126000},
    {.text = "Maybe",                                   .start = 128000,    .end = 128200},
    {.text = " Black Mesa",                             .start = 128200,    .end = 130000,  .event = EVENT_DRAW_BLACKMESA},
    {.text = "...\n",                                   .start = 130000,    .end = 132000},
    {.text = "THAT WAS A JOKE.",                        .start = 132200,    .end = 133800},
    {.text = " FAT CHANCE.\n",                          .start = 134600,    .end = 136000},
    {.text = "Anyway,",                                 .start = 137000,    .end = 138800},
    {.text = " this cake is great.\n",                  .start = 138900,    .end = 140700,  .event = EVENT_DRAW_CAKE},
    {.text = "It's so delicious and moist.\n",          .start = 140700,    .end = 142000},
    {.text = "Look at me still talking\n",              .start = 143000,    .end = 144800,  .event = EVENT_DRAW_GLADOS},
    {.text = "when there's Science to do.\n",           .start = 144800,    .end = 146900,  .event = EVENT_DRAW_RADIOACTIVE},
    {.text = "When I look out there,\n",                .start = 147000,    .end = 148400,  .event = EVENT_DRAW_APERTURE},
    {.text = "it makes me GLaD I'm not you.\n",         .start = 148500,    .end = 150900},
    {.text = "I've experiments to run.\n",              .start = 150900,    .end = 153000,  .event = EVENT_DRAW_ATOM},
    {.text = "There is research to be done.\n",         .start = 153000,    .end = 154800,  .event = EVENT_DRAW_EXPLOSION},
    {.text = "On the people who are\n",                 .start = 154800,    .end = 156000,  .event = EVENT_DRAW_APERTURE},
    {.text = "still alive.\n",                          .start = 156000,    .end = 158000},

    {.text = "",                                        .start = 159000,    .end = 159000,  .event = EVENT_CLEAR_SCREEN},
    {.text = "\n\n\n",                                  .start = 159000,    .end = 159000,                                  .no_echo = 1},
    {.text = "PS: And believe me I am\n",               .start = 159000,    .end = 160000},
    {.text = "still alive.\n",                          .start = 160000,    .end = 161000},
    {.text = "PPS: I'm doing Science and I'm\n",        .start = 162500,    .end = 164000},
    {.text = "still alive.\n",                          .start = 164000,    .end = 166000},
    {.text = "PPPS: I feel FANTASTIC and I'm\n",        .start = 166000,    .end = 168000},
    {.text = "still alive.\n\n",                        .start = 168000,    .end = 169000},
    {.text = "FINAL THOUGHT:\n",                        .start = 170000,    .end = 170500},
    {.text = "While you're dying I'll be\n",            .start = 170500,    .end = 172000},
    {.text = "still alive.\n\n",                        .start = 172000,    .end = 173000},
    {.text = "FINAL THOUGHT PS:\n",                     .start = 173000,    .end = 173500},
    {.text = "And when you're dead I will be\n",        .start = 174000,    .end = 176500},
    {.text = "still alive.\n\n\n",                      .start = 176500,    .end = 178000},
    {.text = "STILL ALIVE\n",                           .start = 178000,    .end = 179000},
    {.text = "",                                        .start = 180000,    .end = 185000},

    {.text = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n    ",        .start = 185000,    .end = 185000,  .event = EVENT_CLEAR_SCREEN,    .no_echo = 1},
    {.text = "Portal is an amazing game!!!",            .start = 185000,    .end = 187000,  .event = EVENT_DRAW_HEART,      .no_echo = 1},

    {.text = 0,                                         .start = 187000,    .end = 190000},
};

///////////////////////////////////////////////////////////////////////////////

static const char song_picture[][PICTURE_SIZE_Y][PICTURE_SIZE_X] = {
    {
        "              .,-:;//;:=,                ",
        "          . :H@@@MM@M#H/.,+%;,           ",
        "       ,/X+ +M@@M@MM%=,-%HMMM@X/,        ",
        "     -+@MM; $M@@MH+-,;XMMMM@MMMM@+-      ",
        "    ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.    ",
        "  ,>MM@@MH ,@%=            .---=-=:=,.   ",
        "  =@#@@@MX .,              -%HX$$%%%+;   ",
        " =-./@M@M$                  .;@MMMM@MM:  ",
        " X@/.-$MM/                    .+MM@@@M$  ",
        ",@M@H: :@:                    . =X#@@@@- ",
        ",@@@MMX, .                    /H- ;@M@M= ",
        ".H@@@@M@+,                    %MM+..%#$. ",
        " /MMMM@MMH/.                  XM@MH, =;  ",
        "  /%+%$XHH@$=              , .H@@@@MX,   ",
        "   .=--------.           -%H.,@@@@@MX,   ",
        "   .%MM@@@HHHXX$$$%+- .:$MMX =M@@MM%.    ",
        "     =XMMM@MM@MM#H;,-+HMM@M+ /MMMX=      ",
        "       =%@M@M#@$-.=$@MM@@@M: %M<=        ",
        "         ,:+$+-,/H#MMMMMMM@= =,          ",
        "               =++%%%%+/:-               ",
    },
    {
        "            .+                           ",
        "             /M.                         ",
        "              H#@:              ,.       ",
        "              -###H-          -@/        ",
        "               %####$.  -;  .%#X         ",
        "                M#####+;#H :M#M.         ",
        "..          .+/;%#########X###-          ",
        " -/%H%+;-,    +##############/           ",
        "    .:$M###MH$%+############X  ,--=;-    ",
        "        -/H#####################H+=.     ",
        "            +#################X.         ",
        "         =%M####################H;.      ",
        "            /@###############+;;/%%;,    ",
        "         -%###################$.         ",
        "       ;H######################M=        ",
        "    ,%#####MH$%:+#####M###-/@####%       ",
        "  :$H%+;=-      -####X.,M#   -+M##@-     ",
        " .              .###.    :      =$##+    ",
        "                .#H,               :XH,  ",
        "                 +                   .:- ",
    },
    {
        "                 =/;;/-                  ",
        "                +:    //                 ",
        "               /,      /:                ",
        "              -X        H.               ",
        ".//;;::;:-,   X=        :+   .-;:=;:;%;. ",
        "M-       ,=;;;#:,      ,:#;;:=,       ,@ ",
        ":%           :%.=/++++/=.$=           %= ",
        " ,%;         %/:+/:,,/++:+/         ;+.  ",
        "   ,+/.    ,;@+,        ,%H;,    ,/+,    ",
        "      ;+;;/= @.  .H##X   -X :///+;       ",
        "      ;+=;;;.@,  .XM@$.  =X.//;=%/.      ",
        "   ,;:      :@%=        =$H:     .+%-    ",
        " ,%=         %;-///==///-//         =%,  ",
        ";+           :%-;;;;;;;;-X-           +: ",
        "@-      .-;;;;M-        =M/;;;-.      -X ",
        " :;;::;;-.    %-        :+    ,-;;-;:==  ",
        "              /X        H..              ",
        "               ./      %=                ",
        "                //    +;                 ",
        "                 ,////,                  ",
    },
    {
        "             =+$HM####@H%;,              ",
        "          /H###############M$            ",
        "          ,:################+            ",
        "           .H##############+             ",
        "             X############/              ",
        "              $##########/               ",
        "               %########/                ",
        "                /X/;;+X/                 ",
        "                                         ",
        "                 -XHHX-                  ",
        "                ,######,                 ",
        "#############X  .M####M.  X############# ",
        "##############-   -//-.  -############## ",
        "X##############%.      .+##############X ",
        "-##############X        X##############- ",
        " %############%          %############%  ",
        "  %##########,            ;##########%   ",
        "   ;#######M=              =M#######.    ",
        "    .+M###@,                ,@###M+.     ",
        "       :XH.                  .HX:        ",
    },
    {
        "                     -$-                 ",
        "                    .H##H.               ",
        "                   +######+              ",
        "                .+#########H.            ",
        "              -$############@.           ",
        "            =H###############@  -X:      ",
        "          .$##################:  @#@-    ",
        "     .;  .M###################;  H###:   ",
        "   ;@#:  @###################@  ,#####:  ",
        " -M###.  M#################@.  .######H  ",
        " M####-  +###############$   =@#######X  ",
        " H####$   -M###########+   :#########M.  ",
        "  /####X-   =########%   :M########@/.   ",
        "    .;%H@X;   .$###X   :##MM@%+;:-       ",
        "                 ..                      ",
        "  -/;:-,.              ,,-==+M########H  ",
        " -##################@HX%%+%%$%%%+:,,     ",
        "    .-/H%%%+%%$H@###############M@+=:/+: ",
        "/XHX%:#####MH%=    ,---:;;;;/%%XHM,:###$ ",
        "$@#MX %+;-                           .   ",
    },
    {
        "                                      :X-",
        "                                   :X### ",
        "                                 ;@####@ ",
        "                               ;M######X ",
        "                             -@########$ ",
        "                           .$##########@ ",
        "                          =M############-",
        "                         +##############$",
        "                       .H############$=. ",
        "          ,/:         ,M##########M,.    ",
        "       -+@###;       =##########M;       ",
        "    =%M#######;     :#########M/         ",
        "-$M############;   :#########/           ",
        " ,;X############; =########$.            ",
        "     ;H##########+#######M=              ",
        "       .+###############+                ",
        "          /M##########@-                 ",
        "            ;M#######%                   ",
        "              +#####;                    ",
        "               ,$M-                      ",
    },
    {
        "           .-,+$XHHHHHHX$+;-.            ",
        "        ,;X@@X%/.=----=:/%X@@X/,         ",
        "      =$@@%=.              .=+H@X:       ",
        "    -XMX:                      =XMX=     ",
        "   /@@:                          =H@+    ",
        "  %@X,                            .$@$   ",
        " +@X.                               $@%  ",
        "-@@.                                .@@= ",
        "%@%                                  +@$ ",
        "H@:                                  :@H ",
        "H@:         :HHHHHHHHHHHHHHHHHHX,    =@H ",
        "%@%         ;@M@@@@@@@@@@@@@@@@@H-   +@$ ",
        "=@@,        :@@@@@@@@@@@@@@@@@@@@@= .@@: ",
        " +@X        :@@@@@@@@@@@@@@@M@@@@@@:%@%  ",
        "  $@$,      ;@@@@@@@@@@@@@@@@@M@@@@@@$.  ",
        "   +@@HHHHHHH@@@@@@@@@@@@@@@@@@@@@@@+    ",
        "    =X@@@@@@@@@@@@@@@@@@@@@@@@@@@@X=     ",
        "      :$@@@@@@@@@@@@@@@@@@@M@@@@$:       ",
        "        ,;$@@@@@@@@@@@@@@@@@@X/-         ",
        "           .-;+$XXHHHHHX$+;-.            ",
    },
    {
        "                          .,---.         ",
        "                        ,/XM#MMMX;,      ",
        "                      -%##########M%,    ",
        "                     -@######%  $###@=   ",
        "      .,--,         -H#######$   $###M:  ",
        "   ,;$M###MMX;     .;##########$;HM###X= ",
        " ,/@##########H=      ;################+ ",
        "-+#############M/,      %##############+ ",
        "%M###############=      /##############: ",
        "H################      .M#############;. ",
        "@###############M      ,@###########M:.  ",
        "X################,      -$=X#######@:    ",
        "/###################%-     +######$-     ",
        ".;##################X     .X#####+,      ",
        " .;H################/     -X####+.       ",
        "   ,;X##############,       .MM/         ",
        "      ,:+$H@M#######M#$-    .$$=         ",
        "           .,-=;+$@###X:    ;/=.         ",
        "                  .,/X$;   .;:,          ",
        "                      .,    ..           ",
    },
    {
        "            ,:/+/-                       ",
        "            /M/              .,-=.//.-   ",
        "       .:/= ;MH/,    ,=/+%$XH@MM#@:      ",
        "      -$##@+$###@H@MMM#######H:.    -/H# ",
        " .,H@H@ X######@ -H#####@+-     -+H###@X ",
        "  .,@##H;      +XM##M/,     =%@###@X;-   ",
        "X%-  :M##########$.    .:%M###@%:        ",
        "M##H,   +H@@@$/-.  ,;$M###@%           - ",
        "M####M=,,---,.-%%H####M$:          ,+@## ",
        "@##################@/.         :%H##@$-  ",
        "M###############H,         ;HM##M$=      ",
        "#################.    .=$M##M$=          ",
        "################H..;XM##M$=          .:+ ",
        "M###################@%=           =+@MH% ",
        "@################M/.          =+H#X%=    ",
        "=+M##############M,       ,/X#X+;,       ",
        "  .;XM##########H=    ,/X#H+:,           ",
        "     .=+HM######M+/+HM@+=.               ",
        "         ,:/%XM####H/.                   ",
        "              ,.:=-.                     ",
    },
    {
        "       #+ @      # #              M#@    ",
        " .    .X  X.%##@,# #   +@#######X. @#%   ",
        "   ,==.   ,######M+  -#####%M####M-    # ",
        "  :H##M%:=##+ .M##M,;#####/+#######% ,M# ",
        " .M########=  =@#@.=#####M=M#######=  X# ",
        " :@@MMM##M.  -##M.,#######M#######. =  M ",
        "             @##..###:.    .H####. @@ X, ",
        "   ############: ###./####;  /##= @#. M  ",
        "           ,M## ,##,@#M;/M#M  @# X#% X#  ",
        " %=   ######M## ##.M#:   ./#M ,M #M ,#$  ",
        "##/         $## #+;#: #### ;#/ M M- @# : ",
        "#+ #M@MM###M-;M #:$#-##$H# .#X @ + $#. # ",
        "      ######/.: #%=# M#:MM./#.-#  @#: H# ",
        "+,.=   @###: /@ %#,@  ##@X #,-#@.##% .@# ",
        "#####+,/##/ @##  @#,+       /#M      X,  ",
        "   ,###M#@ M###H .#M-     ,##M  ;@@; ### ",
        "   .M#M##H ;####X ,@#######M/ -M###$  -H ",
        "    .M###%  X####H  .@@MM@;  .@#M@       ",
        "      H#M    /@####/      ,++.  / ==-,   ",
        "               ,=/:. .+X@MMH@#H  #####$= ",
    },
    {
        "                        .,---.           ",
        "                      ,/XM#MMMX;,        ",
        "                    -%##########M%,      ",
        "                   -@#############@=     ",
        "         .,--,    -H###############M:    ",
        "      ,;$M###MMX;.;#################X    ",
        "    ,/@##########H=;################+    ",
        "   -+###############################+    ",
        "   %M###############################:    ",
        "   H###############################;.    ",
        "   @#############################M:.     ",
        "   X############################@:       ",
        "   /############################$-       ",
        "   .;##########################+,        ",
        "    .;H#######################+.         ",
        "      ,;X##################H/            ",
        "         ,:+$H@M##########$=             ",
        "              .,-=;+$@###X:;             ",
        "                     .,/X$;              ",
        "                        .,               ",
    },
};

