#!/user/bin/env python3
# coding=utf8

# Raspberry Pi to ATmega128 I2C Communication 
# Python Code 
 
import time 
import smbus 
 
# Slave Addresses for AVR
ARDUINO_1_ADDRESS = 0x014 # I2C Address of AVR 
 
# Create the I2C bus 
I2Cbus = smbus.SMBus(1) 
  
SlaveAddress = ARDUINO_1_ADDRESS 

# This function converts a string to an array of bytes. 
def ConvertStringToBytes(src): 
  converted = [] 
  for b in src: 
    converted.append(ord(b))
  # Appending CR code
  converted.append(13)
  return converted
  
while 1:
  out_data = input("Input Command: ")
  # Input Command에서 입력된 String을 Byte array 문자열로 변환 한다. 
  bytes_ch = ConvertStringToBytes(out_data)
  for i in range(len(bytes_ch)):
    # 문자(Byte)를 Slave에 전송 한다.
	# cmd = 0xa0 : 이 코드를 Slave에서 문자 수신 명령으로 해석 한다.
    I2Cbus.write_byte_data(SlaveAddress, 0xa0, bytes_ch[i])
    time.sleep(0.01)

  # 수신한 문자열을 저장할 변수
  out_str = ''
  # 최대 16자의 문자를 CR Code(13) 가 수신될 때 까지 수신 한다.
  for i in range(16):
    # 문자(Byte)를 Slave에서 수신 한다.
	# cmd = 0x20 : 이 코드를 Slave에서 문자 전송 명령으로 해석 한다.
    in_data = I2Cbus.read_byte_data(SlaveAddress, 0x20)
    out_str += chr(in_data)
    time.sleep(0.01)
    # CR Code(13) 가 수신되면 문자 수신을 종료 한다.
    if in_data == 13:
      break
  # 수신된 문자열을 putty 창(모니터)에 출력 한다.
  print('Received data: ' + out_str)

