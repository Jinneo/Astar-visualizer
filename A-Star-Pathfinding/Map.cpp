#pragma once
#include "stdafx.hpp"
#include "Map.hpp"
#include <cmath>
 
Map::Map(sf::RenderWindow& window, int tilesInRow, int tilesInCol) {
    mapSize = window.getSize();
    tilesPerRow = tilesInRow;
    tilesPerCol = tilesInCol;
    tileCount = tilesPerCol * tilesPerRow;
    tileW = mapSize.x / tilesPerRow;
    tileH = mapSize.y / tilesPerCol;
    tileShape.setSize({ tileW, tileH });
    tileShape.setOutlineThickness(1.f);
    tileShape.setOutlineColor(sf::Color::Yellow);
    previewUnitDrop.setRadius(tileW / 2.f);
    //Generate vector for tiles
    tiles = std::vector<tiletype>(tileCount);
    //Create border of walls
    for (int i = 0; i < tileCount; ++i) {
        if (i < tilesPerRow || i > tileCount - tilesPerRow || i % tilesPerRow == 0 || i % tilesPerRow == tilesPerRow - 1) {
            tiles[i] = wall;
        }
        else {
            tiles[i] = ground;
        }
    }
    //Fill walls anywhere you want here
    tiles[37] = wall;
    tiles[62] = wall;
    tiles[87] = wall;
    tiles[112] = wall;
    tiles[137] = wall;
    tiles[162] = wall;
    tiles[187] = wall;
    tiles[212] = wall;
    tiles[237] = wall;
    tiles[238] = wall;
    tiles[239] = wall;
    tiles[240] = wall;
}
 
void Map::drawMap(sf::RenderWindow& window) {
    //Move a single rectangle across the board to draw each tile
    sf::Vector2f tilePos;
    for (int i = 0; i < tileCount; i++) {
        tilePos = getTilePos(i);
        tileShape.setPosition(tilePos);
        if (tiles[i] == wall) {
            tileShape.setFillColor(sf::Color::White);
        }
        else {
            tileShape.setFillColor(sf::Color::Black);
        }
        window.draw(tileShape);
    }
}
 
int Map::getTileN(float x, float y) {
    //Find the index number starting at 0,0 going right then down
    int tileX, tileY;
    tileX = int(x / tileW);
    tileY = int(y / tileH);
    return tileX + (tileY * tilesPerCol);
}
 
sf::Vector2f Map::getTilePos(int N) {
    //Return position of tile's origin in pixels
    float posX, posY;
    posX = (N % tilesPerRow) * tileW;
    posY = (N / tilesPerCol) * tileH;
    return sf::Vector2f(posX, posY);
}
 
std::list<int> Map::astar(int start, int end) {
    int nodeCount = tiles.size();
    int currentNode, neighborNode;
    sf::Vector2f startPos = getTilePos(start);
    sf::Vector2f endPos = getTilePos(end);
    std::vector<int> priorNodes(nodeCount);
    std::set<int> openSet = { start };
    std::set<int> closedSet;
 
    for (int i = 0; i < nodeCount; ++i) {
        if (tiles[i] == wall) {
            closedSet.insert(i);
        }
    }
 
    std::list<int> neighborNodes = {
        -tilesPerRow - 1,
        -tilesPerRow,
        -tilesPerRow + 1,
        -1,
        1,
        tilesPerRow - 1,
        tilesPerRow,
        tilesPerRow + 1
    };
 
    std::vector<float> cost(nodeCount, 99999);
    std::vector<float> score(nodeCount, 99999);
 
    cost[start] = 0;
    score[start] = std::sqrt(std::pow(startPos.x - endPos.x, 2) + std::pow(startPos.y - endPos.y, 2));
 
    while (!openSet.empty()) {
        float lowestScore = 99999;
        for (int node : openSet) {
            if (lowestScore > score[node]) {
                lowestScore = score[node];
                currentNode = node;
             }
        }
        sf::Vector2f currentPos = getTilePos(currentNode);
 
        // Finish algorithm here
        if (currentNode == end) {
            std::list<int> fullPath = { currentNode };
            while (currentNode != start) {
                currentNode = priorNodes[currentNode];
                fullPath.push_front(currentNode);
            }
            return fullPath;
        }
 
        openSet.erase(currentNode);
        closedSet.insert(currentNode);
 
        std::set<int> neighbors;
        for (int node : neighborNodes) {
            neighborNode = currentNode + node;
            if (neighborNode < 0 || neighborNode >= nodeCount) {
                continue;
            }
            else if (tiles[neighborNode] == ground) {
                neighbors.insert(neighborNode);
            }
        }
        for (int node : neighbors) {
            if (closedSet.count(node) > 0) {
                continue;
            }
            if (openSet.count(node) == 0) {
                openSet.insert(node);
            }
 
            sf::Vector2f neighborPos = getTilePos(node);
            float heuristic = std::sqrt(std::pow(neighborPos.x - endPos.x, 2) + std::pow(neighborPos.y - endPos.y, 2));
            float dist = std::sqrt(std::pow(neighborPos.x - currentPos.x, 2) + std::pow(neighborPos.y - currentPos.y, 2));
            float tempCost = cost[currentNode] + dist;
 
            if (tempCost >= cost[node]) {
                continue;
            }
            priorNodes[node] = currentNode;
            cost[node] = tempCost;
            score[node] = tempCost + heuristic;
    }
 
    }
 
 
    std::list<int> emptyList;
    return emptyList;
}
