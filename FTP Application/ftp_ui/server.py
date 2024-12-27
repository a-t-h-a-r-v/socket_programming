import socket
import os

SERVER_PORT = 8080
BUFFER_SIZE = 1024
OUTPUT_FILE = 'received_file.txt'  # The file where the server will save the received data

def receive_file(client_socket):
    with open(OUTPUT_FILE, 'wb') as f:
        print(f"Receiving file and saving to '{OUTPUT_FILE}'")
        while True:
            data = client_socket.recv(BUFFER_SIZE)
            if not data:
                break
            f.write(data)
        print("File received and saved successfully.")

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', SERVER_PORT))
    server_socket.listen(1)
    print(f"Server listening on port {SERVER_PORT}...")
    
    client_socket, client_addr = server_socket.accept()
    print(f"Connection established with {client_addr}")
    
    receive_file(client_socket)
    
    client_socket.close()
    server_socket.close()

if __name__ == "__main__":
    start_server()

