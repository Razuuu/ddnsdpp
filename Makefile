PROJECT     := ddnsd++
VERSION     := 1.0
MAINTAINER  := Tim Zimmermann <tim@linux4.de>
DEB_DEPENDS := bind9, systemd, libcurl4
DESCRIPTION := DDNS daemon rewritten from scratch in C++

CPPFLAGS := -O3 -Wall -Wextra -Werror
CPPFLAGS += -DDDNSD_VERSION="\"$(PROJECT) $(VERSION)\""
LDFLAGS  := -lcurl

PKG_ARCH := $(shell $(CXX) -dumpmachine | \
    sed -e s/i.86.*/i386/ -e s/x86_64.*/amd64/ \
    -e s/arm.*/armhf/ -e s/aarch64.*/arm64/)

SRC := src
OUT := build

OBJS := $(patsubst $(SRC)/%.cpp, $(OUT)/%.o,\
    $(shell find $(SRC) -name "*.cpp"))

default: $(OUT)/$(PROJECT)

$(OUT)/%.o: $(SRC)/%.cpp
	@echo [CXX] $<
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(CPPFLAGS) -c -o $@ $<

$(OUT)/$(PROJECT): $(OBJS)
	@echo [LNK] $@
	@$(CXX) $(CPPFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf build



# Building debian package

CONFIG_DIR := conf
DEB_DIR    := $(OUT)/deb
DEB_CDIR   := $(DEB_DIR)/DEBIAN
DEB_NAME   := $(PROJECT)-$(VERSION)_$(PKG_ARCH).deb

deb-pkg: $(OUT)/$(DEB_NAME)

$(OUT)/$(DEB_NAME): $(OUT)/$(PROJECT)
	@mkdir -p $(DEB_CDIR)
	@echo "Package: $(PROJECT)" > $(DEB_CDIR)/control
	@echo "Version: $(VERSION)" >> $(DEB_CDIR)/control
	@echo "Architecture: $(PKG_ARCH)" >> $(DEB_CDIR)/control
	@echo "Depends: $(DEB_DEPENDS)" >> $(DEB_CDIR)/control
	@echo "Maintainer: $(MAINTAINER)" >> $(DEB_CDIR)/control
	@echo "Description: $(DESCRIPTION)" >> $(DEB_CDIR)/control

	@mkdir -p $(DEB_DIR)/usr/bin
	@cp $< $(DEB_DIR)/usr/bin

	@mkdir -p $(DEB_DIR)/etc/ddns
	@cp $(CONF_DIR)/ddnsd.conf.example $(DEB_DIR)/etc/ddns/ddnsd.conf
	@echo "/etc/ddns/ddnsd.conf" >> $(DEB_CDIR)/conffiles

	@mkdir -p $(DEB_DIR)/lib/systemd/system
	@cp $(CONF_DIR)/ddnsdpp.service $(DEB_DIR)/lib/systemd/system

	@dpkg-deb --build $(DEB_DIR) $@
