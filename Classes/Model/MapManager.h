#ifndef __MapManager_H__
#define __MapManager_H__

#include <map>
#include <vector>
#include <queue>
#include <cstdlib>
#include "math.h"

using namespace std;

struct Grid {
    enum Status {
        InValid     = 0x00000000,
        Normal      = 0x00000001,
        Select      = 0x00000002,
        Match       = 0x00000004,
        Linking     = 0x00000008,
        Empty       = 0x00000010,
        Edge        = 0x10000000
    };
    bool visible() const {
        return !((status & InValid) || (status & Empty) || (status & Edge));
    }
    int row;
    int col;
    int status;
    int imageId;
    int id;
};

typedef vector<int> Path;

struct MatchGrid {
    int head;
    int tail;
    Path path;
};

class MapManager {
public:
    typedef map<int, Grid*> GridArray;
    typedef queue<MatchGrid> MatchQueue;

    MapManager();
    MapManager(int level);
    virtual ~MapManager();

    int getScore() const { return m_score; }
    void setScore(int score) { m_score = score; }
    int getStage() const { return m_stage; }
    void setStage(int stage) { m_stage = stage; }
    float getTime() const { return m_time; }
    void setTime(float time) { m_time = time; }

    GridArray& getGrids() { return m_grids; }
    Grid* getGrid(int gridId);
    Grid* getSelectGrid();
    void setSelectGrid(int gridid);
    void clearSelectGrid();

    Path match(int gridId1, int gridId2);
    bool linkGrid(int gridId1, int gridId2);
    bool isMapClear();
    bool linkable();
    MatchGrid popMatch();
    void reArrange();
    void pushLink(const MatchGrid& m);
    MatchGrid popLink();

private:
    int m_maps[7][10];
    int m_imageTypeCount;
    int m_gridCount;
    int m_score;
    int m_stage;
    int m_selectId;
    float m_time;
    Path m_path;
    MatchQueue m_matchQueue;
    MatchQueue m_linkQueue;
    GridArray m_grids;
    void initManager();
    void initMap(int level = 1);
    bool matchLine(int gridId1, int gridId2);
    bool matchTwoLine(int gridId1, int gridId2);
    bool matchThreeLine(int gridId1, int gridId2);
    bool isRowEmpty(int row, int col1, int col2);
    bool isColEmpty(int col, int row1, int row2);
    bool matchHorizontal(int gridId1, int gridId2);
    bool matchVertical(int gridId1, int gridId2);
    vector<int> getHorizontalEmpty(int gridId);
    vector<int> getVerticalEmpty(int gridId);
    int random(int size) {return rand()%size;};
    void swap(Grid *g1, Grid *g2);
};

#endif // __MapManager_H__
