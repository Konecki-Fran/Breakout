#pragma once

#include "Component.h"
#include "ComponentType.h"
#include "Moveable.h"

#include "Helpers.h"

class Transform : public Component, public Moveable
{
public:
	Transform();
	~Transform() final = default;

	int getX() const override;
	int getY() const override;
	int getH() const;
	int getW() const;
	void setX(int x) override;
	void setY(int y) override;
	void setH(int h);
	void setW(int w);
	void set(int x, int y, int w, int h);
	void set(int x, int y);
	void set(Vector4<int> values);

	void update() override {};

private:
	int _x{};
	int _y{};
	int _h{};
	int _w{};
};