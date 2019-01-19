#!/bin/sh
# 使用此 script 前，請先確認已經修改了下面的相關設定

# 請在這邊修改成自己的設定
#
# 以下名稱設定中皆不能有 &`"\ 等符號
#
#   (1) 不能有 & 符號，因為這是背景執行的保留符號
#   (2) 不能有 ` 符號
#   (3) 除了前後的 "" 以外，不能再有其他的 " 符號
#   (4) 不能有 \ 符號，因為 Big5 編碼的問題，以下這些字也不能使用
#       兝 α 么 功 吒 吭 沔 坼 歿 俞 枯 苒 娉 珮 豹 崤 淚 許 廄 琵
#       跚 愧 稞 鈾 暝 蓋 墦 穀 閱 璞 餐 縷 擺 黠 孀 髏 躡 ? ? 尐
#       佢 汻 岤 狖 垥 柦 胐 娖 涂 罡 偅 惝 牾 莍 傜 揊 焮 茻 鄃 幋
#       滜 綅 赨 塿 槙 箤 踊 嫹 潿 蔌 醆 嬞 獦 螏 餤 燡 螰 駹 礒 鎪
#       瀙 酀 瀵 騱 酅 贕 鱋 鱭
#
# 如果您希望站名等設定內有 &`"\ 等符號的話，請先隨便給個設定，
# 事後再去改 src/include/config.h 即可


schoolname="Steamgay"
bbsname="人類的本質"
bbsname2="Human Nature"
sysopnick="複讀機"
myipaddr="255.255.255.255"
myhostname="local.bbs"
msg_bmw="水球"

# 請修改您的作業系統
# sun linux solaris sol-x86 freebsd bsd

# ostype="freebsd"
ostype="linux"

echo "您所設定的 SCHOOLNAME 是 $schoolname"
echo "您所設定的 BBSNAME    是 $bbsname"
echo "您所設定的 BBSNAME2   是 $bbsname2"
echo "您所設定的 SYSOPNICK  是 $sysopnick"
echo "您所設定的 MYIPADDR   是 $myipaddr"
echo "您所設定的 MYHOSTNAME 是 $myhostname"
echo "您所設定的 MSG_BMW    是 $msg_bmw"
echo "若您有多個 FQDN (Eg: twbbs)，則還需手動修改 src/include/config.h 的 HOST_ALIASES"

echo "您的作業系統          是 $ostype"

echo "您可以在 http://processor.tfcis.org/~itoc 找到最新的程式及安裝文件"

# 回 BBSHOME
cd
echo "[1;36m進行轉換中 [0;5m...[m"


# 換名稱 ip addr

filelist_1="etc/valid src/include/config.h"

for i in $filelist_1
do
  cat $i | sed 's/台南一中/'"$schoolname"'/g' > $i.sed
  mv -f $i.sed $i

  cat $i | sed 's/與南共舞/'"$bbsname"'/g' > $i.sed
  mv -f $i.sed $i

  cat $i | sed 's/WolfBBS/'"$bbsname2"'/g' > $i.sed
  mv -f $i.sed $i

  cat $i | sed 's/狼人長老/'"$sysopnick"'/g' > $i.sed
  mv -f $i.sed $i

  cat $i | sed 's/210.70.137.5/'"$myipaddr"'/g' > $i.sed
  mv -f $i.sed $i

  cat $i | sed 's/bbs.tnfsh.tn.edu.tw/'"$myhostname"'/g' > $i.sed 
  mv -f $i.sed $i
done


# 換 水球

filelist_2="etc/tip \
src/include/config.h src/include/global.h src/include/modes.h \
src/include/struct.h src/include/theme.h src/include/ufo.h src/maple/CHANGE \
src/maple/acct.c src/maple/bbsd.c src/maple/bmw.c src/maple/menu.c \
src/maple/pal.c src/maple/post.c src/maple/talk.c src/maple/visio.c \
src/maple/xover.c"

for i in $filelist_2
do
  cat $i | sed 's/水球/'"$msg_bmw"'/g' > $i.sed
  mv -f $i.sed $i
done


# 安裝 Maple 3.10
echo "[1;36m安裝 BBS 中 [0;5m...[m"
cd src
make clean $ostype install

# 啟動
# 假設開在 port 9987
cd
bin/bbsd 9987
bin/camera
bin/account

# telnet 測試
telnet 0 9987
