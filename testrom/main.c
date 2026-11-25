// Mapper2 バンク切り替え実験 for FC (main.c)
// by Takuya Matsubara
#include "pattern.h"

extern unsigned char gettick(void);    // チック時間取得

#define PPUCTRL      *(char*)0x2000    // PPU制御レジスタ
#define PPUMASK      *(char*)0x2001    // PPUマスクレジスタ
#define PPUSTATUS    *(char*)0x2002    // PPUステータスレジスタ
#define PPUSCROLL    *(char*)0x2005    // PPUスクロールレジスタ
#define PPUADDR      *(char*)0x2006    // PPUアドレスレジスタ
#define PPUDATA      *(char*)0x2007    // PPUデータレジスタ

void ppu_vsync(void);
void ppu_pattern(unsigned char *ptable,short ofs,short size);
void ppu_address(unsigned short adr);
void ppu_palette(char *ptable,char ofs,char size);
void nmi_vsync(void);
void bg_printch(char x,char y,unsigned char c);
void bg_printstr(char x,char y,char *str);
void bg_scroll(unsigned char x, unsigned char y);

// 16KB x 8バンク=128KB(1Mbit)
// ----バンク0
#pragma code-name ("BANK0")
#pragma rodata-name ("BANK0")

void bank0_process(void) {
    PPUMASK = 0; // 表示無効化
    // BGパターンテーブル
    ppu_pattern((unsigned char *)bank0_pattern,0,256);
    nmi_vsync();        // Vブランク待ち
    PPUMASK = (1<<3)+6; // 表示有効化
    bg_scroll(0,0);
}

// ----バンク1
#pragma code-name ("BANK1")
#pragma rodata-name ("BANK1")

void bank1_process(void) {
    PPUMASK = 0; // 表示無効化
    // BGパターンテーブル
    ppu_pattern((unsigned char *)bank1_pattern,0,256);
    nmi_vsync();        // Vブランク待ち
    PPUMASK = (1<<3)+6; // 表示有効化
    bg_scroll(0,0);
}
// ----バンク2
#pragma code-name ("BANK2")
#pragma rodata-name ("BANK2")

void bank2_process(void) {
    PPUMASK = 0; // 表示無効化
    // BGパターンテーブル
    ppu_pattern((unsigned char *)bank2_pattern,0,256);
    nmi_vsync();        // Vブランク待ち
    PPUMASK = (1<<3)+6; // 表示有効化
    bg_scroll(0,0);
}
// ----バンク3
#pragma code-name ("BANK3")
#pragma rodata-name ("BANK3")

void bank3_process(void) {
    PPUMASK = 0; // 表示無効化
    // BGパターンテーブル
    ppu_pattern((unsigned char *)bank3_pattern,0,256);
    nmi_vsync();        // Vブランク待ち
    PPUMASK = (1<<3)+6; // 表示有効化
    bg_scroll(0,0);
}
// ----バンク4
#pragma code-name ("BANK4")
#pragma rodata-name ("BANK4")

void bank4_process(void) {
    PPUMASK = 0; // 表示無効化
    // BGパターンテーブル
    ppu_pattern((unsigned char *)bank4_pattern,0,256);
    nmi_vsync();        // Vブランク待ち
    PPUMASK = (1<<3)+6; // 表示有効化
    bg_scroll(0,0);
}
// ----バンク5
#pragma code-name ("BANK5")
#pragma rodata-name ("BANK5")

void bank5_process(void) {
    PPUMASK = 0; // 表示無効化
    // BGパターンテーブル
    ppu_pattern((unsigned char *)bank5_pattern,0,256);
    nmi_vsync();        // Vブランク待ち
    PPUMASK = (1<<3)+6; // 表示有効化
    bg_scroll(0,0);
}
// ----バンク6
#pragma code-name ("BANK6")
#pragma rodata-name ("BANK6")

void bank6_process(void) {
    PPUMASK = 0; // 表示無効化
    // BGパターンテーブル
    ppu_pattern((unsigned char *)bank6_pattern,0,256);
    nmi_vsync();        // Vブランク待ち
    PPUMASK = (1<<3)+6; // 表示有効化
    bg_scroll(0,0);
}

// ----バンク7
#pragma code-name ("CODE")
#pragma rodata-name ("RODATA")

// バンクテーブル
const unsigned char bank_table[8] = {
    0, 1, 2, 3, 4, 5, 6, 7
};

// バンク切り替え(バンク番号)
void set_bank(char bank) {
    *((volatile unsigned char *)&bank_table[bank]) = bank;
}

// BG用カラーパレット
const char palette[] = { 0x0F ,0x00 ,0x10 ,0x30 };

// PPUアドレス設定(アドレス0x0000～0x3fff)
void ppu_address(unsigned short adr)
{
    PPUADDR = adr >> 8;    // アドレス上位8bit
    PPUADDR = adr & 0xff;  // アドレス下位8bit
}

// カラーパレット設定(格納先ポインタ,オフセット0～7,パレット数1～8)
void ppu_palette(char *ptable,char ofs,char size)
{
    size *= 4;     // 1パレットあたり4バイト
    ppu_address( 0x3f00+(ofs*4) );
    while(size--){
        PPUDATA = *ptable++;
    }
}

// パターンテーブル初期化
void ppu_pattern_init(void)
{
    int size = 512*16; // 16バイトx512パターン
    ppu_address(0);    // パターンテーブルのアドレス
    while(size--){
        PPUDATA = 0;
    }
}

// パターンテーブル設定(格納先ポインタ,オフセット0～511,枚数1～512)
void ppu_pattern(unsigned char *ptable,short ofs,short size)
{
    size *= 16;   // 1枚あたり16バイト
    ppu_address(ofs*16);   // パターンテーブルのアドレス
    while(size--){
        PPUDATA = *ptable++;
    }
}

// BGの初期化
void bg_cls(void)
{
    short i;
    ppu_address( 0x2000 );
    for(i=0; i<0x800; i++){
        PPUDATA = 0;    // ネームテーブルとアトリビュートの消去
    }
}

// BGキャラクタ表示(X座標0～31,Y座標0～29,タイル番号0～255)
void bg_printch(char x,char y,unsigned char c)
{
    ppu_address( 0x2000+x+((unsigned short)y*32) );    //ネームテーブルのアドレス
    PPUDATA = c;
}

// BG文字列表示(X座標0～31,Y座標0～29,文字列ポインタ)
void bg_printstr(char x,char y,char *str)
{
    ppu_address( 0x2000+x+((unsigned short)y*32) );    //ネームテーブルのアドレス
    while (*str != 0){
        PPUDATA = *str++;
    }
}

// BGスクロール設定(X座標0～255, Y座標0～239)
void bg_scroll(unsigned char x, unsigned char y)
{
    PPUSCROLL = x;   // X座標(0～255)
    PPUSCROLL = y;   // Y座標(0～239)
}

// Vブランクを待つ(PPUステータス版)
void ppu_vsync(void)
{
    while((PPUSTATUS & (1<<7))==0);    // bit7が0の場合、画面描画中
}

// Vブランクを待つ(NMI版)
void nmi_vsync(void)
{
    unsigned char tick;
    tick = gettick();
    while(tick == gettick()){
    }
}

// 10進数nケタ変換(格納先ポインタ,数値0～65535,桁数)
void num_to_str(char *str,unsigned short num,char keta)
{
    str[keta] = 0; // NULL
    while(keta){
        keta--;
        str[keta] = (num % 10)+'0';
        num /= 10;
    }
}

unsigned char loopcnt=0;
char bank_num=0;

// メイン関数
void NesMain()
{
    char x,y,i;
    ppu_vsync();        // Vブランク待ち
    PPUMASK = 0; // 表示無効化
    ppu_pattern_init(); // パターンテーブル初期化

    ppu_palette((char *)palette,0,1); // BGカラーパレット設定

    PPUMASK = 0; // 表示無効化
    bg_cls();           // BGの初期化
//    bg_printstr(6,6,"MAPPER2 BANK TEST");
    i=0; // タイル番号
    for(y=0; y<16; y++){
        for(x=0; x<16; x++){
            bg_printch(x+8,y+8,i); // BGに描画
            i++;
        }
    }
    ppu_vsync();        // Vブランク待ち
    bg_scroll(0, 0);    // BGスクロール設定
    PPUCTRL = 0x08 | 0x80;
        // bit7(1=NMI有効/0=NMI無効)
        // bit3(1=SPオフセット 0x1000番地)
        // bit0-1(0=画面Aを選択)

    PPUMASK = (1<<3)+6; // 表示有効化(bit3=BG/bit2-1=左端)

    loopcnt = 0; // フレーム数
    bank_num = 0;  // 
    while(1){   // メインループ
        if(loopcnt >= 120){ // 2秒経過
            loopcnt = 0;
            // バンク切り替え、関数を実行
            set_bank(bank_num);
            switch(bank_num){
            case 0:
                bank0_process();
                break;
            case 1:
                bank1_process();
                break;
            case 2:
                bank2_process();
                break;
            case 3:
                bank3_process();
                break;
            case 4:
                bank4_process();
                break;
            case 5:
                bank5_process();
                break;
            case 6:
                bank6_process();
                break;
            default:
                break;
            }
            bank_num = (bank_num+1)% 7;
        }
        nmi_vsync();  // Vブランク待ち(NMI)
        bg_scroll(0, 0);     // BGスクロール設定
        loopcnt++;    // 1/60秒経過
    }
}
