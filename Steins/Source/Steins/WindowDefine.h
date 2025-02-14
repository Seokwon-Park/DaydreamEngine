#pragma once
#define STEINS_MOUSE_BUTTON_1         0
#define STEINS_MOUSE_BUTTON_2         1
#define STEINS_MOUSE_BUTTON_3         2
#define STEINS_MOUSE_BUTTON_4         3
#define STEINS_MOUSE_BUTTON_5         4
#define STEINS_MOUSE_BUTTON_6         5
#define STEINS_MOUSE_BUTTON_7         6
#define STEINS_MOUSE_BUTTON_8         7
#define STEINS_MOUSE_BUTTON_LAST      STEINS_MOUSE_BUTTON_8
#define STEINS_MOUSE_BUTTON_LEFT      STEINS_MOUSE_BUTTON_1
#define STEINS_MOUSE_BUTTON_RIGHT     STEINS_MOUSE_BUTTON_2
#define STEINS_MOUSE_BUTTON_MIDDLE    STEINS_MOUSE_BUTTON_3

#define STEINS_RELEASE 0
#define STEINS_PRESS 1
#define STEINS_REPEAT 2
#define STEINS_IDLE 3

#define STEINS_KEY_UNKNOWN            -1

/* Printable keys */
#define STEINS_KEY_SPACE              32
#define STEINS_KEY_APOSTROPHE         39  /* ' */
#define STEINS_KEY_COMMA              44  /* , */
#define STEINS_KEY_MINUS              45  /* - */
#define STEINS_KEY_PERIOD             46  /* . */
#define STEINS_KEY_SLASH              47  /* / */
#define STEINS_KEY_0                  48
#define STEINS_KEY_1                  49
#define STEINS_KEY_2                  50
#define STEINS_KEY_3                  51
#define STEINS_KEY_4                  52
#define STEINS_KEY_5                  53
#define STEINS_KEY_6                  54
#define STEINS_KEY_7                  55
#define STEINS_KEY_8                  56
#define STEINS_KEY_9                  57
#define STEINS_KEY_SEMICOLON          59  /* ; */
#define STEINS_KEY_EQUAL              61  /* = */
#define STEINS_KEY_A                  65
#define STEINS_KEY_B                  66
#define STEINS_KEY_C                  67
#define STEINS_KEY_D                  68
#define STEINS_KEY_E                  69
#define STEINS_KEY_F                  70
#define STEINS_KEY_G                  71
#define STEINS_KEY_H                  72
#define STEINS_KEY_I                  73
#define STEINS_KEY_J                  74
#define STEINS_KEY_K                  75
#define STEINS_KEY_L                  76
#define STEINS_KEY_M                  77
#define STEINS_KEY_N                  78
#define STEINS_KEY_O                  79
#define STEINS_KEY_P                  80
#define STEINS_KEY_Q                  81
#define STEINS_KEY_R                  82
#define STEINS_KEY_S                  83
#define STEINS_KEY_T                  84
#define STEINS_KEY_U                  85
#define STEINS_KEY_V                  86
#define STEINS_KEY_W                  87
#define STEINS_KEY_X                  88
#define STEINS_KEY_Y                  89
#define STEINS_KEY_Z                  90
#define STEINS_KEY_LEFT_BRACKET       91  /* [ */
#define STEINS_KEY_BACKSLASH          92  /* \ */
#define STEINS_KEY_RIGHT_BRACKET      93  /* ] */
#define STEINS_KEY_GRAVE_ACCENT       96  /* ` */
#define STEINS_KEY_WORLD_1            161 /* non-US #1 */
#define STEINS_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define STEINS_KEY_ESCAPE             256
#define STEINS_KEY_ENTER              257
#define STEINS_KEY_TAB                258
#define STEINS_KEY_BACKSPACE          259
#define STEINS_KEY_INSERT             260
#define STEINS_KEY_DELETE             261
#define STEINS_KEY_RIGHT              262
#define STEINS_KEY_LEFT               263
#define STEINS_KEY_DOWN               264
#define STEINS_KEY_UP                 265
#define STEINS_KEY_PAGE_UP            266
#define STEINS_KEY_PAGE_DOWN          267
#define STEINS_KEY_HOME               268
#define STEINS_KEY_END                269
#define STEINS_KEY_CAPS_LOCK          280
#define STEINS_KEY_SCROLL_LOCK        281
#define STEINS_KEY_NUM_LOCK           282
#define STEINS_KEY_PRINT_SCREEN       283
#define STEINS_KEY_PAUSE              284
#define STEINS_KEY_F1                 290
#define STEINS_KEY_F2                 291
#define STEINS_KEY_F3                 292
#define STEINS_KEY_F4                 293
#define STEINS_KEY_F5                 294
#define STEINS_KEY_F6                 295
#define STEINS_KEY_F7                 296
#define STEINS_KEY_F8                 297
#define STEINS_KEY_F9                 298
#define STEINS_KEY_F10                299
#define STEINS_KEY_F11                300
#define STEINS_KEY_F12                301
#define STEINS_KEY_F13                302
#define STEINS_KEY_F14                303
#define STEINS_KEY_F15                304
#define STEINS_KEY_F16                305
#define STEINS_KEY_F17                306
#define STEINS_KEY_F18                307
#define STEINS_KEY_F19                308
#define STEINS_KEY_F20                309
#define STEINS_KEY_F21                310
#define STEINS_KEY_F22                311
#define STEINS_KEY_F23                312
#define STEINS_KEY_F24                313
#define STEINS_KEY_F25                314
#define STEINS_KEY_KP_0               320
#define STEINS_KEY_KP_1               321
#define STEINS_KEY_KP_2               322
#define STEINS_KEY_KP_3               323
#define STEINS_KEY_KP_4               324
#define STEINS_KEY_KP_5               325
#define STEINS_KEY_KP_6               326
#define STEINS_KEY_KP_7               327
#define STEINS_KEY_KP_8               328
#define STEINS_KEY_KP_9               329
#define STEINS_KEY_KP_DECIMAL         330
#define STEINS_KEY_KP_DIVIDE          331
#define STEINS_KEY_KP_MULTIPLY        332
#define STEINS_KEY_KP_SUBTRACT        333
#define STEINS_KEY_KP_ADD             334
#define STEINS_KEY_KP_ENTER           335
#define STEINS_KEY_KP_EQUAL           336
#define STEINS_KEY_LEFT_SHIFT         340
#define STEINS_KEY_LEFT_CONTROL       341
#define STEINS_KEY_LEFT_ALT           342
#define STEINS_KEY_LEFT_SUPER         343
#define STEINS_KEY_RIGHT_SHIFT        344
#define STEINS_KEY_RIGHT_CONTROL      345
#define STEINS_KEY_RIGHT_ALT          346
#define STEINS_KEY_RIGHT_SUPER        347
#define STEINS_KEY_MENU               348

#define STEINS_KEY_LAST               STEINS_KEY_MENU
