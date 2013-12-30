#include "MapManager.h"

int g_maps[7][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,1,3,0,1,0,3,2,1,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,2,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0}
};

MapManager::MapManager() {
    initManager();
}

MapManager::~MapManager() {
//    delete [][] m_maps;
}

Grid* MapManager::getGrid(int gridId) {
    int len = m_grids.size();
    for (int i = 0; i < len; i++) {
        if(m_grids[i]->id == gridId) {
            return m_grids[i];
        }
    }
    return NULL;
}

Grid* MapManager::getSelectGrid() {
    int len = m_grids.size();
    for (int i = 0; i < len; i++) {
        if(m_grids[i]->id == m_selectId) {
            return m_grids[i];
        }
    }
    return NULL;
}


void MapManager::initManager() {
    memcpy(m_maps, g_maps, sizeof(m_maps));
    
    m_selectId = -1;
       
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            Grid* grid = new Grid();
            grid->id = 1 + j + i * COLUMN;
            grid->row = i;
            grid->col = j;
            if (m_maps[i][j] != 0) {
                grid->status = Grid::Normal;
            } else {
                grid->status = Grid::Empty;
            }
            grid->imageId = m_maps[i][j];
            m_grids.push_back(grid);
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
        return m_path;
    }
    
    if (matchTwoLine(gridId1, gridId2)) {
        CCLog("MapManager::matchTwoLine done");
        return m_path;
    }
    
    if (matchThreeLine(gridId1, gridId2)) {
        CCLog("MapManager::matchThreeLine done");
        return m_path;
    }
    
    return Path();
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
    int pos = g2->col + g1->row * COLUMN;
    Grid* g3 = m_grids[pos];
    if ( matchLine(g1->id, g3->id) && matchLine(g2->id, g3->id) ) {
        m_path.push_back(gridId1);
        m_path.push_back(g3->id);
        m_path.push_back(gridId2);
        return true;
    }
    
    pos = g1->col + g2->row * COLUMN;
    Grid* g4 = m_grids[pos];
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

bool MapManager::isRowEmpty(int row, int col1, int col2){
    //水平方向上能否消除
    if( abs(col1 - col2) <= 1 ) {
        return true;
    }
    if( col1 < col2 ) {
        for (int i = col1 + 1; i < col2; i++) {
            int pos = i + row * COLUMN;
            if ( m_grids[pos]->status != Grid::Empty ) {
                return false;
            }
        }
    } else {
        for (int i = col2 + 1; i < col1; i++) {
            int pos = i + row * COLUMN;
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
            int pos = col + i * ROW;
            if ( m_grids[pos]->status != Grid::Empty ) {
                return false;
            }
        }
    } else {
        for (int i = row2 + 1; i < row1; i++) {
            int pos = col + i * ROW;
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
        pos = i + grid->row * ROW;
        if (m_grids[pos]->status == Grid::Empty) {
            list.push_back(m_grids[pos]->id);
        } else {
            break;
        }
    }
    for (int i = grid->col + 1; i < COLUMN; i++) {
        pos = i + grid->row * ROW;
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
        pos = grid->col + i * ROW;
        if (m_grids[pos]->status == Grid::Empty) {
            list.push_back(m_grids[pos]->id);
        } else {
            break;
        }
    }
    for (int i = grid->row + 1; i < COLUMN; i++) {
        pos = grid->col + i * ROW;
        if (m_grids[pos]->status == Grid::Empty) {
            list.push_back(m_grids[pos]->id);
        } else {
            break;
        }
    }
    return list;
}

bool MapManager::isMapClear() {
    int len = m_grids.size();
    for (int i = 0; i < len; i++) {
        if(m_grids[i]->status != Grid::Empty) {
            return false;
        }
    }
    return true;
}



