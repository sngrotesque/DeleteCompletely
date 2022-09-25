# 文件彻底删除器
> SN-Grotesque

### ※ 整个代码使用GB2312编码，请勿使用其他编码打开或修改文件 ※

> 目前只将此程序作为Windows系统下的一般程序使用<br>
> 如需移植到Linux或Mac OS，请自行修改代码。

```text
由SN-Grotesque使用C语言编写

使用方式有两种
    1. 直接打开程序输入文件路径，程序自动帮你处理。
    2. 将需要删除的文件拖入程序，程序自动帮你处理。

程序的工作机制
    1. 检测文件是否正常
        无其他程序占用
        不是目录
    2. 读取文件大小并使用随机函数生成的数据覆盖文件
        覆盖次数：3次
        随机数据：3轮
    3. 将文件数据清空
    4. 将文件名重新命名为由随机函数生成的文件名
    5. 删除文件
```
