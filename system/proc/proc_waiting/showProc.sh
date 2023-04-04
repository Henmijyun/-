# 自动循环查看进程的脚本
while :
do 
    ps ajx | 
    head -1 && ps ajx |
    grep myproc |
    grep -v grep 
    sleep 1
    echo "-----------------------------"
done



