#include <gtest/gtest.h>

#include <c10/DeviceGuard.h>

using namespace c10;

// The tests here are mostly covered by InlineDeviceGuard_test, but there
// is some DeviceGuard specific functionality we must test.

// -- DeviceGuard -------------------------------------------------------

TEST(DeviceGuard, ResetDeviceDifferentDeviceType) {
  FakeGuardImpl<DeviceType::CUDA> cuda_impl;
  FakeGuardImpl<DeviceType::HIP> hip_impl;
  FakeGuardImpl<DeviceType::CUDA>::setDeviceIndex(0);
  FakeGuardImpl<DeviceType::HIP>::setDeviceIndex(0);
  DeviceGuard g(Device(DeviceType::CUDA, 1), &cuda_impl);
  g.reset_device(Device(DeviceType::HIP, 2), &hip_impl);
  ASSERT_EQ(FakeGuardImpl<DeviceType::CUDA>::getDeviceIndex(), 0);
  ASSERT_EQ(FakeGuardImpl<DeviceType::HIP>::getDeviceIndex(), 2);
  ASSERT_EQ(g.current_device(), Device(DeviceType::HIP, 2));
  ASSERT_EQ(g.original_device(), Device(DeviceType::HIP, 0));
}

// -- OptionalDeviceGuard -----------------------------------------------

TEST(DeviceGuard, ResetDeviceDifferentDeviceType) {
  FakeGuardImpl<DeviceType::CUDA> cuda_impl;
  FakeGuardImpl<DeviceType::HIP> hip_impl;
  FakeGuardImpl<DeviceType::CUDA>::setDeviceIndex(0);
  FakeGuardImpl<DeviceType::HIP>::setDeviceIndex(0);
  OptionalDeviceGuard g;
  g.reset_device(Device(DeviceType::CUDA, 1), &cuda_impl);
  g.reset_device(Device(DeviceType::HIP, 2), &hip_impl);
  ASSERT_EQ(FakeGuardImpl<DeviceType::CUDA>::getDeviceIndex(), 0);
  ASSERT_EQ(FakeGuardImpl<DeviceType::HIP>::getDeviceIndex(), 2);
  ASSERT_EQ(g.current_device(), make_optional(Device(DeviceType::HIP, 2)));
  ASSERT_EQ(g.original_device(), make_optional(Device(DeviceType::HIP, 0)));
}
