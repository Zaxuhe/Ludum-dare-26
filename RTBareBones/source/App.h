/*
 *  App.h
 *  Created by Seth Robinson on 3/6/09.
 *  For license info, check the license.txt file that should have come with this.
 *
 */

#pragma once

#include "BaseApp.h"

class Point
{
public:
	Point* parent;
	bool closed;
	bool opened;
	int x;
	int y;
	int f;
	int g;
	int h;
	bool walkable;

	Point::Point()
	{
		parent = NULL;
		closed = false;
		opened = false;

		x = y = f = g = h = 0;
	}

	Point::Point(int x, int y, bool w)
	{
		parent = NULL;
		closed = false;
		opened = false;

		x = y = f = g = h = 0;

		this->walkable = w;
		this->x = x;
		this->y = y;
	}

	CL_Vec2f Point::getPosition()
	{
		return CL_Vec2f((float)(x), (float)(y));
	}

	Point* Point::getParent()
	{
		return parent;
	}

	void Point::setParent(Point *p)
	{
		parent = p;
	}

	int Point::getX()
	{
		return x;
	}

	int Point::getY()
	{
		return y;
	}

	float Point::getXf()
	{
		return (float)x;
	}

	float Point::getYf()
	{
		return (float)y;
	}

	int Point::getGScore(Point *p)
	{
		return p->g + ((x == p->x || y == p->y) ? 10 : 14);
	}

	int Point::getHScore(Point *p)
	{
		return (abs(p->x - x) + abs(p->y - y)) * 10;
	}

	int Point::getGScore()
	{
		return g;
	}

	int Point::getHScore()
	{
		return h;
	}

	int Point::getFScore()
	{
		return f;
	}

	void Point::computeScores(Point *end)
	{
		g = getGScore(parent);
		h = getHScore(end);
		f = g + h;
	}

	bool Point::hasParent()
	{
		return parent != NULL;
	}
};

class App: public BaseApp
{
public:
	
	App();
	virtual ~App();
	
	virtual bool Init();
	virtual void Kill();
	virtual void Draw();
	virtual void OnScreenSizeChange();
	virtual void OnEnterBackground();
	virtual void OnEnterForeground();
	virtual void Update();
	void OnExitApp(VariantList *pVarList);
	
	
	//we'll wire these to connect to some signals we care about
	void OnAccel(VariantList *pVList);
	void OnArcadeInput(VariantList *pVList);

private:

	bool m_bDidPostInit;
	Surface m_surf; //for testing
};


App * GetApp();
const char * GetAppName();
const char * GetBundlePrefix();
const char * GetBundleName();
