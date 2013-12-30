#ifndef __MapManager_H__
#define __MapManager_H__

#include <vector>
#include "math.h"

#include "cocos2d.h"

USING_NS_CC;

using namespace std;

struct Grid {
    enum Status {
        InValid     = 0x00000000,
        Normal      = 0x00000001,
        Select      = 0x00000002,
        Empty       = 0x00000003,
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

    GridArray& getGrids() { return m_grids; }
    Grid* getGrid(int gridId);
    Grid* getSelectGrid();
    void setSelectGrid(int gridid) { m_selectId = gridid;}

    Path match(int gridId1, int gridId2);
    bool isMapClear();

    const int ROW = 7;
    const int COLUMN = 10;
private:
    int m_maps[7][10];
    int m_score;
    int m_stage;
    int m_selectId;
    float m_time;
    Path m_path;
    GridArray m_grids;
    void initManager();
    bool matchLine(int gridId1, int gridId2);
    bool matchTwoLine(int gridId1, int gridId2);
    bool matchThreeLine(int gridId1, int gridId2);
    bool isRowEmpty(int row, int col1, int col2);
    bool isColEmpty(int col, int row1, int row2);
    bool matchHorizontal(int gridId1, int gridId2);
    bool matchVertical(int gridId1, int gridId2);
    vector<int> getHorizontalEmpty(int gridId);
    vector<int> getVerticalEmpty(int gridId);
};

#endif // __MapManager_H__
