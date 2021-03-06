/*-
 * This header is BSD licensed so anyone can use the definitions to implement
 * compatible drivers/servers.
 * Copyright (c) 2015 Micrium Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of IBM nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL IBM OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef _VIRTIO_H_
#define _VIRTIO_H_

#include "virtqueue.h"

/* VirtIO device IDs. */
#define VIRTIO_ID_NETWORK    0x01
#define VIRTIO_ID_BLOCK      0x02
#define VIRTIO_ID_CONSOLE    0x03
#define VIRTIO_ID_ENTROPY    0x04
#define VIRTIO_ID_BALLOON    0x05
#define VIRTIO_ID_IOMEMORY   0x06
#define VIRTIO_ID_RPMSG		 0x07 /* virtio remote remote_proc messaging */
#define VIRTIO_ID_SCSI       0x08
#define VIRTIO_ID_9P         0x09

/* Status byte for guest to report progress. */
#define VIRTIO_CONFIG_STATUS_RESET     0x00
#define VIRTIO_CONFIG_STATUS_ACK       0x01
#define VIRTIO_CONFIG_STATUS_DRIVER    0x02
#define VIRTIO_CONFIG_STATUS_DRIVER_OK 0x04
#define VIRTIO_CONFIG_STATUS_FAILED    0x80

/*
 * Generate interrupt when the virtqueue ring is
 * completely used, even if we've suppressed them.
 */
#define VIRTIO_F_NOTIFY_ON_EMPTY (1 << 24)

/*
 * The guest should never negotiate this feature; it
 * is used to detect faulty drivers.
 */
#define VIRTIO_F_BAD_FEATURE (1 << 30)

/*
 * Some VirtIO feature bits (currently bits 28 through 31) are
 * reserved for the transport being used (eg. virtio_ring), the
 * rest are per-device feature bits.
 */
#define VIRTIO_TRANSPORT_F_START      28
#define VIRTIO_TRANSPORT_F_END        32

typedef struct _virtio_dispatch_ virtio_dispatch;

struct virtio_feature_desc {
    uint32_t      vfd_val;
    const char    *vfd_str;
};

/*
 * Structure definition for virtio devices for use by the
 * applications/drivers
 *
 */

struct virtio_device {
    /*
     * Since there is no generic device structure so
     * keep its type as void. The driver layer will take
     * care of it.
     */
    void *device;

    /* Device name */
    char *name;

    /* List of virtqueues encapsulated by virtio device. */
    //TODO : Need to implement a list service for ipc stack.
    void *vq_list;

    /* Virtio device specific features */
    uint32_t features;

    /* Virtio dispatch table */
    virtio_dispatch *func;

    /*
     * Pointer to hold some private data, useful
     * in callbacks.
     */
    void *data;
};

/*
 * Helper functions.
 */
const char *virtio_dev_name(uint16_t devid);
void       virtio_describe(struct virtio_device *dev, const char *msg,
           uint32_t features, struct virtio_feature_desc *feature_desc);

/*
 * Functions for virtio device configuration as defined in Rusty Russell's paper.
 * Drivers are expected to implement these functions in their respective codes.
 *
 */

struct _virtio_dispatch_ {
    int (*create_virtqueues)(struct virtio_device *dev, int flags, int nvqs,
                    const char *names[], vq_callback *callbacks[],
                    struct virtqueue *vqs[]);
    uint8_t (*get_status)(struct virtio_device *dev);
    void (*set_status)(struct virtio_device *dev, uint8_t status);
    uint32_t (*get_features)(struct virtio_device *dev);
    void (*set_features)(struct virtio_device *dev, uint32_t feature);
    uint32_t (*negotiate_features)(struct virtio_device *dev, uint32_t features);

    /*
     * Read/write a variable amount from the device specific (ie, network)
     * configuration region. This region is encoded in the same endian as
     * the guest.
     */
    void (*read_config)(struct virtio_device *dev, uint32_t offset, void *dst,
                    int length);
    void (*write_config)(struct virtio_device *dev, uint32_t offset, void *src,
                    int length);
    void (*reset_device)(struct virtio_device *dev);

};

#endif /* _VIRTIO_H_ */
