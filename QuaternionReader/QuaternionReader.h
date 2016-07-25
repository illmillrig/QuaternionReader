#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

 
class QuaternionReader : public MPxNode
{
public:
	QuaternionReader();
	virtual	~QuaternionReader(); 
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
public:
	static MTypeId id;
	static MObject inMatrix;
	static MObject x;
	static MObject y;
	static MObject z;
	static MObject w;
};


