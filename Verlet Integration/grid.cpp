#include "grid.h"


void Grid::addParticle(Ball* particle)
{
    if (std::isnan(particle->pos.x) || std::isnan(particle->pos.y))
        return;

	int rowID = static_cast<int>(std::floor(particle->pos.y / cellSize));
	int colID = static_cast<int>(std::floor(particle->pos.x / cellSize));

	particle->cell = { colID, rowID };
	cells[rowID][colID].push_back(particle);
}

void Grid::removeParticle(Ball* particle)
{
    if (std::isnan(particle->pos.x) || std::isnan(particle->pos.y))
        return;

	int index = findIndex<Ball*>(cells[particle->cell.y][particle->cell.x], particle);

	cells[particle->cell.y][particle->cell.x].erase(cells[particle->cell.y][particle->cell.x].begin() + index);	
}

std::vector<Ball*> Grid::getNeighbors(Ball* particle)
{
    std::vector<Ball*> neighbors{};

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            int x = particle->cell.x + j;
            int y = particle->cell.y + i;

            if (x >= 0 && x < numCols && y >= 0 && y < numRows)
            {
                for (Ball* p : cells[y][x])
                {
                    if (!contiene<Ball*>(neighbors, p))
                        neighbors.push_back(p);
                }
            }
        }
    }

	return neighbors;
}

void Grid::processCollisions(int startRow, int endRow, int startCol, int endCol) {
    for (int i = startRow; i < endRow && i < numRows; i++) {
        for (int j = startCol; j < endCol && j < numCols; j++) {
            for (Ball* ball : cells[i][j]) {
                auto neighbors = getNeighbors(ball);
                CheckCollisions(neighbors);
            }
        }
    }
}

void Grid::collisionDetectionWithThreadPool(ThreadPool& pool, int numTasks) {
    int partsRow = static_cast<int>(std::sqrt(numTasks));
    if (partsRow <= 0) partsRow = 1;
    int partsCol = (numTasks + partsRow - 1) / partsRow; 
    int totalTasks = partsRow * partsCol;

    int rowStep = numRows / partsRow;
    int colStep = numCols / partsCol;

    for (int i = 0; i < partsRow; i++) {
        for (int j = 0; j < partsCol; j++) {
            int startRow = i * rowStep;
            int endRow = (i == partsRow - 1) ? numRows : startRow + rowStep;
            int startCol = j * colStep;
            int endCol = (j == partsCol - 1) ? numCols : startCol + colStep;
            pool.enqueueTask([this, startRow, endRow, startCol, endCol]() {
                this->processCollisions(startRow, endRow, startCol, endCol);
                });
        }
    }
    pool.waitAll();
}