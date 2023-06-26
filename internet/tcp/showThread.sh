# 循环查看当前运行的线程数  
# 监控脚本 sh XX.sh

while :; 
do 
ps -aL | 
head -1 && ps -aL |
grep tcp_server;
sleep 2; 
echo "-------------------------"; 
done

