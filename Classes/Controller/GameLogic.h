#ifndef __GameLogic_H__
#define __GameLogic_H__

#include <vector>

using namespace std;

class GameLayer;
class MapManager;

class GameLogic {
public:
    enum State {
        Init,
        Select,
        Match
    };

    GameLogic();
    virtual ~GameLogic();

    MapManager* currentMap() const { return m_currentMap; }
    void startNewGame();
    void startStage(int stage);
    void update(float delta);
    void touchGrid(int gridId);

private:
    GameLayer* view() { return m_view; }
private:
    typedef vector<MapManager*> MapPool;
    State m_state;
    MapPool m_maps;
    GameLayer* m_view;
    MapManager* m_currentMap;
};

#endif // __GameLogic_H__
