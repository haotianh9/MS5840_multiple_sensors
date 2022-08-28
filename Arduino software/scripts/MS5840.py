import numpy as np
class MS5840():
    def __init__(self):
        self.coefficient=np.zeros(8,dtype=np.uint16)
        self.D1_list=[]
        self.D2_list=[]
        self.pressure_time_list=[]
        self.temperature_time_list=[]
        self.pressure_list=[]
        self.temperature_list=[]
    def read_coefficient(self,coefficient):
        for i in range(8):
            self.coefficient[i]=np.int32(int(coefficient[i]))
    def read_D1(self,t,D1):
        self.D1_list.append(D1)
        self.pressure_time_list.append(t)
    def read_D2(self,t,D2):
        self.D2_list.append(D2)
        self.temperature_time_list.append(t)
    

    def process_data(self):
        if len(self.D2_list) > len(self.D1_list):
            self.D2_list=self.D2_list[:len(self.D1_list)]
            self.temperature_time_list=self.temperature_time_list[:len(self.D1_list)]
        if len(self.D1_list) > len(self.D2_list):
            self.D1_list=self.D1_list[:len(self.D2_list)]
            self.pressure_time_list=self.pressure_time_list[:len(self.D2_list)]
        for i in range(len(self.D1_list)):
            # print(i)
            pressure,temperature=self._process_data(self.D1_list[i],self.D2_list[i])
            self.pressure_list.append(pressure)
            self.temperature_list.append(temperature)

            
    def _process_data(self,D1,D2):
        temp_calc=np.int32(0)
        pressure_calc=np.int32(0)
        dT=np.int32(0)
        # print(D2)
        # print(type(D2))
        # print((np.int32(self.coefficient[5]) << 5))
        # print(D2)
        # print(self.coefficient)
        dT=D2-(np.int32(self.coefficient[5]) << 8)
        # print(dT)
        # print(coefficient[6])
        
        temp_calc=np.int32((np.int64(dT)*np.int64(self.coefficient[6]) >> 23) +2000)
        # print(temp_calc)
        TI=np.int64(0)
        OFFI=np.int64(0)
        SENSI=np.int64(0)
        OFF=np.int64(0)
        SENS=np.int64(0)
        OFF2=np.int64(0)
        SENS2=np.int64(0)
        if temp_calc > 2000:
            TI=0
            OFFI=0
            SENSI=0
        else:
            if temp_calc > 1000:
               TI = np.int64(12 * np.int64(dT*dT)/ np.power(2,35))
               OFFI = np.int64(30 * ((temp_calc - 2000) * (temp_calc - 2000)) / 256)
               SENSI = 0
            else:
                TI = np.int64(14 * (np.int64(dT*dT))/np.power(2,35))
                OFFI = np.int64(35 * ((temp_calc - 2000) * (temp_calc - 2000)) / 8)
                SENSI =  np.int64(63 * ((temp_calc - 2000) * (temp_calc - 2000)) / 32)
        OFF = (np.int64(self.coefficient[2] << 17)) + (((self.coefficient[4] * np.int64(dT))) >> 6)
        SENS = (np.int64(self.coefficient[1]) << 16) + (((self.coefficient[3] * np.int64(dT))) >> 7)
        temp_calc = temp_calc - TI
        OFF2 = OFF - OFFI
        SENS2 = SENS - SENSI
        pressure_calc = (((SENS2 * D1) / 2097152 ) - OFF2) / 32768
        # print(pressure_calc,temp_calc)
        return pressure_calc,temp_calc
if __name__ == "__main__":
    ms5840=MS5840()
    coefficient=['26785', '41335', '41466', '26840' ,'27317','31225' ,'28780' ,'65535'  ]
    ms5840.read_coefficient(coefficient)
    ms5840.read_D1(0,6735203)
    ms5840.read_D2(0,8073757)
    ms5840.process_data()