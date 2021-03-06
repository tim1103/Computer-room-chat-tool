# Computer-room-chat-tool

基于<https://www.luogu.com.cn/discuss/show/272544>进行部分修改。

# 使用说明(复制于 <https://www.luogu.com.cn/discuss/show/272544> )

### 介绍区

C++ chatRoom 是一个纯 C++ 打造的 windows-兼容(?) 聊天室。目前拥有普通聊天室和 24 点聊天室两种服务器形式。客户端代码兼容任意一种服务器，也就是说可以通过编写服务器代码改变聊天室功能。

~~其实就是抄了网上的 winsock 代码然后改改~~

* * *

### 图示

布局请以第二张图片为准。

![](https://s3.ax1x.com/2020/11/21/D12Q3T.png)

![](https://s3.ax1x.com/2020/11/21/D12Jb9.png)

* * *

### 代码区

注意：请打开 -lwsock32 开关进行编译。操作同打开 -std=c++11 -O2 开关。

客户端：[https://www.luogu.com.cn/paste/f2jvabpy](https://www.luogu.com.cn/paste/f2jvabpy)

纯聊天室：[https://www.luogu.com.cn/paste/upbhqkzt](https://www.luogu.com.cn/paste/upbhqkzt)

24 点聊天室：[https://www.luogu.com.cn/paste/bg9jcly3](https://www.luogu.com.cn/paste/bg9jcly3)

* * *

### 使用方法区

编译代码后，服务器侧程序要求输入端口和聊天室名字。输入完之后可以进入面板，随后不可进行控制（也就是目前没有管理员权限一说）。客户端首先输入服务器的 IP，然后输入端口和用户名即可进入聊天室。服务器可以使用多个端口打开多个聊天室，用户也可以使用多个程序进入多个聊天室。

目前实现的功能：

`/cls` 清屏

`/(w|s|a|d)` 配合上下左右，分别显示前面、后面、最早、最晚的消息

`/r` 重新构建窗口，防止刷屏带来的混乱（无法解决）

`@username` at 一个人，对方将会以黄底黑字显示 `@username` 段

`/exit` 离开聊天室

在 24 点服务器中：

`/start` 新建问题或者查看当前的问题

`?answer` 提交答案

* * *

### 说明

聊天室设有 ban 人系统。当一个人过快发言超过三次则发出提醒。超过五次则封禁一段时间，在 24 点聊天室中清空分数。

24 点分数模仿乐多赛制，满分 10 分，在提交第三次开始每次减 10% 直到低于 50%（答对不计）。

用户可以发送的消息长度建议不要超出框，影响布局。

如果客户端排版出现问题，请在 `MessageBox` 的 `printSide` 函数中将里面的两条杠改为一条，如果全是问号则阅读剪贴板代码进行对应修改。

* * *

### 展望

我们将会以这份代码为基底开发简单比赛系统，但是由于语言本身的限制，我们可能无法实现代码编辑器，所以需要用户在自己的代码编辑器上进行编辑。

电脑间的连接不需要外网，因此可以在断开外网后通过局域网连接。

同时，我们可能会加入管理员系统，方便管理员进行成员操作。

* * *

### 反馈……？

如果有任何问题请在保证 C++ 语言可以实现的情况下提出，我们将会进行修复。

~~啊又是肝到掉头发的一天。~~
