import matplotlib.pyplot as plt 

# Data arrays
time = ["1", "3", "5", "7", "9", "11"]
adc_module_1 = [745.0, 750.0, 800.0, 550.0, 450.0, 380.0]
adc_module_2 = [780.0, 785.0, 854.0, 415.0, 400.0, 310.0]
actual_values = [750.0, 790.0, 845.0, 423.0, 380.0, 267.0]

# Plot information
plt.title("VRAJ's ADC Module Performance Comparison")
plt.xlabel("seconds")
plt.ylabel("ADC resolution")
plt.xlim(1, 12)
plt.axis
plt.ylim(0, 1023)

# Plot data lines
plt.plot(time, adc_module_1, linestyle='--', marker='D', color='green', label='ADC Module 1')
plt.plot(time, adc_module_2, linestyle=':', marker='*', color='blue', label='ADC Module 2')
plt.plot(time, actual_values, linestyle='-', marker='o', color='red', label='Actual Values')

# Show legend
plt.legend()

# Save plot to PDF following local path
plt.savefig(r"C:\Users\vrajp\projects\my_project_2\ELNC_6060\ADC_Module_Performance_Comparison.pdf")

# Show plot
plt.show()
