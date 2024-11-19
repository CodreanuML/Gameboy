from BLE_client import BLEManager
import time
import asyncio


device_address = "58:BF:25:9C:78:FE"

device_complete_address="58:BF:25:9C:78:FE: Gameboy"

device_services={
    "Buttons":"19B10001-E8F2-537E-4F6C-D104768A1214" ,
    "Gamedetails":"29B10000-E8F2-537E-4F6C-D104768A1214" , 
} 

device_caracteristics={
   "Button_OK":"19B10001-E8F2-537E-4F6C-D104768A1214",
   "Button_NOK":"19B10002-E8F2-537E-4F6C-D104768A1214",
   "Button_UP":"19B10003-E8F2-537E-4F6C-D104768A1214",
   "Button_DOWN":"19B10004-E8F2-537E-4F6C-D104768A1214",
   "Button_LEFT":"19B10005-E8F2-537E-4F6C-D104768A1214",
   "Button_RIGHT":"19B10006-E8F2-537E-4F6C-D104768A1214",  

   "POZITIE_MINGE_X": "29B10001-E8F2-537E-4F6C-D104768A1214",
   "POZITIE_MINGE_Y": "29B10002-E8F2-537E-4F6C-D104768A1214",
   "DIRECTIE_MINGE": "29B10003-E8F2-537E-4F6C-D104768A1214" ,
   "DEVITATIE_MINGE": "29B10004-E8F2-537E-4F6C-D104768A1214",
   "COLIDER_X_MINGE": "29B10005-E8F2-537E-4F6C-D104768A1214" ,
   "COLIDER_Y_MINGE": "29B10006-E8F2-537E-4F6C-D104768A1214",

   "POZITIE_PLATFORMA_STANGA_X" : "29B10007-E8F2-537E-4F6C-D104768A1214" , 
   "POZITIE_PLATFORMA_STANGA_Y" : "29B10008-E8F2-537E-4F6C-D104768A1214" ,
   "POZITIE_PLATFORMA_DREAPTA_X" : "29B10009-E8F2-537E-4F6C-D104768A1214" ,
   "POZITIE_PLATFORMA_DREAPTA_Y" : "29B10010-E8F2-537E-4F6C-D104768A1214" ,
}

""" 
BLEByteCharacteristic POZITIE_MINGE_X("29B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic POZITIE_MINGE_Y("29B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic DIRECTIE_MINGE("29B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic DEVITATIE_MINGE("29B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic COLIDER_X_MINGE("29B10005-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic COLIDER_Y_MINGE("29B10006-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);

BLEByteCharacteristic POZITIE_PLATFORMA_STANGA_X("29B10007-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic POZITIE_PLATFORMA_STANGA_Y("29B10008-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic POZITIE_PLATFORMA_DREAPTA_X("29B10009-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic POZITIE_PLATFORMA_DREAPTA_Y("29B10010-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);

"""

async def main():
    BLE=BLEManager()
    await BLE.connec_to_device(device_address)

    time.sleep(1)

    val_up=0
    val_down=0

    max_up=4
    max_down=126

    current_platform_position_x=0
    current_platform_position_y=0

    ball_direction=0
    ball_deviation=0
    ball_pozition_x=0
    ball_pozition_y=0


    while(1):
        current_platform_position_x=await BLE.get_caracteristic(device_caracteristics["POZITIE_PLATFORMA_STANGA_X"])
        current_platform_position_x=int.from_bytes(current_platform_position_x, byteorder='big')
        current_platform_position_y=await BLE.get_caracteristic(device_caracteristics["POZITIE_PLATFORMA_STANGA_Y"])
        current_platform_position_y=int.from_bytes(current_platform_position_y, byteorder='big')
        ball_direction=await BLE.get_caracteristic(device_caracteristics["DIRECTIE_MINGE"])
        ball_direction=int.from_bytes(ball_direction, byteorder='big')
        ball_deviation=await BLE.get_caracteristic(device_caracteristics["DEVITATIE_MINGE"])
        ball_deviation=int.from_bytes(ball_deviation, byteorder='big')
        ball_pozition_x=await BLE.get_caracteristic(device_caracteristics["POZITIE_MINGE_X"])
        ball_pozition_x=int.from_bytes(ball_pozition_x, byteorder='big')
        ball_pozition_y=await BLE.get_caracteristic(device_caracteristics["POZITIE_MINGE_Y"])
        ball_pozition_y=int.from_bytes(ball_pozition_y, byteorder='big')
        print("Pozitie platforma x :"+str(current_platform_position_x))
        print("Pozitie platforma y :"+str(current_platform_position_y))
        print("Directie minge :"+str(ball_direction))
        print("ball_pozition_x:"+str(ball_pozition_x))
        print("ball_pozition_y"+str(ball_pozition_y))


        if (ball_direction==2) :
            print("vine catre platforma mingea")
            if (ball_pozition_y<current_platform_position_y):
                
                if (current_platform_position_y>=4):
                    
                    val_up=1
                    val_down=0
                    await BLE.set_caracteristic(device_caracteristics["Button_LEFT"],val_up.to_bytes(1, 'big'))
                    await BLE.set_caracteristic(device_caracteristics["Button_UP"],val_down.to_bytes(1, 'big'))
            else:
                
                if (current_platform_position_y<120):
                   
                    val_down=1
                    val_up=0
                    await BLE.set_caracteristic(device_caracteristics["Button_LEFT"],val_up.to_bytes(1, 'big'))
                    await BLE.set_caracteristic(device_caracteristics["Button_UP"],val_down.to_bytes(1, 'big'))
        
        else :

            if (current_platform_position_y>=60):
                    
                    val_up=1
                    val_down=0
                    await BLE.set_caracteristic(device_caracteristics["Button_LEFT"],val_up.to_bytes(1, 'big'))
                    await BLE.set_caracteristic(device_caracteristics["Button_UP"],val_down.to_bytes(1, 'big'))
            else:
                
                if (current_platform_position_y<60):
                   
                    val_down=1
                    val_up=0
                    await BLE.set_caracteristic(device_caracteristics["Button_LEFT"],val_up.to_bytes(1, 'big'))
                    await BLE.set_caracteristic(device_caracteristics["Button_UP"],val_down.to_bytes(1, 'big'))
            
            


        
    

    await BLE.disconnect_a_device()
    #await discover_device_services_and_characteristics(device_address)
    #while 1 :
    #    Buton_up=await get_caracteristic(client_conn,device_caracteristics["Button_UP"])
    #    print(Buton_up)
    #    await set_caracteristic(client_conn,device_caracteristics["Button_UP"],bytearray(b'\x01'))
    #    Buton_up=await get_caracteristic(client_conn,device_caracteristics["Button_UP"])
    #    print(Buton_up)
    #    time.sleep(0.1)
    #await disconnect_a_device(client_conn)



if __name__=="__main__":

    #RULEAZA EVENT lOOPUL

    asyncio.run(main())

