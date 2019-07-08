#pragma once

class CIncopiable
{
protected:
	CIncopiable() {}
	virtual ~CIncopiable() {}

private:
	// Disable copy of singletons.
	CIncopiable(const CIncopiable&);
	void operator=(const CIncopiable&);
};