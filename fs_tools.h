#ifndef __FS_TOOLS_H___
#define __FS_TOOLS_H___

#include <Arduino.h>
#include <FS.h>

bool fs_tools_FS_begin(void);
bool fs_tools_FS_end(void);

int fs_tools_readFile(const char* filePath, String& text);
int fs_tools_readFile(const String& filePath, String& text, size_t length = 0);
int fs_tools_readFile(const String& filePath, char* text);
int fs_tools_readFile(const char* filePath, char* text, size_t length = 0);

#endif  // !__FS_TOOLS_H___
