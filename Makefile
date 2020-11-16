#
# Copyright (C) 2020  Tim Zimmermann <tim@linux4.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

PROJECT     := ddnsd++
VERSION     := 1.0
MAINTAINER  := Tim Zimmermann <tim@linux4.de>
DEB_DEPENDS := bind9, systemd, libcurl4

DESCRIPTION := \
DDNSD++ is a background service to dynamically update\n\
your IP address in a DNS Zone file.\n\
It currently supports two secondary dns services\n\
which it is capable of updating master-ip for.\n\
Supported secondary dns services are:\n\
https://puck.nether.net/dns\n\
https://dns.he.net\n

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

CONF_DIR := conf
DEB_DIR  := $(OUT)/deb
DEB_CDIR := $(DEB_DIR)/DEBIAN
DEB_NAME := $(PROJECT)-$(VERSION)_$(PKG_ARCH).deb

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
	@echo "/etc/ddns/ddnsd.conf" > $(DEB_CDIR)/conffiles

	@mkdir -p $(DEB_DIR)/lib/systemd/system
	@cp $(CONF_DIR)/ddnsdpp.service $(DEB_DIR)/lib/systemd/system

	@dpkg-deb --build $(DEB_DIR) $@
