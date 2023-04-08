# 查看当前运行的线程数

ps -aL |   # L选项查看线程
head -1 && ps -aL |
grep myThread
