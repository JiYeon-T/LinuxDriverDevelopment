
1. free
查看內存使用情況

2. rpm (red hat package management)
rmp --veriosn
RPM version 4.12.0.1
rpm -ivh filename.rpm # install
rpm -Uvh filename.rpm # Update
rpm -e filename.rpm # uninstall 
rpm -qpi filename.rpm # query software information
rpm -qpl filename.rpm # list query inforamtion
rpm -qf filename # 查詢文件屬於那個rpm

3. serarch file
which rpm    # /usr/bin/rpm
which yum 	# /usr/bin/yum，系統默認的可執行文件都放在這個目錄下面

4. Yum 軟件倉庫（Yum有自己的yum 軟件服務器）
yum replist all 	# 查看所有的倉庫
yum list all 	# 查看倉庫中所有的軟件
yum info xxx 	# 查看軟件包信息
yum install xxx 	# 安裝軟件包  
yum reinstall xxx	# 重新安裝
yum update xxx		#升級安裝包
yum remove xxx		# 卸載安裝包
yum clean all		# 清楚所有的倉庫緩存
yum check-update	# 檢查可更新的軟件 
yum grouplist		# 查看系統中已經安裝的軟件包組
yum groupinstall	#
yum groupremove		# 
yum groupinfo		#

5. Linux 操作系統的初始化過程：
RHEL5 或者 RHEL6 系統：初始化進程服務， System V init
RHEL7 系統替換掉了 Sytem V init, 使用 systemd初始化進程服務
Linux 操作系統的開機過程：BIOS -> Boot Loader -> 加載系統內核 -> 內核初始化 -> 最後啓動初始化進程 -> 初始化進程作爲Linux系統的第一個進程，需要完成Linux系統中相關的初始化工作；
參考FreeRTOS操作系統的初始化，過程都類似；
Linux 在啓動的時候進行了大量的初始化工作，比如：掛載文件系統分區，啓動各類進程服務，

6. Service
RHEL6 使用 service, chkconfig 等命令來管理系統服務
RHEL7 使用 systemctl 命令來管理服務
RHEL6				RHEL7
systemctl 管理服務的啓動，重啓，停止，重載，查看狀態
service foo start -> systemctl start foo.service
service foo restart -> systemctl restart foo.service
service foo stop -> systemctl stop foo.service
service foo reload -> systemctl reload foo.service
service foo status -> systemctl status foo.service, systemctl status(查看所有服務的狀態)

systemctl 服務的開機啓動，不啓動，查看各個登記下的服務啓動狀態
chkconfg foo on -> systemctl enable foo.service # 開機自動啓動
chkconfig foo off -> systemctl disable foo.service # 開機不自動啓動
chkconfig foo -> systemctl is-enabled foo.service # 查看特定服務是否是開機啓動狀態
chkconfig --list -> systemctl list-unit-files --type=service # 查看各個級別下服務的啓動與靜止情況

7. 通用命令
passwd # 修改密碼
who 	# 查看用戶信息
hostname # 追擊名
Linux 切換終端是什麼意思？
Ctrl + Alt + F1~F7(F7 是切換到圖形界面)
系統內核與shell終端之間的關系, bash 解釋器。
GUI界面消耗資源（甚至不夠靈活），所以通常Linux服務器運維工作都不安裝UI界面，

9. Linux 常用命令
man -h
man --help 	# manual, 幫助手冊 
×× 系統內核對於操作系統來說釋放你的重要，因此，一般不建議直接修改內核的參數，而是讓用戶通過基於系統調用借口開發出的程序來管理或服務來管理計算機，以滿足日常工作中的需要 ××
man sprintf() 	 

man 中的操作:文本操作
space 		翻頁
page down	下翻
page up		上翻
home		回到首頁
end 		到尾頁
/		從上之下搜索某個關鍵字： n/N 下一個
?		從上之下搜索某個關鍵字

echo		# 打印, eg: echo $SHELL

date		# 獲取以及設置時間
date "+%Y-%m-%d %H:%M:%S， %j" # 前面添加 + ，則設置時間， %j-表示一年中的第幾天
reboot 		# 重啓（涉及硬件資源的管理權限，需要管理員權限）
poweroff	# 這類軟件
wget		# 用於下載網絡中的文件, eg:wget -b https://www.baidu.com，下載整個網頁
eg2: wget http://www.linuxprobe.com/docs/LinuxProbe.pdf # 直接獲取網站中的資源文件
-b	# 後臺下載
-P 	# 下載到制定目錄
-t	# 最大嘗試次數
-c	# 斷點續傳
-p 	# 下載頁面中的所有內容
-r	# 遞歸下載所有內容， eg: wget -r -p http://www.linuxprobe.com

查看系統中所有進程狀態
ps	# eg: ps -a -u -x , 獲取 ps aux
參數：
-a	# 查看所有進程（包括其他用戶的進程）
-u	# 顯示用戶以及其他詳細信息
-x 	# 顯示沒有控制終端的進程

NOTE: Linux命令有長格式與短格式之分，短格式與短格式之間可以合並
eg: 
ps -a -u -x -> ps aux
tar -zxvf

top 	# 動態查看系統中的進程運行情況（強化版的windows任務管理器）,前5行可以查看到系統整體的使用情況，如：
CPU使用情況，內存使用情況，運行進程數目等等

pidof	# 獲取進程 PID, 唯一的號碼
pidof sshd
pidof bash

kill	# 終止指定 pid 所代表的進程
eg:變異一個 a.out 可執行文件
pidof a.out
kill 4649 	# 結束進程

killall 	# 通常某個復雜的服務程序可能有多個進程協同爲用戶提供服務，killall 結束某一個服務有關的全部進程
eg:
pidof httpd
killall httpd

可以通過 ctrl + C 來結束這個進程，還可以通過在執行命令結尾加上 & 來讓進程後臺運行
./build.sh &

系統命令：
ifconfig	# 查看網卡信息：
uname		# 查看系統信息，如：內核名，主機名，內核版本，節點名，系統時間，硬件平臺，處理器類型...
uname --help
uname -a	# all
cat /etc/redhat-release	# 對於“紅帽”，查看系統版本的詳細信息
uptime		# 查看系統運行時間，以及負載情況
free		# 查看系統內存使用情況, free --help
who		# 顯示正在登入這臺主機的用戶信息以及他們開啓的終端情況
who -a		# who --help
last 		# 查看主機的歷史登錄記錄，這個記錄是保存在系統日志中，不可以通過這個來判斷是否有黑客入侵
history 顯示歷史記錄 # 具體可以在　/etc/profile 文件中定義變量 HISTSIZE 來修改保存歷史記錄的個數, 歷史記錄保存在　~/.bash_history 文件中, Linux 中以　.　開頭的文件都是隱藏文件（這些大都是系統服務文件）
history --help
history -c 	# clear
!2013		# ! + 命令索引的方式來重復執行某一條命令
###### 那　git push 的時候不是方便多了？
sosreport 	# 系統出現問題時，可以通過 sosreport 提前將部分問題信息發給技術人員
sosreport --help
sudo agt-get install dlm
sosreport -o dlm -k dlm.lockdump

##### 目錄切換
pwd		# present word directory
cd -		# 返回上次所在的目錄
cd ..		# 返回上一級目錄
cd ~		# 切換到當前用戶的家目錄
cd ~whz		# 切換到其他用戶的家目錄
ls		# 
ls --help
-d, --directory # list directories themselves, not their contents
ls -ld /etc	# 列出某一個文件夾的信息，不包含他的子目錄

##### 查看文本以及文本編輯
cat		# 查看內容比較少的純文本文件
cat -n test.cpp	# display line number
more		# 閱讀比較打的文本文件
head -n 20 test.cpp	# 查看文本文件的前　n 行
tail -n 20 test.cpp	# 末尾　n 行, tail 可以動態刷新內容，可用於查看系統日志等功能
tail -f /var/log/message
tr [OPTION] SET1 [SET2]		# 文本替換，eg: cat test.cpp | tr [a-z] [A-Z]
wc [-c -l -w]		# 可以用於統計文本文件的行數,單詞數，字節數等(word counter)
stat		# 用於查看文件的創建時間，修改時間，文件大小等信息
cut		# 選擇要查看的文本內容
cut -d: -f1	/etc/passwd	# 僅僅顯示一列; 分隔符:
diff		# 比較兩個文本文件之間的差異
diff --brief a.txt b.txt　# 僅僅顯示是否不同
diff -c a.txt b.txt

##### 對文件的操作
touch 		# -a, -m, -d　修改文件讀取時間，修改時間
touch -d　"2001-1-1 3:11" test.cpp
mkdir 		# 創建文件夾
mkdir -p	# 地櫃創建文件夾
mkdir -p a/b/c/d/e/f/g/h
cp		# 拷貝文件
-p		# 保留源文件屬性
-d		# 若對象爲鏈接文件，則保留該“鏈接文件”屬性
-r		# 遞歸復制
-i		# 若文件存在，則詢問是否覆蓋
mv		# 剪切文件或者重命名
eg:mv test.cpp class_test.cpp
rm 		# 刪除文件或目錄
dd		# 按照指定的大小復制或者轉換文件
eg: dd if=/dev/zero of=560_file bs=560M count=1
file		#　由於 Linux 文件，目錄，設備等都叫做文件，可以通過 file 來判斷具體類型

###### 文件壓縮與解壓（相同網速情況下，小體積的壓縮文件傳輸時間更短，格式：.tar, .tar.gz, .tar.bz2）
tar
-c 		# 創建壓縮文件
-x		# 解開壓縮文件
-t		# 查看壓縮包中有什麼壓縮文件
-z		# 使用　Gzip 壓縮或者解壓
-j		# 使用　bzip2 壓縮/解壓
-v 		#　顯示壓縮/解壓過程
-f		# 目標文件名
-p		# 保留原始文件屬性
eg:　
tar -czvf test.tar.gz ./	# 壓縮文件到　test.tar.gz
tar -xzvf test.tar.gz -C ./test # 解壓到　test　目錄

grep		# 用於在關鍵詞中搜富, man grep, grep --help
-b		# 將可執行文件當做二進制文件搜索
-c		# 僅僅顯示找到的行好
-i		# 忽略大小寫
-n		# 顯示行號
-v		# 反向選擇，顯示沒有字符的行
Linux 系統中，/etc/passwd 下保存這所有的用戶信息，兒一旦用戶登錄的終端被設置成　/sbin/nologin ，則不在允許登錄系統；
eg: grep /sbin/nologin	/etc/passwd

find		# 用戶按照文件名、大小、修改時間、權限等信息查找匹配的文件f
-name		# 匹配名稱
-perm		# 匹配權限(mode 爲完全匹配, -mode包含即可)
-user		# 匹配所有者
-group		# 匹配所有組
-mtime -n +n	# 匹配修改時間(-n n　天以內, +n n 天以前)
-atime -n +n  	# 匹配訪問時間
-ctime -n +n 	# 匹配修改文件權限的時間
-nouser		# 匹配沒有所有者文件
-nogroup	# 匹配沒有所有組的文件
-newer f1 !f2	# 匹配比　f1　新但是比 f2 就的文件
--type b/d/c/p/l/f	# 按文件類型匹配
-size		# 按照文件大小進行匹配, +50KB 表示查找超過　50KB 的文件
--prune		# 忽略某個目錄
-exec ....... {} \ # 後面可以跟用於進一步處理搜索結果的命令
eg:
man find
find --help
find /etc -name "yum" -print
find /etc -name "host*" -print	# 可以使用正則表達式的方式進行匹配
find / -perm -4000 -print	# -4000 表示匹配權限中包括　SUID 的所有文件
-exec {} \ # {} 表示搜索到的每一個文件, 該命令的結尾必須是　\
find / -user linuxprobe	-exec cp -a {} /root/findresults/ \

which 用於搜索
whereis 
ls /bin/	# 用於搜索　bin 目錄下面都支持哪些命令
ls /bin		# 查看所有的 Linux 命令

##### 管道福, 重定向，　與環境變量
輸入重定向就是把文件輸入到命令中，而輸出重定向就是把命令輸出到文件；
輸入設備：命令, 文件, 按鍵
輸出設備：屏幕，文件
輸入重定向：
命令 < 文件
命令 << 分界符 	# 從標準輸入讀取, 知道遇到分界符才停下
命令 < 文件１　> 文件2		# 輸入使用文件1, 輸出到文件2
eg: 
man bash > bash_help.txt
cat bash_help.txt
輸出重定向:
command > file 		# 標準輸出重定向到文件並覆蓋原來內容
command 2> file 	# 錯誤輸出
command >> file		# 標準輸出重定向到一個文件中, 並且覆蓋原來內容
command 2>> file	# 將錯誤輸出重定向到一個文件中, 並且覆蓋原來內容
command >> file 2>&1 或者　command &>> file # 將標準輸出與錯誤書蟲共同寫入到文件（追加）
eg:
man ls &> help_ls.txt
man a &>> help_ls.txt # append
2. 如何吧錯誤信息保存到文件中(log)，而不是在終端顯示，這種方法在自動化腳本中很有用
ls -l xxxx > log.txt
ls -l xxxx 2>> log.txt
3. 輸入重定向的使用, 統計文本文件的行數
wc -l < help_ls.txt 	# cat help_ls.txt | wc -l

管道運算符, 把前一個命令的輸出作爲有一個命令的輸入
eg:
histoy | grep clone # 等效與 history > history.txt && grep clone < history.txt
grep "sbin/nologin" /etc/passwd	| wc -l
ls -l /etc | more	# 使用 more 更方便
echo "linuxprobe" | passwd --stdin root	# 修改密碼
#　滿裝郵件有關的內容
sudo apt-get install mailutils
echo "Conten" | mail -s "Subject" whz	# 發送郵件
echo "This is a test mail" | mail -s "Subject" qz # 自己給自己發送郵件
su - qz	# 獲取權限

# 通配符
NOTE:　硬盤設備文件都是以　sda 開頭並且保存在　/dev/　目錄下, 
*		# 匹配　0　個　或者　多個字符(0 - n　個)
?		# 代表匹配單個字符(只能是一個)
[0-9]		# 匹配 0-9 之間的單個字符
[abc]		# 匹配　abc 中的任意一個字符
ls -l /dev/sda*		# 通配符的使用
ls -l /dev/sd?
ls -l /dev/sd*
ls -l /dev/sd[a-z]?
ls -l /dev/sd[a-z]*

#　轉移字符
\		# 是變量變成單純的字符串
''		# 所有變量都變成單程的字符串
""		# 保留變量屬性不進行轉移處理
``		# 把其中的命令執行後返回結果
PRICE = 5
echo "price is $PRICE"
echo "price is \$$PRICE"
echo "SHELL path is:$SHELL"
echo `uname -a`	#如果需要某個命令的執行結果，則使用　``(反引號)

# 環境變量定義了系統運行的一些參數, 比如每個用戶的家目錄, 郵件存放地址等
env | wc -l		# 查看系統所有的環境變量
當用戶從 shell 輸入一條命令後系統會進行的操作:
(1)判斷是否以絕對路徑方式輸入命令;
(2)判斷是否是個別名命令;	# 可以使用 alias 設置 與　取消　unalias
(3)bash 解釋器判斷用戶輸入的是內部命令還是外部命令; 可使用 type command 來判斷是內部還是外部;
(4)系統在所有的環境變量中超照, 是否能找到這個命令;
eg:
alias rm='rm -i'	# 設置別名
alias rm		# 查看
unalias rm		# 取消設置別名

常用的環境變量:
echo $HOME
echo $SHELL
echo $HISTSIZE		# shell 保存的最大歷史命令個數
echo $MAIL		# 郵件保存路徑
echo　$LANG		# 系統語言
echo $RANDOM		# 生成一個隨機數, echo "random number: $RANDOM" 
echo $PS1		# bash 計時器的提示符
echo $PATH
echo $EDITOR		# 系統默認編輯器

Linux 是一個多用戶多任務的操作系統, 能偶爲每一個用戶提供合適的、獨立的運行環境.因此,一個相同的變量,在不同的用戶去運行返回的結果也不相同。
su		# 切換用戶身份
su - whz
echo $HOME	# 變量不同

變量實際上有固定的變量名與系統的設置的變量值兩部分組成，用戶可以自己定義
WORDDIR = /home/workdir
cd $WORKDIR	# 這種變量通常僅僅當前用戶可見，　切換到其他用戶則不可用, su - whz
export WORKDIR  # 如果想讓其他用戶頁可用, 則可以使用　export, 是的全局可見

# VIM 與　shell 腳本()
(1)編輯文檔;
(2)配置主機名, 網卡參數, 以及　yum 軟件倉庫;
(3)通過　at　命令 與　crond 計劃任務服務分別實現一次性的系統任務配置以及長期的任務配置;
dd		# 剪切(刪除)光標所在的鄭航, 命令模式
5dd		# 剪切()
yy		# 復制
5yy		# 復制　5 行
n		# 顯示搜索的下一個位置
N		# 顯示搜索的上一個位置
u		# 撤銷上一不操作
p		# 粘貼
ESC　進入末行模式
:q		# 退出
:q		# 退出
:q!		# 強制退出(放棄修改)
:wq		# 保存修改退出
:wq!
:set nu		# 顯示行號
:set nonu	# 不顯示
:命令		# 執行命令
:整數		# 調到改行
:s/one/two	# 將所在光標的行的第一個　one 替換成　two
:s/one/two/g	# 將光標所在行的所有　one 替換成　two
:%s/one/two/g	#　將全文所有的　one 替換成　two
?字符串		# 從上至下搜索
/字符串		# 從下至上搜索
eg:
vi /etc/hostname # 修改主機名
hostname	# 查看當前主機名, 又是配置完要重啓才會生效
網卡配置文件保存在　/etc/sysconfig/network-scripts
vi /etc/sysconfig/network-scripts　# /etc/network 目錄
ifconfig	# 查看網卡配置情況
systemctl start network # 配置好後, 重啓網絡服務

yum 軟件倉庫的配置
cd /etc/yum.repos.d	# /etc/yum.repos.d 保存着 yum　的配置文件, /etc/yum/repos.d/　目錄
rhel7.repo	# 配置文件
按照配置參數的路徑掛在光盤,並把光盤信息,  寫入到　/etc/fstab 文件中
mkdir -p /media/cdrom
mount /dev/cdrom /media/cdrom	# 掛在　cdrom
vi /etc/fstab
yum install httpd	# 使用 yum 安裝軟件

su - whz
cd WORKDIR

# shell 腳本
bash test.sh 	# 執行腳本
./test.sh	# 輸入完整路徑來執行腳本, 不可以直接運行, 因爲沒有權限
chmod u+x test.sh # 增加可執行的權限就可以運行
./test.sh




































































































































　





































































































