APP      := gtkmmcalc
APP_ID   := gtkmmcalc
NAME     := Gtkmmcalc
PREFIX   ?= $(HOME)/.local
SRC      := $(wildcard *.cpp)

CXXFLAGS += -std=c++23 -Wall $(shell pkg-config --cflags gtkmm-4.0 2>/dev/null)
LDLIBS   += $(shell pkg-config --libs gtkmm-4.0 2>/dev/null)

.DEFAULT_GOAL := all
.PHONY: all check-deps install-deps install uninstall clean

all: $(APP)

# Fail with install instructions if gtkmm isn't present
check-deps:
	@pkg-config --exists gtkmm-4.0 2>/dev/null || { \
	  echo "gtkmm-4.0 development files not found. Run: make install-deps"; \
	  exit 1; }

# Install build tools and gtkmm using the system package manager
install-deps:
	@set -e; \
	if   command -v apt    >/dev/null; then sudo apt install -y build-essential pkg-config libgtkmm-4.0-dev; \
	elif command -v dnf    >/dev/null; then sudo dnf install -y gcc-c++ make pkgconf gtkmm4.0-devel; \
	elif command -v pacman >/dev/null; then sudo pacman -S --needed base-devel gtkmm-4.0; \
	elif command -v zypper >/dev/null; then sudo zypper install -y gcc-c++ make gtkmm4-devel; \
	else echo "Unknown distro, install the gtkmm-4.0 dev package manually"; exit 1; fi

$(APP): check-deps $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@ $(LDLIBS)

install: $(APP)
	install -Dm755 $(APP) $(DESTDIR)$(PREFIX)/bin/$(APP)
	install -d $(DESTDIR)$(PREFIX)/share/applications
	printf '%s\n' \
	  '[Desktop Entry]' \
	  'Type=Application' \
	  'Name=$(NAME)' \
	  'Exec=$(PREFIX)/bin/$(APP)' \
	  'Icon=$(APP)' \
	  'Terminal=false' \
	  'Categories=Utility;GTK;' \
	  > $(DESTDIR)$(PREFIX)/share/applications/$(APP_ID).desktop
	-update-desktop-database $(DESTDIR)$(PREFIX)/share/applications 2>/dev/null

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(APP)
	rm -f $(DESTDIR)$(PREFIX)/share/applications/$(APP_ID).desktop

clean:
	rm -f $(APP)
