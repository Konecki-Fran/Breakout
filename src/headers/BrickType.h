#pragma once

#include "Helpers.h"

class BrickType 
{
public:
	BrickType(
		char id, 
		StringView texture,
		StringView hitPoints,
		StringView hitSound, 
		StringView breakSound, 
		StringView breakScore
	);
	virtual ~BrickType() = default;

	char getId() const;
	Path getTexture() const;
	int getHitPoints() const;
	Path getHitSound() const;
	Path getBreakSound() const;
	int getBreakScore() const;

private:
	char _id;
	Path _texture;
	int _hitPoints;
	Path _hitSound;
	Path _breakSound;
	int _breakScore;
};