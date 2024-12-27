import socket
import tkinter as tk
from tkinter import scrolledtext
import threading

SERVER_TCP_PORT = 5750  # Port number for the server

# Function to handle incoming client connections
def handle_client(client_socket, client_address, chat_window):
    chat_window.insert(tk.END, f"Connection from {client_address} established\n")
    chat_window.yview(tk.END)

    while True:
        try:
            message = client_socket.recv(1024).decode()
            if not message:  # If the message is empty, the client has disconnected
                break
            chat_window.insert(tk.END, f"Client: {message}\n")
            chat_window.yview(tk.END)
        except ConnectionResetError:
            break

    client_socket.close()
    chat_window.insert(tk.END, f"Connection from {client_address} closed\n")
    chat_window.yview(tk.END)

# Function to start the server
def start_server(chat_window, server_socket, root, client_socket_ref):
    server_socket.bind(('127.0.0.1', SERVER_TCP_PORT))
    server_socket.listen(5)
    chat_window.insert(tk.END, "Server started. Waiting for clients...\n")
    chat_window.yview(tk.END)

    def stop_server():
        server_socket.close()
        root.quit()  # Gracefully quit the Tkinter window

    root.protocol("WM_DELETE_WINDOW", stop_server)

    while True:
        client_socket, client_address = server_socket.accept()
        client_socket_ref[0] = client_socket  # Set the client_socket to the reference
        threading.Thread(target=handle_client, args=(client_socket, client_address, chat_window), daemon=True).start()

# Function to send message from server to client
def send_message_to_client(client_socket, message_entry):
    message = message_entry.get()
    if message:
        client_socket.send(message.encode())
        message_entry.delete(0, tk.END)

# Tkinter GUI for server
def server_gui():
    root = tk.Tk()
    root.title("Chat Server")

    # Create a Text widget to display messages
    chat_window = scrolledtext.ScrolledText(root, width=50, height=20)
    chat_window.pack()

    # Create a message entry box and send button for the server to send messages
    message_entry = tk.Entry(root, width=40)
    message_entry.pack()

    send_button = tk.Button(root, text="Send", command=lambda: send_message_to_client(client_socket_ref[0], message_entry))
    send_button.pack()

    # Create server socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # This is a reference to store the client socket that will be set when the server accepts a client
    client_socket_ref = [None]

    # Start the server in a separate thread
    threading.Thread(target=start_server, args=(chat_window, server_socket, root, client_socket_ref), daemon=True).start()

    root.mainloop()

if __name__ == "__main__":
    server_gui()

