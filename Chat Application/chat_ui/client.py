import socket
import tkinter as tk
from tkinter import scrolledtext
import threading

SERVER_TCP_PORT = 5750  # Port number for the server

# Function to handle receiving messages from the server
def receive_messages(client_socket, chat_window):
    while True:
        try:
            message = client_socket.recv(1024).decode()
            if message:
                chat_window.insert(tk.END, f"Server: {message}\n")
                chat_window.yview(tk.END)
        except:
            break

# Function to send messages to the server
def send_message(client_socket, message_entry):
    message = message_entry.get()
    if message:
        client_socket.send(message.encode())
        message_entry.delete(0, tk.END)

# Function to connect to the server and initialize communication
def connect_to_server(chat_window, message_entry, send_button, root):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('127.0.0.1', SERVER_TCP_PORT))

    # Start receiving messages from the server in a separate thread
    threading.Thread(target=receive_messages, args=(client_socket, chat_window), daemon=True).start()

    # Send messages when the user clicks the "Send" button
    send_button.config(command=lambda: send_message(client_socket, message_entry))

    def close_connection():
        client_socket.close()  # Close the client socket
        root.quit()  # Gracefully quit the Tkinter window

    # Close the connection when the user closes the window
    root.protocol("WM_DELETE_WINDOW", close_connection)

# Tkinter GUI for the client
def client_gui():
    root = tk.Tk()
    root.title("Chat Client")

    # Create a Text widget to display messages
    chat_window = scrolledtext.ScrolledText(root, width=50, height=20)
    chat_window.pack()

    # Create an Entry widget to type messages
    message_entry = tk.Entry(root, width=40)
    message_entry.pack()

    # Create a Send button
    send_button = tk.Button(root, text="Send")
    send_button.pack()

    # Connect to the server and start communication
    connect_to_server(chat_window, message_entry, send_button, root)

    root.mainloop()

if __name__ == "__main__":
    client_gui()

