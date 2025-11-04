#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart.h"
#include <vector>

namespace esphome {
namespace iwr6843_flasher {

// TI mmWave Bootloader Opcodes
static const uint8_t OPCODE_ACK = 0xCC;
static const uint8_t OPCODE_NACK = 0x33;
static const uint8_t OPCODE_PING = 0x20;
static const uint8_t OPCODE_START_DOWNLOAD = 0x21;
static const uint8_t OPCODE_FILE_CLOSE = 0x22;
static const uint8_t OPCODE_GET_LAST_STATUS = 0x23;
static const uint8_t OPCODE_SEND_DATA = 0x24;
static const uint8_t OPCODE_SEND_DATA_RAM = 0x26;
static const uint8_t OPCODE_DISCONNECT = 0x27;
static const uint8_t OPCODE_ERASE = 0x28;
static const uint8_t OPCODE_FILE_ERASE = 0x2E;
static const uint8_t OPCODE_GET_VERSION = 0x2F;
static const uint8_t SYNC_PATTERN = 0xAA;
static const uint8_t OPCODE_RET_SUCCESS = 0x40;
static const uint8_t OPCODE_RET_ACCESS_IN_PROGRESS = 0x4B;

// File Types
enum FileType : uint32_t {
  RADARSS_BUILD = 0,
  CALIB_DATA = 1,
  CONFIG_INFO = 2,
  MSS_BUILD = 3,
  META_IMAGE1 = 4,
  META_IMAGE2 = 5,
  META_IMAGE3 = 6,
  META_IMAGE4 = 7
};

// Storage Types
enum StorageType : uint32_t {
  SDRAM = 0,
  FLASH = 1,
  SFLASH = 2,
  EEPROM = 3,
  SRAM = 4
};

class IWR6843Flasher : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::LATE; }

  // Configuration
  void set_firmware_url(const std::string &url) { this->firmware_url_ = url; }
  void set_reset_pin(int pin) { this->reset_pin_ = pin; }

  // Public API for flash operations
  bool start_flash_procedure(const std::vector<uint8_t> &firmware_data);
  bool send_break_signal();
  bool send_reset_signal();
  
  // Bootloader Protocol Functions
  bool connect_bootloader();
  bool send_ping();
  bool get_version(std::string &version);
  bool erase_storage(StorageType storage, uint32_t offset = 0, uint32_t capacity = 0);
  bool download_file(const std::vector<uint8_t> &data, FileType file_type, StorageType storage);
  
  // Progress callback
  void set_progress_callback(std::function<void(int)> callback) {
    this->progress_callback_ = callback;
  }
  
  // Status callback
  void set_status_callback(std::function<void(const std::string &)> callback) {
    this->status_callback_ = callback;
  }

 protected:
  // Low-level protocol functions
  bool send_packet(const std::vector<uint8_t> &data);
  bool receive_packet(std::vector<uint8_t> &payload, size_t expected_length);
  bool read_ack();
  bool send_command(const std::vector<uint8_t> &data, size_t response_len = 1);
  
  // Helper functions
  uint8_t calculate_checksum(const std::vector<uint8_t> &data);
  void report_progress(int percentage);
  void report_status(const std::string &status);
  
  // Member variables
  std::string firmware_url_;
  int reset_pin_{-1};
  bool flash_in_progress_{false};
  int progress_percentage_{0};
  
  // Callbacks
  std::function<void(int)> progress_callback_;
  std::function<void(const std::string &)> status_callback_;
  
  // Constants
  static const size_t CHUNK_SIZE = 240;
  static const uint32_t DEFAULT_TIMEOUT_MS = 2000;
  static const uint32_t ACK_TIMEOUT_MS = 1000;
};

}  // namespace iwr6843_flasher
}  // namespace esphome

