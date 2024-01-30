#include <BrickType.h>

BrickType::BrickType(
	char id,
	StringView texture,
	StringView hitPoints,
	StringView hitSound, 
	StringView breakSound, 
	StringView breakScore)
	: _id(id), _texture(texture), _hitPoints(std::stoi(hitPoints.data())),
	_hitSound(hitSound), _breakSound(breakSound), _breakScore(std::stoi(breakScore.data())) {};

char BrickType::getId() const { return _id; }
Path BrickType::getTexture() const { return _texture; }
int BrickType::getHitPoints() const { return _hitPoints; }
Path BrickType::getHitSound() const { return _hitSound; }
Path BrickType::getBreakSound() const { return _breakSound; }
int BrickType::getBreakScore() const { return _breakScore; }

