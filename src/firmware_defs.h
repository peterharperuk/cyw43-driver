/*
 * This file is part of the cyw43-driver
 *
 * Copyright (C) 2019-2022 George Robotics Pty Ltd
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Any redistribution, use, or modification in source or binary form is done
 *    solely for personal benefit and not for any commercial purpose or for
 *    monetary gain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE LICENSOR AND COPYRIGHT OWNER "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE LICENSOR OR COPYRIGHT OWNER BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This software is also available for use with certain devices under different
 * terms, as set out in the top level LICENSE file.  For commercial licensing
 * options please email contact@georgerobotics.com.au.
 */

#ifndef FIRMWARE_DEFS_H
#define FIRMWARE_DEFS_H

/*!
 * \brief Structure to store firmware details
 */
//!\{
typedef struct cyw43_firmware_details {
    size_t raw_wifi_fw_size;    ///< Size in bytes of the wifi firmware data before extraction
    const uint8_t *raw_data;    ///< Pointer to the wifi firmware data before extraction
    size_t wifi_fw_size;        ///< Size of the wifi firmware in bytes after extraction
    size_t clm_size;            ///< Size of the clm blob in bytes after extraction
    const uint8_t *wifi_fw_addr;///< Pointer to the raw wifi firmware
    const uint8_t *clm_addr;    ///< Pointer to the raw clm blob in uncompressed firmware
    size_t wifi_nvram_len;      ///< Size of nvram data
    const uint8_t *wifi_nvram_data; ///< Pointer to nvram data
    #if CYW43_ENABLE_BLUETOOTH
    size_t raw_bt_fw_size;      ///< size of bluetooth firmware data before extraction
    size_t bt_fw_size;          ///< size of bluetooth firmware data after extraction
    const uint8_t *bt_fw_addr;  ///< Pointer to the bluetooth firmware
    #endif
} cyw43_firmware_details_t;
//!\}

extern cyw43_firmware_details_t firmware_details;

/*!
 * \brief Structure to hold function pointers for loading firmware
 */
//!\{
typedef struct cyw43_firmware_funcs {
    int (*start_wifi_fw)(const cyw43_firmware_details_t *fw_details); ///< start wifi firmware loading
    int (*start_bt_fw)(const cyw43_firmware_details_t *fw_details); ///< start bt firmware loading
    const uint8_t* (*get_wifi_fw)(const uint8_t *addr, size_t sz_in, uint8_t *buffer, size_t buffer_len); ///< get block of wifi firmware data
    const uint8_t* (*get_bt_fw)(const uint8_t *addr, size_t sz_in, uint8_t *buffer, size_t buffer_len); ///< get block of bt firmware data
    const uint8_t* (*get_nvram)(const uint8_t *addr, size_t sz_in, uint8_t *buffer, size_t buffer_len); ///< get block of nvram data
    int (*copy_clm)(uint8_t *dst, const uint8_t *src, uint32_t len); ///< copy clm data
    void (*end)(void); ///< end firmware loading
} cyw43_firmware_funcs_t;
//!\}

/*!
 * \brief get firmware data from flash
 *
 * Loads firmware data from flash and returns a pointer to it
 *
 * \param addr Address of firmware data required
 * \param sz_in Amount of firmware data required in bytes
 * \param buffer Temporary buffer that can be used to load and return firmware data
 * \param buffer_len Length of temporary buffer in bytes
 * \return Requested firmware data
 */
const uint8_t *wifi_firmware_get_storage(const uint8_t *addr, size_t sz_in, uint8_t *buffer, size_t buffer_len);

/*!
 * \brief get firmware data embedded in the elf file binary
 *
 * Loads firmware data from the elf file and returns a pointer to it
 *
 * \param addr Address of firmware data required
 * \param sz_in Amount of firmware data required in bytes
 * \param buffer Temporary buffer that can be used to load and return firmware data
 * \param buffer_len Length of temporary buffer in bytes
 * \return Requested firmware data
 */
const uint8_t *cyw43_firmware_embedded_get(const uint8_t *addr, size_t sz_in, uint8_t *buffer, size_t buffer_len);

/*!
 * \brief get a copy of firmware data embedded in the elf file binary
 *
 * Loads firmware and copies it to the supplied buffer
 *
 * \param dst Required destination of firmare data
 * \param src Source address of the firmware data
 * \param len Amount of data to be copied in bytes
 * \return >=0 on success or <0 on error
 */
int cyw43_firmware_copy_embedded(uint8_t *dst, const uint8_t *src, uint32_t len);

/*!
 * \brief Start wifi firmware decompression process
 *
 * Prepares and allocates resources needed to decompress firmware
 *
 * \param fw_details Details of the firmware
 * \see cyw43_firmware_details_t
 * \return >=0 on success or <0 on error
 */
int cyw43_wifi_firmware_decompress_start(const cyw43_firmware_details_t* fw_details);

/*!
 * \brief Start bt firmware decompression process
 *
 * Prepares and allocates resources needed to decompress firmware
 *
 * \param fw_details Details of the firmware
 * \see cyw43_firmware_details_t
 * \return >=0 on success or <0 on error
 */
int cyw43_bt_firmware_decompress_start(const cyw43_firmware_details_t* fw_details);

/*!
 * \brief get and decompress firmware data embedded in the elf file binary
 *
 * Loads firmware data from the elf file, decompressed it and returns a pointer to it
 *
 * \param addr Address of firmware data required
 * \param sz_in Amount of firmware data required in bytes
 * \param buffer Temporary buffer that can be used to load and return firmware data
 * \param buffer_len Length of temporary buffer in bytes
 * \return Requested firmware data
 */
const uint8_t *cyw43_firmware_decompress_get(const uint8_t *addr, size_t sz_in, uint8_t *buffer, size_t buffer_len);

/*!
 * \brief Decompress the clm data embedded in the elf file binary and copy it to the supplied buffer
 *
 * Loads clm data from flash, decompresses it and copies it to the supplied buffer
 *
 * \param dst Destination of clm data
 * \param src Source address of the clm data
 * \param len Amount of data required in bytes
 * \return >=0 on success or <0 on error
 */
int cyw43_firmware_decompress_copy(uint8_t *dst, const uint8_t *src, uint32_t len);

/*!
 * \brief End firmware decompression process
 *
 * Frees resources used to decompress firmware
 *
 */
void cyw43_firmware_decompress_end(void);

#endif
