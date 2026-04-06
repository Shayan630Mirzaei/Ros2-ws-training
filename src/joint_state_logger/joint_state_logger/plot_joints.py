import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load CSV files
state_file = "/home/shayan630/joint_logs/joint_states_2026-02-09_11-14-51.csv"
traj_file  = "/home/shayan630/joint_logs/joint_trajectory_2026-02-09_11-14-51.csv"

state_df = pd.read_csv(state_file)
traj_df  = pd.read_csv(traj_file)

# Make sure time starts at 0
state_time = state_df["time"] - state_df["time"].iloc[0]
traj_time  = traj_df["time"] - traj_df["time"].iloc[0]

# Define canonical joint order
joint_order = ["joint1_pos", "joint2_pos", "joint3_pos", "joint4_pos", "joint5_pos", "joint6_pos"]

# ===============================
# Plotting
# ===============================
fig, axes = plt.subplots(3, 2, figsize=(12, 8))
axes = axes.flatten()

for i, joint in enumerate(joint_order):
    # Get correct column in each dataframe by name
    if joint in state_df.columns:
        axes[i].plot(state_time, np.degrees(state_df[joint]), color="red", linewidth=2, linestyle="-", label="State")
    if joint in traj_df.columns:
        axes[i].plot(traj_time, np.degrees(traj_df[joint]), color="blue", linewidth=2, linestyle="--", label="Trajectory")

    axes[i].set_xlabel("Time (s)")
    axes[i].set_ylabel(f"{joint} (deg)")
    axes[i].grid(True)
    axes[i].legend()

plt.tight_layout()
plt.show()
