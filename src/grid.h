#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/exponential.hpp>
#include <vector>

class GridSpace {
    public:
    glm::vec2 GridRes;
    glm::vec3 Origin;
    std::vector<int> Data;

    GridSpace(glm::vec2 pixelResolution, glm::vec3 origin) {
        GridRes = pixelResolution;
        Origin = origin;
    }

    void addSquare(int arrayX, int arrayY) {
        Data.push_back(arrayX);
        Data.push_back(arrayY);
    }

    glm::mat4 GetTransformMatrix(unsigned int index) {
        int arrayX = index;
        int arrayY = index + 1;
        glm::mat4 Matrix(1.0f);

        Matrix = glm::translate(Matrix, Origin);
        Matrix = glm::translate(Matrix, glm::vec3(GridRes.x * arrayX, GridRes.y * arrayY, 0.0));
        Matrix = glm::scale(Matrix, glm::vec3(GridRes.x, GridRes.y, 1.0));
        
        return Matrix;
    }
};