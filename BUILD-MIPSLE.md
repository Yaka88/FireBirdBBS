# MIPS32 Little-Endian 浜ゅ弶缂栬瘧璇存槑

鏈粨搴撳凡閰嶇疆 GitHub Actions 鑷姩鍖栧伐浣滄祦锛岀敤浜庡皢 FireBird BBS 浜ゅ弶缂栬瘧鍒� MIPS32 Little-Endian 骞冲彴銆�

## 鐗规€�

- 鉁� 浜ゅ弶缂栬瘧鐩爣锛歮ipsel-linux-gnu (MIPS32 灏忕)
- 鉁� 浠庢簮鐮佺紪璇戜緷璧栧簱锛歵ermcap, ncurses, libxcrypt (鏇夸唬 libcrypt)
- 鉁� 闈欐€侀摼鎺ユ墍鏈変緷璧栵紙鍖呮嫭 libc锛�
- 鉁� 淇濈暀 GB18030 涓枃缂栫爜
- 鉁� 浣跨敤 32 浣� time_t锛堝吋瀹� 2000 骞翠唬 x86 32 浣嶅钩鍙帮級
- 鉁� 缂栬瘧鏍囧織锛歚-mips32 -mabi=32 -EL -fno-stack-protector -fcommon -O2`
- 鉁� 鍖呭惈璋冭瘯绗﹀彿锛歚-g3 -gdwarf-2`
- 鉁� 绂佺敤鏍堜繚鎶�
- 鉁� 寮哄埗 32 浣嶆枃浠跺亸绉�

## 濡備綍鎺ㄩ€佸苟瑙﹀彂鏋勫缓

宸茬粡鍒涘缓浜� commit锛岄渶瑕佹帹閫佸埌 GitHub锛�

```bash
cd /home/yaka/Documents/FireBirdBBS
git push origin main
```

## 鏋勫缓娴佺▼

GitHub Actions 灏嗚嚜鍔ㄦ墽琛屼互涓嬫楠わ細

1. **璁剧疆鐜**锛氬畨瑁� MIPS 浜ゅ弶缂栬瘧宸ュ叿閾�
2. **缂栬瘧 termcap**锛氫粠 GNU termcap 1.3.1 婧愮爜缂栬瘧闈欐€佸簱
3. **缂栬瘧 ncurses**锛氫粠 ncurses 5.9 婧愮爜缂栬瘧闈欐€佸簱
4. **缂栬瘧 libxcrypt**锛氫粠 libxcrypt 4.4.28 婧愮爜缂栬瘧闈欐€佸簱锛圖ES crypt 鍏煎锛�
5. **缂栬瘧 libBBS**锛氱紪璇� BBS 涓撶敤搴�
6. **缂栬瘧涓荤▼搴�**锛氱紪璇� bbsd, chatd, thread, expire 绛夊彲鎵ц鏂囦欢
7. **缂栬瘧宸ュ叿绋嬪簭**锛氱紪璇� util 鐩綍涓嬬殑宸ュ叿
8. **鎵撳寘**锛氬垱寤� `bbs-mipsle-binaries.tar.gz` 鍘嬬缉鍖�
9. **涓婁紶**锛氬皢缂栬瘧缁撴灉涓婁紶鍒颁粨搴撳拰 GitHub Actions Artifacts

## 缂栬瘧杈撳嚭

缂栬瘧瀹屾垚鍚庯紝灏嗙敓鎴愪互涓嬫枃浠讹細

- `bbsd` - BBS 涓诲畧鎶よ繘绋�
- `chatd` - 鑱婂ぉ瀹ゅ畧鎶よ繘绋�
- `thread` - 璁ㄨ涓插鐞嗙▼搴�
- `expire` - 杩囨湡鏂囩珷娓呯悊绋嬪簭
- `paging.so` - 鍛煎彨妯″潡锛堝鏋滅紪璇戞垚鍔燂級
- util 鐩綍涓嬬殑鍚勭宸ュ叿绋嬪簭

鎵€鏈変簩杩涘埗鏂囦欢灏嗚鎵撳寘鍒� `bbs-mipsle-binaries.tar.gz` 涓€�

## 涓嬭浇缂栬瘧缁撴灉

### 鏂规硶 1锛氫粠 GitHub Releases 鎴� Artifacts 涓嬭浇

1. 璁块棶 Actions 椤甸潰锛歨ttps://github.com/Yaka88/FireBirdBBS/actions
2. 鐐瑰嚮鏈€鏂扮殑 workflow run
3. 涓嬭浇 `bbs-mipsle-binaries` artifact

### 鏂规硶 2锛氫粠浠撳簱鏍圭洰褰曚笅杞�

濡傛灉 workflow 鎺ㄩ€佹垚鍔燂紝鍙互鐩存帴浠庝粨搴撴牴鐩綍涓嬭浇 `bbs-mipsle-binaries.tar.gz`銆�

## 楠岃瘉缂栬瘧缁撴灉

瑙ｅ帇鍚庡彲浠ヤ娇鐢� `file` 鍛戒护楠岃瘉锛�

```bash
tar xzf bbs-mipsle-binaries.tar.gz
cd bbs-mipsle-binaries
file bbsd
```

搴旇鏄剧ず绫讳技锛�
```
bbsd: ELF 32-bit LSB executable, MIPS, MIPS32 version 1 (SYSV), statically linked, ...
```

## 閮ㄧ讲鍒� MIPS 璁惧

1. 灏� `bbs-mipsle-binaries.tar.gz` 澶嶅埗鍒� MIPS 璁惧
2. 瑙ｅ帇锛歚tar xzf bbs-mipsle-binaries.tar.gz`
3. 鏍规嵁鍘� BBS 閰嶇疆鏂囦欢閮ㄧ讲浜岃繘鍒舵枃浠�
4. 纭繚璁剧疆姝ｇ‘鐨勬潈闄愬拰鎵€鏈夎€�

## 璋冭瘯淇℃伅

鎵€鏈夊彲鎵ц鏂囦欢鍖呭惈瀹屾暣鐨勮皟璇曠鍙凤紙-g3 -gdwarf-2锛夛紝鍙互鍦� MIPS 骞冲彴涓婁娇鐢� gdb 杩涜璋冭瘯锛�

```bash
mipsel-linux-gnu-gdb bbsd
```

鎴栧湪 MIPS 璁惧涓婏細

```bash
gdb bbsd
```

## 娉ㄦ剰浜嬮」

1. **缂栫爜**锛氭簮浠ｇ爜淇濇寔 GB18030 缂栫爜锛屾湭鍋氫换浣曚慨鏀�
2. **閰嶇疆鏂囦欢**锛氭湭淇敼 `configure.inf` 鍜屼换浣曞ご鏂囦欢
3. **32浣嶅吋瀹�**锛氭墍鏈夌紪璇戦€夐」纭繚 32 浣嶅吋瀹规€э紝鍖呮嫭 time_t
4. **闈欐€侀摼鎺�**锛氭墍鏈変緷璧栧畬鍏ㄩ潤鎬侀摼鎺ワ紝鏃犻渶鍦ㄧ洰鏍囪澶囧畨瑁呴澶栧簱
5. **鏍堜繚鎶�**锛氬凡绂佺敤鏍堜繚鎶わ紙-fno-stack-protector锛�

## 鏁呴殰鎺掓煡

濡傛灉鏋勫缓澶辫触锛岃锛�

1. 鏌ョ湅 GitHub Actions 鏃ュ織
2. 妫€鏌ユ槸鍚︽湁缂栬瘧閿欒
3. 纭鎵€鏈変緷璧栧簱鏄惁鎴愬姛缂栬瘧
4. 楠岃瘉 MIPS 宸ュ叿閾炬槸鍚︽纭畨瑁�

## 鎶€鏈弬鏁�

```
鐩爣鏋舵瀯: mipsel-linux-gnu
ABI: o32 (32-bit)
瀛楄妭搴�: Little-Endian
浼樺寲绾у埆: -O2
璋冭瘯绾у埆: -g3 -gdwarf-2
缂栬瘧鍣ㄦ爣蹇�: -mips32 -mabi=32 -EL -fno-stack-protector -fcommon -O2 -U_TIME_BITS -U_FILE_OFFSET_BITS -D_FILE_OFFSET_BITS=32
閾炬帴鏂瑰紡: 瀹屽叏闈欐€侀摼鎺�
```

## 渚濊禆搴撶増鏈�

- termcap: 1.3.1
- ncurses: 5.9
- libxcrypt: 4.4.28 (浣滀负 libcrypt 鏇夸唬锛屾敮鎸� DES)
- libbbs: 椤圭洰鑷甫鐗堟湰

## 鏋勫缓鏃堕棿

棰勮鏋勫缓鏃堕棿锛�10-15 鍒嗛挓锛堝彇鍐充簬 GitHub Actions 杩愯鍣ㄦ€ц兘锛�
