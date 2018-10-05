SRC  = $(wildcard *.md)
HTML = $(SRC:.md=.html)

all: $(HTML)

clean:
	$(RM) $(HTML)

.PHONY: all clean

# uses https://github.com/W4RH4WK/Dogx
WD := $(realpath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
include $(HOME)/git/dogx/template/settings.mk
