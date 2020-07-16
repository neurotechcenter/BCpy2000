#ifndef __TOBII_SDK_CPP_TRACKING_GAZE_DATA_ITEM_HPP__
#define __TOBII_SDK_CPP_TRACKING_GAZE_DATA_ITEM_HPP__

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/noncopyable.hpp>
#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/Types.hpp>
#include <tobii/sdk/cpp/ParamStack.hpp>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class GazeDataItem 
{
public:
	typedef boost::shared_ptr<GazeDataItem> pointer_t;
	enum extension_value_uint32_t { EXTENSION_TRIG_SIGNAL };

	uint64_t timestamp;

	Point3d leftEyePosition3d;
	Point3d rightEyePosition3d;

	uint32_t leftValidity;
	uint32_t rightValidity;

	Point3d leftEyePosition3dRelative;
	Point3d rightEyePosition3dRelative;

	Point3d leftGazePoint3d;
	Point3d rightGazePoint3d;

	Point2d leftGazePoint2d;
	Point2d rightGazePoint2d;

	float leftPupilDiameter;
	float rightPupilDiameter;

	bool tryGetExtensionValue(extension_value_uint32_t extension, uint32_t& value)
	{
		return doTryGetExtensionValue(extension, value);
	}

protected:
	GazeDataItem()
		: timestamp(0), leftValidity(0), rightValidity(0), leftPupilDiameter(0), rightPupilDiameter(0)
	{
	}

private:
	virtual bool doTryGetExtensionValue(extension_value_uint32_t extension, uint32_t& value) = 0;
};

class GazeDataItemImpl : public GazeDataItem, boost::noncopyable
{
public:
	GazeDataItemImpl(const ParamStack& stack)
		: stack_(stack)
	{
		parseGazeDataRow();
	}

private:
	ParamStack stack_;

	void parseGazeDataRow();
	void parseGazeDataColumn(uint32_t& index);
	void parseUnknownContent(uint32_t& index);
	uint32_t findColumnIndex(uint32_t targetColumnId);
	bool doTryGetExtensionValue(extension_value_uint32_t extension, uint32_t& value);

	static const uint32_t ROW_ID					= 3000;
	static const uint32_t COLUMN_ID					= 3001;

	static const uint32_t TIMESTAMP_ID 				= 1;

	static const uint32_t LEFT_EYE_POS_3D_ID 		= 2;
	static const uint32_t LEFT_EYE_POS_3D_REL_ID 	= 3;
	static const uint32_t LEFT_GAZE_POINT_3D_ID 	= 4;
	static const uint32_t LEFT_GAZE_POINT_2D_ID 	= 5;
	static const uint32_t LEFT_PUPIL_ID 			= 6;
	static const uint32_t LEFT_VALIDITY_ID			= 7;

	static const uint32_t RIGHT_EYE_POS_3D_ID 		= 8;
	static const uint32_t RIGHT_EYE_POS_3D_REL_ID 	= 9;
	static const uint32_t RIGHT_GAZE_POINT_3D_ID 	= 10;
	static const uint32_t RIGHT_GAZE_POINT_2D_ID 	= 11;
	static const uint32_t RIGHT_PUPIL_ID			= 12;
	static const uint32_t RIGHT_VALIDITY_ID 		= 13;

	static const uint32_t TRIG_SIGNAL_ID			= 14;
};

inline void GazeDataItemImpl::parseGazeDataRow()
{
	uint32_t index = 0;
	NodeProlog row_prolog = stack_.getNodeProlog(index++);

	if(row_prolog.getId() != ROW_ID)
		throw new std::runtime_error("unexpected data in param stack");

	for (uint32_t column = 0; column < row_prolog.getLength(); ++column) {
		parseGazeDataColumn(index);
	}
}

inline void GazeDataItemImpl::parseGazeDataColumn(uint32_t& index)
{
	NodeProlog column_prolog = stack_.getNodeProlog(index++);
	if(column_prolog.getId() != COLUMN_ID)
		throw new std::runtime_error("unexpected data in param stack");
	if(column_prolog.getLength() != 2)
		throw new std::runtime_error("unexpected column data length in param stack");

	// Read column id
	uint32_t id = stack_.getUInt32(index++);

	switch (id) {
		case TIMESTAMP_ID:
			timestamp = stack_.getInt64(index++);
			break;

		case LEFT_EYE_POS_3D_ID:
			leftEyePosition3d = stack_.readPoint3d(index);
			break;
		case LEFT_EYE_POS_3D_REL_ID:
			leftEyePosition3dRelative = stack_.readPoint3d(index);
			break;
		case LEFT_GAZE_POINT_3D_ID:
			leftGazePoint3d = stack_.readPoint3d(index);
			break;
		case LEFT_GAZE_POINT_2D_ID:
			leftGazePoint2d = stack_.readPoint2d(index);
			break;
		case LEFT_PUPIL_ID:
			leftPupilDiameter = stack_.getFixed15x16AsFloat32(index++);
			break;
		case LEFT_VALIDITY_ID:
			leftValidity = stack_.getUInt32(index++);
			break;

		case RIGHT_EYE_POS_3D_ID:
			rightEyePosition3d = stack_.readPoint3d(index);
			break;
		case RIGHT_EYE_POS_3D_REL_ID:
			rightEyePosition3dRelative = stack_.readPoint3d(index);
			break;
		case RIGHT_GAZE_POINT_3D_ID:
			rightGazePoint3d = stack_.readPoint3d(index);
			break;
		case RIGHT_GAZE_POINT_2D_ID:
			rightGazePoint2d = stack_.readPoint2d(index);
			break;
		case RIGHT_PUPIL_ID:
			rightPupilDiameter = stack_.getFixed15x16AsFloat32(index++);
			break;
		case RIGHT_VALIDITY_ID:
			rightValidity = stack_.getUInt32(index++);
			break;

		default:
			parseUnknownContent(index);
			break;
	}
}

inline void GazeDataItemImpl::parseUnknownContent(uint32_t& index)
{
	if (stack_.isNodeProlog(index))
	{
		NodeProlog prolog = stack_.getNodeProlog(index++);
		uint32_t num_children = prolog.getLength();
		for (uint32_t child = 0; child < num_children; ++child) {
			// Recurse further down
			parseUnknownContent(index);
		}
	}
	else
	{
		// Skip item
		index++;
	}
}

inline uint32_t GazeDataItemImpl::findColumnIndex(uint32_t targetColumnId)
{
	uint32_t index = 0;
	NodeProlog row_prolog = stack_.getNodeProlog(index++);

	if(row_prolog.getId() != ROW_ID)
		throw new std::runtime_error("unexpected data in param stack");

	for (uint32_t column = 0; column < row_prolog.getLength(); ++column) {
		NodeProlog column_prolog = stack_.getNodeProlog(index++);
		if(column_prolog.getId() != COLUMN_ID)
			throw new std::runtime_error("unexpected data in param stack");
		if(column_prolog.getLength() != 2)
			throw new std::runtime_error("unexpected column data length in param stack");

		uint32_t id = stack_.getUInt32(index++);
		if (id == targetColumnId) {
			return index;
		}
		else {
			parseUnknownContent(index);
		}
	}

	return 0;
}

inline bool GazeDataItemImpl::doTryGetExtensionValue(GazeDataItem::extension_value_uint32_t extension, uint32_t& value)
{
	uint32_t index(0);
	switch (extension) 
	{
		case EXTENSION_TRIG_SIGNAL:
			index = findColumnIndex(TRIG_SIGNAL_ID);
			break;
	}

	if (index > 0) // 0 is not a valid index since it points to the node prolog
	{
		value = stack_.getUInt32(index);
		return true;
	}

	return false;
}

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif  // __TOBII_SDK_CPP_TRACKING_GAZE_DATA_ITEM_HPP__
