//------------------------------------------------------------------
//传感器定义
//只需在此文件定义就可以使用
//------------------------------------------------------------------
var Sensors = [
    //---------------------------------------------------CWT-TH01S定义开始
    {
        name: "CWT-THXXS",//在下拉中显示的名称
        img: "CWT-THXXS.jpg",//显示传感器的图片(imges中)
        slaveAddr: "",//默认传感器地址
        Registers: [
            {
                regName: "1: Humidity",
                regAddr: 1,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1
            },
            {
                regName: "2: Temperature",
                regAddr: 2,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1,
            },
            {
                regName: "3: temperature calibratizon",
                regAddr: 81,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1,
                write: true,
            },
            {
                regName: "4: Humidity calibratizon",
                regAddr: 82,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 0.1,
                write: true,
            },
            {
                regName: "5: Baud",
                regAddr: 2002,
                regType: Modbus.RegType.KEEP,
                valueType: Modbus.ValueType.Signed,
                valueScale: 1,
                write: true,
            },
        ],
        QueryAddrHex: "FF 03 07 D0 00 01 91 59",//查询地址的数据，如果注释掉则无读从机地址的按钮
        slaveRegAddr: 0x7D0,//从机的地址所在的寄存器
        displayBtnWriteAddr: 1,//值0/1,0表示无写从机地址的按钮;1表示有写从机地址的按钮
        Tips: "Red: DC12V  Black: GND  Yellow: A+  Green: B-",
    },
    //---------------------------------------------------CWT-TH01S定义结束

    //---------------------------------------------------Customize定义开始
    {
        name: "Customize",//传感器名称(下拉中显示的名称)
        img: "noimg.png",//传感器图片
        slaveAddr: "1",//默认传感器地址
        Registers: null,//寄存器定义(null可以输入，而不是下拉)
        QueryAddrHex: null,//查询地址的数据，如果注释掉则无读从机地址的按钮
        displayBtnWriteAddr: 1,//值0/1,0表示无写从机地址的按钮;1表示有写从机地址的按钮
        Tips: "",
    },
    //---------------------------------------------------Customize定义结束
];