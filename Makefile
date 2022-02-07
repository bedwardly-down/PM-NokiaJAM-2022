TARGET = pm-nokia

C_SOURCES := src/isr.c src/main.c
ASM_SOURCES := src/startup.asm

include ~/c88-pokemini/pm.mk
