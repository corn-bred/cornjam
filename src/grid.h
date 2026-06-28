#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/exponential.hpp>

class GridSpace {
    public:
    glm::vec2 GridRes;
    glm::vec3 Origin;
    bool GridData[15][15];

    GridSpace(glm::vec2 pixelResolution, glm::vec3 origin) {
        GridRes = pixelResolution;
        Origin = origin;
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                GridData[i][j] = false;
            }
        }
    }

    glm::mat4 GetTransformMatrix(unsigned int arrayX, unsigned int arrayY) {
        glm::mat4 Matrix(1.0f);

        Matrix = glm::translate(Matrix, Origin);
        Matrix = glm::translate(Matrix, glm::vec3(GridRes.x * arrayX, GridRes.y * arrayY, 0.0));
        Matrix = glm::scale(Matrix, glm::vec3(GridRes.x, GridRes.y, 1.0));
        
        return Matrix;
    }
};