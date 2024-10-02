// ArduinoNvs.cpp

// Copyright (c) 2018 Sinai RnD
// Copyright (c) 2016-2017 TridentTD

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "ArduinoNvs.h"

ArduinoNvs::ArduinoNvs()
{
  _nvs_namespace[16] = {0};
  nvs_iterator_t it = NULL;
  // Constructor implementation
  printf( " setting up ArduinoNvs\n" );
}

esp_err_t ArduinoNvs::_init(nvs_sec_cfg_t *keys)
{
  // If encryption is supported - make additional moves for retrieving keys:
  // - try to use user-provided keys if any, OR
  // - check is keypartition present, if no - as a last hope - try to open NVS in non-encrypted mode
  #ifdef CONFIG_NVS_ENCRYPTION
  bool noKeyPartition = ( NULL == esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS_KEYS, NULL) );

  if (keys)
  {
    return nvs_flash_secure_init(keys);
  }
  else if (noKeyPartition)    
  {
    DEBUG_PRINTLN("W: You are trying to open Non-encrypted NVS on Encryption-enabled system. Maybe something is miscofigured");
    return nvs_flash_init_partition(NVS_DEFAULT_PART_NAME);
  }  
  #endif

  // in all other cases use usual init process
  return nvs_flash_init();
}

bool ArduinoNvs::begin(String namespaceNvs) 
{
  return begin(namespaceNvs, NULL);
}

bool ArduinoNvs::begin(String namespaceNvs, nvs_sec_cfg_t *keys)
{  
  // set namespace
  strcpy(_nvs_namespace, namespaceNvs.c_str());
  esp_err_t err = _init(keys);
  if (err != ESP_OK)
  {
    DEBUG_PRINTLN("W: NVS. Cannot init flash mem");
    if (err != ESP_ERR_NVS_NO_FREE_PAGES)
      return false;

    // erase and reinit
    DEBUG_PRINTLN("NVS. Try reinit the partition");
    err = format();
    if (err)
      return false;
    err = _init(keys);
    if (err)
      return false;
    DEBUG_PRINTLN("NVS. Partition re-formatted");
  }

  err = nvs_open(_nvs_namespace, NVS_READWRITE, &_nvs_handle);

  return err == ESP_OK;
}

bool ArduinoNvs::format() {
  const esp_partition_t *nvs_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
  if (nvs_partition == NULL) {
    DEBUG_PRINTLN("E: NVS. No NVS partition");
    return false;
  }
  esp_err_t err = esp_partition_erase_range(nvs_partition, 0, nvs_partition->size);
  if (err != ESP_OK)
    DEBUG_PRINTF("E: NVS. Cannot format the partition [%d]\n", err);
  return err == ESP_OK;
}

bool ArduinoNvs::close(bool deinit_partition)
{
  nvs_close(_nvs_handle);
  _nvs_handle = nullptr;
  if (deinit_partition == false)
    return true;
  
  // deinit parttion if needed  
  esp_err_t err = nvs_flash_deinit();
  if (err != ESP_OK)
    DEBUG_PRINTF("W: NVS. Cannot deinit the partition [%d]\n", err);
  
  return err == ESP_OK;
}

bool ArduinoNvs::eraseAll(bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_erase_all(_nvs_handle);
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}

bool ArduinoNvs::erase(String key, bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_erase_key(_nvs_handle, key.c_str());
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}

bool ArduinoNvs::commit()
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_commit(_nvs_handle);

  return err == ESP_OK;
}

bool ArduinoNvs::setInt(String key, uint8_t value, bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_set_u8(_nvs_handle, (char *)key.c_str(), value);
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}

bool ArduinoNvs::setInt(String key, int16_t value, bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_set_i16(_nvs_handle, (char *)key.c_str(), value);
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}

bool ArduinoNvs::setInt(String key, uint16_t value, bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_set_u16(_nvs_handle, (char *)key.c_str(), value);
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}

bool ArduinoNvs::setInt(String key, int32_t value, bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_set_i32(_nvs_handle, (char *)key.c_str(), value);
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}

bool ArduinoNvs::setInt(String key, uint32_t value, bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_set_u32(_nvs_handle, (char *)key.c_str(), value);
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}
bool ArduinoNvs::setInt(String key, int64_t value, bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_set_i64(_nvs_handle, (char *)key.c_str(), value);
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}

bool ArduinoNvs::setInt(String key, uint64_t value, bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  esp_err_t err = nvs_set_u64(_nvs_handle, (char *)key.c_str(), value);
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}

bool ArduinoNvs::setString(String key, String str, bool forceCommit)
{
  if( !_nvs_handle || key == "" ) return false;
  if( str.length()>3999 ){
    DEBUG_PRINTF("NVS limits strings to 4000 bytes max!");
    return false;
  }
  esp_err_t err = nvs_set_str(_nvs_handle, (char *)key.c_str(), str.c_str());
  if (err != ESP_OK)
    return false;
  return forceCommit ? commit() : true;
}

// setBlob("name",&data[3], 5); uint8_t data[10] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 ... }
bool ArduinoNvs::setBlob(String key, uint8_t *blob, size_t length, bool forceCommit)
{
  DEBUG_PRINTF("ArduinoNvs::setObject(): set obj addr = [0x%X], length = [%d]\n", (int32_t)blob, length);
  if (length == 0) return false;
  if (length > 508000 ){
    DEBUG_PRINTF("NVS limits blobs to 508000 bytes (or 98% of partition size)");
    return false;
  }
  esp_err_t err = nvs_set_blob(_nvs_handle, (char *)key.c_str(), blob, length);
  if (err)
  {
    DEBUG_PRINTF("ArduinoNvs::setObjct(): err = [0x%X]\n", err);
    return false;
  }
  return forceCommit ? commit() : true;
}

// setBlob("name",data); std::vector<uint8_t> data = { 0x01, 0x02, 0x03 }
bool ArduinoNvs::setBlob(String key, std::vector<uint8_t> &blob, bool forceCommit)
{
  return setBlob(key, blob.data(), blob.size(), forceCommit);
}

// setBlob("name", { 0x01, 0x02, 0x03 })
bool ArduinoNvs::setBlob(String key, std::initializer_list<uint8_t> blob, bool forceCommit) 
{
    size_t len = blob.size();
    uint8_t buffer[len] = {0};
    std::copy(blob.begin(), blob.end(), buffer);
    return setBlob(key, buffer, len, forceCommit);
}

int64_t ArduinoNvs::getInt(String key, int64_t default_value)
{
  if( !_nvs_handle || key == "" ) return false;

  uint8_t v_u8;
  int16_t v_i16;
  uint16_t v_u16;
  int32_t v_i32;
  uint32_t v_u32;
  int64_t v_i64;
  uint64_t v_u64;

  esp_err_t err;
  err = nvs_get_u8(_nvs_handle, (char *)key.c_str(), &v_u8);
  if (err == ESP_OK)
    return (int64_t)v_u8;

  err = nvs_get_i16(_nvs_handle, (char *)key.c_str(), &v_i16);
  if (err == ESP_OK)
    return (int64_t)v_i16;

  err = nvs_get_u16(_nvs_handle, (char *)key.c_str(), &v_u16);
  if (err == ESP_OK)
    return (int64_t)v_u16;

  err = nvs_get_i32(_nvs_handle, (char *)key.c_str(), &v_i32);
  if (err == ESP_OK)
    return (int64_t)v_i32;

  err = nvs_get_u32(_nvs_handle, (char *)key.c_str(), &v_u32);
  if (err == ESP_OK)
    return (int64_t)v_u32;

  err = nvs_get_i64(_nvs_handle, (char *)key.c_str(), &v_i64);
  if (err == ESP_OK)
    return (int64_t)v_i64;

  err = nvs_get_u64(_nvs_handle, (char *)key.c_str(), &v_u64);
  if (err == ESP_OK)
    return (int64_t)v_u64;

  return default_value;
}

bool ArduinoNvs::getString(String key, String &res)
{
  if( !_nvs_handle || key == "" ) return false;

  size_t required_size;
  esp_err_t err;

  err = nvs_get_str(_nvs_handle, key.c_str(), NULL, &required_size);
  if (err)
    return false;

  char value[required_size];
  err = nvs_get_str(_nvs_handle, key.c_str(), value, &required_size);
  if (err)
    return false;
  res = value;
  return true;
}

String  ArduinoNvs::getString(String key, const char* default_value) {
  String res;
  bool ok = getString(key, res);
  
  return !ok ? String(default_value) : res;
}

size_t  ArduinoNvs::getBlobSize(String key)
{
  return getBlob(key,nullptr);
}

size_t ArduinoNvs::getBlob(String key, uint8_t *blob)
{
  if( !_nvs_handle || key == "" ) return false;

  size_t required_size; // do NOT assign a value or it will fail!
  esp_err_t err = nvs_get_blob(_nvs_handle, key.c_str(), blob, &required_size);
  if (err) return 0;

  return required_size;
}

std::vector<uint8_t> ArduinoNvs::getBlob(String key)
{
  std::vector<uint8_t> blob_vector = {};
  if( !_nvs_handle || key == "" ) return blob_vector;

  size_t blob_size = getBlobSize(key); // need to resize vector
  if( blob_size>0 ){
    blob_vector.resize(blob_size);
    size_t len = getBlob( key, blob_vector.data() );
    if( len == 0 ) blob_vector.clear();
  }
  return blob_vector;
}

bool ArduinoNvs::setFloat(String key, float value, bool forceCommit)
{
  return setBlob(key, (uint8_t *)&value, sizeof(float), forceCommit);
}

float ArduinoNvs::getFloat(String key, float default_value)
{
  std::vector<uint8_t> res(sizeof(float));
  if (!getBlob(key, res))
    return default_value;
  return *(float *)(&res[0]);
}

bool ArduinoNvs::exists(String key){
  uint8_t   v_u8;
  int16_t   v_i16;
  uint16_t  v_u16;
  int32_t   v_i32;
  uint32_t  v_u32;
  int64_t   v_i64;
  uint64_t  v_u64;
  esp_err_t _err;

  _err = nvs_get_u8(_nvs_handle, (char*)key.c_str(), &v_u8);
  if(_err == ESP_OK) return true;

  _err = nvs_get_i16(_nvs_handle, (char*)key.c_str(), &v_i16);
  if(_err == ESP_OK) return true;

  _err = nvs_get_u16(_nvs_handle, (char*)key.c_str(), &v_u16);
  if(_err == ESP_OK) return true;

  _err = nvs_get_i32(_nvs_handle, (char*)key.c_str(), &v_i32);
  if(_err == ESP_OK) return true;

  _err = nvs_get_u32(_nvs_handle, (char*)key.c_str(), &v_u32);
  if(_err == ESP_OK) return true;

  _err = nvs_get_i64(_nvs_handle, (char*)key.c_str(), &v_i64);
  if(_err == ESP_OK) return true;

  _err = nvs_get_u64(_nvs_handle, (char*)key.c_str(), &v_u64);
  if(_err == ESP_OK) return true;

  size_t required_size;
  _err = nvs_get_str(_nvs_handle, key.c_str(), NULL, &required_size);
  if(_err == ESP_OK) return true;

  _err = nvs_get_blob(_nvs_handle, key.c_str(), NULL, &required_size);
  if(_err == ESP_OK) return true;

  return false;
}

uint8_t ArduinoNvs::stats(){
  nvs_stats_t nvs_stats;
  nvs_get_stats(NULL, &nvs_stats);
  printf("Count: UsedEntries = (%lu), FreeEntries = (%lu), AllEntries = (%lu)\n",
        nvs_stats.used_entries, nvs_stats.free_entries, nvs_stats.total_entries);
  return nvs_stats.used_entries;
}

std::vector<String> ArduinoNvs::entryFind(String search_key) {
  // may not work till esp-idf is up to 5.x.x *sigh*
  std::vector<String> matches;
  nvs_iterator_t it = nvs_entry_find(_nvs_namespace, search_key.c_str(), NVS_TYPE_ANY);

  if (it == NULL) {
    printf("Error: No entries found for '%s'\n", search_key.c_str());
  } else {
    while (it != NULL) {
      nvs_entry_info_t info;
      nvs_entry_info(it, &info);
      printf("key '%s', type '%d' \n", info.key, info.type);
      matches.push_back(info.key);
      it = nvs_entry_next(it);
    }
  }

  nvs_release_iterator(it);
  return matches;
}


ArduinoNvs NVS;
