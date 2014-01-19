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

    GameLogic(GameLayer* view);
    virtual ~GameLogic();

    MapManager* currentMap() const { return m_currentMap; }
    void startNewGame();
    void startStage(int stage);
    void update(float delta);
    void updateScore();
    void touchGrid(int gridId);
    void onLinkEffectEnd();

private:
    GameLayer* view() { return m_view; }
    void showLinkEffect();
private:
    typedef vector<MapManager*> MapPool;
    State m_state;
    MapPool m_maps;
    GameLayer* m_view;
    MapManager* m_currentMap;
    int currentMapIndex;
};

#endif // __GameLogic_H__
