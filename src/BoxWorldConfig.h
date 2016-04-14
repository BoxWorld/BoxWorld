//
//  Config.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_Config_h
#define BoxWorld_Config_h

#define BOXWORLD_WIDTH  1024
#define BOXWORLD_HEIGHT 768

/*
 * Define current running hardware platform.
 */
#ifdef _WIN32
  #ifdef _WIN64
    #define BOXWORLD_DEV_WIN
  #endif
#elif defined __APPLE__
    #define BOXWORLD_DEV_MAC
#elif defined __arm__
    #define BOXWORLD_TARGET_ARM
#elif defined __linux
    #define BOXWORLD_TARGET_LINUX
#endif

#endif
