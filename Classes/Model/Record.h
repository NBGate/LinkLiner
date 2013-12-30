#ifndef __Record_H__
#define __Record_H__

#include <string>

using namespace std;

class Record {
public:
    static Record* instance();
    virtual ~Record();

    bool load();
    bool save();

    void setHighScore(int score) { m_highscore = score; }
    int getHighScore() const { return m_highscore; }
    void setUserName(string name) { m_username = name; }
    string getUserName() const { return m_username; }
protected:
    Record();
    Record(const Record&) { }
private:
    string m_username;
    int m_highscore;
    bool m_mute;
    string m_dbPath;
    static Record* m_instance;
};

#endif // __Record_H__
