# 濡備綍鎺ㄩ€佷唬鐮佸苟瑙﹀彂 MIPS32LE 鑷姩鏋勫缓

## 蹇€熷紑濮�

宸蹭负鎮ㄥ垱寤轰簡瀹屾暣鐨� GitHub Actions 鑷姩鏋勫缓宸ヤ綔娴併€傚彧闇€鎺ㄩ€佷唬鐮佸埌 GitHub 鍗冲彲瑙﹀彂鏋勫缓銆�

### 鏂规硶涓€锛氫娇鐢ㄦ彁渚涚殑鑴氭湰锛堟帹鑽愶級

```bash
./push-to-github.sh
```

### 鏂规硶浜岋細鎵嬪姩鎺ㄩ€�

```bash
git push origin main
```

## 褰撳墠鐘舵€�

鉁� 宸插垱寤� GitHub Actions 宸ヤ綔娴佹枃浠�
鉁� 宸查厤缃� MIPS32LE 浜ゅ弶缂栬瘧鐜
鉁� 宸茶缃墍鏈夌紪璇戝弬鏁板拰鏍囧織
鉁� 宸插噯澶囧ソ鎺ㄩ€佸埌 GitHub

寰呭畬鎴愶細
- 鎺ㄩ€佷唬鐮佸埌 GitHub 浠撳簱
- GitHub Actions 灏嗚嚜鍔ㄥ紑濮嬫瀯寤�

## 鎺ㄩ€佸悗浼氬彂鐢熶粈涔�

1. **瑙﹀彂鏋勫缓**锛氭帹閫佸悗 GitHub Actions 浼氳嚜鍔ㄦ娴嬪苟寮€濮嬪伐浣滄祦
2. **鏋勫缓杩囩▼**锛堢害 10-15 鍒嗛挓锛夛細
   - 瀹夎 MIPS 浜ゅ弶缂栬瘧宸ュ叿閾�
   - 浠庢簮鐮佺紪璇� termcap 1.3.1
   - 浠庢簮鐮佺紪璇� ncurses 5.9
   - 浠庢簮鐮佺紪璇� libxcrypt 4.4.28锛圖ES crypt 鍏煎锛�
   - 缂栬瘧 libBBS 搴�
   - 缂栬瘧 BBS 涓荤▼搴忥紙bbsd, chatd, thread, expire锛�
   - 缂栬瘧宸ュ叿绋嬪簭
   - 鎵撳寘鎵€鏈変簩杩涘埗鏂囦欢
3. **瀹屾垚**锛氱敓鎴� `bbs-mipsle-binaries.tar.gz` 骞朵笂浼犲埌浠撳簱

## 鐩戞帶鏋勫缓杩涘害

鎺ㄩ€佸悗锛岃闂互涓嬬綉鍧€鏌ョ湅鏋勫缓鐘舵€侊細

```
https://github.com/Yaka88/FireBirdBBS/actions
```

鐐瑰嚮鏈€鏂扮殑 workflow run 鍙互鐪嬪埌锛�
- 瀹炴椂鏋勫缓鏃ュ織
- 姣忎釜姝ラ鐨勮缁嗚緭鍑�
- 鎴愬姛/澶辫触鐘舵€�
- 鏋勫缓鎽樿锛堝寘鍚紪璇戠殑鏂囦欢鍒楄〃鍜屽ぇ灏忥級

## 涓嬭浇缂栬瘧缁撴灉

### 浠� Actions Artifacts 涓嬭浇锛堟帹鑽愶級

1. 杩涘叆 Actions 椤甸潰
2. 鐐瑰嚮瀹屾垚鐨� workflow run
3. 涓嬭浇椤甸潰搴曢儴鐨� `bbs-mipsle-binaries` artifact

### 浠庝粨搴撲笅杞�

濡傛灉宸ヤ綔娴佹垚鍔熸帹閫侊紝涔熷彲浠ョ洿鎺ヤ粠浠撳簱鏍圭洰褰曞厠闅嗗苟鑾峰彇 `bbs-mipsle-binaries.tar.gz`銆�

## 缂栬瘧閰嶇疆璇︽儏

### 鐩爣骞冲彴
- **鏋舵瀯**锛歮ipsel-linux-gnu (MIPS32 灏忕)
- **ABI**锛歰32 (32-bit)
- **瀛楄妭搴�**锛歀ittle-Endian

### 缂栬瘧鏍囧織
```
-mips32           # MIPS32 鎸囦护闆�
-mabi=32          # 32浣� ABI
-EL               # 灏忕瀛楄妭搴�
-fno-stack-protector  # 绂佺敤鏍堜繚鎶�
-fcommon          # 鍏佽甯歌鐨勫巻鍙� C 琛屼负
-O2               # 浼樺寲绾у埆 2
-U_TIME_BITS      # 涓嶅畾涔� _TIME_BITS
-U_FILE_OFFSET_BITS  # 涓嶅畾涔� _FILE_OFFSET_BITS
-D_FILE_OFFSET_BITS=32  # 寮哄埗 32 浣嶆枃浠跺亸绉�
-g3               # 鏈€澶ц皟璇曚俊鎭�
-gdwarf-2         # DWARF-2 璋冭瘯鏍煎紡
```

### 閾炬帴鏂瑰紡
- **闈欐€侀摼鎺�**锛氭墍鏈夊簱锛堝寘鎷� libc锛夐兘闈欐€侀摼鎺�
- **鏃犲姩鎬佷緷璧�**锛氱敓鎴愮殑鍙墽琛屾枃浠跺畬鍏ㄧ嫭绔�

### 渚濊禆搴�
鎵€鏈変緷璧栧簱閮戒粠婧愮爜缂栬瘧涓� MIPS32LE 闈欐€佸簱锛�

| 搴� | 鐗堟湰 | 璇存槑 |
|---|---|---|
| termcap | 1.3.1 | 缁堢鑳藉姏鏁版嵁搴� |
| ncurses | 5.9 | 缁堢 UI 搴� |
| libxcrypt | 4.4.28 | 瀵嗙爜鍔犲瘑搴擄紙鏇夸唬 libcrypt锛� |
| libBBS | 椤圭洰鑷甫 | BBS 涓撶敤搴� |

## 淇濊瘉鐨勫吋瀹规€�

鉁� **淇濈暀 GB18030 缂栫爜**锛氭簮浠ｇ爜鍜屾敞閲婁繚鎸佸師鏍�
鉁� **32浣� time_t**锛氬吋瀹� 2000 骞翠唬 32 浣嶅钩鍙�
鉁� **鏈慨鏀归厤缃�**锛歝onfigure.inf 鍜屽ご鏂囦欢瀹屽叏鏈敼
鉁� **瀹屾暣璋冭瘯绗﹀彿**锛氫究浜庡湪 MIPS 骞冲彴璋冭瘯

## 閮ㄧ讲鍒� MIPS 璁惧

1. 涓嬭浇骞惰В鍘� `bbs-mipsle-binaries.tar.gz`
2. 灏嗕簩杩涘埗鏂囦欢澶嶅埗鍒� MIPS 璁惧
3. 璁剧疆姝ｇ‘鐨勬潈闄愶細
   ```bash
   chmod 550 bbsd chatd thread expire
   chown bbs:bbs bbsd chatd thread expire
   ```
4. 鏍规嵁鍘� BBS 閰嶇疆閮ㄧ讲鏂囦欢

## 楠岃瘉浜岃繘鍒舵枃浠�

鍦� MIPS 璁惧鎴栦氦鍙夌紪璇戠幆澧冧腑锛�

```bash
file bbsd
```

搴旇鏄剧ず锛�
```
bbsd: ELF 32-bit LSB executable, MIPS, MIPS32 version 1 (SYSV), statically linked, ...
```

妫€鏌ユ槸鍚﹂潤鎬侀摼鎺ワ細
```bash
ldd bbsd
```

搴旇鏄剧ず锛�
```
not a dynamic executable
```

## 璋冭瘯

鎵€鏈夊彲鎵ц鏂囦欢鍖呭惈瀹屾暣璋冭瘯绗﹀彿锛屽彲浠ヤ娇鐢� gdb锛�

```bash
# 鍦� x86 寮€鍙戞満涓婏紙浣跨敤浜ゅ弶 gdb锛�
mipsel-linux-gnu-gdb bbsd

# 鍦� MIPS 璁惧涓�
gdb bbsd
```

## 鏁呴殰鎺掓煡

### 鏋勫缓澶辫触

1. 鏌ョ湅 Actions 鏃ュ織涓殑閿欒淇℃伅
2. 妫€鏌ユ瘡涓瀯寤烘楠ょ殑杈撳嚭
3. 楠岃瘉渚濊禆搴撴槸鍚︽垚鍔熺紪璇�

### 鎺ㄩ€佸け璐�

濡傛灉浣跨敤 HTTPS 涓旈渶瑕佽璇侊細

```bash
# 浣跨敤 gh CLI 鐧诲綍
gh auth login

# 鎴栭厤缃� Git 鍑瘉
git config --global credential.helper store
```

鎴栦娇鐢� SSH锛�

```bash
git remote set-url origin git@github.com:Yaka88/FireBirdBBS.git
```

### 浜岃繘鍒舵枃浠舵棤娉曡繍琛�

纭繚锛�
1. 鍦ㄦ纭殑 MIPS 骞冲彴涓婅繍琛岋紙32浣嶅皬绔級
2. 鏂囦欢鏉冮檺姝ｇ‘
3. 蹇呰鐨勭郴缁熺洰褰曞瓨鍦紙/home/bbs 绛夛級

## 鎶€鏈敮鎸�

濡傛灉閬囧埌闂锛�

1. 妫€鏌� GitHub Actions 鏃ュ織
2. 鏌ョ湅 BUILD-MIPSLE.md 璇︾粏鏂囨。
3. 楠岃瘉 MIPS 鐩爣骞冲彴瑙勬牸

## 棰勬湡杈撳嚭鏂囦欢

鏋勫缓鎴愬姛鍚庡皢鐢熸垚浠ヤ笅鏂囦欢锛�

### 涓荤▼搴�
- `bbsd` - BBS 涓诲畧鎶よ繘绋�
- `chatd` - 鑱婂ぉ瀹ゅ畧鎶よ繘绋�
- `thread` - 璁ㄨ涓插鐞嗙▼搴�
- `expire` - 杩囨湡鏂囩珷娓呯悊绋嬪簭

### 鍙€夋枃浠�
- `paging.so` - 鍛煎彨妯″潡
- util 鐩綍涓嬬殑鍚勭宸ュ叿绋嬪簭

鎵€鏈夋枃浠堕兘灏嗘墦鍖呭湪 `bbs-mipsle-binaries.tar.gz` 涓€�

---

**鍑嗗濂戒簡鍚楋紵杩愯 `./push-to-github.sh` 寮€濮嬫瀯寤猴紒**
