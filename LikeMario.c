/*---------------------------------------------------------------------------------
    Mario Prototype - PVSNESLIB 
    Suporte a HiROM + Mapas Longos (3424px)
---------------------------------------------------------------------------------*/
#include <snes.h>
#include "soundbank.h"

#include "mario_sprite.inc"
#include "mariofont.inc"
#include "tiles.inc"

//---------------------------------------------------------------------------------
extern char SOUNDBANK__;
extern char jumpsnd, jumpsndend;

#define GRAVITY 48
#define MARIO_MAXACCEL 0x0140
#define MARIO_RUN_MAXACCEL (MARIO_MAXACCEL * 2)
#define MARIO_ACCEL 0x0038
#define MARIO_JUMPING 0x0394
#define MARIO_HIJUMPING 0x0594
#define SKID_THRESHOLD 0x0090 

#define MAP_WIDTH 3424  // Sua largura real
#define MAP_HEIGHT 480

enum {
    MARIOSKID = 0,
    MARIOJUMPING = 1,
    MARIOWALK = 2,
    MARIOSTAND = 6 
};

//---------------------------------------------------------------------------------
extern char mapmario, objmario;
extern char tilesetdef, tilesetatt;

brrsamples Jump; 
u16 pad0; 

t_objs *marioobj; 
s16 *marioox, *mariooy; 
s16 *marioxv, *marioyv; 
u16 mariox, marioy; 
u8 mariofidx, marioflp, flip; 
u8 skid_timer = 0;

//---------------------------------------------------------------------------------
void marioinit(u16 xp, u16 yp, u16 type, u16 minx, u16 maxx) {
    if (objNew(type, xp, yp) == 0) return;
    objGetPointer(objgetid);
    marioobj = &objbuffers[objptr - 1];
    marioobj->width = 16;
    marioobj->height = 16;
    
    marioox = (u16 *)&(marioobj->xpos + 1);
    mariooy = (u16 *)&(marioobj->ypos + 1);
    marioxv = (short *)&(marioobj->xvel);
    marioyv = (short *)&(marioobj->yvel);

    marioobj->action = ACT_STAND;
    oambuffer[0].oamframeid = 6; 
    oambuffer[0].oamrefresh = 1;
    oambuffer[0].oamattribute = 0x60; 
    oambuffer[0].oamgraphics = &mario_sprite_til;
    setPalette(&mario_sprite_pal, 128 + 0 * 16, 16 * 2);
}

//---------------------------------------------------------------------------------
void mariowalk(u8 idx) {
    flip++;
    if ((flip & 3) == 3) {
        mariofidx++;
        mariofidx = mariofidx & 1;
        oambuffer[0].oamframeid = marioflp + mariofidx;
        oambuffer[0].oamrefresh = 1;
    }
}

//---------------------------------------------------------------------------------
void marioupdate(u8 idx) {
    pad0 = padsCurrent(0);
    u16 current_max_accel = (pad0 & KEY_Y) ? MARIO_RUN_MAXACCEL : MARIO_MAXACCEL;

    if (pad0 & (KEY_RIGHT | KEY_LEFT | KEY_B)) {
        if (pad0 & KEY_LEFT) {
            if (*marioxv > SKID_THRESHOLD && marioobj->tilestand != 0) {
                skid_timer = 10;
                *marioxv -= (MARIO_ACCEL * 2);
            } else {
                if (marioobj->action != ACT_JUMP && marioobj->action != ACT_FALL) {
                    if ((marioflp > 3) || (marioflp < 2)) marioflp = 2;
                    marioobj->action = ACT_WALK;
                }
                *marioxv -= (MARIO_ACCEL);
            }
            oambuffer[0].oamattribute &= ~0x40; 
            if (*marioxv <= (-current_max_accel)) *marioxv = (-current_max_accel);
        }
        else if (pad0 & KEY_RIGHT) {
            if (*marioxv < -SKID_THRESHOLD && marioobj->tilestand != 0) {
                skid_timer = 10;
                *marioxv += (MARIO_ACCEL * 2);
            } else {
                if (marioobj->action != ACT_JUMP && marioobj->action != ACT_FALL) {
                    if ((marioflp > 3) || (marioflp < 2)) marioflp = 2;
                    marioobj->action = ACT_WALK;
                }
                *marioxv += (MARIO_ACCEL);
            }
            oambuffer[0].oamattribute |= 0x40; 
            if (*marioxv >= (current_max_accel)) *marioxv = (current_max_accel);
        }

        if (pad0 & KEY_B && marioobj->tilestand != 0) {
            marioobj->action = ACT_JUMP;
            skid_timer = 0;
            if (pad0 & KEY_UP) *marioyv = -(MARIO_HIJUMPING);
            else *marioyv = -(MARIO_JUMPING);
            spcPlaySound(0);
        }
    }

    objCollidMap(idx);

    if (marioobj->action == ACT_WALK) mariowalk(idx);
    else if (marioobj->action == ACT_FALL || marioobj->action == ACT_JUMP) {
        oambuffer[0].oamframeid = 1;
        oambuffer[0].oamrefresh = 1;
        if (*marioyv >= 0) marioobj->action = ACT_FALL;
        if (marioobj->tilestand != 0) marioobj->action = ACT_STAND;
    }
    else if (marioobj->action == ACT_STAND) {
        oambuffer[0].oamframeid = 6;
        oambuffer[0].oamrefresh = 1;
    }

    objUpdateXY(idx);

    if (skid_timer > 0 && marioobj->tilestand != 0) {
        oambuffer[0].oamframeid = 0; 
        oambuffer[0].oamrefresh = 1;
        skid_timer--;
    }

    // --- CORREÇÃO DA BARREIRA INVISÍVEL ---
    if (*marioox <= 0) *marioox = 0;
    if (*marioox >= (MAP_WIDTH - 16)) *marioox = (MAP_WIDTH - 16); 
    if (*mariooy <= 0) *mariooy = 0;

    mariox = (*marioox);
    marioy = (*mariooy);

    // Atualiza a câmera para seguir o Mario
    mapUpdateCamera(mariox, marioy);

    // Ajusta a posição do sprite na tela (considerando o scroll)
    oambuffer[0].oamx = mariox - x_pos;
    oambuffer[0].oamy = marioy - y_pos;
    
    oamDynamic16Draw(0);
}

//---------------------------------------------------------------------------------
int main(void) {
    spcBoot();
    consoleSetTextMapPtr(0x6000);
    consoleSetTextGfxPtr(0x3000);
    consoleInitText(1, 16 * 2, &mariofont_til, &mariofont_pal);

    spcSetBank(&SOUNDBANK__);
    spcAllocateSoundRegion(39);
    spcLoad(MOD_OVERWORLD);
    spcSetSoundEntry(15, 8, 6, &jumpsndend - &jumpsnd, &jumpsnd, &Jump);

    bgSetGfxPtr(1, 0x3000);
    bgSetMapPtr(1, 0x6000, SC_32x32);
    
    bgInitTileSet(0, &tiles_til, &tiles_pal, 0, (&tiles_tilend - &tiles_til), 16 * 2, BG_16COLORS, 0x2000);
    bgSetMapPtr(0, 0x6800, SC_64x32); // Buffer de scroll 512x256

    oamInitDynamicSprite(0x0000, 0x1000, 0, 0, OBJ_SIZE8_L16);
    objInitEngine();
    objInitFunctions(0, &marioinit, &marioupdate, NULL);
    objLoadObjects((char *)&objmario);

    // Carrega o mapa gigante
    mapLoad((u8 *)&mapmario, (u8 *)&tilesetdef, (u8 *)&tilesetatt);

    setMode(BG_MODE1, 0);
    bgSetDisable(2);
    
    setScreenOn();
    spcPlay(0);
    spcSetModuleVolume(100);

    while (1) {
        objUpdateAll();
        mapUpdate(); // Essencial para mapas > 512px
        
        oamInitDynamicSpriteEndFrame();
        spcProcess();
        WaitForVBlank();
        mapVblank();
        oamVramQueueUpdate();
    }
    return 0;
}