#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Tomodachi Life Save Editor V2 - By Darkness Metal
// Base V1 - Sem baloes - Sem toque
// SELECT = escolher save - A = Adicionar Tudo - START = Sair

static const char *paths[] = {
    "sdmc:/3ds/tlse/savedataArc.txt",
    "sdmc:/3ds/Checkpoint/saves/Tomodachi Life/savedataArc.txt",
    "sdmc:/3ds/Checkpoint/saves/TomodachiLife/savedataArc.txt",
    "sdmc:/TomodachiLife/savedataArc.txt"
};
static const int numPaths = 4;

static void fillRange(unsigned char *data, long size, long start, long end, unsigned char val) {
    if (start < 0) start = 0;
    if (end >= size) end = size - 1;
    for (long i = start; i <= end; i++) {
        if (data[i] == 0xFD || data[i] == 0x00) data[i] = val;
    }
}

static int tryAddAll(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (size <= 0x20) { fclose(f); return 0; }
    unsigned char *data = (unsigned char*)malloc(size);
    if (!data) { fclose(f); return 0; }
    if (fread(data, 1, size, f) != (size_t)size) { free(data); fclose(f); return 0; }
    fclose(f);

    // Backup .bak antes de mudar
    char bak[512];
    snprintf(bak, sizeof(bak), "%s.bak", path);
    FILE *bf = fopen(bak, "wb");
    if (bf) { fwrite(data, 1, size, bf); fclose(bf); }

    // Dinheiro Max - 99999999 = 0x05F5E0FF
    if (size > 0x1E4BB8 + 4) {
        data[0x1E4BB8] = 0xFF;
        data[0x1E4BB8+1] = 0xE0;
        data[0x1E4BB8+2] = 0xF5;
        data[0x1E4BB8+3] = 0x05;
    }
    // Compat V1
    if (size > 0x20) {
        data[0x1C] = 0x7F;
        data[0x1D] = 0x96;
        data[0x1E] = 0x98;
        data[0x1F] = 0x00;
    }

    // Tudo para o maximo - pesquisa GBAtemp / Brion
    fillRange(data, size, 0x30, 0xE27, 0x63);     // Roupas / Conjuntos / Acessorios = 99
    fillRange(data, size, 0x17F0, 0x18D6, 0x63);   // Comidas = 99
    fillRange(data, size, 0x1A14, 0x1A44, 0x63);   // Comidas Especiais = 99
    fillRange(data, size, 0x1760, 0x17E0, 0x55);   // Interiores = 85
    fillRange(data, size, 0x18F0, 0x19A7, 0x55);   // Tesouros / Presentes = 85
    fillRange(data, size, 0x00, 0x2F, 0x63);       // Itens = 99

    FILE *w = fopen(path, "wb");
    if (!w) { free(data); return 0; }
    fwrite(data, 1, size, w);
    fclose(w);
    free(data);
    return 1;
}

static void drawTop(PrintConsole *top, int selSave, int done, const char *msg) {
    consoleSelect(top);
    consoleClear();
    printf("TOMODACHI LIFE SAVE EDITOR\n");
    printf("by Darkness Metal\n");
    printf("------------------------------\n");
    printf("Save: %d / %d\n", selSave + 1, numPaths);
    printf("%s\n\n", paths[selSave]);
    printf("SELECT = Trocar save\n");
    printf("A = Adicionar Tudo\n");
    printf("START = Sair p/ menu do 3DS\n\n");
    if (done) printf("STATUS: TUDO ADICIONADO!\n");
    else printf("STATUS: Pronto.\n");
    printf("%s\n", msg);
}

static void drawBottom(PrintConsole *bottom, int done) {
    consoleSelect(bottom);
    consoleClear();
    printf("\n");
    printf("      ****************\n");
    printf("    **                **\n");
    printf("   *    Adicionar      *\n");
    printf("   *      Tudo         *\n");
    printf("    **                **\n");
    printf("      ****************\n");
    printf("\n");
    if (!done) printf("       Aperte A\n");
    else printf("     Aperte Start\n");
    printf("\nSELECT = Escolher save\n");
}

int main(int argc, char **argv) {
    gfxInitDefault();
    PrintConsole top, bottom;
    consoleInit(GFX_TOP, &top);
    consoleInit(GFX_BOTTOM, &bottom);

    int selSave = 0;
    int done = 0;
    char msg[256];
    snprintf(msg, sizeof(msg), "Use Checkpoint antes!");

    drawTop(&top, selSave, done, msg);
    drawBottom(&bottom, done);

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        int changed = 0;

        if (kDown & KEY_SELECT) {
            selSave = (selSave + 1) % numPaths;
            done = 0;
            snprintf(msg, sizeof(msg), "Save trocado! Aperte A.");
            changed = 1;
        }

        if (kDown & KEY_A) {
            if (!done) {
                if (tryAddAll(paths[selSave])) {
                    done = 1;
                    snprintf(msg, sizeof(msg), "OK! Tudo adicionado! Backup .bak feito.");
                } else {
                    snprintf(msg, sizeof(msg), "Save nao achado! Use Checkpoint e SELECT.");
                }
                changed = 1;
            }
        }

        if (kDown & KEY_START) break;

        if (changed) {
            drawTop(&top, selSave, done, msg);
            drawBottom(&bottom, done);
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
