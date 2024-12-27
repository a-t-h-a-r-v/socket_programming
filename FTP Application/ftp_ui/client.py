import socket
import tkinter as tk
from tkinter import filedialog
from tkinter import messagebox

SERVER_IP = '127.0.0.1'  # Server IP address (localhost for testing)
SERVER_PORT = 8080
BUFFER_SIZE = 1024

def send_file(file_path):
    try:
        # Connect to the server
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((SERVER_IP, SERVER_PORT))
        print(f"Connected to server at {SERVER_IP}:{SERVER_PORT}")

        # Open the selected file in binary mode and send it
        with open(file_path, 'rb') as f:
            while (data := f.read(BUFFER_SIZE)):
                client_socket.send(data)
        
        print("File sent successfully")
        messagebox.showinfo("Success", f"File '{file_path}' sent successfully!")

    except Exception as e:
        print(f"Error: {e}")
        messagebox.showerror("Error", f"Failed to send file: {str(e)}")
    finally:
        client_socket.close()

def browse_file():
    file_path = filedialog.askopenfilename(title="Select a file")
    if file_path:
        send_file(file_path)

def create_gui():
    # Create the root window
    root = tk.Tk()
    root.title("File Upload Client")
    
    # Add a button to browse and select a file to upload
    upload_button = tk.Button(root, text="Select File to Upload", command=browse_file)
    upload_button.pack(pady=20)

    # Start the GUI event loop
    root.mainloop()

if __name__ == "__main__":
    create_gui()

