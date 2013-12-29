#include "GameLogic.h"
#include <Model/MapManager.h>
#include <View/GameLayer.h>

GameLogic::GameLogic() {
    for (int i = 0; i < 20; i++) {
        MapManager* map = new MapManager();
        m_maps.push_back(map);
    }
    m_currentMap = m_maps[0];
}

GameLogic::~GameLogic() {
    for (int i = 0; i < m_maps.size(); i++) {
        delete m_maps[i];
    }
    m_maps.clear();
    m_currentMap = 0;
}

void GameLogic::startNewGame() {
    startStage(0);
}

void GameLogic::startStage(int stage) {
    if (stage >= 0 && stage < m_maps.size()) {
        m_currentMap = m_maps[stage];
    }
}

void GameLogic::update(float delta) {
    float time = m_currentMap->getTime() - delta;
    if (time <= 0) {
        m_currentMap->setTime(0);
        view()->timesUp();
    }
    else {
        m_currentMap->setTime(time);
    }
}

void GameLogic::touchGrid(int gridId) {
    Grid* grid = m_currentMap->getSelectGrid();
    
    if (grid == NULL) {
        m_currentMap->setSelectGrid(gridId);
    }
    else {
        printf("%d %d", grid->id, gridId);
        MapManager::Path path = m_currentMap->match(grid->id, gridId);
        if (path.size() == 0) {
            m_currentMap->setSelectGrid(gridId);
        }
        else {
            printf("xiao chu");
            grid->status = Grid::Empty;
            m_currentMap->getGrid(gridId)->status = Grid::Empty;
            m_currentMap->setSelectGrid(-1);
        }
    }
}

