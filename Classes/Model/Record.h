#ifndef __Record_H__
#define __Record_H__

#include <string>

using namespace std;

class Record {
public:
    Record();
    virtual ~Record();

    bool load();
    bool save();

    void setHighScore(int score) { m_highscore = score; }
    int getHighScore() const { return m_highscore; }
    void setUserName(string name) { m_username = name; }
    string getUserName() const { return m_username; }
private:
    int m_highscore;
    string m_username;
    string m_dbPath;
};

#endif // __Record_H__
