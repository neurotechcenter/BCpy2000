
#ifndef __TOBII_SDK_CPP_TRACKING_CALIBRATION_HPP__
#define __TOBII_SDK_CPP_TRACKING_CALIBRATION_HPP__

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>
#include <tobii/sdk/cpp/Types.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

struct CalibrationPlotItem 
{
	Point2d truePosition;

	Point2d leftMapPosition;
	int32_t leftStatus;

	Point2d rightMapPosition;
	int32_t rightStatus;
};

class Calibration 
{
public:
	typedef boost::shared_ptr<Calibration> pointer_t;
	typedef boost::shared_ptr<std::vector<CalibrationPlotItem> > plot_data_vector_t;

	virtual ~Calibration() {}
	
	Blob& getRawData()
	{
		return doGetRawData();
	}

	plot_data_vector_t getPlotData()
	{
		return doGetPlotData();
	}

private:
	virtual Blob& doGetRawData() = 0;
	virtual plot_data_vector_t doGetPlotData() = 0;
};

class CalibrationImpl : public Calibration, boost::noncopyable
{
public:
	CalibrationImpl(const Blob& data) :
		rawData_(data),
		plotData_(new std::vector<CalibrationPlotItem>())
	{
		extractPlotData();
	}

	CalibrationImpl() :
		plotData_(new std::vector<CalibrationPlotItem>())
	{

	}

private:
	Blob rawData_;
	plot_data_vector_t plotData_;

	Blob& doGetRawData()
	{
		return rawData_;
	}

	plot_data_vector_t doGetPlotData()
	{
		return plotData_;
	}

	void extractPlotData()
	{
		if(!rawData_.getLength())
			return;

		plotData_->clear();

		uint8_t* currentPos_ = rawData_.getData().get();

		int32_t skipLength_ = *reinterpret_cast<int32_t*>(currentPos_);
		currentPos_ += sizeof(int32_t);
		currentPos_ += skipLength_;

		uint32_t numberOfItems_ = *reinterpret_cast<uint32_t*>(currentPos_);
		currentPos_ += sizeof(uint32_t);

		for (uint32_t i = 0; i < numberOfItems_; ++i) {
			extractOneItem(currentPos_);
		}
	}

	void extractOneItem(uint8_t*& currentPos_)
	{
		CalibrationPlotItem item;
		
		item.truePosition.x = *reinterpret_cast<float*>(currentPos_);
		currentPos_ += sizeof(float);
		item.truePosition.y = *reinterpret_cast<float*>(currentPos_);
		currentPos_ += sizeof(float);

		item.leftMapPosition.x = *reinterpret_cast<float*>(currentPos_);
		currentPos_ += sizeof(float);
		item.leftMapPosition.y = *reinterpret_cast<float*>(currentPos_);
		currentPos_ += sizeof(float);
		item.leftStatus =  *reinterpret_cast<int*>(currentPos_);
		currentPos_ += sizeof(int);
		currentPos_ += sizeof(float); // Skip quality value

		item.rightMapPosition.x = *reinterpret_cast<float*>(currentPos_);
		currentPos_ += sizeof(float);
		item.rightMapPosition.y = *reinterpret_cast<float*>(currentPos_);
		currentPos_ += sizeof(float);
		item.rightStatus =  *reinterpret_cast<int*>(currentPos_);
		currentPos_ += sizeof(int);
		currentPos_ += sizeof(float); // Skip quality value

		plotData_->push_back(item);
	}
};


} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif  // __TOBII_SDK_CPP_TRACKING_CALIBRATION_HPP__
