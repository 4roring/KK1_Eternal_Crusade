#pragma once

class CObserver
{
private:
	CObserver();

public:
	virtual	~CObserver();

public:
	void Update();
};