import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from MS5840 import MS5840
n_sensor=8
sensor_list=[]
for i in range(n_sensor):
    sensor_list.append(MS5840())


file=open('./test.txt','r')

line = file.readline() 
while(line):
    # print(line)
    if "begin" in line:
        # print("begin reading")
        break
    line = file.readline() 

while(line):
    line_seg=line.split()
    # print(line)
    if "coefficient" in line:
        sensor_list[int(line_seg[2])].read_coefficient(line_seg[-8:])
    if   'p' in line:
        D1=np.int64(int(line_seg[-1]))
        if D1 !=0:
            sensor_list[int(line_seg[1])].read_D1(float(line_seg[0]),D1)
    if   ('t' in line) and not ("coefficient" in line):
        D2=np.int64(int(line_seg[-1]))
        if D2 !=0:
            sensor_list[int(line_seg[1])].read_D2(float(line_seg[0]),D2)
    line = file.readline() 

# clean outliners
for i in range(n_sensor):
    mean_D1=np.mean(sensor_list[i].D1_list)
    std_D1=np.std(sensor_list[i].D1_list)
    mean_D2=np.mean(sensor_list[i].D2_list)
    std_D2=np.std(sensor_list[i].D2_list)
    # print(std_D1,std_D2)
    j=0
    while (j < len(sensor_list[i].D1_list)):
        if np.abs(sensor_list[i].D1_list[j] - mean_D1) > 6*std_D1:
            del sensor_list[i].D1_list[j]
            del sensor_list[i].pressure_time_list[j]
        j+=1
    j=0
    while (j < len(sensor_list[i].D2_list)):
        if np.abs(sensor_list[i].D2_list[j] - mean_D2) > 6*std_D2:
            del sensor_list[i].D2_list[j]
            del sensor_list[i].temperature_time_list[j]
        j+=1
# print("finished reading")
# for i in range(n_sensor):
#     j=0
#     while (j < len(sensor_list[i].D1_list)-1):
#         if abs(sensor_list[i].D1_list[j+1]-sensor_list[i].D1_list[j]) >20000:
#             del sensor_list[i].D1_list[j+1]
#             del sensor_list[i].pressure_time_list[j+1]
#         j+=1
#     j=0
#     while (j < len(sensor_list[i].D2_list)-1):
#         if abs(sensor_list[i].D2_list[j+1]-sensor_list[i].D2_list[j]) >20000:
#             del sensor_list[i].D2_list[j+1]
#             del sensor_list[i].temperature_time_list[j+1]
#         j+=1
    # print("finished processing sensor {}".format(i))
tmin_list=[]
# for i in range(n_sensor):
for i in [1,5,6,7]:
    sensor_list[i].process_data()
    tmin_list.append(np.min(sensor_list[i].pressure_time_list))
    tmin_list.append(np.min(sensor_list[i].temperature_time_list))
t0=np.min(np.array(tmin_list))

# for i in range(n_sensor):
for i in [1,5,6,7]:
    sensor_list[i].pressure_time_list=sensor_list[i].pressure_time_list-t0
    sensor_list[i].temperature_time_list=sensor_list[i].temperature_time_list-t0
    # sensor_list[i].pressure_list=sensor_list[i].pressure_list-np.mean(sensor_list[i].pressure_list[30:90])

# for i in range(n_sensor):
#     # if i ==0 or i == 7:
#     # if i ==1 or i == 6:
#     if i ==2 or i == 5:
#         plt.plot(sensor_list[i].pressure_time_list,sensor_list[i].D1_list,label="sensor {} D1".format(i))
#         # plt.plot(sensor_list[i].temperature_time_list,sensor_list[i].D2_list,label="sensor {} D2".format(i))
matplotlib.rcParams['xtick.direction'] = 'in' 
matplotlib.rcParams['ytick.direction'] = 'in' 
bwith = 0.25 #边框宽度设置为2

for i in range(n_sensor):

    plt.plot(sensor_list[i].pressure_time_list,sensor_list[i].pressure_list,label=" sensor {}".format(i))

       
plt.legend()
# plt.ylim([-300,300])
plt.xlabel('time')
plt.ylabel('pressure')
plt.show()