

#ifndef __TOBII_SDK_CPP_TRACKING_TYPES_HPP__
#define __TOBII_SDK_CPP_TRACKING_TYPES_HPP__

#include <string>
#include <vector>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <tobii/sdk/core.h>

namespace tobii {

namespace sdk {

namespace cpp {

class Blob {

public:
	typedef boost::shared_array<uint8_t> data_type_t;

	Blob() :
		length_(0)
	{ }

	Blob(boost::shared_array<uint8_t> bytes, uint32_t byteLength) :
		data_(bytes),
		length_(byteLength)
	{ }

	uint32_t getLength()
	{
		return length_;
	}

	boost::shared_array<uint8_t> getData()
	{
		return data_;
	}

private:
	boost::shared_array<uint8_t> data_;
	uint32_t length_;
};

struct AuthorizeChallenge {
	uint32_t realmId;
	uint32_t algorithm;
	Blob challengeData;
};

struct UnitInfo {
	typedef boost::shared_ptr<UnitInfo> pointer_t;

	std::string serialNumber;
	std::string model;
	std::string generation;
	std::string firmwareVersion;
};


struct PayPerUseInfo {
	bool enabled;
	bool authorized;
	uint32_t realm;
};

struct Point2d {
	double x;
	double y;
	Point2d() :x(0.0), y(0.0)
	{}
	Point2d(double argX, double argY) :x(argX), y(argY)
	{}
};

struct Point3d {
	double x;
	double y;
	double z;
	Point3d() :x(0.0), y(0.0), z(0.0)
	{}
	Point3d(double argX, double argY, double argZ) :x(argX), y(argY), z(argZ)
	{}
};

inline std::ostream& operator<< (std::ostream& stream, const Point2d& pt)
{
	stream << "point 2d [";
	stream << pt.x << " " << pt.y;
	stream << "]";
	return stream;
}

inline std::ostream& operator<< (std::ostream& stream, const Point3d& pt)
{
	stream << "point 3d [";
	stream << pt.x << " " << pt.y << " " << pt.z;
	stream << "]";
	return stream;
}


struct Extension {
	typedef boost::shared_ptr<std::vector<Extension> > vector_pointer_t;

	uint32_t extensionId;
	std::string name;
	uint32_t realm;
	uint32_t protocolVersion;
};

struct XConfiguration {
	typedef boost::shared_ptr<XConfiguration> pointer_t;

	Point3d upperLeft;
	Point3d upperRight;
	Point3d lowerLeft;
};

struct TrackBox {
	typedef boost::shared_ptr<TrackBox> pointer_t;

	Point3d point1;
	Point3d point2;
	Point3d point3;
	Point3d point4;
	Point3d point5;
	Point3d point6;
	Point3d point7;
	Point3d point8;
};



} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif  // __TOBII_SDK_CPP_TRACKING_TYPES_HPP__
