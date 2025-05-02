export V ?= 0

OUT_BASE      := $(CURDIR)/out
BUILD_OUT     := $(OUT_BASE)/build-$(MAKECMDGOALS)
FINAL_TA_DIR  := $(OUT_BASE)/ta

CROSS_COMPILE     ?= aarch64-linux-gnu-
TA_CROSS_COMPILE  ?= $(CROSS_COMPILE)

ifeq ($(MAKECMDGOALS),domain)
CFLAGS += -DMAKECMDGOAL_DOMAIN
srcs-y  += ../source/domain/client.c
srcs-y  += ../source/common/sockets.c
srcs-y  += ../source/common/arguments.c
TA_UUID := 11111111-1111-1111-1111-111111111111
else ifeq ($(MAKECMDGOALS),eventfd)
CFLAGS += -DMAKECMDGOAL_EVENTFD
srcs-y  += ../source/eventfd/client.c
TA_UUID := 22222222-2222-2222-2222-222222222222
else ifeq ($(MAKECMDGOALS),fifo)
CFLAGS += -DMAKECMDGOAL_FIFO
srcs-y  += ../source/fifo/client.c
TA_UUID := 33333333-3333-3333-3333-333333333333
else ifeq ($(MAKECMDGOALS),mmap)
CFLAGS += -DMAKECMDGOAL_MMAP
srcs-y  += ../source/mmap/client.c
TA_UUID := 44444444-4444-4444-4444-444444444444
else ifeq ($(MAKECMDGOALS),mq)
CFLAGS += -DMAKECMDGOAL_MQ
srcs-y  += ../source/mq/client.c
TA_UUID := 55555555-5555-5555-5555-555555555555
else ifeq ($(MAKECMDGOALS),pipe)
CFLAGS += -DMAKECMDGOAL_PIPE
srcs-y  += ../source/pipe/client.c
TA_UUID := 66666666-6666-6666-6666-666666666666
else ifeq ($(MAKECMDGOALS),shm)
CFLAGS += -DMAKECMDGOAL_SHM
srcs-y  += ../source/shm/client.c
TA_UUID := 77777777-7777-7777-7777-777777777777
else ifeq ($(MAKECMDGOALS),shm-sync)
CFLAGS += -DMAKECMDGOAL_SHM_SYNC
srcs-y  += ../source/shm_sync/client.c
TA_UUID := 88888888-8888-8888-8888-888888888888
else ifeq ($(MAKECMDGOALS),signal)
CFLAGS += -DMAKECMDGOAL_SIGNAL
srcs-y  += ../source/signal/client.c
TA_UUID := 99999999-9999-9999-9999-999999999999
else ifeq ($(MAKECMDGOALS),tcp)
CFLAGS += -DMAKECMDGOAL_TCP
srcs-y  += ../source/tcp/client.c
TA_UUID := aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa
else
$(error Unknown directory: $(BASENAME))
endif

BINARY := $(TA_UUID)

CFG_TEE_TA_LOG_LEVEL ?= 4
CFLAGS += -O3 -DOPTEE_ENV
CFLAGS += -I$(CURDIR)/../source
CPPFLAGS += -DCFG_TEE_TA_LOG_LEVEL=$(CFG_TEE_TA_LOG_LEVEL)

ifeq ($(wildcard $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk), )
$(error "TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR) is invalid. Please specify correct path to ta_dev_kit.mk")
endif

O := $(BUILD_OUT)
-include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk

.PHONY: all clean $(MAKECMDGOALS)
$(MAKECMDGOALS): all

all: $(FINAL_TA_DIR)/$(BINARY).ta

.PHONY: $(FINAL_TA_DIR)/$(BINARY).ta

$(FINAL_TA_DIR)/$(BINARY).ta:
	@mkdir -p $(FINAL_TA_DIR)
	@echo "Waiting for .ta file to appear..."
	@while [ ! -f "$(O)/$(BINARY).ta" ]; do sleep 0.2; done
	cp -v $(O)/$(BINARY).ta $@

clean:
	rm -rf $(OUT_BASE)
