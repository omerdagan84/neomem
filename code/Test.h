// Test.h: interface for the CTest class.
#pragma once



class CNeoMem;



class CTest 
{
public:
	CTest();
	virtual ~CTest();



	static void DoTests(CNeoMem& app);
};



