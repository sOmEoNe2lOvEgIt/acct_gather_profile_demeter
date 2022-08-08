##
## ATOS PROJECT 2022
## MAKEFILE
## File description:
## Wow, such make, much file!
##

SLURM_ROOT_DIR = /usr
SLURM_INC_DIR = /root/SLURM/slurm.build
SLURM_LIB_DIR = /usr/lib64/slurm
SLURM_BUILD = 21.08.8-2
SLURM_BUILD_DIR = /root/rpmbuild/BUILD/slurm-$(SLURM_BUILD)

PLUGIN_TYPE = prep
PLUGIN_NAME = demeter
PLUGIN_FILE = $(PLUGIN_TYPE)_$(PLUGIN_NAME).so

SRC_FILE = demeter.c

CC      = gcc
CFLAGS  ?= -Wall -fPIC -g -I$(SLURM_INC_DIR) -I$(SLURM_BUILD_DIR) 
LDFLAGS ?= --shared -L.

all: $(PLUGIN_FILE)

default: $(PLUGIN_FILE)

$(PLUGIN_FILE): $(SRC_FILE)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(PLUGIN_FILE)

mrproper: clean
