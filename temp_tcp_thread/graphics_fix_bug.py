import sqlite3
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation
matplotlib.use('TkAgg')  # 使用TkAgg后端

# 初始化数据库连接
conn = sqlite3.connect('/home/hoostar/projects/remote_temp_v1.1/remote_temp/temp_tcp_thread/temperature_data.db')
cursor = conn.cursor()

# 初始化图表
fig, ax = plt.subplots()
xdata, ydata = [], []
ln, = plt.plot([], [], 'b-', animated=True)

# 设置图表的轴范围
ax.set_ylim(-10, 40)
ax.set_xlim(0, 100)
plt.xlabel('Time (s)')
plt.ylabel('Temperature (°C)')
plt.title('Real-time Temperature Monitoring')

def init():
    ln.set_data(xdata, ydata)
    return ln,

def update(frame):
    cursor.execute('SELECT Temp FROM Temperature ORDER BY Id DESC LIMIT 1')
    row = cursor.fetchone()
    if row:
        temp = row[0]
        xdata.append(frame)
        ydata.append(temp)
        if len(xdata) > 100:
            xdata.pop(0)
            ydata.pop(0)
        ln.set_data(xdata, ydata)
        
        # 更新x轴的范围，使图表向左移动
        ax.set_xlim(frame - 100, frame)

    return ln,

# 动画函数，每秒更新一次
ani = animation.FuncAnimation(fig, update, frames=range(1000), init_func=init, blit=True, interval=1000,repeat=True)

plt.show()

# 关闭数据库连接
conn.close()