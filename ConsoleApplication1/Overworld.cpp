#include "Overworld.h"
#include "NPC.h"
#include <cstdlib>  // for rand()
#include <random>
#include <ctime>    // for time()
#include <queue>
#include <utility>

// Generate a tile name based on type
std::string GenerateTileName(TileType type) {
    switch (type) {
    case TileType::Plains: return "Plains";
    case TileType::Forest: return "Forest";
    case TileType::Mountain: return "Mountain";
    case TileType::Water: return "Water";
    case TileType::Town: return "Town";
    case TileType::Dungeon: return "Dungeon";
    case TileType::Landmark: return "Landmark";
    default: return "Unknown Region";
    }
}

// Initializes a procedural overworld map
Overworld GenerateOverworld(int width, int height) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // varied maps

    Overworld world;
    world.width = width;
    world.height = height;
    world.map.resize(height);

    // Step 1: Fill map with Plains
    for (int y = 0; y < height; ++y) {
        std::vector<OverworldTile> row;
        row.reserve(width);
        for (int x = 0; x < width; ++x) {
            OverworldTile tile;
            tile.type = TileType::Plains;
            tile.isWalkable = true;
            tile.name = GenerateTileName(tile.type);
            tile.description = "A region of " + tile.name + ".";
            row.emplace_back(std::move(tile));
        }
        world.map[y] = std::move(row);
    }

    // Step 2: Grow terrain clusters
    for (int i = 0; i < 100; ++i) { // Cluster amount
        int x = rand() % width;
        int y = rand() % height;
        int size = 10 + rand() % 30; // Cluster size

        TileType type;
        int roll = rand() % 100;
        if (roll < 40) type = TileType::Forest;
        else if (roll < 70) type = TileType::Mountain;
        else type = TileType::Water;

        GrowTerrainCluster(world, x, y, type, size);
    }

    // Step 3: Inject handcrafted starting dungeon
    OverworldTile startingDungeon;
    startingDungeon.type = TileType::Dungeon;
    startingDungeon.isHandcrafted = true;
    startingDungeon.name = "Forgotten Cell";
    startingDungeon.description = "A damp, stone-walled cell where your journey begins.";
    world.map[10][10] = std::move(startingDungeon);


    // Step 4: Inject handcrafted pieces
    std::vector<PieceGenerator> pieceGenerators = {
        MakeForestClearingWithDungeon,
        //MakeVillageCluster,
        //MakeMountainPass
    };

    int townClusterCount = 5; // Number of towns to place

    for (int i = 0; i < townClusterCount; ++i) {
        OverworldPiece town = MakeProceduralTown();

        int x = rand() % (world.width - town.width);
        int y = rand() % (world.height - town.height);

        if (CanPlaceOverworldPiece(world, town, x, y)) {
            PlaceOverworldPiece(world, town, x, y);
        }

        for (int ty = 1; ty < town.height - 1; ++ty) {
            for (int tx = 1; tx < town.width - 1; ++tx) {
                if (rand() % 100 < 30) { // 30% chance to place an NPC
                    int wx = x + tx;
                    int wy = y + ty;
                    NPC npc = GenerateProceduralNPC(wx, wy, "Town");
                    world.npcs.push_back(npc);
                    world.map[wy][wx].description += " You see someone here.";
                }
            }
        }

    }


    for (int i = 0; i < 10; ++i) {
        auto generator = pieceGenerators[rand() % pieceGenerators.size()];
        OverworldPiece piece = generator();

        int x = rand() % (world.width - piece.width);
        int y = rand() % (world.height - piece.height);

        if (CanPlaceOverworldPiece(world, piece, x, y)) {
            PlaceOverworldPiece(world, piece, x, y);
        }

    }
    return world;
}


// Check if piece can be placed in overworld
bool CanPlaceOverworldPiece(const Overworld& world, const OverworldPiece& piece, int startX, int startY) {
    for (int y = 0; y < piece.height; ++y) {
        for (int x = 0; x < piece.width; ++x) {
            int wx = startX + x;
            int wy = startY + y;
            if (wx >= 0 && wx < world.width && wy >= 0 && wy < world.height) {
                if (world.map[wy][wx].isHandcrafted) return false;
            }
        }
    }
    return true;
}

void PlaceOverworldPiece(Overworld& world, OverworldPiece& piece, int startX, int startY) {
    for (int y = 0; y < piece.height; ++y) {
        for (int x = 0; x < piece.width; ++x) {
            int wx = startX + x;
            int wy = startY + y;
            if (wx >= 0 && wx < world.width && wy >= 0 && wy < world.height) {
                world.map[wy][wx] = std::move(piece.tiles[y][x]);
            }
        }
    }
}

OverworldPiece MakeForestClearingWithDungeon() {
    OverworldPiece piece(8, 8);
    piece.name = "Forest Clearing";

    for (int y = 0; y < piece.height; ++y) {
        for (int x = 0; x < piece.width; ++x) {
            OverworldTile tile;
            tile.type = TileType::Forest;
            tile.isHandcrafted = true;
            tile.name = "Thick Woods";
            tile.description = "Dense trees surround the clearing.";
            piece.tiles[y][x] = std::move(tile);
        }
    }

    // Place dungeon in center
    int cx = piece.width / 2;
    int cy = piece.height / 2;
    piece.tiles[cy][cx].type = TileType::Dungeon;
    piece.tiles[cy][cx].name = "Ancient Hollow";
    piece.tiles[cy][cx].description = "A moss-covered ruin leading underground.";

    return piece;
}

OverworldPiece MakeProceduralTown() {
    int width = 7;
    int height = 7;
    OverworldPiece town(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            OverworldTile tile;
            tile.type = TileType::Town;
            tile.isHandcrafted = true;
            tile.isWalkable = true;

            bool canPlaceBuilding = true;

            // Check adjacent tiles to avoid placing buildings next to each other
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue; // skip self
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        if (!town.tiles.empty() && town.tiles[ny][nx].name == "Town Building") {
                            canPlaceBuilding = false;
                        }
                    }
                }
            }

            if (canPlaceBuilding && rand() % 100 < 20) {
                tile.name = "Town Building";
                tile.description = "A modest home with a sloped roof.";
            }
            else {
                tile.name = "Plain Tile";
                tile.description = "A grassy patch between buildings.";
            }

            town.tiles[y][x] = std::move(tile);
        }
    }

    return town;
}

void GrowTerrainCluster(Overworld& world, int startX, int startY, TileType type, int size) {
    std::queue<std::pair<int, int>> frontier;
    frontier.push({ startX, startY });

    int placed = 0;
    while (!frontier.empty() && placed < size) {
        std::pair<int, int> current = frontier.front();
        int x = current.first;
        int y = current.second;
        frontier.pop();

        if (x < 0 || x >= world.width || y < 0 || y >= world.height)
            continue;

        OverworldTile& tile = world.map[y][x];
        if (tile.type != TileType::Plains) continue; // Only overwrite plains

        tile.type = type;
        tile.name = GenerateTileName(type);
        tile.description = "A region of " + tile.name + ".";
        tile.isWalkable = (type != TileType::Mountain && type != TileType::Water);

        ++placed;

        // Randomly expand to neighbors
        if (rand() % 100 < 80) frontier.push({ x + 1, y });
        if (rand() % 100 < 80) frontier.push({ x - 1, y });
        if (rand() % 100 < 80) frontier.push({ x, y + 1 });
        if (rand() % 100 < 80) frontier.push({ x, y - 1 });
    }
}

