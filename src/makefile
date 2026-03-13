ASM = nasm
QEMU = qemu-system-x86_64
OUTDIR = out

all: solarix.bin

$(OUTDIR):
	mkdir -p $(OUTDIR)

boot.bin: boot.asm | $(OUTDIR)
	$(ASM) -f bin boot.asm -o $(OUTDIR)/boot.bin

kernel.bin: kernel.asm | $(OUTDIR)
	$(ASM) -f bin kernel.asm -o $(OUTDIR)/kernel.bin

solarix.bin: boot.bin kernel.bin
	cat $(OUTDIR)/boot.bin $(OUTDIR)/kernel.bin > $(OUTDIR)/solarix.bin

run: solarix.bin
	$(QEMU) -drive format=raw,file=$(OUTDIR)/solarix.bin

clean:
	rm -rf $(OUTDIR)/*.bin