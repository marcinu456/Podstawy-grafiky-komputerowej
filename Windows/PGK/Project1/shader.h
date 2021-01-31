#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <epoxy/gl.h>


class Shader
{
public:
    unsigned int ID_;

    Shader(const char *vertex_path, const char * fragment_path, const char *geometry_path = nullptr)
    {
        std::string vertex_code;
        std::string fragment_code;
        std::string geometry_code;

        std::ifstream v_shader_file;
        std::ifstream f_shader_file;
        std::ifstream g_shader_file;

        v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        if(geometry_path)
        {
            g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        }

        try
        {
            v_shader_file.open(vertex_path);
            f_shader_file.open(fragment_path);
            if(geometry_path)
            {
                g_shader_file.open(geometry_path);
            }

            std::stringstream v_shader_stream;
            std::stringstream f_shader_stream;
            std::stringstream g_shader_stream;

            v_shader_stream << v_shader_file.rdbuf();
            f_shader_stream << f_shader_file.rdbuf();
            if(geometry_path)
            {
                g_shader_stream << g_shader_file.rdbuf();
            }
            v_shader_file.close();
            f_shader_file.close();
            if(geometry_path)
            {
                g_shader_file.close();
            }

            vertex_code = v_shader_stream.str();
            fragment_code = f_shader_stream.str();

            if(geometry_path)
            {
                geometry_code = g_shader_stream.str();

            }
        }
        catch(std::ifstream::failure &e)
        {
            std::cerr << "ERROR::SHADER_FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
        }

        const char *v_shader_code = vertex_code.c_str();
        const char *f_shader_code = fragment_code.c_str();
        const char *g_shader_code = geometry_code.c_str();

        unsigned int vertex = 0;
        unsigned int fragment = 0;
        unsigned int geometry = 0;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, nullptr);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, nullptr);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT");

        if(geometry_path)
        {
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &g_shader_code, nullptr);
            glCompileShader(geometry);
            CheckCompileErrors(geometry, "GEOMETRY");
        }

        ID_ = glCreateProgram();
        glAttachShader(ID_, vertex);
        glAttachShader(ID_, fragment);
        if(geometry_path)
        {
            glAttachShader(ID_, geometry);
        }
        glLinkProgram(ID_);
        CheckCompileErrors(ID_, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(geometry_path)
        {
            glDeleteShader(geometry);
        }

    }
    void use()
    {
        glUseProgram(ID_);
    }
private:
    void CheckCompileErrors(unsigned int shader, std::string type)
    {
        int succes = 0;
        char info_log[1024];

        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
            if(!succes)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, info_log);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &succes);
            if(!succes)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, info_log);
                std::cerr << "ERROR::SHADER_LINKING_ERROR of type: " << type << "\n" << info_log << std::endl;
            }
        }
    }

};

#endif // SHADER_H
