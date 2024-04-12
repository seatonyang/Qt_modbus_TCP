from pymodbus.client.sync import ModbusTcpClient
from time import sleep

# Modbus TCP服务器的地址和端口
MODBUS_SERVER_IP = '192.168.0.150'
MODBUS_SERVER_PORT = 502
# 要读取的寄存器地址
REGISTER_ADDRESS = 0x0A

# 创建Modbus TCP客户端实例
client = ModbusTcpClient(MODBUS_SERVER_IP, port=MODBUS_SERVER_PORT)

try:
    # 连接到Modbus TCP服务器
    connection = client.connect()
    if connection:
        print("Connected to the Modbus server")
    else:
        print("Failed to connect to the Modbus server")
        exit(1)

    # 存储上次读取的寄存器值
    last_value = None

    client.write_register(REGISTER_ADDRESS, 0, unit=1)
    while True:
        # 读取0x0A寄存器的值
        result = client.read_holding_registers(REGISTER_ADDRESS, count=1, unit=1)
        if not result.isError():
            current_value = result.registers[0]
            if current_value == 1:
                print("切换相机")
            elif current_value == 2:
                print("测量")
            client.write_register(REGISTER_ADDRESS, 0, unit=1)
        else:
            print("Failed to read register 0x0A")

        # 等待一段时间再进行下一次读取
        sleep(1)

except Exception as e:
    print(f"Error: {e}")

finally:
    # 关闭连接
    client.close()
