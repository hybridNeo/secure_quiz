out-dir := $(CURDIR)/out
-include $(TA_DEV_KIT_DIR)/host_include/conf.mk
.PHONY: all
ifneq ($(wildcard $(TA_DEV_KIT_DIR)/host_include/conf.mk),)
all: host 
else
all:
	$(q)echo "TA_DEV_KIT_DIR is not correctly defined" && false
endif
.PHONY: host
host:
	$(q)$(MAKE) -C host CROSS_COMPILE="$(CROSS_COMPILE_HOST)" \
			     --no-builtin-variables \
			     q=$(q) \
			     O=$(out-dir)/ndkhost \
			     $@
