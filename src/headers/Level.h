#pragma once

#include "BrickType.h"
#include "Window.h"
#include "Texture.h"
#include "Force.h"
#include "Helpers.h"
#include "GameManager.h"

#include "tinyxml/tinyxml.h"

#include <array>

class Level 
{
public:
	explicit Level(int level);
	virtual ~Level() = default;

	int getRowCount() const;
	int getColumnCount() const;
	int getRowSpacing() const;
	int getColumnSpacing() const;
	int getBrickWidth() const;
	int gteBrickHeight() const;
	CharMatrix getBricks() const;

	std::optional<char> getBrickType(int i, int j) const;
	std::optional<BrickType> getBrickType(char t) const;

	bool isEmptySpace(int i, int j) const;
	std::pair<int, int> getBrickScreenPosition(int i, int j, int w, int h, int columnSpacing, int rowSpacing) const;
	std::optional<String> getBrickTexturePath(int i, int j) const;

	void loadConfig(const Path& configPath);
	CharMatrix CreateCharMatrix(String config) const;

	void initializeBricks(GameManager& manager, const Window& window) const;
	void initializePlayer(GameManager& manager, const Window& window) const;
	void initializeBall(GameManager& manager, const Window& window) const;
	void resetBall(const GameObject& ball) const;
	void resetPlayer(const GameObject& player) const;

private:
	int _rowCount;
	int _columnCount;
	int _rowSpacing;
	int _columnSpacing;
	int _brickWidth;
	int _brickHeight;
	CharMatrix _bricks;	
	Path _backgroundTexture;
	std::vector<BrickType> _brickTypes;
};
