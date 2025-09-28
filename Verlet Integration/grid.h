#pragma once
#include "pch.h"
#include "globals.h"
#include "functions.h"
#include "ThreadPool.h"

class Ball;

class Grid
{
	public:
		float cellSize;
		int numCols;
		int numRows;

		std::vector<std::vector<std::vector<Ball*>>> cells;
		std::atomic<int> collisionsDone;

		Grid(float cellSize) : cellSize(cellSize * 2)
		{
			numRows = static_cast<int>(std::ceil(gl::DisplaySize.y / this->cellSize));
			numCols = static_cast<int>(std::ceil(gl::DisplaySize.x / this->cellSize));

			collisionsDone.store(0);
			cells = std::vector<std::vector<std::vector<Ball*>>>(numRows, std::vector<std::vector<Ball*>>(numCols));
		}

		void addParticle(Ball* particle);
		void removeParticle(Ball* particle);
		std::vector<Ball*> getNeighbors(Ball* particle);
		void MultiThreadCollisions(int startRow, int endRow, int startCol, int endCol);
		void InitMultiThreadCollisions(int numThreads);
		void processCollisions(int startRow, int endRow, int startCol, int endCol);
		void collisionDetectionWithThreadPool(ThreadPool& pool, int numTasks);
};