ECOS_GLOBAL_CFLAGS = -msoft-float -mcpu=v4.00.a -Wall -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fdata-sections -fno-exceptions -I. -I/home/rihuber/reconos/projects/sort_demo/hw/edk-static/microblaze_0/include -I/home/rihuber/reconos/core/ecos/include
ECOS_GLOBAL_LDFLAGS = -msoft-float -mcpu=v4.00.a -g -nostdlib -Wl,--gc-sections -Wl,-static
ECOS_COMMAND_PREFIX = mb-
