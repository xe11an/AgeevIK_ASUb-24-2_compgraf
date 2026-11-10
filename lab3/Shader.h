#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "GL/glew.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Мини-библиотека для работы с шейдерными программами (задание №2):
//  - загрузка вершинного и фрагментного шейдеров из файлов;
//  - компиляция, линковка и проверка ошибок;
//  - отправка uniform-переменных одной строкой.
// Перед setXXX() нужно вызвать use().
class Shader {
public:
    GLuint ID = 0;   // 0 - программа не создана (была ошибка)

    Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vCode = readFile(vertexPath);
        std::string fCode = readFile(fragmentPath);
        if (vCode.empty() || fCode.empty()) return;

        GLuint vs = compile(GL_VERTEX_SHADER, vCode, "VERTEX");
        GLuint fs = compile(GL_FRAGMENT_SHADER, fCode, "FRAGMENT");
        if (!vs || !fs) {
            if (vs) glDeleteShader(vs);
            if (fs) glDeleteShader(fs);
            return;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(program, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n";
            glDeleteProgram(program);
        } else {
            ID = program;
        }
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    bool valid() const { return ID != 0; }
    void use() const { glUseProgram(ID); }
    void destroy() { if (ID) { glDeleteProgram(ID); ID = 0; } }

    // ---- uniform: одно значение ----
    void setBool (const std::string& n, bool v)     const { glUniform1i (loc(n), (int)v); }
    void setInt  (const std::string& n, int v)      const { glUniform1i (loc(n), v); }
    void setUInt (const std::string& n, unsigned v) const { glUniform1ui(loc(n), v); }
    void setFloat(const std::string& n, float v)    const { glUniform1f (loc(n), v); }

    // ---- uniform: векторы (по компонентам) ----
    void setVec2(const std::string& n, float x, float y)                   const { glUniform2f(loc(n), x, y); }
    void setVec3(const std::string& n, float x, float y, float z)          const { glUniform3f(loc(n), x, y, z); }
    void setVec4(const std::string& n, float x, float y, float z, float w) const { glUniform4f(loc(n), x, y, z, w); }

    // ---- uniform: векторы (из массива float) ----
    void setVec2(const std::string& n, const float* v) const { glUniform2fv(loc(n), 1, v); }
    void setVec3(const std::string& n, const float* v) const { glUniform3fv(loc(n), 1, v); }
    void setVec4(const std::string& n, const float* v) const { glUniform4fv(loc(n), 1, v); }

    // ---- uniform: матрицы (массив из 9 или 16 float, по столбцам) ----
    void setMat3(const std::string& n, const float* m) const { glUniformMatrix3fv(loc(n), 1, GL_FALSE, m); }
    void setMat4(const std::string& n, const float* m) const { glUniformMatrix4fv(loc(n), 1, GL_FALSE, m); }

private:
    GLint loc(const std::string& name) const {
        return glGetUniformLocation(ID, name.c_str());
    }

    static std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cout << "ERROR::SHADER::FILE_NOT_FOUND: " << path << "\n";
            return "";
        }
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    static GLuint compile(GLenum type, const std::string& code, const char* typeName) {
        GLuint shader = glCreateShader(type);
        const char* src = code.c_str();
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << typeName
                      << "\n" << infoLog << "\n";
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }
};
