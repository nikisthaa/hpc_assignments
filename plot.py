import pandas as pd
import matplotlib.pyplot as plt


basic_time = pd.read_csv("basic_result.csv")
advance_time = pd.read_csv("advance_result.csv")

# Dropping the first rows
basic_time = basic_time.drop(basic_time.index[0])
advance_time = advance_time.drop(advance_time.index[0])

# basic_time.drop(basic_time.index[0])
# basic_time.plot(x="Threads", y="Speadup", kind='line')
# plt.title('Speedup Vs Thread Plot')
# plt.xlabel('No. of Thread')
# plt.ylabel('Speedup')
# plt.savefig("basic_speedup_plot.png")

# basic_time.plot(x="Threads", y="Efficiency", kind='line')
# plt.title('Efficiency Vs Thread Plot')
# plt.xlabel('No. of Thread')
# plt.ylabel('Speedup')
# plt.savefig("basic_efficiency_plot.png")

# advance_time.drop(advance_time.index[0])
# advance_time.plot(x="Threads", y="Speadup", kind='line')
# plt.title('Speedup Vs Thread Plot')
# plt.xlabel('No. of Thread')
# plt.ylabel('Speedup')
# plt.savefig("advanced_speedup_plot.png")

# advance_time.plot(x="Threads", y="Efficiency", kind='line')
# plt.title('Efficiency Vs Thread Plot')
# plt.xlabel('No. of Thread')
# plt.ylabel('Speedup')
# plt.savefig("advanced_efficiency_plot.png")

# Speedup comparison
plt.figure()
plt.plot(basic_time["Threads"], basic_time["Speadup"], label='Basic Speedup', color='blue')
plt.plot(advance_time["Threads"], advance_time["Speadup"], label='Advanced Speedup', color='red')
plt.title('Speedup Vs Thread Plot')
plt.xlabel('No. of Thread')
plt.ylabel('Speedup')
plt.legend()
plt.savefig("speedup_comparison_plot.png")

# Efficiency comparison
plt.figure()
plt.plot(basic_time["Threads"], basic_time["Efficiency"], label='Basic Efficiency', color='green')
plt.plot(advance_time["Threads"], advance_time["Efficiency"], label='Advanced Efficiency', color='orange')
plt.title('Efficiency Vs Thread Plot')
plt.xlabel('No. of Thread')
plt.ylabel('Efficiency')
plt.legend()
plt.savefig("efficiency_comparison_plot.png")
