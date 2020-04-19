#pragma once

#include <QGraphicsView>
#include <QGraphicsItem>

typedef struct _tag_loc
{
    int      id;                  // id of the location
    int      type;                // 0 is room, 1 is hall, 2 is starting location
    const char*    name;
    QRect    bb;                  // bounding box of the location
    int      maxToons;            // max number of avatars in the location
    int      curToons;            // current count of occupants
    int      links[4];            // number of neighbors, -1 is not used
} locT, *plocT;

class QGraphicsScene;

class gameBoard : public QGraphicsView
{
    Q_OBJECT

public:
    gameBoard(QWidget* parent);

signals:
    void currentLoc(int);


protected:

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

private:
    QGraphicsScene*       m_pScene;

    QMap<int, QGraphicsItem*>   m_pTokens;  // map of player ID -> tokens.
    QMap<QGraphicsItem*, int>   m_pRevTokens; // map of tokens -> player ID.


    int                        m_cntLocs;  // number of nodes in the graph house
    locT                       house[27];  // graph representation of the house
    QGraphicsItem*             m_pToken;   // token we are going to move
    int                        m_startLoc; // starting location of the token
    int                        m_endingLoc;// ending location of the token
    bool                       m_bMove;    // are we doing a move

    // functions to draw map...
    void              initHouse();
    void              drawPlayers();
    void              placeLabels();

    // functions to manage moves...
    bool              validMove();

    // debugging functions ....
    void              highlight();
    void              drawGrid();


};
