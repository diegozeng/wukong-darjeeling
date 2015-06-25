__author__ = 'Ququ'
# coding: utf-8

import pandas as pd
from pandas import Series, DataFrame, Panel
import numpy as np
import matplotlib.pyplot as plt
import datetime
import time as timee

# plt.style.use('ggplot')

data0 = pd.read_csv('export.csv')
data = pd.DataFrame(data0)
time = [datetime.datetime.strptime(str(x),'%Y%m%d%H%M%S') for x in data['timestamp']]



d = {'time':pd.Series(time),
     'value':pd.Series(data['value'])}
df = pd.DataFrame(d)
df = df.set_index('time')
df = df['2015-05-26 22:00:00':'2015-05-27 06:00:00']

hourmean = df.resample('H', how=['mean'])
hourmean.columns = ['mean']
hourdiff = hourmean
# print(hourmean)
# hourmean.plot(linewidth=2)
# plt.xlabel("Time",fontsize=16)
# plt.ylabel("Value",fontsize=16)
# plt.rc('xtick', labelsize=24)
# plt.rc('ytick', labelsize=24)
# plt.title("Mean Value Per Hour in a Day",fontsize=26)

minuteMean = df.resample('1Min', how=['mean'])

for i in range(2,40,3):
    sec = str(i) + 'Min'
    newdf = minuteMean.asfreq(sec)
    hourmean_cur = newdf.resample('H', how='mean')
    hourmean_cur.columns = [sec]
    hourmean = pd.merge(hourmean, hourmean_cur, left_index=True, right_index=True, how='inner');
    hourdiff_cur = abs(hourmean_cur[sec]**2-hourmean['mean']**2)**(1/2)  #距离种类不确定
    # hourdiff_cur = abs(hourme an_cur[sec]-hourmean['mean'])
    hourdiff_cur.columns = [sec]
    hourdiff_cur = pd.merge(hourmean, hourmean_cur, left_index=True, right_index=True, how='inner');

diff_sum = hourdiff_cur.sum().drop('mean')
min = diff_sum.index[diff_sum==diff_sum.min()]
print(str(min))
diff_sum.plot()
plt.xlabel("Frequency",fontsize=16)
plt.ylabel("Sum Distance",fontsize=16)
plt.rc('xtick', labelsize=24)
plt.rc('ytick', labelsize=24)
plt.title("Sum of Distance Under Different Sampling Frequency ",fontsize=26)

# min.to_csv('out.csv')

# hour = df.resample('H', how=['max','min','mean','std'])
# # hour.plot(subplots = True)
# hour.plot()

# hourstd = df.resample('H', how='std')
# print(hourstd.max())


plt.show()