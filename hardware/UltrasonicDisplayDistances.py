import serial
import tkinter as tk

ser = serial.Serial('/dev/tty.usbmodem142101', 9600)

root = tk.Tk()
root.title("Robot Sensor Simulation")

# Canvas to display the robot and sensor data
canvas_width = 500
canvas_height = 500
canvas = tk.Canvas(root, width=canvas_width, height=canvas_height)
canvas.pack()

# Robot dimensions
rect_width, rect_height = 50, 60
rect_x1 = (canvas_width - rect_width) / 2
rect_y1 = (canvas_height - rect_height) / 2
rect_x2 = rect_x1 + rect_width
rect_y2 = rect_y1 + rect_height

# Draw robot rectangle
robot_rectangle = canvas.create_rectangle(
    rect_x1, rect_y1, rect_x2, rect_y2, outline="black", width=3
)

# Labels to show sensor data
sensor_label = tk.Label(root, text="Sensor Data:", font=("Arial", 12))
sensor_label.pack()
left_frame = tk.Frame(root)
right_frame = tk.Frame(root)
left_frame.pack(side="left", padx=10, pady=10)
right_frame.pack(side="right", padx=10, pady=10)

left_label = tk.Label(left_frame, text="Front Left: 0 cm\nSide Left: 0 cm\nBack Left: 0 cm", font=("Arial", 10))
left_label.grid(row=0, column=0, sticky="n")
right_label = tk.Label(right_frame, text="Front Right: 0 cm\nSide Right: 0 cm\nBack Right: 0 cm", font=("Arial", 10))
right_label.grid(row=0, column=0, sticky="n")

# Variables to store previous distances
prev_distances = [0, 0, 0, 0, 0, 0]

# Function to update the display with data from Arduino
def update_display():
    global prev_distances
    if ser.in_waiting > 0:  # Check if data is available
        line = ser.readline().decode('utf-8').strip()

        if line.startswith("Distances:"):  # Ensure the data is correct
            try:
                distances = list(map(int, line.replace("Distances:", "").strip().split(',')))

                # Smooth the transition between old and new distances
                smoothed_distances = [
                    int(prev + 0.2 * (current - prev))  # Adjust the 0.2 for transitions
                    for prev, current in zip(prev_distances, distances)
                ]
                prev_distances = smoothed_distances

                distanceFL, distanceFR, distanceSL, distanceSR, distanceBL, distanceBR = smoothed_distances

                # Update robot and sensor lines
                canvas.delete("all") 
                canvas.create_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, outline="black", width=3)

                canvas.create_line(rect_x1, rect_y1, rect_x1, rect_y1 - distanceFL, arrow=tk.LAST, fill="green")
                canvas.create_line(rect_x2, rect_y1, rect_x2, rect_y1 - distanceFR, arrow=tk.LAST, fill="green")
                canvas.create_line(rect_x1, rect_y1 + rect_height / 2, rect_x1 - distanceSL, rect_y1 + rect_height / 2, arrow=tk.LAST, fill="orange")
                canvas.create_line(rect_x2, rect_y1 + rect_height / 2, rect_x2 + distanceSR, rect_y1 + rect_height / 2, arrow=tk.LAST, fill="purple")
                canvas.create_line(rect_x1, rect_y2, rect_x1, rect_y2 + distanceBL, arrow=tk.LAST, fill="red")
                canvas.create_line(rect_x2, rect_y2, rect_x2, rect_y2 + distanceBR, arrow=tk.LAST, fill="red")

                # Update sensor labels
                left_label.config(text=f"Front Left: {distanceFL} cm\nSide Left: {distanceSL} cm\nBack Left: {distanceBL} cm")
                right_label.config(text=f"Front Right: {distanceFR} cm\nSide Right: {distanceSR} cm\nBack Right: {distanceBR} cm")

            except ValueError:
                print(f"Invalid data received: {line}")

    root.after(50, update_display)  # Refresh display
# Start updating the display
update_display()
# Run the Tkinter window
root.mainloop()
