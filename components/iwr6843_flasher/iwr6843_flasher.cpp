#include "iwr6843_flasher.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include <cinttypes>

namespace esphome {
namespace iwr6843_flasher {

static const char *const TAG = "iwr6843_flasher";

void IWR6843Flasher::setup() {
  ESP_LOGCONFIG(TAG, "Setting up IWR6843 Flasher...");
}

void IWR6843Flasher::loop() {
  // Nothing to do in loop for now
}

void IWR6843Flasher::dump_config() {
  ESP_LOGCONFIG(TAG, "IWR6843 Flasher:");
  ESP_LOGCONFIG(TAG, "  Firmware URL: %s", this->firmware_url_.c_str());
  if (this->reset_pin_ >= 0) {
    ESP_LOGCONFIG(TAG, "  Reset Pin: GPIO%d", this->reset_pin_);
  }
}

bool IWR6843Flasher::send_break_signal() {
  ESP_LOGI(TAG, "Sending UART break signal...");
  
  // Implementation depends on hardware - typically:
  // 1. Set TX line LOW for 150ms
  // 2. Return TX line to HIGH
  // 3. Restore UART control
  
  // This would need platform-specific code (ESP32)
  // For now, log the attempt
  ESP_LOGW(TAG, "Break signal implementation is platform-specific");
  
  return true;
}

bool IWR6843Flasher::send_reset_signal() {
  if (this->reset_pin_ < 0) {
    ESP_LOGW(TAG, "Reset pin not configured");
    return false;
  }
  
  ESP_LOGI(TAG, "Sending reset signal on GPIO%d...", this->reset_pin_);
  
  // Pull reset pin LOW for 100ms, then HIGH
  // Implementation depends on ESPHome GPIO abstraction
  
  return true;
}

uint8_t IWR6843Flasher::calculate_checksum(const std::vector<uint8_t> &data) {
  uint32_t checksum = 0;
  for (uint8_t byte : data) {
    checksum += byte;
  }
  return checksum & 0xFF;
}

bool IWR6843Flasher::send_packet(const std::vector<uint8_t> &data) {
  ESP_LOGV(TAG, "Sending packet of %zu bytes", data.size());
  
  // Calculate checksum
  uint8_t checksum = calculate_checksum(data);
  
  // Message size = data + 2 (for checksum + sync)
  uint16_t msg_size = data.size() + 2;
  
  // Build packet: SYNC + SIZE(2 bytes, big-endian) + CHECKSUM + DATA
  std::vector<uint8_t> packet;
  packet.push_back(SYNC_PATTERN);
  packet.push_back((msg_size >> 8) & 0xFF);  // Size high byte
  packet.push_back(msg_size & 0xFF);          // Size low byte
  packet.push_back(checksum);
  packet.insert(packet.end(), data.begin(), data.end());
  
  // Send via UART
  this->write_array(packet.data(), packet.size());
  this->flush();
  
  return true;
}

bool IWR6843Flasher::receive_packet(std::vector<uint8_t> &payload, size_t expected_length) {
  ESP_LOGV(TAG, "Receiving packet, expecting %zu bytes", expected_length);
  
  // Read header: 3 bytes (2 for length, 1 for checksum)
  uint8_t header[3];
  uint32_t start_time = millis();
  size_t bytes_read = 0;
  
  while (bytes_read < 3 && (millis() - start_time) < DEFAULT_TIMEOUT_MS) {
    if (this->available()) {
      header[bytes_read++] = this->read();
    }
    delay(1);
  }
  
  if (bytes_read < 3) {
    ESP_LOGW(TAG, "Timeout reading packet header");
    return false;
  }
  
  // Parse length and checksum
  uint16_t packet_length = (header[0] << 8) | header[1];
  uint8_t received_checksum = header[2];
  packet_length -= 2;  // Subtract header size
  
  if (expected_length != packet_length) {
    ESP_LOGW(TAG, "Length mismatch: expected %zu, got %u", expected_length, packet_length);
  }
  
  // Read payload
  payload.clear();
  payload.reserve(packet_length);
  
  start_time = millis();
  while (payload.size() < packet_length && (millis() - start_time) < DEFAULT_TIMEOUT_MS) {
    if (this->available()) {
      payload.push_back(this->read());
    }
    delay(1);
  }
  
  if (payload.size() < packet_length) {
    ESP_LOGW(TAG, "Timeout reading payload: got %zu/%u bytes", payload.size(), packet_length);
    return false;
  }
  
  // Verify checksum
  uint8_t calculated_checksum = calculate_checksum(payload);
  if (calculated_checksum != received_checksum) {
    ESP_LOGW(TAG, "Checksum mismatch: calculated 0x%02X, received 0x%02X", 
             calculated_checksum, received_checksum);
    return false;
  }
  
  // Send ACK
  this->write(OPCODE_ACK);
  this->flush();
  
  ESP_LOGV(TAG, "Packet received successfully, %zu bytes", payload.size());
  return true;
}

bool IWR6843Flasher::read_ack() {
  ESP_LOGV(TAG, "Waiting for ACK...");
  
  uint32_t start_time = millis();
  std::vector<uint8_t> buffer;
  
  // Read until we get ACK or NACK (or timeout)
  while ((millis() - start_time) < ACK_TIMEOUT_MS) {
    if (this->available()) {
      uint8_t byte = this->read();
      buffer.push_back(byte);
      
      // Check for ACK or NACK
      if (byte == OPCODE_ACK) {
        ESP_LOGV(TAG, "Received ACK");
        return true;
      } else if (byte == OPCODE_NACK) {
        ESP_LOGW(TAG, "Received NACK");
        return false;
      }
    }
    delay(1);
  }
  
  ESP_LOGW(TAG, "ACK timeout");
  return false;
}

bool IWR6843Flasher::send_command(const std::vector<uint8_t> &data, size_t response_len) {
  ESP_LOGV(TAG, "Sending command...");
  
  // Send packet
  if (!send_packet(data)) {
    return false;
  }
  
  // Wait for ACK
  if (!read_ack()) {
    return false;
  }
  
  // Send GET_LAST_STATUS command
  std::vector<uint8_t> status_cmd = {OPCODE_GET_LAST_STATUS};
  send_packet(status_cmd);
  
  // Receive status response
  std::vector<uint8_t> status_payload;
  if (!receive_packet(status_payload, response_len)) {
    return false;
  }
  
  return true;
}

bool IWR6843Flasher::send_ping() {
  ESP_LOGI(TAG, "Sending PING to bootloader...");
  
  std::vector<uint8_t> ping_data = {OPCODE_PING};
  return send_command(ping_data);
}

bool IWR6843Flasher::get_version(std::string &version) {
  ESP_LOGI(TAG, "Getting bootloader version...");
  
  std::vector<uint8_t> version_data = {OPCODE_GET_VERSION};
  
  if (!send_packet(version_data)) {
    return false;
  }
  
  if (!read_ack()) {
    return false;
  }
  
  // Read version data packet
  std::vector<uint8_t> version_payload;
  if (!receive_packet(version_payload, 12)) {  // Typical version response is 12 bytes
    return false;
  }
  
  // Convert version to hex string
  version = format_hex_pretty(version_payload.data(), version_payload.size());
  ESP_LOGI(TAG, "Bootloader version: %s", version.c_str());
  
  return true;
}

bool IWR6843Flasher::erase_storage(StorageType storage, uint32_t offset, uint32_t capacity) {
  ESP_LOGI(TAG, "Erasing storage (type=%u, offset=%" PRIu32 ", capacity=%" PRIu32 ")...", 
           storage, offset, capacity);
  
  report_status("Erasing flash...");
  
  std::vector<uint8_t> erase_data;
  erase_data.push_back(OPCODE_ERASE);
  
  // Add storage type (4 bytes, big-endian)
  erase_data.push_back((storage >> 24) & 0xFF);
  erase_data.push_back((storage >> 16) & 0xFF);
  erase_data.push_back((storage >> 8) & 0xFF);
  erase_data.push_back(storage & 0xFF);
  
  // Add offset (4 bytes, big-endian)
  erase_data.push_back((offset >> 24) & 0xFF);
  erase_data.push_back((offset >> 16) & 0xFF);
  erase_data.push_back((offset >> 8) & 0xFF);
  erase_data.push_back(offset & 0xFF);
  
  // Add capacity (4 bytes, big-endian)
  erase_data.push_back((capacity >> 24) & 0xFF);
  erase_data.push_back((capacity >> 16) & 0xFF);
  erase_data.push_back((capacity >> 8) & 0xFF);
  erase_data.push_back(capacity & 0xFF);
  
  if (!send_packet(erase_data)) {
    ESP_LOGE(TAG, "Failed to send erase command");
    return false;
  }
  
  if (!read_ack()) {
    ESP_LOGE(TAG, "Erase command not acknowledged");
    return false;
  }
  
  ESP_LOGI(TAG, "Storage erased successfully");
  report_status("Erase complete");
  return true;
}

bool IWR6843Flasher::download_file(const std::vector<uint8_t> &data, FileType file_type, StorageType storage) {
  ESP_LOGI(TAG, "Starting file download: %zu bytes, file_type=%u, storage=%u", 
           data.size(), file_type, storage);
  
  report_status("Starting download...");
  report_progress(0);
  
  // Send START_DOWNLOAD command
  std::vector<uint8_t> start_download;
  start_download.push_back(OPCODE_START_DOWNLOAD);
  
  // File size (4 bytes, big-endian)
  uint32_t file_size = data.size();
  start_download.push_back((file_size >> 24) & 0xFF);
  start_download.push_back((file_size >> 16) & 0xFF);
  start_download.push_back((file_size >> 8) & 0xFF);
  start_download.push_back(file_size & 0xFF);
  
  // Storage type (4 bytes, big-endian)
  start_download.push_back((storage >> 24) & 0xFF);
  start_download.push_back((storage >> 16) & 0xFF);
  start_download.push_back((storage >> 8) & 0xFF);
  start_download.push_back(storage & 0xFF);
  
  // File type (4 bytes, big-endian)
  start_download.push_back((file_type >> 24) & 0xFF);
  start_download.push_back((file_type >> 16) & 0xFF);
  start_download.push_back((file_type >> 8) & 0xFF);
  start_download.push_back(file_type & 0xFF);
  
  // Mirror enabled (4 bytes, 0 = disabled)
  start_download.push_back(0);
  start_download.push_back(0);
  start_download.push_back(0);
  start_download.push_back(0);
  
  if (!send_command(start_download)) {
    ESP_LOGE(TAG, "START_DOWNLOAD failed");
    return false;
  }
  
  // Send data in chunks
  size_t offset = 0;
  size_t total_chunks = (data.size() + CHUNK_SIZE - 1) / CHUNK_SIZE;
  size_t chunk_num = 0;
  
  while (offset < data.size()) {
    size_t chunk_size = std::min(CHUNK_SIZE, data.size() - offset);
    
    std::vector<uint8_t> chunk_data;
    chunk_data.push_back(storage == SRAM ? OPCODE_SEND_DATA_RAM : OPCODE_SEND_DATA);
    chunk_data.insert(chunk_data.end(), data.begin() + offset, data.begin() + offset + chunk_size);
    
    if (!send_command(chunk_data, storage == SRAM ? 4 : 1)) {
      ESP_LOGE(TAG, "Failed to send chunk at offset %zu", offset);
      return false;
    }
    
    offset += chunk_size;
    chunk_num++;
    
    // Report progress
    int progress = (chunk_num * 100) / total_chunks;
    report_progress(progress);
    
    if (chunk_num % 10 == 0) {
      ESP_LOGI(TAG, "Progress: %zu/%zu chunks (%d%%)", chunk_num, total_chunks, progress);
    }
  }
  
  // Send FILE_CLOSE command
  std::vector<uint8_t> close_data;
  close_data.push_back(OPCODE_FILE_CLOSE);
  
  // File type (4 bytes, big-endian)
  close_data.push_back((file_type >> 24) & 0xFF);
  close_data.push_back((file_type >> 16) & 0xFF);
  close_data.push_back((file_type >> 8) & 0xFF);
  close_data.push_back(file_type & 0xFF);
  
  if (!send_command(close_data)) {
    ESP_LOGE(TAG, "FILE_CLOSE failed");
    return false;
  }
  
  ESP_LOGI(TAG, "File download completed successfully");
  report_status("Download complete");
  report_progress(100);
  
  return true;
}

bool IWR6843Flasher::connect_bootloader() {
  ESP_LOGI(TAG, "Connecting to bootloader...");
  
  report_status("Connecting...");
  
  // Flush UART
  while (this->available()) {
    this->read();
  }
  
  // Try to ping bootloader
  for (int attempt = 0; attempt < 3; attempt++) {
    if (send_ping()) {
      ESP_LOGI(TAG, "Bootloader connected successfully");
      return true;
    }
    ESP_LOGW(TAG, "Ping attempt %d failed, retrying...", attempt + 1);
    delay(500);
  }
  
  ESP_LOGE(TAG, "Failed to connect to bootloader");
  return false;
}

bool IWR6843Flasher::start_flash_procedure(const std::vector<uint8_t> &firmware_data) {
  if (this->flash_in_progress_) {
    ESP_LOGW(TAG, "Flash already in progress");
    return false;
  }
  
  this->flash_in_progress_ = true;
  ESP_LOGI(TAG, "=== Starting IWR6843 Flash Procedure ===");
  ESP_LOGI(TAG, "Firmware size: %zu bytes", firmware_data.size());
  
  // Step 1: Connect to bootloader
  report_status("Connecting to bootloader...");
  if (!connect_bootloader()) {
    ESP_LOGE(TAG, "Failed to connect to bootloader");
    this->flash_in_progress_ = false;
    return false;
  }
  
  // Step 2: Get version
  std::string version;
  if (get_version(version)) {
    ESP_LOGI(TAG, "Bootloader version: %s", version.c_str());
  }
  
  // Step 3: Erase flash
  report_status("Erasing flash...");
  if (!erase_storage(SFLASH, 0, 0)) {
    ESP_LOGE(TAG, "Flash erase failed");
    this->flash_in_progress_ = false;
    return false;
  }
  
  // Step 4: Download firmware
  report_status("Flashing firmware...");
  if (!download_file(firmware_data, META_IMAGE1, SFLASH)) {
    ESP_LOGE(TAG, "Firmware download failed");
    this->flash_in_progress_ = false;
    return false;
  }
  
  ESP_LOGI(TAG, "=== Flash Procedure Completed Successfully ===");
  report_status("Flash complete!");
  this->flash_in_progress_ = false;
  
  return true;
}

void IWR6843Flasher::report_progress(int percentage) {
  this->progress_percentage_ = percentage;
  if (this->progress_callback_) {
    this->progress_callback_(percentage);
  }
}

void IWR6843Flasher::report_status(const std::string &status) {
  if (this->status_callback_) {
    this->status_callback_(status);
  }
}

}  // namespace iwr6843_flasher
}  // namespace esphome

