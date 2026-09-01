import csv

filename = "DATA.csv"

total_energy_mWh = 0.0
total_time_ms = 0

previous_time = None
previous_energy = None

with open(filename, "r") as file:
    reader = csv.DictReader(file)

    for row in reader:

        time_ms = int(row["Time_ms"])
        energy_mWh = float(row["Energy_mWh"])

        # First measurement
        if previous_time is None:
            previous_time = time_ms
            previous_energy = energy_mWh
            continue

        # Detect Arduino restart:
        # millis() starts again from zero
        if time_ms < previous_time:

            # Add the energy produced during the previous session
            total_energy_mWh += previous_energy

            # Add the duration of the previous session
            total_time_ms += previous_time

        previous_time = time_ms
        previous_energy = energy_mWh


# Add the final measurement session
if previous_energy is not None:
    total_energy_mWh += previous_energy
    total_time_ms += previous_time


# Convert mWh -> Wh
total_energy_Wh = total_energy_mWh / 1000.0

# Convert milliseconds -> seconds / minutes / hours
total_seconds = total_time_ms / 1000.0
total_minutes = total_seconds / 60.0
total_hours = total_seconds / 3600.0


# Calculate average energy produced per hour
# Wh/h is equivalent to average power in watts
if total_hours > 0:
    energy_per_hour_Wh = total_energy_Wh / total_hours
    energy_per_hour_mWh = total_energy_mWh / total_hours
else:
    energy_per_hour_Wh = 0.0
    energy_per_hour_mWh = 0.0

# Average power
# 1 Wh/h = 1 W and 1 mWh/h = 1 mW
average_power_W = energy_per_hour_Wh
average_power_mW = energy_per_hour_mWh


print("------ Measurement Results ------")
print(f"Total Energy:    {total_energy_Wh:.6f} Wh")
print(f"                 {total_energy_mWh:.3f} mWh")
print(f"Total Time:      {total_seconds:.2f} seconds")
print(f"                 {total_minutes:.2f} minutes")
print(f"                 {total_hours:.4f} hours")
print(f"Energy per hour: {energy_per_hour_Wh:.6f} Wh/h")
print(f"                 {energy_per_hour_mWh:.3f} mWh/h")
print(f"Average Power:   {average_power_W:.6f} W")
print(f"                 {average_power_mW:.3f} mW")
