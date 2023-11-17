#!/usr/bin/env python3
import rospy
import smbus
import struct
from std_msgs.msg import Int32
from sensor_msgs.msg import JointState
from robot_control.msg import encoder_data, velocity_data
from time import sleep

# Autores: Matheus Paiva Angarola e William

# Constantes
ESP32_ADDRESS = 0x08  # Endereço do dispositivo ESP32 no barramento I2C
I2C_BUS = 1  # Número do barramento I2C no Raspberry Pi
REG_ADDRESS = 10  # Endereço de registro (offset) a ser usado pelo PID da direita
WAIT_TIME_SECONDS = 2  # Tempo de espera entre leituras/escritas (em segundos)

# Classe para comunicação I2C
class I2CCommunication:
    # "Construtor" da classe, definindo seus atributos principais
    def __init__(self):

        self.left_wheel_velocity = 0
        self.right_wheel_velocity = 0

        self.i2c = smbus.SMBus(I2C_BUS)  # Define o barramento que será usado na comunicação
        self.device_address = ESP32_ADDRESS  # Define o endereço da ESP32 ao qual queremos nos comunicar

        # Cria um objeto de publicação para enviar dados para o tópico ROS
        self.pub_encoder = rospy.Publisher('/encoder_data', encoder_data, queue_size=10)
        self.sub_joints = rospy.Subscriber('/velocity_command', velocity_data, self.joints_callback)

        self.encoder_msg = encoder_data()

        self.encoder_msg.left_encoder_data = 0
        self.encoder_msg.right_encoder_data = 0
        
        self.update()

    def read_data(self):
        try:
            data = self.i2c.read_i2c_block_data(self.device_address, REG_ADDRESS, 8)  # Faz a leitura da ESP32

            value_right = struct.unpack('!i', bytes(data[:4])) 
            value_left = struct.unpack('!i', bytes(data[4:])) 

            #rospy.loginfo(f'Valor lido: {value_left}, {value_right}')

            enc_msg = encoder_data()

            enc_msg.left_encoder_data = value_left[0]
            enc_msg.right_encoder_data = value_right[0]

            self.pub_encoder.publish(enc_msg)

        except Exception as e:
            rospy.logerr(f"Erro na leitura: {str(e)}")
            return None

    def write_data(self):
        try:

            a = 50
            b = 50

            data = struct.pack('!ii', a, b)  # "Empacota" o valor mandado como parâmetro da função
            self.i2c.write_i2c_block_data(self.device_address, REG_ADDRESS, list(data))  # Escreve valor para a ESP32
            
            #rospy.loginfo(f'Valor enviado: {self.left_wheel_velocity}, {self.right_wheel_velocity}')

        except Exception as e:
            rospy.logerr(f"Erro na escrita: {str(e)}")

    def joints_callback(self, msg):

        joint_msg = velocity_data()
        joint_msg = msg

        if joint_msg is not None and joint_msg.front_left_velocity != 0.0:

            self.left_wheel_velocity = int(joint_msg.front_left_velocity * 100)
            self.right_wheel_velocity = int(joint_msg.front_right_velocity * 100)

        else:
            return None

    def update(self):

        rate = rospy.Rate(10)  # 10Hz

        while not rospy.is_shutdown():
            self.read_data()
            self.write_data()

            rate.sleep()


if __name__ == "__main__":
    try:
        rospy.init_node('i2c_master', anonymous=True)
        i2c_communication = I2CCommunication()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
