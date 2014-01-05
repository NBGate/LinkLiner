#include <Config.h>
#include "MapManager.h"
#include <algorithm>

#include "cocos2d.h"
USING_NS_CC;

int g_maps[7][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};

int g_counts[40] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};


int g_images[20][2] = {
    {10, 3},
    {12, 3},
    {14, 4},
    {16, 4},
    {20, 4},
    {22, 5},
    {24, 5},
    {26, 5},
    {28, 5},
    {30, 6},
    {32, 6},
    {34, 6},
    {36, 6},
    {38, 7},
    {40, 7},
    {40, 8},
    {40, 8},
    {40, 9},
    {40, 9},
    {40, 10}
};

MapManager::MapManager() {
    initMap(1);
    initManager();
}

MapManager::MapManager(int level) {
    initMap(level);
    initManager();
}

void MapManager::initMap(int level) {
    memcpy(m_maps, g_maps, sizeof(g_maps));
    vector<int> v_counts;
    for (int i = 0; i < 40; i++) {
        v_counts.push_back(i);
    }

    m_imageTypeCount = g_images[level-1][1];
    m_gridCount = g_images[level-1][0];

    int tempTypeCount = m_imageTypeCount;
    int tempType;
    int tempCount = m_gridCount;

    srand((int)time(0));
    for (int count = 0; count < (TOTAL_ROW - 2) * (TOTAL_COl - 2); count++) {
        if (tempCount <= 0) break;
        if ((count % 2) == 0) { //确保生成同样的个数是偶数
            if (tempTypeCount > 0) { //确保所有图像都能用上
                tempType = tempTypeCount;
                tempTypeCount--;
            }
            else
            {
                tempType = random(m_imageTypeCount) + 1;
            }
        }
        int pos = random(v_counts.size());
        int randPos = v_counts[pos];
        int row = randPos / 8;
        int col = randPos % 8;
        m_maps[row+1][col+1] = tempType;
        vector<int>::iterator iter;
        iter = remove(v_counts.begin(), v_counts.end(), randPos);
        v_counts.erase(iter);
        tempCount--;
    }
}

MapManager::~MapManager() {
    //    delete [][] m_maps;
    for (int i = 0; i < m_grids.size(); i++) {
        delete m_grids[i];
    }
}

Grid* MapManager::getGrid(int gridId) {
    if (m_grids.find(gridId) != m_grids.end())
        return m_grids[gridId];
    return NULL;
}

Grid* MapManager::getSelectGrid() {
    return getGrid(m_selectId);
}

void MapManager::setSelectGrid(int gridId) {
    if (gridId == m_selectId) {
        return;
    }
    
    clearSelectGrid();
    
    if (m_grids.find(gridId) == m_grids.end()) {
        m_selectId = -1;
        return;
    }

    m_selectId = gridId;
    m_grids[m_selectId]->status |= Grid::Select;
}

void MapManager::clearSelectGrid() {
    if (m_grids.find(m_selectId) == m_grids.end())
        return;

    m_grids[m_selectId]->status &= ~Grid::Select;
}

void MapManager::initManager() {
    m_selectId = -1;

    for(int i = 0; i < TOTAL_ROW; i++) {
        for(int j = 0; j < TOTAL_COl; j++) {
            Grid* grid = new Grid();
            grid->id = j + i * TOTAL_COl;
            grid->row = i;
            grid->col = j;
            if (m_maps[i][j] != 0) {
                grid->status = Grid::Normal;
            } else {
                grid->status = Grid::Empty;
            }
            grid->imageId = m_maps[i][j];
            m_grids.insert(pair<int, Grid*>(grid->id, grid));
        }
    }
}

MapManager::Path MapManager::match(int gridId1, int gridId2) {
    if (gridId1 == gridId2 || gridId1 < 0 || gridId2 < 0) {
        return Path();
    }

    Grid* g1 = getGrid(gridId1);
    Grid* g2 = getGrid(gridId2);

    if (g1 == NULL || g2 == NULL) {
        return Path();
    }

    if (g1->imageId != g2->imageId) {
        return Path();
    }

    if (matchLine(gridId1, gridId2)) {
        m_path.push_back(gridId1);
        m_path.push_back(gridId2);
        CCLog("MapManager::matchLine done");
    } else if (matchTwoLine(gridId1, gridId2)) {
        CCLog("MapManager::matchTwoLine done");
    } else if (matchThreeLine(gridId1, gridId2)) {
        CCLog("MapManager::matchThreeLine done");
    }

    return m_path;
}

bool MapManager::linkGrid(int gridId1, int gridId2) {
    Path p = match(gridId1, gridId2);
    if (p.size() > 0) {
        Match m;
        m.head = gridId1;
        m.tail = gridId2;
        m.path = p;
        m_matchQueue.push(m);
        return true;
    }
    return false;
}

bool MapManager::matchLine(int gridId1, int gridId2) {
    //判断一条连线上是否能消除
    m_path.clear();

    Grid* g1 = getGrid(gridId1);
    Grid* g2 = getGrid(gridId2);

    if (g1 == NULL || g2 == NULL) {
        return false;
    }

    if (g1->row == g2->row) {
        return isRowEmpty(g1->row, g1->col, g2->col);
    }

    if (g1->col == g2->col) {
        return isColEmpty(g1->col, g1->row, g2->row);
    }

    return false;
}

bool MapManager::matchTwoLine(int gridId1, int gridId2) {
    //判断一次折线能否消除
    m_path.clear();

    Grid* g1 = this->getGrid(gridId1);
    Grid* g2 = this->getGrid(gridId2);

    if (g1 == NULL || g2 == NULL) {
        return false;
    }

    //得到矩形区域的另外两个点
    int pos = g2->col + g1->row * TOTAL_COl;
    Grid* g3 = m_grids[pos];
    if (g3->status != Grid::Empty) {
        return false;
    }
    if ( matchLine(g1->id, g3->id) && matchLine(g2->id, g3->id) ) {
        m_path.push_back(gridId1);
        m_path.push_back(g3->id);
        m_path.push_back(gridId2);
        return true;
    }

    pos = g1->col + g2->row * TOTAL_COl;
    Grid* g4 = m_grids[pos];
    if (g4->status != Grid::Empty) {
        return false;
    }
    if ( matchLine(g1->id, g4->id) && matchLine(g2->id, g4->id) ) {
        m_path.push_back(gridId1);
        m_path.push_back(g4->id);
        m_path.push_back(gridId2);
        return true;
    }

    return false;
}

bool MapManager::matchThreeLine(int gridId1, int gridId2) {
    //两次折线能否消除
    m_path.clear();

    if (matchHorizontal(gridId1, gridId2)) {
        return true;
    }
    if (matchVertical(gridId1, gridId2)) {
        return true;
    }
    return false;
}

bool MapManager::isRowEmpty(int row, int col1, int col2) {
    //水平方向上能否消除
    if( abs(col1 - col2) <= 1 ) {
        return true;
    }
    if( col1 < col2 ) {
        for (int i = col1 + 1; i < col2; i++) {
            int pos = i + row * TOTAL_COl;
            if ( m_grids[pos]->status != Grid::Empty ) {
                return false;
            }
        }
    } else {
        for (int i = col2 + 1; i < col1; i++) {
            int pos = i + row * TOTAL_COl;
            if ( m_grids[pos]->status != Grid::Empty ) {
                return false;
            }
        }
    }
    return true;
}

bool MapManager::isColEmpty(int col, int row1, int row2) {
    //垂直方向上能否消除
    if( abs(row1 - row2) <= 1 ) {
        return true;
    }
    if( row1 < row2 ) {
        for (int i = row1 + 1; i < row2; i++) {
            int pos = col + i * TOTAL_COl;
            if ( m_grids[pos]->status != Grid::Empty ) {
                return false;
            }
        }
    } else {
        for (int i = row2 + 1; i < row1; i++) {
            int pos = col + i * TOTAL_COl;
            if ( m_grids[pos]->status != Grid::Empty ) {
                return false;
            }
        }
    }
    return true;
}

bool MapManager::matchHorizontal(int gridId1, int gridId2) {
    //两点在水平方向上，能否通过2次折线连接
    Grid* g1 = this->getGrid(gridId1);
    Grid* g2 = this->getGrid(gridId2);

    if (g1 == NULL || g2 == NULL) {
        return false;
    }
    vector<int> g1List = getHorizontalEmpty(g1->id);
    vector<int> g2List = getHorizontalEmpty(g2->id);
    int g1Len = g1List.size();
    int g2Len = g2List.size();
    for (int i = 0; i < g1Len; i++) {
        Grid* g1Temp = this->getGrid(g1List[i]);
        for (int j = 0; j < g2Len; j++) {
            Grid* g2Temp = this->getGrid(g2List[j]);
            if (g1Temp->col == g2Temp->col) {
                if (isColEmpty(g1Temp->col, g1Temp->row, g2Temp->row)) {
                    m_path.push_back(gridId1);
                    m_path.push_back(g1Temp->id);
                    m_path.push_back(g2Temp->id);
                    m_path.push_back(gridId2);
                    return true;
                }
            }
        }
    }

    return false;
}

bool MapManager::matchVertical(int gridId1, int gridId2) {
    //两点在垂直方向上，能否通过2次折线连接
    Grid* g1 = this->getGrid(gridId1);
    Grid* g2 = this->getGrid(gridId2);

    if (g1 == NULL || g2 == NULL) {
        return false;
    }
    vector<int> g1List = getVerticalEmpty(g1->id);
    vector<int> g2List = getVerticalEmpty(g2->id);
    int g1Len = g1List.size();
    int g2Len = g2List.size();
    for (int i = 0; i < g1Len; i++) {
        Grid* g1Temp = this->getGrid(g1List[i]);
        for (int j = 0; j < g2Len; j++) {
            Grid* g2Temp = this->getGrid(g2List[j]);
            if (g1Temp->row == g2Temp->row) {
                if (isRowEmpty(g1Temp->row, g1Temp->col, g2Temp->col)) {
                    m_path.push_back(gridId1);
                    m_path.push_back(g1Temp->id);
                    m_path.push_back(g2Temp->id);
                    m_path.push_back(gridId2);
                    return true;
                }
            }
        }
    }

    return false;
}

vector<int> MapManager::getHorizontalEmpty(int gridId) {
    //节点在水平方向上，左右可以延伸的id列表
    vector<int> list;
    Grid* grid = this->getGrid(gridId);
    int pos;
    for (int i = grid->col - 1; i >= 0; i--) {
        pos = i + grid->row * TOTAL_COl;
        if (m_grids[pos]->status == Grid::Empty) {
            list.push_back(m_grids[pos]->id);
        } else {
            break;
        }
    }
    for (int i = grid->col + 1; i < TOTAL_COl; i++) {
        pos = i + grid->row * TOTAL_COl;
        if (m_grids[pos]->status == Grid::Empty) {
            list.push_back(m_grids[pos]->id);
        } else {
            break;
        }
    }
    return list;
}

vector<int> MapManager::getVerticalEmpty(int gridId) {
    //节点在垂直方向上，上下可以延伸的id列表
    vector<int> list;
    Grid* grid = this->getGrid(gridId);
    int pos;
    for (int i = grid->row - 1; i >= 0; i--) {
        pos = grid->col + i * TOTAL_COl;
        if (m_grids[pos]->status == Grid::Empty) {
            list.push_back(m_grids[pos]->id);
        } else {
            break;
        }
    }
    for (int i = grid->row + 1; i < TOTAL_ROW; i++) {
        pos = grid->col + i * TOTAL_COl;
        if (m_grids[pos]->status == Grid::Empty) {
            list.push_back(m_grids[pos]->id);
        } else {
            break;
        }
    }
    return list;
}

bool MapManager::isMapClear() {
    GridArray::iterator pos = m_grids.begin();
    for (; pos != m_grids.end(); ++pos) {
        if(pos->second->status != Grid::Empty) {
            return false;
        }
    }
    return true;
}

MapManager::Match MapManager::getMatch() {
    Match m = m_matchQueue.front();
    m_matchQueue.pop();
    return m;
}

bool MapManager::linkable() {
    int len = m_grids.size();
    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            if (m_grids[i]->status == Grid::Empty || m_grids[j]->status == Grid::Empty) {
                continue;
            }
            if (m_grids[i]->imageId == m_grids[j]->imageId) {
                Path p = match(m_grids[i]->id, m_grids[j]->id);
                if (p.size() > 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

void MapManager::reArrange() {
    int len = m_grids.size();
    for (int i = 0; i < len; i++) {
        for (int j = len - 1; j > i; j--) {
            if (m_grids[i]->status == Grid::Empty || m_grids[j]->status == Grid::Empty) {
                continue;
            }
            if (m_grids[i]->imageId != m_grids[j]->imageId) {
                swap(m_grids[i], m_grids[j]);
                continue;
            }
        }
    }
}

void MapManager::swap(Grid *g1, Grid *g2) {
    Grid temp = *g1;
    *g1 = *g2;
    *g2 = temp;
}



