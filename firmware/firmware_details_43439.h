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

#ifndef FIRMWARE_DETAILS_43439_H
#define FIRMWARE_DETAILS_43439_H

#include CYW43_CHIPSET_FIRMWARE_INCLUDE_FILE // actually just wifi firmware
#include CYW43_WIFI_NVRAM_INCLUDE_FILE

#if CYW43_ENABLE_BLUETOOTH
#include CYW43_BT_FIRMWARE_INCLUDE_FILE
#endif

#include "firmware_defs.h"

/*!
 * \brief Get the firmware binary details
 *
 * This method returns the details of the firmware binary
 *
 * \param firmware_details Structure to be filled with firmware details
 * \see cyw43_firmware_details_t
 */
static inline void cyw43_firmware_details(cyw43_firmware_details_t *firmware_details) {
    firmware_details->raw_wifi_fw_size = fw_data_len;
    firmware_details->raw_data = fw_data;
    firmware_details->wifi_fw_size = CYW43_WIFI_FW_LEN;
    firmware_details->clm_size = CYW43_CLM_LEN,
    firmware_details->wifi_fw_addr = fw_data;
    firmware_details->clm_addr = fw_data + ((CYW43_WIFI_FW_LEN + 511) & ~511);
    firmware_details->wifi_nvram_len = (sizeof(wifi_nvram_4343) + 63) & ~63;
    firmware_details->wifi_nvram_data = wifi_nvram_4343;
    #if CYW43_ENABLE_BLUETOOTH
    firmware_details->raw_bt_fw_size = bt_fw_data_len;
    firmware_details->bt_fw_size = CYW43_BT_FW_LEN;
    firmware_details->bt_fw_addr = bt_fw_data;
    #endif
}

/*!
 * \brief Get the functions used to load firmware
 *
 * This method returns pointers to functions that load firmware
 *
 * \return structure that contains functions that load firmware
 * \see cyw43_firmware_funcs_t
 */
static inline const cyw43_firmware_funcs_t *cyw43_firmware_funcs(void) {
    static const cyw43_firmware_funcs_t firmware_funcs = {
        #if CYW43_ENABLE_FIRMWARE_COMPRESSION
        .start_wifi_fw = cyw43_wifi_firmware_decompress_start,
        .start_bt_fw = cyw43_bt_firmware_decompress_start,
        .get_wifi_fw = cyw43_firmware_decompress_get,
        .get_bt_fw = cyw43_firmware_decompress_get,
        .get_nvram = cyw43_firmware_embedded_get, // not compressed
        .copy_clm = cyw43_firmware_decompress_copy,
        .end = cyw43_firmware_decompress_end,
        #else
        .start_wifi_fw = NULL,
        .start_bt_fw = NULL,
        .get_wifi_fw = cyw43_firmware_embedded_get,
        .get_bt_fw = cyw43_firmware_embedded_get,
        .get_nvram = cyw43_firmware_embedded_get,
        .copy_clm = cyw43_firmware_copy_embedded,
        .end = NULL,
        #endif
    };
    return &firmware_funcs;
}

#endif