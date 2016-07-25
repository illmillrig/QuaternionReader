
#include "QuaternionReader.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MGlobal.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MQuaternion.h>
#include <maya/MMatrix.h>
#include <maya/MFnNumericAttribute.h>
#include <cmath>


inline double max(double a, double b) {
	return (a < b) ? b : a;
}

MQuaternion quatFromMatrix(const MMatrix &tfm) {
	double x, y, z, w;
	w = std::sqrt(max(0.0, 1.0 + tfm[0][0] + tfm[1][1] + tfm[2][2])) / 2.0;
	x = std::sqrt(max(0.0, 1.0 + tfm[0][0] - tfm[1][1] - tfm[2][2])) / 2.0;
	y = std::sqrt(max(0.0, 1.0 - tfm[0][0] + tfm[1][1] - tfm[2][2])) / 2.0;
	z = std::sqrt(max(0.0, 1.0 - tfm[0][0] - tfm[1][1] + tfm[2][2])) / 2.0;
	x = std::copysign(x, tfm[2][1] - tfm[1][2]);
	y = std::copysign(y, tfm[0][2] - tfm[2][0]);
	z = std::copysign(z, tfm[1][0] - tfm[0][1]);

	return MQuaternion(x,y,z,w);
}


MTypeId QuaternionReader::id(0x001226D7);
MObject QuaternionReader::inMatrix;
MObject QuaternionReader::x;
MObject QuaternionReader::y;
MObject QuaternionReader::z;
MObject QuaternionReader::w;

QuaternionReader::QuaternionReader() {}
QuaternionReader::~QuaternionReader() {}

void* QuaternionReader::creator() {
	return new QuaternionReader();
}

MStatus QuaternionReader::initialize() {
	MStatus stat;

	MFnMatrixAttribute fnMat;
	MFnNumericAttribute fnNum;

	inMatrix = fnMat.create("inMatrix", "in", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = QuaternionReader::addAttribute(inMatrix);
	CHECK_MSTATUS(stat);

	x = fnNum.create("X", "x", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat)
	fnNum.setStorable(false);
	fnNum.setWritable(false);
	stat = QuaternionReader::addAttribute(x);
	CHECK_MSTATUS(stat);

	y = fnNum.create("Y", "y", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat)
	fnNum.setStorable(false);
	fnNum.setWritable(false);
	stat = QuaternionReader::addAttribute(y);
	CHECK_MSTATUS(stat);

	z = fnNum.create("Z", "z", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat)
	fnNum.setStorable(false);
	fnNum.setWritable(false);
	stat = QuaternionReader::addAttribute(z);
	CHECK_MSTATUS(stat);

	w = fnNum.create("W", "w", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat)
	fnNum.setStorable(false);
	fnNum.setWritable(false);
	stat = QuaternionReader::addAttribute(w);
	CHECK_MSTATUS(stat);

	QuaternionReader::attributeAffects(inMatrix, x);
	QuaternionReader::attributeAffects(inMatrix, y);
	QuaternionReader::attributeAffects(inMatrix, z);
	QuaternionReader::attributeAffects(inMatrix, w);

	return MS::kSuccess;
}

MStatus QuaternionReader::compute( const MPlug& plug, MDataBlock& data ){

	MQuaternion rot = quatFromMatrix(data.inputValue(inMatrix).asMatrix());
	data.outputValue(x).setDouble(rot.x);
	data.outputValue(y).setDouble(rot.y);
	data.outputValue(z).setDouble(rot.z);
	data.outputValue(w).setDouble(rot.w);

	data.setClean(x);
	data.setClean(y);
	data.setClean(z);
	data.setClean(w);

	return MS::kSuccess;
}