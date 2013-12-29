#ifndef __MapManager_H__
#define __MapManager_H__

#include <vector>

using namespace std;

struct Grid {
    enum Status {
        InValid     = 0x00000000,
        Normal      = 0x00000001,
        Select      = 0x00000002,
        Edge        = 0x10000000
    };
    int row;
    int col;
    Status status;
    int imageId;
    int id;
};

class MapManager {
public:
    typedef vector<Grid*> GridArray;
    typedef vector<int> Path;

    MapManager();
    virtual ~MapManager();

    int getScore() const { return m_score; }
    void setScore(int score) { m_score = score; }
    int getStage() const { return m_stage; }
    void setStage(int stage) { m_stage = stage; }
    float getTime() const { return m_time; }
    void setTime(float time) { m_time = time; }

    const GridArray& getGrids() const { return m_grids; }
    const Grid* getGrid(int gridId) const { return 0; }
    const Grid* getSelectGrid() const { return 0; }
    void setSelectGrid(int gridid) { }

    Path match(int gridId1, int gridId2) { return Path(); }
    bool isMapClear() const { return false; }
private:
    int m_score;
    int m_stage;
    float m_time;
    GridArray m_grids;
};

#endif // __MapManager_H__
