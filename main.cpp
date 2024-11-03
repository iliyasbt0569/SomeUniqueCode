#include <iostream>
#include <vector>
#include <queue>

typedef std::vector<std::vector<int>> Matrix;
typedef std::pair<int, int> Pos;

std::string getStrMatrix(const Matrix &matrix){
    std::string result;
    for(const auto &v : matrix){
        result += "{ ";
        for(int i = 0; i < v.size(); ++i){
            result += std::to_string( v[i] ) + ((i != v.size() - 1) ? ", " : " ");
        }
        result += "}\n";
    }
    return result;
}

std::string  getStrPos(const Pos &pos){
    return "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ")";
}

enum Direction{
    HORIZONTAL,
    VERTICAL,
};

std::string getStrDirection(const Direction &dir){
    switch(dir){
        case Direction::HORIZONTAL : return "HORIZONTAL";
        case Direction::VERTICAL   : return "VERTICAL";
    }
    return "";
}

int countMatrixDirectionOnes(const Pos &startPos, const Direction &dir, const Matrix &matrix) {
    int ones = 0;
    switch(dir){
        case Direction::HORIZONTAL :{
            for(int i = startPos.second; i < matrix[startPos.first].size(); ++i){
                if(matrix[startPos.first][i] == 1)
                    ++ones;
            }
            return ones;
        }
        case Direction::VERTICAL: {
            for(int i = startPos.first; i < matrix.size(); ++i){
                if(matrix[i][startPos.second] == 1)
                    ++ones;
            }
            return ones;
        }
    };
    return ones;
}

auto getMaxOnesDirection(const Matrix &matrix){
    std::pair<Pos, Direction> result;
    int maxOneDirection = 0;
    for(int i = 0; i < matrix.size(); ++i){
        int temp = countMatrixDirectionOnes(Pos(i, 0), Direction::HORIZONTAL, matrix);
        if(temp > maxOneDirection){
            maxOneDirection = temp;
            result = {Pos(i, 0), Direction::HORIZONTAL};
        }
    }
    for(int i = 0; i < matrix[0].size(); ++i){
        int temp = countMatrixDirectionOnes(Pos(0, i), Direction::VERTICAL, matrix);
        if(temp > maxOneDirection){
            maxOneDirection = temp;
            result = {Pos(0, i), Direction::VERTICAL};
        }    
    }
    return result;
}

std::vector<Pos> getZerosInDirectionPos(const Pos &startPos, const Direction &dir, const Matrix &matrix){
    std::vector<Pos> zerosPos;
    switch(dir){
        case Direction::HORIZONTAL :{
            for(int i = startPos.second; i < matrix[startPos.first].size(); ++i){
                if(matrix[startPos.first][i] == 0)
                    zerosPos.push_back({startPos.first, i});
            }
            return zerosPos;
        }
        case Direction::VERTICAL: {
            for(int i = startPos.first; i < matrix.size(); ++i){
                if(matrix[i][startPos.second] == 0)
                    zerosPos.push_back({i, startPos.second});
            }
            return zerosPos;
        }
    };
    return zerosPos;
}

bool posInDirection(const Pos &start, const Direction &dir, const Pos &end){
    switch(dir){
        case Direction::HORIZONTAL :{
            return start.first == end.first;
        }
        case Direction::VERTICAL: {
            return start.second == end.second;
        }
    };
    return false;
}

Pos nearestTargetPosition(Matrix &matrix, const Pos &startPos, Pos expectPos, Direction expectDir) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    auto [startX, startY] = startPos;
    if (startX < 0 || startX >= rows || startY < 0 || startY >= cols) 
        return {-1, -1}; 
    
    if (matrix[startX][startY] == 1 && !posInDirection({startX, startY}, expectDir, expectPos)) 
        return {startX, startY};
    

    std::vector<Pos> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::queue<Pos> q;
    q.push({startX, startY});
    
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    visited[startX][startY] = true;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (auto [dx, dy] : directions) {
            int newX = x + dx;
            int newY = y + dy;

            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && !visited[newX][newY]) {
                if (matrix[newX][newY] == 1 && !posInDirection({newX, newY}, expectDir, expectPos)) {
                    return {newX, newY};
                }
                q.push({newX, newY});
                visited[newX][newY] = true;
            }
        }
    }
    return {-1, -1};
}

void swapMatrixEls(const Pos &start, const Pos &end, Matrix &matrix){
    int tempEl = matrix[start.first][start.second];
    matrix[start.first][start.second] = matrix[end.first][end.second];
    matrix[end.first][end.second] = tempEl;
}

int main(){
    std::vector<Matrix> matrixes = {
        {
            {1, 0, 1},
            {0, 1, 0},
            {0, 0, 0},
        },
        {
            {1, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {1, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
        },
        {
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 1},
            {0, 0, 0, 1},
        }

    }; 
    for(auto matrix : matrixes){
        std::cout << getStrMatrix(matrix);

        auto maxOnesDirPos = getMaxOnesDirection(matrix);
        std::cout << "max ones: " 
            << getStrPos(maxOnesDirPos.first) << " -> " 
            << getStrDirection(maxOnesDirPos.second) << std::endl;

        int countMoves = 0;
        auto zerosPos = getZerosInDirectionPos(maxOnesDirPos.first, maxOnesDirPos.second, matrix);
        for(const auto &e : zerosPos){
            Pos nearestElement = nearestTargetPosition(matrix, e, maxOnesDirPos.first, maxOnesDirPos.second);
            std::cout << getStrPos(e) << ": Target = 1. Nearest  -> " << getStrPos(nearestElement) << std::endl;
            swapMatrixEls(e, nearestElement, matrix);
            countMoves += std::abs(e.first - nearestElement.first) + std::abs(e.second - nearestElement.second);
            std::cout << std::endl << getStrMatrix(matrix) << std::endl;
        }  
        std::cout << "Count moves : " << countMoves << "\n\n\n\n";
    }
}