/*
 * Copyright 2016, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(D61_BSD)
 */

#ifndef __CROSS_VM_EVENT_COMMON_H
#define __CROSS_VM_EVENt_COMMON_H

/* Constants for cross vm events needed by
 * both the vmm and guest. */

#define EVENT_CONTEXT_MAGIC 42

#define EVENT_IRQ_NUM 11

#define EVENT_VMCALL_HANDLER_TOKEN 2

#define EVENT_CMD_INIT 1
#define EVENT_CMD_ACK 2

#define MAX_NUM_EVENTS 256

typedef struct event_context {
    unsigned int id; // id of the currently interrupting event
    unsigned int magic; // magic number to sanity check shared memory
} event_context_t;

#endif