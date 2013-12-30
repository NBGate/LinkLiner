#include "Record.h"
#include <Utils/Utils.h>

Record::Record() {
    m_highscore = 0;
    m_dbPath = writablePath() + "save.db";
}

Record::~Record() {
}

bool Record::load() {
    FILE* fp = fopen(m_dbPath.c_str(), "rb");
    if (fp == NULL)
        return false;

    int length = 0;
    fread(&length, sizeof(uint32_t), 1, fp);
    if (length > 0) {
        char *name = new char[length];
        fread(name, sizeof(char), length, fp);
        m_username = string(name);
        delete[] name;
    }
    fread(&this->m_highscore, sizeof(uint32_t), 1, fp);
    fclose(fp);
    return true;
}

bool Record::save() {
    FILE* fp = fopen(m_dbPath.c_str(), "rb");
    if (fp == NULL)
        return false;

    int length = m_username.length();
    fwrite(&length, sizeof(uint32_t), 1, fp);
    if (length > 0) {
        fwrite(m_username.c_str(), sizeof(char), length, fp);
    }
    fwrite(&m_highscore, sizeof(uint32_t), 1, fp);
    fclose(fp);
    return true;
}

