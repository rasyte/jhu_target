#include "gameBoard.h"
#include "../common/common.h"
#include "logger.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QMessageBox>

#include <iostream>
#include <iterator>
#include <algorithm>

//const QPoint  startLoc[6] = { {460, 30}, {640, 190}, {460, 585}, {210, 585}, {30, 430},{30, 190} };
const QColor  tokenClr[6] = { Qt::red, Qt::yellow, Qt::white, Qt::green, QColor(128, 128, 255), QColor(128, 0, 128) };
const char*   lpszName[9] = { "Study", "Hall", "Lounge", "Library", "Billiard Room", "Dining Room", "Conservatory", "Ballroom", "Kitchen" };

gameBoard::gameBoard(QWidget* parent) : QGraphicsView(parent), m_pToken(nullptr), m_startLoc(-1),m_endingLoc(-1),m_bMove(false)
{
    QPixmap board = QPixmap("Resources/board.png");
    initHouse();
    
    m_pScene = new QGraphicsScene();              // Create the scene
        
    // draw house
    for (int ndx = 0; ndx < 21; ndx++)
    {
        m_pScene->addRect(house[ndx].bb, QPen(Qt::black));
    }

    placeLabels();          // add room labels
    drawPlayers();          // add players

    this->setScene(m_pScene);
}


void gameBoard::drawPlayers()
{
    float    r = 15;             // radius of player icon

    //QVector<QGraphicsEllipseItem*>   vecTokens;
 
    for (int ndx = 0; ndx < 6; ndx++)
    {
        QGraphicsEllipseItem* pTemp = m_pScene->addEllipse(QRect(startLoc[ndx].x()-r, startLoc[ndx].y()-r, 2 * r, 2 * r), QPen(Qt::black), QBrush(tokenClr[ndx], Qt::SolidPattern));
        pTemp->setFlag(QGraphicsItem::ItemIsMovable);
        pTemp->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

        m_pTokens.insert(ndx, pTemp);
        m_pRevTokens.insert(pTemp, ndx);
    }

    m_pScene->update();
}

void gameBoard::placeLabels()
{
    QFont font;
    font.setFamily(QString::fromUtf8("AR BERKLEY"));
    font.setPointSize(16);

    for (int ndx = 0; ndx < m_cntLocs; ndx++)
    {
        if (house[ndx].name != nullptr)
        {
            QGraphicsTextItem* pTemp = m_pScene->addText(QString(house[ndx].name), font);
            pTemp->setPos(QPoint(house[ndx].bb.x(), house[ndx].bb.y()));
        }
    }
}


void gameBoard::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton)
    {
        m_pToken = itemAt(pEvent->pos());

        if (m_pToken && (m_pRevTokens.contains(m_pToken)))
        {
            int ndx;

            for (ndx = 0; ndx < m_cntLocs; ndx++)
            {
                if (house[ndx].bb.contains(pEvent->pos(), true))
                {
                    m_pToken->setData(0, mapToScene(pEvent->pos()));
                    m_startLoc = ndx; /* house[ndx].id; */
                    break;
                }
            }
        }
        else
        {
            QMessageBox::warning(this, "error", "Please select a token to move");
        }
    }
    else if (pEvent->button() == Qt::RightButton)
    {
        bool       bInHouse = false;
        if (m_pToken)                                     // we have a token to move.....
        {
            for (int ndx = 0; ndx < m_cntLocs; ndx++)
            {
                if (house[ndx].bb.contains(pEvent->pos(), true))
                {
                    bInHouse = true;
                    m_endingLoc = ndx; /* house[ndx].id; */
                    if (validMove())                                      // is move a legal move
                    {
                        QPointF  p = m_pToken->data(0).toPointF();
                        QPointF sp = mapToScene(pEvent->pos());

                        m_pToken->setPos(m_pToken->pos() + sp - p);       // finally perform the move
                        m_pToken->setData(0, sp);

                        m_pScene->update();

                        house[m_startLoc].curToons--;               // decrement the previous location occupancey
                        house[m_endingLoc].curToons++;              // increment the new location occupancy

                        emit currentLoc(m_endingLoc);

                        if (house[m_endingLoc].type == 0)     // ended in a room
                            emit sendSuggestion(house[m_endingLoc].id);
                        m_pToken = nullptr;
                        
                    }
                    break;
                }
            }

            if (!bInHouse)
            {
                QMessageBox::warning(nullptr, "error", "Please select either a room or a hallway");
            }
        }
    }
    else
    {
        std::cout << "only supporting left & right mouse clicks." << std::endl;
    }
}

bool gameBoard::validMove()
{
    bool bConnected = false;
    bool bRet = false;

    for (int ndx = 0; ndx < 4; ndx++)                                    // check to see if locations are connected.
    {
        if (house[m_startLoc].links[ndx] == m_endingLoc)                 // locations are connected
        {
            bConnected = true;
            break;
        }
    }

    if (bConnected)
    {
        if (house[m_endingLoc].maxToons >= house[m_endingLoc].curToons + 1)
        {
            bRet = true;
        }
        else
        {
            QMessageBox::warning(nullptr, "error", "location blocked by another toon");
            bRet = false;
        }
    }
    else
    {
        QMessageBox::warning(nullptr, "error", "Locations are not connected");
        bRet = false;
    }

    return bRet;
}


void gameBoard::mouseReleaseEvent(QMouseEvent*)
{

}


// initialized the house structure
void gameBoard::initHouse()
{
    int    roomWidth = 150;         // room dimensions ..,
    int    roomHeight = 130;
    int    hallWidth = 100;         // hall dimensions....
    int    hallHeight = 52;
    int    startHeight = 30;        // starting loc dimensions....
    int    startWidth = 30;

    m_cntLocs = sizeof(house) / sizeof(house[0]);

    // format is: id, type, name, bbox, max toons, cur toons, neighbors
    house[ 0] = { 0, 0, lpszName[0],QRect{10 , 10, roomWidth, roomHeight}, 6, 0, {1,5,20,-1}  };     // correct
    house[ 1] = { 9, 1, nullptr,    QRect{160, 50, hallWidth, hallHeight}, 1, 0, {0,2,-1,-1}  };
    house[ 2] = { 1, 0, lpszName[1],QRect{260, 10, roomWidth, roomHeight}, 6, 0, {1,3, 6,-1}  };     // correct
    house[ 3] = {10, 1, nullptr,    QRect{410, 50, hallWidth, hallHeight}, 1, 0, {2,4,-1,-1}  };
    house[ 4] = { 2, 0, lpszName[2],QRect{510, 10, roomWidth, roomHeight}, 6, 0, {3,7,16,-1}  };     // correct
    house[ 5] = {11, 1, nullptr,    QRect{60 ,140, hallHeight, hallWidth}, 1, 0, {0,8,-1,-1}  };     // dimensions reversed due to verticle orientation
    house[ 6] = {12, 1, nullptr,    QRect{310,140, hallHeight, hallWidth}, 1, 0, {2,10,-1,-1} };
    house[ 7] = {13, 1, nullptr,    QRect{560,140, hallHeight, hallWidth}, 1, 0, {4,12,-1,-1} };
    house[ 8] = { 3, 0, lpszName[3],QRect{10 ,240, roomWidth, roomHeight}, 6, 0, {5,9,13,-1}  };     // correct
    house[ 9] = {14, 1, nullptr,    QRect{160,280, hallWidth, hallHeight}, 1, 0, {8,10,-1,-1} };
    house[10] = { 4, 0, lpszName[4],QRect{260,240, roomWidth, roomHeight}, 6, 0, {6,9,11,14}  };     // correct
    house[11] = {15, 1, nullptr,    QRect{410,280, hallWidth, hallHeight}, 1, 0, {10,12,-1,-1}};
    house[12] = { 5, 0, lpszName[5],QRect{510,240, roomWidth, roomHeight}, 6, 0, {7,11,15,-1} };     // correct
    house[13] = {16, 1, nullptr,    QRect{60 ,370, hallHeight, hallWidth}, 1, 0, {8,16,-1,-1} };
    house[14] = {17, 1, nullptr,    QRect{310,370, hallHeight, hallWidth}, 1, 0, {10,18,-1,-1}};
    house[15] = {18, 1, nullptr,    QRect{560,370, hallHeight, hallWidth}, 1, 0, {12,20,-1,-1}};
    house[16] = { 6, 0, lpszName[6],QRect{10 ,470, roomWidth, roomHeight}, 1, 0, {13,17,4,-1} };     // correct
    house[17] = {19, 1, nullptr,    QRect{160,510, hallWidth, hallHeight}, 1, 0, {16,18,-1,-1}};
    house[18] = { 7, 0, lpszName[7],QRect{260,470, roomWidth, roomHeight}, 1, 0, {14,17,19,-1}};     // correct
    house[19] = {20, 1, nullptr,    QRect{410,510, hallWidth, hallHeight}, 1, 0, {18,20,-1,-1}};
    house[20] = { 8, 0, lpszName[8],QRect{510,470, roomWidth, roomHeight}, 1, 0, {15,19,0,-1} };     // correct
    // add in player starting locations, they only communicate with a hall and can not be moved into.
    house[21] = {21, 2, nullptr,    QRect{445, 15, startWidth,startHeight}, 0, 1, {3,-1,-1,-1}};     // correct
    house[22] = {22, 2, nullptr,    QRect{625,175, startWidth,startHeight}, 0, 1, {7,-1,-1,-1}};     // correct
    house[23] = {23, 2, nullptr,    QRect{445,570, startWidth,startHeight}, 0, 1, {19,-1,-1,-1}};     // correct
    house[24] = {24, 2, nullptr,    QRect{195,570, startWidth,startHeight}, 0, 1, {17,-1,-1,-1}};     // correct
    house[25] = {25, 2, nullptr,    QRect{15 ,415, startWidth,startHeight}, 0, 1, {13,-1,-1,-1}};     // correct
    house[26] = {26, 2, nullptr,    QRect{15 ,175, startWidth,startHeight}, 0, 1, {5,-1,-1,-1}};     // correct
}

// This is a debugging function, designed to highlight a given room or hall...
void gameBoard::highlight()
{
    // debugging -- draw a center for each room
    for (int ndx = 0; ndx < m_cntLocs; ndx++)
    {
        QPoint ptCenter = house[ndx].bb.center();
        QRect  rectBox = QRect(ptCenter.x() - 3, ptCenter.y() - 3, 6, 6);
        m_pScene->addRect(rectBox, QPen(Qt::red), QBrush(Qt::red, Qt::SolidPattern));
    }
    // debugging -- draw a rect around rooms 
    m_pScene->addRect(house[0].bb, QPen(Qt::red), QBrush(Qt::red, Qt::VerPattern));
    m_pScene->addRect(house[1].bb, QPen(Qt::blue), QBrush(Qt::blue, Qt::VerPattern));
    m_pScene->addRect(house[2].bb, QPen(Qt::red), QBrush(Qt::red, Qt::VerPattern));
    m_pScene->addRect(house[3].bb, QPen(Qt::blue), QBrush(Qt::blue, Qt::VerPattern));
    m_pScene->addRect(house[4].bb, QPen(Qt::red), QBrush(Qt::red, Qt::VerPattern));
    m_pScene->addRect(house[5].bb, QPen(Qt::green), QBrush(Qt::green, Qt::VerPattern));
    m_pScene->addRect(house[6].bb, QPen(Qt::green), QBrush(Qt::green, Qt::VerPattern));
    m_pScene->addRect(house[7].bb, QPen(Qt::green), QBrush(Qt::green, Qt::VerPattern));
    m_pScene->addRect(house[8].bb, QPen(Qt::red), QBrush(Qt::red, Qt::VerPattern));
    m_pScene->addRect(house[9].bb, QPen(Qt::blue), QBrush(Qt::blue, Qt::VerPattern));
    m_pScene->addRect(house[10].bb, QPen(Qt::red), QBrush(Qt::red, Qt::VerPattern));
    m_pScene->addRect(house[11].bb, QPen(Qt::blue), QBrush(Qt::blue, Qt::VerPattern));
    m_pScene->addRect(house[12].bb, QPen(Qt::red), QBrush(Qt::red, Qt::VerPattern));
    m_pScene->addRect(house[13].bb, QPen(Qt::green), QBrush(Qt::green, Qt::VerPattern));
    m_pScene->addRect(house[14].bb, QPen(Qt::green), QBrush(Qt::green, Qt::VerPattern));
    m_pScene->addRect(house[15].bb, QPen(Qt::green), QBrush(Qt::green, Qt::VerPattern));
    m_pScene->addRect(house[16].bb, QPen(Qt::red), QBrush(Qt::red, Qt::VerPattern));
    m_pScene->addRect(house[17].bb, QPen(Qt::blue), QBrush(Qt::blue, Qt::VerPattern));
    m_pScene->addRect(house[18].bb, QPen(Qt::red), QBrush(Qt::red, Qt::VerPattern));
    m_pScene->addRect(house[19].bb, QPen(Qt::blue), QBrush(Qt::blue, Qt::VerPattern));
    m_pScene->addRect(house[20].bb, QPen(Qt::red), QBrush(Qt::red, Qt::VerPattern)); 
}

// debugging function -- draw a grid to help locate item
void gameBoard::drawGrid()
{
    QRect size = this->rect();
    int width = size.width();

    for (int x = 0; x < width; x += 20)
    {
        m_pScene->addLine(x, 0, x, size.height(), QPen(Qt::gray));
    }

    for (int y = 0; y < size.height(); y += 20)
    {
        m_pScene->addLine(0, y, size.width(), y, QPen(Qt::gray));
    }

    m_pScene->update();
}