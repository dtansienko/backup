import time 
import serial 
import keyboard

#constatns
UP = bytes((1).to_bytes(1, byteorder='big'))
DOWN = bytes((2).to_bytes(1, byteorder='big'))
RIGHT = bytes((3).to_bytes(1, byteorder='big'))
LEFT = bytes((4).to_bytes(1, byteorder='big'))

new_event_time = 0
old_event_time = 30

input_delay_time = 100 #in miliseconeds


ser = serial.Serial()
ser.baudrate = 57600
ser.port = '/dev/ttyUSB0'
ser.open()



def send_key_press(direction):
	#print('ger')
	#print(direction)
	ser.write(direction)
	
def current_mili_time():
	return round(time.time() * 1000)

while(True):

	
	if(keyboard.is_pressed('up') or keyboard.is_pressed('down') or keyboard.is_pressed('right') or keyboard.is_pressed('left')):
	
		new_event_time = current_mili_time()
		
		if((new_event_time - old_event_time) > input_delay_time): #manage input after delay met

			if(keyboard.is_pressed('up')):
				#print('der')
				send_key_press(UP);
				
			if(keyboard.is_pressed('down')):
				send_key_press(DOWN);
			if(keyboard.is_pressed('right')):
				send_key_press(RIGHT);
			if(keyboard.is_pressed('left')):
				send_key_press(LEFT);
				
			old_event_time = current_mili_time()
	

	
