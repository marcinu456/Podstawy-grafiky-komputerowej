#include "myshape.h"

myShape::myShape(unsigned int dimensions, int seed) : kDimensions(dimensions), engine(std::random_device()())
{
    if(seed != 0)
    {
        engine.seed(static_cast<unsigned long>(seed));
    }
    setShaders();
    setMidpoints(-15, -15, 15, 15);
    setRotations();
    setScales();
    setBuffers();
    setTriangleCache();
}

void myShape::setMidpoints(float xStart, float yStart, float zStart, float dimensionSize)
{
    const float kPositionStep = (dimensionSize - 0.2f) / kDimensions;
    for(unsigned int i = 0; i < kDimensions; ++i) //xy
    {
        for(unsigned int j = 0; j < kDimensions; ++j)//y
        {
            for(unsigned int k = 0; k < kDimensions; ++k)
            {
                    trianglesMidpoints.push_back(glm::vec3{xStart + kPositionStep * i, yStart + kPositionStep *j, zStart + kPositionStep * k});
            }
        }
    }
}

void myShape::draw(float tx, float ty, float scale, glm::mat4 view, const glm::mat4 &projection)
{
    bindProgram();
    glBindVertexArray(VAO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(0, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(1, 1, GL_FALSE, &projection[0][0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1000);
}


bool myShape::isCollision(glm::vec3 spherePosition, float sphereRay)
{
    for(auto &triangle : triangleCache)
    {
        glm::vec3 closestPoint = triangle.nearestPoint(spherePosition);
        float length = glm::length(closestPoint - spherePosition);
        if(length < sphereRay * 2)
        {
            std::cout << glm::to_string(closestPoint) << " sphere " <<glm::to_string(spherePosition) <<  "\n";
        }
    }
    return false;
}


void myShape::setShaders()
{
    compileShadersFromFile("/home/uriziel/pgk04/myShape.vs", "/home/uriziel/pgk04/myShape.fs");
}

void myShape::setBuffers()
{
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &midpointsVBO);
    glGenBuffers(1, &scalesVBO);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);    //set triangle vertices relative position
    glBufferData(GL_ARRAY_BUFFER, triangleVertices.size() * sizeof(float), triangleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);//enabling vertex atrributes for model layout
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, midpointsVBO);//set triangle vertices middle position
    std::vector<glm::mat4> matrixes;

    for(unsigned int i = 0; i < trianglesMidpoints.size(); ++i)//creating model matrix
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(trianglesMidpoints[i]));
        model = glm::rotate(model, glm::radians(trianglesAngles[i]), glm::vec3(1,1,1));
        matrixes.push_back(model);
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*trianglesMidpoints.size(), matrixes.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)16);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)32);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)48);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, scalesVBO);
    glBufferData(GL_ARRAY_BUFFER, trianglesScales.size() * sizeof(float), trianglesScales.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glVertexAttribDivisor(5, 1);

}

void myShape::setRotations()
{
    for(unsigned int i = 0; i < kDimensions * kDimensions * kDimensions; ++i)
    {
        trianglesAngles.push_back(angleDist(engine));
    }
}

void myShape::setScales()
{
    for(unsigned int i = 0; i < trianglesMidpoints.size(); ++i)
    {
        trianglesScales.push_back(scaleDist(engine));
    }
}

void myShape::setTriangleCache()
{
    for(unsigned int i = 0; i < trianglesMidpoints.size(); ++i)//creating model matrix
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(trianglesMidpoints[i]));
        model = glm::rotate(model, glm::radians(trianglesAngles[i]), glm::vec3(1,1,1));

        glm::vec3 pointA = model * glm::vec4(glm::vec3(triangleVertices[0], triangleVertices[1], triangleVertices[2]) * trianglesScales[i], 1);
        glm::vec3 pointB = model * glm::vec4(glm::vec3(triangleVertices[3], triangleVertices[4], triangleVertices[5]) * trianglesScales[i], 1);
        glm::vec3 pointC = model * glm::vec4(glm::vec3(triangleVertices[6], triangleVertices[7], triangleVertices[8]) * trianglesScales[i], 1);
        triangleCache.push_back(Triangle(pointA, pointB, pointC));
    }
}





