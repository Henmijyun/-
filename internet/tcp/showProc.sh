# 循环查看当前运行的进程数  
# 监控脚本 sh XX.sh

while :; 
do 
ps ajx | 
head -1 && ps ajx |
grep tcp_server |
grep -v grep;
sleep 2; 
echo "-------------------------"; 
done

