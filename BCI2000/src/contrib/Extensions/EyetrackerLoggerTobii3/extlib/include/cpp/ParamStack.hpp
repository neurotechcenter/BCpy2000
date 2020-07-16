#ifndef __TOBII_SDK_CPP_PARAM_STACK_HPP__
#define __TOBII_SDK_CPP_PARAM_STACK_HPP__

#include <string>
#include <cstring>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/cstdint.hpp>
#include <tobii/sdk/core.h>
#include <tobii/sdk/cpp/EyeTrackerException.hpp>
#include <tobii/sdk/cpp/Types.hpp>

namespace tobii {

namespace sdk {

namespace cpp {

class NodeProlog 
{
public:
	NodeProlog(uint32_t val) : value_(val)
	{
	}

	NodeProlog(uint32_t length, uint32_t id)
	{
		value_ = 0;
		value_ |= (id & ID_MASK);
		value_ |= (length << LENGTH_SHIFT) & LENGTH_MASK;
	}

	uint32_t getId() const
	{
		return (value_ & ID_MASK);
	}

	uint32_t getLength() const
	{
		return (value_ & LENGTH_MASK) >> LENGTH_SHIFT;
	}

	uint32_t getValue() const
	{
		return value_;
	}

private:
	static const uint32_t ID_MASK = 0x0000FFFF;
	static const uint32_t LENGTH_MASK = 0x0FFF0000;
	static const int32_t LENGTH_SHIFT = 16;

	uint32_t value_;
};

class ParamStack
{
public:
	ParamStack()
	{
		tobii_sdk_error_t* error = 0;
		handle_.reset(tobii_sdk_param_stack_create(&error), &tobii_sdk_param_stack_destroy);
		EyeTrackerException::throwOnError(error);
	}

	ParamStack(const tobii_sdk_param_stack_t* stack)
	{
		clone(stack);
	}

	ParamStack(const ParamStack& other)
	{
		clone(other.getHandle());
	}

	ParamStack& operator = (const ParamStack& other)
	{
		clone(other.getHandle());
		return *this;
	}

	tobii_sdk_param_stack_t* getHandle() const
	{
		return handle_.get();
	}

	void pushInt32 (int32_t value)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_stack_push_int32(handle_.get(), value, &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushUInt32 (uint32_t value)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_stack_push_uint32(handle_.get(), value, &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushInt64 (int64_t value)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_stack_push_int64(handle_.get(), value, &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushUInt64 (uint64_t value)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_stack_push_uint64(handle_.get(), value, &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushFloat32AsFixed15x16 (float value)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_stack_push_float32_as_fixed_15x16(handle_.get(), value, &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushFloat64AsFixed22x41 (double value)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_stack_push_float64_as_fixed_22x41(handle_.get(), value, &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushNodeProlog (NodeProlog value)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_stack_push_node_prolog(handle_.get(), value.getValue(), &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushString (const std::string& value)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_stack_push_string(handle_.get(), value.c_str(), static_cast<boost::uint32_t>(value.length()), &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushBlob (const unsigned char* blob, uint32_t blob_length)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_stack_push_blob(handle_.get(), blob, blob_length, &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushPoint3d(const Point3d& point)
	{
		NodeProlog prolog(3, 8001);
		tobii_sdk_error_t* error = 0;

		tobii_sdk_param_stack_push_node_prolog(handle_.get(), prolog.getValue(), &error);
		EyeTrackerException::throwOnError(error);
		tobii_sdk_param_stack_push_float64_as_fixed_22x41(handle_.get(), point.x, &error);
		EyeTrackerException::throwOnError(error);
		tobii_sdk_param_stack_push_float64_as_fixed_22x41(handle_.get(), point.y, &error);
		EyeTrackerException::throwOnError(error);
		tobii_sdk_param_stack_push_float64_as_fixed_22x41(handle_.get(), point.z, &error);
		EyeTrackerException::throwOnError(error);
	}

	void pushVector(const std::vector<uint32_t>& vector)
	{
		tobii_sdk_error_t* error = 0;
		boost::shared_ptr<tobii_sdk_vector_t> p_vec(
				tobii_sdk_vector_create(static_cast<boost::uint32_t>(vector.size()), TOBII_SDK_PARAM_TYPE_UINT32, &error),
				&tobii_sdk_vector_destroy);
		EyeTrackerException::throwOnError(error);

		for(size_t i=0; i<vector.size(); i++)
		{
			tobii_sdk_vector_set_uint32(p_vec.get(), static_cast<boost::uint32_t>(i), vector[i], &error);
			EyeTrackerException::throwOnError(error);
		}

		tobii_sdk_param_stack_push_vector(handle_.get(), p_vec.get(), &error);
		EyeTrackerException::throwOnError(error);
	}

	int32_t getInt32 (uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		int32_t result =  tobii_sdk_param_stack_get_int32(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		return result;
	}

	uint32_t getUInt32 (uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		uint32_t result = tobii_sdk_param_stack_get_uint32(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		return result;
	}

	int64_t getInt64 (uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		int64_t result = tobii_sdk_param_stack_get_int64(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		return result;
	}

	uint64_t getUInt64 (uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		uint64_t result = tobii_sdk_param_stack_get_uint64(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		return result;
	}

	float getFixed15x16AsFloat32 (uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		float result = tobii_sdk_param_stack_get_fixed_15x16_as_float32(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		return result;
	}

	double getFixed22x41AsFloat64 (uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		double result = tobii_sdk_param_stack_get_fixed_22x41_as_float64(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		return result;
	}

	bool isNodeProlog(uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		tobii_sdk_param_type_t type = tobii_sdk_param_stack_get_type(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		return type == TOBII_SDK_PARAM_TYPE_NODE_PROLOG;
	}

	NodeProlog getNodeProlog (uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		uint32_t result = tobii_sdk_param_stack_get_node_prolog(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		return NodeProlog(result);
	}

	std::string getString (uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		const char* c_str = tobii_sdk_param_stack_get_string(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		std::string result(c_str);
		tobii_sdk_free_string(c_str);
		return result;
	}

	Blob getBlob (uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		const unsigned char* c_blob = tobii_sdk_param_stack_get_blob(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);
		uint32_t blob_length = tobii_sdk_param_stack_get_blob_length(handle_.get(), index, &error);
		EyeTrackerException::throwOnError(error);

		boost::shared_array<uint8_t> result(new uint8_t[blob_length]);
		std::memcpy(result.get(), c_blob, blob_length);
		return Blob(result, blob_length);
	}

	std::vector<float> getVectorFloat(uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		boost::shared_ptr<tobii_sdk_vector_t> vec(tobii_sdk_param_stack_get_vector(handle_.get(), index, &error),
				tobii_sdk_vector_destroy);
		EyeTrackerException::throwOnError(error);

		uint32_t length = tobii_sdk_vector_get_length(vec.get(), &error);
		EyeTrackerException::throwOnError(error);

		std::vector<float> return_vec;
		for (uint32_t i = 0; i < length; ++i) {
			float f = tobii_sdk_vector_get_fixed_15x16_as_float32(vec.get(), i, &error);
			EyeTrackerException::throwOnError(error);
			return_vec.push_back(f);
		}

		return return_vec;
	}

	std::vector<std::string> getVectorString(uint32_t index)
	{
		tobii_sdk_error_t* error = 0;
		boost::shared_ptr<tobii_sdk_vector_t> vec(tobii_sdk_param_stack_get_vector(handle_.get(), index, &error),
				tobii_sdk_vector_destroy);
		EyeTrackerException::throwOnError(error);

		uint32_t length = tobii_sdk_vector_get_length(vec.get(), &error);
		EyeTrackerException::throwOnError(error);

		std::vector<std::string> return_vec;
		for (uint32_t i = 0; i < length; ++i) {
			const char* c_str = tobii_sdk_vector_get_string(vec.get(), i, &error);
			EyeTrackerException::throwOnError(error);
			return_vec.push_back(std::string(c_str));
			tobii_sdk_free_string(c_str);
		}

		return return_vec;
	}

	Point3d readPoint3d(uint32_t& index)
	{
		tobii_sdk_error_t* error = 0;
		NodeProlog prolog(tobii_sdk_param_stack_get_node_prolog(handle_.get(), index++, &error));
		EyeTrackerException::throwOnError(error);

		Point3d pt;
		pt.x = tobii_sdk_param_stack_get_fixed_22x41_as_float64(handle_.get(), index++, &error);
		pt.y = tobii_sdk_param_stack_get_fixed_22x41_as_float64(handle_.get(), index++, &error);
		pt.z = tobii_sdk_param_stack_get_fixed_22x41_as_float64(handle_.get(), index++, &error);
		EyeTrackerException::throwOnError(error);

		return pt;
	}

	Point2d readPoint2d(uint32_t& index)
	{
		tobii_sdk_error_t* error = 0;
		NodeProlog prolog(tobii_sdk_param_stack_get_node_prolog(handle_.get(), index++, &error));
		EyeTrackerException::throwOnError(error);

		Point2d pt;
		pt.x = tobii_sdk_param_stack_get_fixed_22x41_as_float64(handle_.get(), index++, &error);
		pt.y = tobii_sdk_param_stack_get_fixed_22x41_as_float64(handle_.get(), index++, &error);
		EyeTrackerException::throwOnError(error);

		return pt;
	}

private:
	boost::shared_ptr<tobii_sdk_param_stack_t> handle_;

	void clone(const tobii_sdk_param_stack_t* stack)
	{
		tobii_sdk_error_t* error = 0;
		handle_.reset(tobii_sdk_param_stack_clone(stack, &error), &tobii_sdk_param_stack_destroy);
		EyeTrackerException::throwOnError(error);
	}
};

} // namespace cpp

} // namespace sdk

} // namespace tobii

#endif  // __TOBII_SDK_CPP_PARAM_STACK_HPP__
