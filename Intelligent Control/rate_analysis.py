__author__ = 'Ququ'
# coding: utf-8


__author__ = 'Ququ'
# coding: utf-8

import pandas as pd
from pandas import Series, DataFrame, Panel
import numpy as np
import matplotlib.pyplot as plt
import datetime
import time as timee


data0 = pd.read_csv('export.csv')
data = pd.DataFrame(data0)
time = [datetime.datetime.strptime(str(x),'%Y%m%d%H%M%S') for x in data['timestamp']]



d = {'time':pd.Series(time),
     'value':pd.Series(data['value'])}
df = pd.DataFrame(d)
df = df.set_index('time')

df1 = df['2015-05-26 18:00:00':'2015-05-26 18:30:00']
minutestd1 = df1.resample('2Min', how=['std'])
print('std at 18:00:00:', minutestd1.sum())
#
#
#
df2 = df['2015-05-27 05:00:00':'2015-05-27 05:30:00']
minutestd2 = df2.resample('2Min', how=['std'])
print('std at 18:00:00:',minutestd2.sum())

rate = 100 *(minutestd1.sum() /minutestd2.sum())
print ("The rate is ",rate)
# # plt.plot([minutestd1,minutestd2])
# plt.figure(1)
# ax1 = plt.subplot(111)
# # ax2 = plt.subplot(212)
# plt.sca(ax1)
# ax1.plot(minutestd1)
# # minutestd1.plot()
# plt.xlabel('time')
# plt.ylabel('Standard deviation ')
#
# # plt.sca(ax2)
# ax1.plot(minutestd2)
# # minutestd2.plot()
# plt.xlabel('time')
# plt.ylabel('Standard deviation ')
# plt.ylim(10,19)





plt.show()