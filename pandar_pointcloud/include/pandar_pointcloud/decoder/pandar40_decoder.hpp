#pragma once

#include <array>
#include "pandar_pointcloud/calibration.hpp"
#include "packet_decoder.hpp"
#include "pandar40.hpp"

static const float ROTATION_RESOLUTION = 0.001f;
static const uint32_t ROTATION_MAX_UNITS = 360000u;

namespace pandar_pointcloud
{
namespace pandar40
{
class Pandar40Decoder : public PacketDecoder
{
public:
  enum class ReturnMode : int8_t
  {
    DUAL,
    STRONGEST,
    LAST,
  };

  Pandar40Decoder(Calibration& calibration, float scan_phase = 0.0f, ReturnMode return_mode = ReturnMode::DUAL);
  void unpack(const pandar_msgs::msg::PandarPacket& raw_packet) override;
  bool hasScanned() override;
  PointcloudXYZIRADT getPointcloud() override;

private:
  bool parsePacket(const pandar_msgs::msg::PandarPacket& raw_packet);
  PointcloudXYZIRADT convert(const size_t block_id);
  PointcloudXYZIRADT convert_dual(const size_t block_id);

  std::array<float, LASER_COUNT> elev_angle_;
  std::array<float, LASER_COUNT> azimuth_offset_;

  std::array<float, LASER_COUNT> firing_offset_;
  std::array<float, BLOCKS_PER_PACKET> block_offset_single_;
  std::array<float, BLOCKS_PER_PACKET> block_offset_dual_;

  std::array<size_t, LASER_COUNT> firing_order_;

  ReturnMode return_mode_;
  Packet packet_;

  PointcloudXYZIRADT scan_pc_;
  PointcloudXYZIRADT overflow_pc_;

  uint16_t scan_phase_;
  int last_phase_;
  bool has_scanned_;

  double sin_rot_table_[ROTATION_MAX_UNITS];
  double cos_rot_table_[ROTATION_MAX_UNITS];

};

}  // namespace pandar40
}  // namespace pandar_pointcloud