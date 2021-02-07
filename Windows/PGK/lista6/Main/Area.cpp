#include "Area.hpp"

Area::Area() :
    g_vertex_buffer_data(generateMesh(0, 0, 3,500)),
    pos(0, 0, 0),
    scale(1, 1, 1),
    loadBuff(false)
{
    setShaders();
    setBuffers();
    thread = std::thread(&Area::threadLoop, this);
    std::vector<unsigned int> index_buffer;
    index_buffer.reserve(6 * 149 * 149);
    for (int i = 0; i < 149; i++) {
        for (int j = 0; j < 149; j++) {
            index_buffer.emplace_back(i * 150 + j + 1);
            index_buffer.emplace_back((i + 1) * 150 + j);
            index_buffer.emplace_back(i * 150 + j);

            index_buffer.emplace_back((i + 1) * 150 + j);
            index_buffer.emplace_back(i * 150 + (j + 1));
            index_buffer.emplace_back((i + 1) * 150 + j + 1);

        }
    }

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(unsigned int), &index_buffer[0], GL_STATIC_DRAW);
}

Area::~Area() {
    static std::unique_lock<std::mutex> lc(endThread_mux, std::defer_lock);
    lc.lock();
    endThread = true;

    lc.unlock();
    thread.join();
}

void Area::threadLoop() {
    static std::unique_lock<std::mutex> lc_cam(curCamPos_mux, std::defer_lock);
    static std::unique_lock<std::mutex> lc_buff(loadBuff_mux, std::defer_lock);

    std::vector<GLfloat> vert;


    glm::vec3 camPos(0, 0, 3);
    glm::vec3 lastCamPos(0, 0, 3);
    int test;

    while (!endThread) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));

        lc_cam.lock();
        camPos = curCamPos;
        test = curvar;
        lc_cam.unlock();

        float radius = glm::length(camPos);
        float verRad = std::asin(camPos.y / radius);
        float horRad = std::asin(camPos.x / (radius * cosf(verRad)));

        vert = generateMesh(verRad, horRad, radius, test);
        
        lc_buff.lock();
        g_vertex_buffer_data = vert;
        loadBuff = true;
        lc_buff.unlock();
    }
}




void Area::refresh(glm::vec3 camPos, int _var) {

    /*
    float radius = glm::length(camPos);
   
    float verRad = std::asin(camPos.y / radius);
    float horRad = std::asin(camPos.x / (radius * cosf(verRad)));

    //if (camPos.z < 0) horRad = M_PI - horRad;

    g_vertex_buffer_data = generateMesh(verRad, horRad, radius);
    setBuffers();
    */
    static std::unique_lock<std::mutex> lc_cam(curCamPos_mux, std::defer_lock);
    static std::unique_lock<std::mutex> lc_buff(loadBuff_mux, std::defer_lock);

    lc_cam.lock();
    curCamPos = camPos;
    curvar = _var;
    lc_cam.unlock();
    lc_buff.lock();
    if (loadBuff) {
        setBuffers();
        loadBuff = false;
    }
    lc_buff.unlock();
}


std::vector<GLfloat> Area::generateMesh(float verRad, float horRad, float _radius,int _var) {
    static FileLoader fl;
    const int fRes = 1201;

    int horIter = 150;
    int verIter = 150;
    int stepInedx = _var * (_radius -1);
    
    int16_t height;

    long double radius;

    std::vector<GLfloat> vertices;

    vertices.reserve(verIter * horIter );

    float step = (M_PI / 180) / (fRes - 1);

    int startStack = floor(verRad / step) - (verIter / 2*stepInedx);
    int startSector = floor(horRad / step) - (horIter / 2*stepInedx);

    const long double heightScale = 1.0 / 6371000.0;
    for (int i = 0; i < verIter; i++) {
        int stack = startStack + i * stepInedx;
        GLfloat stackAngle = step * stack;
        for (int j = 0; j < horIter; j++) {
            int sector = startSector + j * stepInedx;
            GLfloat sectorAngle = step * sector;


            fl.getHeight(height,
                static_cast<size_t>(floor(static_cast<float>(stack) / fRes) + 90) % 180,
                static_cast<size_t>(floor(static_cast<float>(sector) / fRes + 180)) % 360,
                (fRes * (fRes - (stack % fRes))) % (fRes * fRes),
                sector % fRes);

            radius = 1.0 + heightScale * height;

            vertices.emplace_back(radius * cosf(stackAngle) * sinf(sectorAngle));
            vertices.emplace_back(radius * sinf(stackAngle));
            vertices.emplace_back(radius * cosf(stackAngle) * cosf(sectorAngle));

        }
    }
    return vertices;
}
void Area::setShaders() {
    compileShaders(R"END(

        #version 330 core
        #extension GL_ARB_explicit_uniform_location : require
        #extension GL_ARB_shading_language_420pack : require

        layout(location = 0) in vec3 vertexPosition;
        layout(location = 0) uniform vec4 pos;
        layout(location = 1) uniform vec4 scale;

        layout(location = 2)uniform mat4 MVP;


        out vec3 vcol;

        void main(){    
            gl_Position =  MVP * (vec4(vertexPosition,1) * scale + pos);
            
            float earth_r= 6371000.0;
            const float ht = length(vertexPosition) - 1;
            const float ght = 500 / earth_r;
            const float yht = 1000 / earth_r;
            const float rht = 2000 / earth_r;
            
            vcol = vec3(0);
            if     (ht <= 0   )    vcol = vec3( 0.0,           0.0,           1.0          );
            else if(ht <  ght)    vcol = vec3( 0.0,           ht/ght,      0.0          );
            else if(ht <  yht)    vcol = vec3( ht/ght - 1,  1,             0.0          );
            else if(ht <  rht)    vcol = vec3( 1,             2.0-ht/yht,  0.0          );
            else                    vcol = vec3( 1,             ht/rht - 1,  ht/rht - 1 );

        }
    )END", R"END(

        #version 330 core

        in vec3 vcol;

        out vec4 color;

        void main(){
            color = vec4(vcol,1);
        }
    )END");
}

void Area::setBuffers() {
    bindBuffers();

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * g_vertex_buffer_data.size(), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
}

void Area::draw(glm::mat4& MVP, glm::vec3& light) {
    bindProgram();
    bindBuffers();
    glUniform4f(0, pos[0], pos[1], pos[2], 0);
    glUniform4f(1, scale[0], scale[1], scale[2], 1);
    glUniformMatrix4fv(2, 1, GL_FALSE, &MVP[0][0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glDrawElements(GL_TRIANGLES, 149 * 149 * 6, GL_UNSIGNED_INT, (void*)0); // 12*3 indices starting at 0 -> 12 triangles


}