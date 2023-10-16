#include "fs_tools.h"

bool fs_tools_FS_begin(void)
{
    if (SPIFFS.begin()) return true;
    else return false;
}

bool fs_tools_FS_end(void)
{
    SPIFFS.end();
    return true;
}

int fs_tools_readFile(const char* filePath, String& text)
{
    return fs_tools_readFile(String(filePath), text, 0);
}

int fs_tools_readFile(const String& filePath, String& text, size_t length)
{
    if (!SPIFFS.exists(filePath)) return -2;

    File f = SPIFFS.open(filePath, "r");
    if (length == 0) {
        length = f.size();
    }
    text = f.readString();
    f.close();

    return 0;
}

int fs_tools_readFile(const String& filePath, char* text)
{
    return fs_tools_readFile(filePath.c_str(), text, 0);
}

int fs_tools_readFile(const char* filePath, char* text, size_t length)
{
    if (!SPIFFS.exists(filePath)) return -2;

    File f = SPIFFS.open(filePath, "r");
    if (length == 0) {
        length = f.size();
    }
    f.readBytes(text, length);
    f.close();

    return 0;
}
