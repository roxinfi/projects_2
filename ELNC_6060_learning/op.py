import csv
import os
import matplotlib.pyplot as plt

# Constants
ENERGY_MODES = ["EM0", "EM1", "EM2", "EM3", "EM4"]

COLUMN_MCU = "MCU"
COLUMN_ENERGY_MODE = "Energy Mode"
COLUMN_CLOCK_FREQUENCY = "Clock Frequency (MHz)"
COLUMN_OPERATING_TEMP = "Operating Temperature"
COLUMN_VDD_SUPPLY = "VDD Supply (V)"
COLUMN_CURRENT_CONSUMPTION = "Current Consumption (uA/MHz)"

# Data Structures
em0_dataset = {}
em1_dataset = {}
em2_dataset = {}
em3_dataset = {}
em4_dataset = {}

all_frequencies = []

def read_valid_integer(prompt, min_val, max_val):
    # Function to read a valid integer within a range
    while True:
        try:
            value = int(input(prompt))
            if min_val <= value <= max_val:
                return value
            else:
                print(f"Please enter a number between {min_val} and {max_val}.")
        except ValueError:
            print("Invalid input. Please enter a number.")

def get_valid_file_path():
    # Function to get a valid file path
    while True:
        print()  # Adding line space
        file_path = input("Enter the path to the CSV file or 'q' to quit: ")
        print()  # Adding line space
        if file_path.lower() == 'q':
            exit()
        elif os.path.isfile(file_path) and file_path.endswith('.csv'):
            return file_path
        else:
            print("Invalid file path or not a CSV file. Please try again.")

def append_power_value(dataset, mcu_name, power_value):
    # Function to append power value to the dataset
    if mcu_name not in dataset:
        dataset[mcu_name] = []
    dataset[mcu_name].append(power_value)

def print_power_report(energy_mode, dataset, frequency_index, frequencies):
    # Function to print the power report
    frequency = frequencies[frequency_index]
    print(f"\n{energy_mode} Power Consumption Report at {frequency} MHz")
    print("MCU Family  |   Power Consumption")
    print("-----------------------------")

    min_power = float('inf')
    most_efficient_mcu = None

    for mcu, powers in dataset.items():
        power = powers[frequency_index]
        print(f"{mcu:12} | {power:10.2f} uW")
        if power < min_power:
            min_power = power
            most_efficient_mcu = mcu

    print(f"\nMost power-efficient MCU in {energy_mode} at {frequency} MHz: {most_efficient_mcu}")

def create_plot(energy_mode, dataset, frequencies, folder_path):
    # Function to create and save the plot
    for mcu, powers in dataset.items():
        plt.plot(frequencies, powers, label=mcu, marker='o')
    
    plt.xlabel("Clock Frequency (MHz)")
    plt.ylabel("Power Consumption (uW)")
    plt.title(f"{energy_mode} Power Consumption")
    plt.legend()
    plt.grid(True)

    pdf_path = os.path.join(folder_path, f"{energy_mode}-PowerConsumption.pdf")
    plt.savefig(pdf_path)
    print("\nPlot saved, Close plot window to continue.")
    plt.show()
    plt.clf()

def main():
    print("Welcome to the MCU Energy Mode Analyzer")

    while True:
        file_path = get_valid_file_path()
        unrecognized_modes = set()

        with open(file_path, 'r') as file:
            reader = csv.DictReader(file)
            columns = reader.fieldnames

            em0_dataset.clear()
            em1_dataset.clear()
            em2_dataset.clear()
            em3_dataset.clear()
            em4_dataset.clear()
            all_frequencies.clear()

            for row in reader:
                mcu = row[COLUMN_MCU]
                energy_mode = row[COLUMN_ENERGY_MODE]
                clock_frequency = float(row[COLUMN_CLOCK_FREQUENCY])
                vdd_supply = float(row[COLUMN_VDD_SUPPLY])
                current_consumption = float(row[COLUMN_CURRENT_CONSUMPTION])

                power_consumption = current_consumption * vdd_supply * clock_frequency

                if clock_frequency not in all_frequencies:
                    all_frequencies.append(clock_frequency)

                if energy_mode == ENERGY_MODES[0]:
                    append_power_value(em0_dataset, mcu, power_consumption)
                elif energy_mode == ENERGY_MODES[1]:
                    append_power_value(em1_dataset, mcu, power_consumption)
                elif energy_mode == ENERGY_MODES[2]:
                    append_power_value(em2_dataset, mcu, power_consumption)
                elif energy_mode == ENERGY_MODES[3]:
                    append_power_value(em3_dataset, mcu, power_consumption)
                elif energy_mode == ENERGY_MODES[4]:
                    append_power_value(em4_dataset, mcu, power_consumption)
                else:
                    unrecognized_modes.add(energy_mode)

        for mode in unrecognized_modes:
            print(f"Warning: energy mode {mode} is not supported")

        all_frequencies.sort()

        print("Select the energy mode to analyze:")
        for i, mode in enumerate(ENERGY_MODES):
            print(f"{i + 1}. {mode}")
        print()  # Adding line space
        energy_mode_index = read_valid_integer("Enter the number corresponding to the energy mode: ", 1, 5) - 1
        energy_mode = ENERGY_MODES[energy_mode_index]

        print()  # Adding line space
        for i, freq in enumerate(all_frequencies):
            print(f"{i}: {freq} MHz")
        print()  # Adding line space
        frequency_index = read_valid_integer("Select the frequency index: ", 0, len(all_frequencies) - 1)

        if energy_mode == ENERGY_MODES[0]:
            print_power_report(ENERGY_MODES[0], em0_dataset, frequency_index, all_frequencies)
            create_plot(ENERGY_MODES[0], em0_dataset, all_frequencies, os.path.dirname(file_path))
        elif energy_mode == ENERGY_MODES[1]:
            print_power_report(ENERGY_MODES[1], em1_dataset, frequency_index, all_frequencies)
            create_plot(ENERGY_MODES[1], em1_dataset, all_frequencies, os.path.dirname(file_path))
        elif energy_mode == ENERGY_MODES[2]:
            print_power_report(ENERGY_MODES[2], em2_dataset, frequency_index, all_frequencies)
            create_plot(ENERGY_MODES[2], em2_dataset, all_frequencies, os.path.dirname(file_path))
        elif energy_mode == ENERGY_MODES[3]:
            print_power_report(ENERGY_MODES[3], em3_dataset, frequency_index, all_frequencies)
            create_plot(ENERGY_MODES[3], em3_dataset, all_frequencies, os.path.dirname(file_path))
        elif energy_mode == ENERGY_MODES[4]:
            print_power_report(ENERGY_MODES[4], em4_dataset, frequency_index, all_frequencies)
            create_plot(ENERGY_MODES[4], em4_dataset, all_frequencies, os.path.dirname(file_path))

if __name__ == "__main__":
    main()
