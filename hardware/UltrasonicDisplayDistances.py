import serial
import tkinter as tk
#Windows
# Set up serial communication (ensure COM port and baud rate match Arduino)
ser = serial.Serial('COM3', 9600)
#------
#Mac
# Set up serial communication (ensure port and baud rate match Arduino)
#ser = serial.Serial('/dev/tty.usbmodem142201', 9600)
#---------

# Initialize the main window
root = tk.Tk()
root.title("Robot Sensor Simulation")

# Set canvas size and create the canvas
canvas_width = 500
canvas_height = 500
canvas = tk.Canvas(root, width=canvas_width, height=canvas_height)
canvas.pack()

# Define the robot's dimensions and position (centered on the canvas)
rect_width, rect_height = 50, 60  # Updated robot's width and length (increased size)

# Center the rectangle in the window
rect_x1 = (canvas_width - rect_width) / 2
rect_y1 = (canvas_height - rect_height) / 2
rect_x2 = rect_x1 + rect_width
rect_y2 = rect_y1 + rect_height

# Create the central rectangle (representing the robot)
robot_rectangle = canvas.create_rectangle(
    rect_x1, rect_y1, rect_x2, rect_y2, outline="black", width=3
)

# Add a label to display the sensor values
sensor_label = tk.Label(root, text="Sensor Data:", font=("Arial", 12))
sensor_label.pack()

# Create labels for sensor data to organize text in grid layout
left_frame = tk.Frame(root)
right_frame = tk.Frame(root)
left_frame.pack(side="left", padx=10, pady=10)
right_frame.pack(side="right", padx=10, pady=10)

# Labels for left side and right side
left_label = tk.Label(left_frame, text="Front Left: 0 cm\nSide Left: 0 cm\nBack Left: 0 cm", font=("Arial", 10))
left_label.grid(row=0, column=0, sticky="n")
right_label = tk.Label(right_frame, text="Front Right: 0 cm\nSide Right: 0 cm\nBack Right: 0 cm", font=("Arial", 10))
right_label.grid(row=0, column=0, sticky="n")

# Function to update the display based on sensor data
def update_display():
    # Read the serial data
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        distances = list(map(int, line.split(',')))

        # Map the sensor data to positions around the rectangle
        distanceFL, distanceFR, distanceSR, distanceBR, distanceBL, distanceSL = distances

        # Update the position of sensors (represented as lines extending from the rectangle)
        canvas.delete("all")  # Clear previous drawing
        canvas.create_rectangle(
            rect_x1, rect_y1, rect_x2, rect_y2, outline="black", width=3
        )

        # Front Left sensor (distance mapped to line length) pointing straight up (90 degrees)
        canvas.create_line(rect_x1, rect_y1, rect_x1, rect_y1 - distanceFL, arrow=tk.LAST, fill="green")
        # Front Right sensor (distance mapped to line length) pointing straight up (90 degrees)
        canvas.create_line(rect_x2, rect_y1, rect_x2, rect_y1 - distanceFR, arrow=tk.LAST, fill="green")

        # Back Right sensor (distance mapped to line length) pointing down
        canvas.create_line(rect_x2, rect_y2, rect_x2, rect_y2 + distanceBR, arrow=tk.LAST, fill="red")
        # Back Left sensor (distance mapped to line length) pointing down
        canvas.create_line(rect_x1, rect_y2, rect_x1, rect_y2 + distanceBL, arrow=tk.LAST, fill="red")

        # Side Right sensor (distance mapped to line length) pointing to the right
        canvas.create_line(rect_x2, rect_y1 + rect_height / 2, rect_x2 + distanceSR, rect_y1 + rect_height / 2, arrow=tk.LAST, fill="purple")
        # Side Left sensor (distance mapped to line length) pointing to the left
        canvas.create_line(rect_x1, rect_y1 + rect_height / 2, rect_x1 - distanceSL, rect_y1 + rect_height / 2, arrow=tk.LAST, fill="orange")
        # Update the label with the sensor data and organize text
        left_label.config(
            text=f"Front Left: {distanceFL} cm\nSide Left: {distanceSL} cm\nBack Left: {distanceBL} cm"
        )
        right_label.config(
            text=f"Front Right: {distanceFR} cm\nSide Right: {distanceSR} cm\nBack Right: {distanceBR} cm"
        )

    # Refresh every 100 ms
    root.after(100, update_display)

# Start the update loop
update_display()

# Start the Tkinter event loop
root.mainloop()
