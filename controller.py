# coding:utf-8
import tkinter as tk
from tkinter import ttk
import serial
import time

duty = {"1":"F015","2":"F040", "3":"F050", "4":"F060","5":"F070", "6":"F080","7":"F090", "8":"F100","9":"F110","10":"F120",\
 "0":"S000",\
"-1":"B015","-2":"B040", "-3":"B050", "-4":"B060","-5":"B070", "-6":"B080","-7":"B090", "-8":"B120"}

#Arduinoのデバイス名を指定
#ser = serial.Serial('/dev/arduino_uno', 9600)
#ser = serial.Serial('/dev/ttyACM0', 9600)
ser = serial.Serial('COM5', 9600)

class Application(tk.Frame):

    def __init__(self, master=None,speed=0):
        super().__init__(master)
        master.title("Arduino N-controller")
        master.bind('<Up>', self.forward)
        master.bind('<space>', self.stop)
        master.bind('<Down>', self.backward)
        self.create_widgets(master,speed)

    def create_widgets(self,master,speed):
        super().__init__(master)
        frame = ttk.Frame(master, padding=10)
        frame.pack()

        self.name_area = tk.Label(frame, text='Arduino N-Controller', relief=tk.FLAT, bd=2)
        self.name_area.pack()

        self.text_area = tk.Label(frame, text=speed, width="80",relief=tk.FLAT, bd=2,background="white")
        self.text_area.pack()

        self.space_area = tk.Label(frame, text='', relief=tk.FLAT, bd=2)
        self.space_area.pack()

        self.photo_button = tk.Button(frame,text="NOTCH UP ↑", width="80", command=self.forward)
        self.photo_button.pack()

        self.photo_button = tk.Button(frame,text="STOP", width="80", command=self.stop)
        master.bind('f', lambda event: self.func())
        self.photo_button.pack()

        self.photo_button = tk.Button(frame,text="NOTCH DOWN ↓", width="80", command=self.backward)
        self.photo_button.pack()
        return

    def send_command(self,notch):
        print("notch ===> " + str(notch))
        print("arg ===> " + str(duty[str(notch)]))
        arg = str(duty[str(notch)]) + ";"
        arg_byte=arg.encode('utf-8')
        ser.write(arg_byte)
        return

    def forward(self, event=None):
        print("NOTCH UP")
        notch = self.text_area["text"]
        if notch < 10:
            notch = notch + 1
        self.text_area["text"] = notch
        self.send_command(notch)
        return

    def stop(self, event=None):
        print("STOP")
        self.text_area["text"] = 0
        notch = 0
        self.send_command(notch)
        return

    def backward(self, event=None):
        print("NOTCH DOWN")
        notch = self.text_area["text"]
        if notch > -8:
            notch = notch - 1
        self.text_area["text"] = notch
        self.send_command(notch)
        return



def main():
    root = tk.Tk()
    root.geometry("300x200")
    app = Application(master=root)
    app.mainloop()

if __name__ == '__main__':
    main()
