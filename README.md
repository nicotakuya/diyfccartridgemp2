# DIY FC cartridge mapper2

## overview
・Mapper2のFC用のカートリッジ基板です。

・容量は16KB x 8バンク=128KB(または16バンク=256KB)。

・8bitworkshop.com「solarian game」、Morphcat Games「Spacegulls」で動作を確認しました。

<img width="361" height="463" alt="mapper2" src="https://github.com/user-attachments/assets/0327010c-28e5-401e-858b-b95780a6da15" />

## files
・fccart_mp2_schematics.png：回路図。

・testromフォルダ：バンクを2秒おきに切り替えるプログラム(16KB×8バンク=128KB)。rom.nesがROMイメージです。build.batを実行するとCC65でコンパイルします。

・FC_CARTMP2.COMP : プリント基板のCADデータ(CADLUS X用).

<img width="942" height="455" alt="fccart_pcb" src="https://github.com/user-attachments/assets/7d19552f-7b8f-45bd-b2ec-794f1df2bb14" />

## Parts
・U1：Flash memory。SST39SF040(128KB)/SST39SF020A(256KB)/SST39SF010A(512KB)。

・U2：32KB SRAM。SRM2B256SLMX55。SOP28パッケージ。

・U3：74HC161。

・U4：74HC32。

・R1/2：10K OHM 抵抗

・R3/4：4.7K OHM 抵抗

・C1：100MICRO F

・PS1：ポリスイッチ0.1A

・LED1/2

## 回路の解説
・カードエッジのA0～A13はフラッシュメモリに接続しています。一度にアクセスできる容量は16KBまでです。A14～A17の信号はアドレスデコーダ(74HC161+74HC32)から供給されます。

・JP1(ジャンパ)のPPU A10=VRAM A10を接続するとVミラーリング、PPU A11=VRAM A10を接続するとHミラーリングになります。

・JP3=ONの場合は全16バンクとなります(256KBのフラッシュメモリで動作)。OFFの場合は全8バンクです(128KBのフラッシュメモリで動作)。

・74HC161はバンク番号を記憶するために使用します。記憶したバンク番号はPRG-ROMのA14～A17に出力されます。

・74HC32はA14=Highの時にバンクを強制的に最大値にするために使用します。

・JP2はアドレスデコーダの電源供給スイッチです。
フラッシュメモリに書き込むさいにはJP2をOFFにします。A14～A17の信号はCN2を通じてライターから送ります。

・0x8000～0xBFFF番地は切り替え可能なバンク1~6です。

・0xC000～0xFFFF番地は固定されたバンク7です。

・0xC000～0xFFF9番地のどこかに番号テーブルを用意します。番号テーブルには0x00～0x07を連番で格納します。(番号テーブルの先頭アドレス+バンク番号)番地にバンク番号を書き込むと、0x8000～0xBFFF番地の内容が切り替わります。この時、CPUからのデータバスとフラッシュメモリからの出力データが適合しないと信号が衝突して、バンクが正常に切り替わりません（暴走します）。信号を衝突させないため、以下の方法でソフトウェア的に対策する必要があります。

## 0xC000番地に番号テーブルを置いた例
・0xC000番地：0x00

・0xC001番地：0x01

・0xC002番地：0x02

 (中略)

・0xC007番地：0x07

## バンク切り替えの例
・0xC000番地に0を書き込む→バンク0を選択

・0xC007番地に7を書き込む→バンク7を選択

## フラッシュメモリの書き込み
こちらのライターを使います。

https://github.com/nicotakuya/diyfccartridge

## TestROMの実行画面とデータ内容
<img width="514" height="500" alt="mapper2_testrom" src="https://github.com/user-attachments/assets/cfec957a-da0d-443d-b2b4-2881d06f6a82" />

<img width="655" height="606" alt="mapper2_testrom_view" src="https://github.com/user-attachments/assets/81900b72-d5bf-4077-92c1-291ad7216c22" />
