include $(TOPDIR)/rules.mk

PKG_NAME:=Ci40-ew17-uart-to-ds
PKG_VERSION:=HEAD
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=git@gitlab.flowcloud.systems:creator/Ci40-ew17-uart-to-ds.git

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

CMAKE_INSTALL:=1

CMAKE_OPTIONS += -DSTAGING_DIR=$(STAGING_DIR)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/Ci40-ew17-uart-to-ds
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:= +awalwm2m +letmecreate
  TITLE:=Ci40 to contiki to DS through UART
endef

define Package/Ci40-ew17-uart-to-ds/description
  Imagination Technologies - Ci40 to contiki to DS through UART
endef

define Package/Ci40-ew17-uart-to-ds/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin
	$(INSTALL_BIN)  $(PKG_BUILD_DIR)/script/* $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
