# GameShell_Joypad
一, 介绍：
    这个是一块新的Gameshell的按键模块。用switch的摇杆更换了原先的方向键并且多出了个摇杆z轴按键。优点是可以直接平替原先的按键模块keypad无需对主机进行任何程序的更改。

二, 复刻前准备：
    一双灵巧的双手，一个清醒的脑瓜子，风枪，电烙铁，镊子，一台Gameshell等。

三, joypad硬件制作过程：
    1. 首先下载github库里的pcb文件夹里的.xlsx文件下来去某宝购买物料。
    2. 下载github库里的pcb文件夹里的.zip文件去嘉立创免费打样。
    3. 将买到的物料按照物料表进行焊接。
    
四, 将程序上载至joypad：
    1. 在电脑上下载Arduino IDE。
    2. 下载GameShell_Joypad/Arduino/下的两个文件夹里的全部文件，UsbKeyboard为必须引入的库文件，另一个joypad_x.x.x文件夹为joypad主程序。
    3. 先导入UsbKeyboard库，再打开joypad主程序选择板子端口和板子型号为arduino uno上传主程序。
    
五, 校准摇杆：
    1. 将lightkey接入joypad，同时按下L2和R2键持续五秒钟，当两个板载LED从闪烁变为停止闪烁时校准完成。按键按住触发校准的校准时间可以在joypad的主程序中修改。
    2. 注意：
        （1）如果校准完成后将板子接入Gameshell主板时按键出现无法使用的现象，请重新连接主板与joypad。
        （2）理论上可以将joypad同时连接电脑和Gameshell主板，但是不建议这么做，因为貌似串口会出现冲突导致按键无法使用的现象，需要将joypad与Gameshell主机重新连接方可解决。

六， 使用：
    1. 可以直接将原版的keypad用joypad替换装到Gameshell内，但是要注意要用巧劲使摇杆的头穿过Gameshell的前面板进行安装，否则可能会导致摇杆漂移的现象发生，具体为Sameshell会自动触发等现象。
