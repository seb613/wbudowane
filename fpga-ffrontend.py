import subprocess
import tkinter as tk
from tkinter import messagebox

# Function to send commands to the C application
# def send_command(command):
    # process.stdin.write(command + "\n")
    # process.stdin.flush()
    # output = process.stdout.readline().strip()
    # return output

# Function to set variables
def set_variables():
    var1 = entry_var1.get()
    var2 = entry_var2.get()
    var3 = entry_var3.get()
    # send_command(f"set_var1 {var1}")
    # send_command(f"set_var2 {var2}")
    # send_command(f"set_var3 {var3}")
    messagebox.showinfo("Info", "Variables set successfully")

# Function to get variables
def get_variables():
    # output = send_command("get_vars")
    messagebox.showinfo("Variables", 'xd')#output)

# Start the C application
# process = subprocess.Popen(
#     ["./myapp"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True
# )

# Wait for the C application to indicate readiness
# print(process.stdout.readline().strip())  # Output: READY

# Create the main window
root = tk.Tk()
root.title("Variable Setter")

# Create and place labels and entry fields
tk.Label(root, text="Var1:").grid(row=0, column=0)
entry_var1 = tk.Entry(root)
entry_var1.grid(row=0, column=1)

tk.Label(root, text="Var2:").grid(row=1, column=0)
entry_var2 = tk.Entry(root)
entry_var2.grid(row=1, column=1)

tk.Label(root, text="Var3:").grid(row=2, column=0)
entry_var3 = tk.Entry(root)
entry_var3.grid(row=2, column=1)

# Create and place buttons
tk.Button(root, text="Set Variables", command=set_variables).grid(row=3, column=0, columnspan=2)
tk.Button(root, text="Get Variables", command=get_variables).grid(row=4, column=0, columnspan=2)

# Run the Tkinter event loop
root.mainloop()

# Exit the C application
# process.stdin.write("exit\n")
# process.stdin.flush()
# process.wait()