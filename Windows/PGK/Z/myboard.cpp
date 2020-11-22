#include "myboard.h"

MyBoard::MyBoard(GLuint dimensions, unsigned int xIndexPlayer, unsigned int yIndexPlayer) :engine(std::random_device()()), kDimensions(dimensions), xIndexPlayer(xIndexPlayer), yIndexPlayer(yIndexPlayer)
{

}

MyBoard::MyBoard(GLuint dimensions, unsigned int xIndexPlayer, unsigned int yIndexPlayer, int seed) :engine(std::random_device()()), kDimensions(dimensions), xIndexPlayer(xIndexPlayer), yIndexPlayer(yIndexPlayer)
{
    if(seed != 0)
    {
        std::cout << seed << "\n";
        engine.seed(static_cast<unsigned int>(seed));
    }
}

std::array<int, 2>MyBoard::rotateLine(unsigned int xIndex, unsigned int yIndex, GLfloat angle)
{
    auto &ref = lineTable.at(xIndex).at(yIndex);
    auto copy = ref.get()->getVertices();
    ref->setPosition(rotateVertices(ref->getVertices(), angle));
    auto temp = detectCollision(xIndex, yIndex);
    if(temp[0] != -1)
    {
        ref->setPosition(copy);
    }
    return temp;
}

std::array<int, 2> MyBoard::pushLine(unsigned int xIndex, unsigned int yIndex, GLfloat speed)
{
    auto &ref = lineTable.at(xIndex).at(yIndex);
    std::array<std::array<GLfloat, 2>, 2> array = lineTable.at(xIndex).at(yIndex)->getVertices();
    GLfloat xPush = array[1][0] - array[0][0];
    GLfloat yPush = array[1][1] - array[0][1];
    GLfloat vectorLength = sqrt(xPush*xPush + yPush*yPush);
    ref->setPosition(array[0][0] + (xPush/vectorLength) * speed, array[0][1] + (yPush/vectorLength) * speed,
            array[1][0] + (xPush/vectorLength) * speed, array[1][1] + (yPush/vectorLength) * speed);
    auto temp = detectCollision(xIndex, yIndex);
    if(temp[0] != -1)
    {
        ref->setPosition(array);
    }
        return temp;
}

GLfloat MyBoard::randomAngle()
{
    return dist(engine);
}
std::array<std::array<GLfloat, 2>, 2>  MyBoard::rotateVertices(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat angle)
{
    GLfloat xMidpoint = (x1 + x2)/2; //x coordinate of line segment
    GLfloat yMidpoint = (y1 + y2)/2; //y coordinate of line segment
    GLfloat xVector = x2 - xMidpoint;
    GLfloat yVector = y2 - yMidpoint;
    GLfloat xVectorPrim = xVector * cos(angle) - yVector * sin(angle);//x coordinate of rotated vector
    GLfloat yVectorPrim = xVector * sin(angle) + yVector * cos(angle);//y coordinate of rotated vector
    return {{ {xMidpoint - xVectorPrim, yMidpoint - yVectorPrim},
            {xMidpoint + xVectorPrim, yMidpoint + yVectorPrim}}};
}

std::array<std::array<GLfloat, 2>, 2>  MyBoard::rotateVertices(const std::array<std::array<GLfloat, 2>, 2>& array, GLfloat angle)
{
    GLfloat xMidpoint = (array[0][0] + array[1][0])/2; //x coordinate of line segment
    GLfloat yMidpoint = (array[0][1] + array[1][1])/2; //y coordinate of line segment
    GLfloat xVector = array[1][0] - xMidpoint;
    GLfloat yVector = array[1][1] - yMidpoint;
    GLfloat xVectorPrim = xVector * cos(angle) - yVector * sin(angle);//x coordinate of rotated vector
    GLfloat yVectorPrim = xVector * sin(angle) + yVector * cos(angle);//y coordinate of rotated vector
    return {{ {xMidpoint - xVectorPrim, yMidpoint - yVectorPrim},
            {xMidpoint + xVectorPrim, yMidpoint + yVectorPrim}}};
}

void MyBoard::setVertices(GLfloat kLineGap, GLfloat windowSize)
{
    lineTable.resize(kDimensions);
    const GLfloat kPositionStep = windowSize/kDimensions; //distancce between two midpoints
    GLfloat x = -1.0f;
    GLfloat y = 1.0f - kPositionStep/2;
    for(unsigned int i = 0; i < kDimensions; ++i)
    {
        for(unsigned int j = 0; j < kDimensions; ++j)
        {
            std::unique_ptr<MyLine> ptr(std::make_unique<MyLine>(rotateVertices( x + kLineGap ,y ,x + kPositionStep - kLineGap, y, randomAngle())));
            lineTable[i].push_back(std::move(ptr));
            x += kPositionStep;
        }
        y -= kPositionStep;
        x = -1.0f;
    }
}

void MyBoard::drawAll(GLfloat xWindowCenter, GLfloat yWindowCenter, GLfloat scale, glm::mat4 matrix)
{
    for(auto &r: lineTable)
    {
        for(auto &c: r)
        {
            c->draw(xWindowCenter, yWindowCenter, scale, matrix);
        }
    }
}

bool MyBoard::isCollision(const std::array<std::array<GLfloat, 2>, 2>& ref1, const std::array<std::array<GLfloat, 2>, 2>& ref2)
{
    GLfloat detABxAC = vectorProduct(ref1[0][0], ref1[0][1], ref1[1][0], ref1[1][1], ref2[0][0], ref2[0][1]);
    GLfloat detABxAD = vectorProduct(ref1[0][0], ref1[0][1], ref1[1][0], ref1[1][1], ref2[1][0], ref2[1][1]);
    GLfloat detCDxCA = vectorProduct(ref2[0][0], ref2[0][1], ref2[1][0], ref2[1][1], ref1[0][0], ref1[0][1]);
    GLfloat detCDxCB = vectorProduct(ref2[0][0], ref2[0][1], ref2[1][0], ref2[1][1], ref1[1][0], ref1[1][1]);
    if(detABxAC * detABxAD < 0 && detCDxCA * detCDxCB < 0)
    {
        return true;
    }
    return false;
}
std::array<int, 2> MyBoard::detectCollision(GLuint xIndex, GLuint yIndex)
{
    const auto& positions = lineTable.at(xIndex).at(yIndex)->getVertices();
    GLfloat windowSize = 2.0;
    GLuint tableSize = lineTable.size();
    const GLfloat kPositionStep = windowSize/kDimensions;
    GLfloat xStart = -1;
    GLfloat yStart = 1;
    GLfloat xVectorLen = std::abs(xStart - (positions[0][0] + positions[1][0])/2);
    GLfloat yVectorLen = std::abs(yStart - (positions[0][1] + positions[1][1])/2);
    GLint nearestIndexY = static_cast<int>(xVectorLen/kPositionStep);
    GLint nearestIndexX = static_cast<int>(yVectorLen/kPositionStep);
    std::array<std::array<GLfloat, 2>, 2> tempPositions;
    --nearestIndexX;
    --nearestIndexY;
    GLint firstY= nearestIndexY;
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if(nearestIndexX >= 0 && nearestIndexX < tableSize && nearestIndexY >= 0 && nearestIndexY < tableSize)
            {
                tempPositions = (*lineTable[static_cast<unsigned int>(nearestIndexX)][static_cast<unsigned int>(nearestIndexY)]).getVertices();
                if(isCollision(positions, tempPositions))
                {
                    return {nearestIndexX, nearestIndexY};
                }
            }
            ++nearestIndexY;
        }
        ++nearestIndexX;
        nearestIndexY = firstY;
    }
    return {-1,-1};
}

void MyBoard::setPlayerColor(unsigned int xIndexPlayer, unsigned int yIndexPlayer, GLfloat r1, GLfloat g1, GLfloat b1, GLfloat r2, GLfloat g2, GLfloat b2)
{
    lineTable.at(xIndexPlayer).at(yIndexPlayer)->setFaceColor(r1, g1, b1);
    lineTable.at(xIndexPlayer).at(yIndexPlayer)->setBackColor(r2, g2, b2);
}

GLfloat MyBoard::vectorProduct(GLfloat xA, GLfloat yA, GLfloat xB, GLfloat yB, GLfloat xC, GLfloat yC)
{
    GLfloat x1 = xB - xA;
    GLfloat y1 = yB - yA;
    GLfloat x2 = xC - xA;
    GLfloat y2 = yC - yA;
    return x1*y2 - x2*y1;
}
